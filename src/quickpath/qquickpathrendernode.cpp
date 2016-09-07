/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQuickPath module
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qquickpathrendernode_p.h"
#include "qquickpathrendermaterial_p.h"
#include "qquickpathitem_p.h"
#include <QtGui/private/qtriangulator_p.h>

QT_BEGIN_NAMESPACE

static const qreal SCALE = 100;

struct ColoredVertex // must match QSGGeometry::ColoredPoint2D
{
    float x, y;
    QQuickPathRenderer::Color4ub color;
    void set(float nx, float ny, QQuickPathRenderer::Color4ub ncolor)
    {
        x = nx; y = ny; color = ncolor;
    }
};

static inline QQuickPathRenderer::Color4ub operator *(QQuickPathRenderer::Color4ub c, float t)
{
    c.a *= t; c.r *= t; c.g *= t; c.b *= t; return c;
}

static inline QQuickPathRenderer::Color4ub operator +(QQuickPathRenderer::Color4ub a, QQuickPathRenderer::Color4ub b)
{
    a.a += b.a; a.r += b.r; a.g += b.g; a.b += b.b; return a;
}

static inline QQuickPathRenderer::Color4ub colorToColor4ub(const QColor &c)
{
    QQuickPathRenderer::Color4ub color = {
        uchar(qRound(c.redF() * c.alphaF() * 255)),
        uchar(qRound(c.greenF() * c.alphaF() * 255)),
        uchar(qRound(c.blueF() * c.alphaF() * 255)),
        uchar(qRound(c.alphaF() * 255))
    };
    return color;
}

QQuickPathRootRenderNode::QQuickPathRootRenderNode(QQuickItem *item, bool hasFill, bool hasStroke)
    : m_item(item),
      m_fillNode(nullptr),
      m_strokeNode(nullptr)
{
    if (hasFill) {
        m_fillNode = new QQuickPathRenderNode(item);
        appendChildNode(m_fillNode);
    }
    if (hasStroke) {
        m_strokeNode = new QQuickPathRenderNode(item);
        appendChildNode(m_strokeNode);
    }
}

QQuickPathRootRenderNode::~QQuickPathRootRenderNode()
{
}

QQuickPathRenderNode::QQuickPathRenderNode(QQuickItem *item)
    : m_geometry(QSGGeometry::defaultAttributes_ColoredPoint2D(), 0, 0)
{
    setGeometry(&m_geometry);

    // Use vertexcolor material. Items with different colors remain batchable
    // this way, at the expense of having to provide per-vertex color values.
    m_material.reset(QQuickPathRenderMaterialFactory::createVertexColor(item->window()));
    setMaterial(m_material.data());
}

QQuickPathRenderNode::~QQuickPathRenderNode()
{
}

void QQuickPathRenderer::beginSync()
{
    m_needsNewGeom = false;
    m_needsNewColor = false;
}

void QQuickPathRenderer::setPath(const QPainterPath &path)
{
    m_path = path;
    m_needsNewGeom = true;
}

void QQuickPathRenderer::setFillColor(const QColor &color)
{
    m_fillColor = colorToColor4ub(color);
    m_needsNewColor = true;

    if (color == Qt::transparent) {
        delete m_rootNode->m_fillNode;
        m_rootNode->m_fillNode = nullptr;
    } else if (!m_rootNode->m_fillNode) {
        m_rootNode->m_fillNode = new QQuickPathRenderNode(m_rootNode->m_item);
        if (m_rootNode->m_strokeNode)
            m_rootNode->removeChildNode(m_rootNode->m_strokeNode);
        m_rootNode->appendChildNode(m_rootNode->m_fillNode);
        if (m_rootNode->m_strokeNode)
            m_rootNode->appendChildNode(m_rootNode->m_strokeNode);
        m_needsNewGeom = true;
    }
}

void QQuickPathRenderer::setStrokeColor(const QColor &color)
{
    m_strokeColor = colorToColor4ub(color);
    m_needsNewColor = true;

    if (color == Qt::transparent) {
        delete m_rootNode->m_strokeNode;
        m_rootNode->m_strokeNode = nullptr;
    } else if (!m_rootNode->m_strokeNode) {
        m_rootNode->m_strokeNode = new QQuickPathRenderNode(m_rootNode->m_item);
        m_rootNode->appendChildNode(m_rootNode->m_strokeNode);
        m_needsNewGeom = true;
    }
}

void QQuickPathRenderer::setStrokeWidth(qreal w)
{
    m_pen.setWidthF(w);
    m_needsNewGeom = true;

    if (qFuzzyIsNull(w)) {
        delete m_rootNode->m_strokeNode;
        m_rootNode->m_strokeNode = nullptr;
    } else if (!m_rootNode->m_strokeNode) {
        m_rootNode->m_strokeNode = new QQuickPathRenderNode(m_rootNode->m_item);
        m_rootNode->appendChildNode(m_rootNode->m_strokeNode);
    }
}

void QQuickPathRenderer::setFlags(RenderFlags flags)
{
    m_flags = flags;
    m_needsNewGeom = true;
}

void QQuickPathRenderer::setJoinStyle(QQuickPathItem::JoinStyle joinStyle, int miterLimit)
{
    m_pen.setJoinStyle(Qt::PenJoinStyle(joinStyle));
    m_pen.setMiterLimit(miterLimit);
    m_needsNewGeom = true;
}

void QQuickPathRenderer::setCapStyle(QQuickPathItem::CapStyle capStyle)
{
    m_pen.setCapStyle(Qt::PenCapStyle(capStyle));
    m_needsNewGeom = true;
}

void QQuickPathRenderer::setStrokeStyle(QQuickPathItem::StrokeStyle strokeStyle,
                                        qreal dashOffset, const QVector<qreal> &dashPattern)
{
    m_pen.setStyle(Qt::PenStyle(strokeStyle));
    if (strokeStyle == QQuickPathItem::DashLine) {
        m_pen.setDashPattern(dashPattern);
        m_pen.setDashOffset(dashOffset);
    }
    m_needsNewGeom = true;
}

void QQuickPathRenderer::endSync()
{
    if (m_needsNewGeom || m_needsNewColor) {
        fill();
        stroke();
    }
}

void QQuickPathRenderer::fill()
{
    if (!m_rootNode->m_fillNode)
        return;

    QQuickPathRenderNode *n = m_rootNode->m_fillNode;
    n->markDirty(QSGNode::DirtyGeometry);

    QSGGeometry *g = &n->m_geometry;
    if (m_path.isEmpty()) {
        g->allocate(0, 0);
        return;
    }

    if (m_needsNewColor && !m_needsNewGeom) {
        ColoredVertex *vdst = reinterpret_cast<ColoredVertex *>(g->vertexData());
        for (int i = 0; i < g->vertexCount(); ++i)
            vdst[i].set(vdst[i].x, vdst[i].y, m_fillColor);
        return;
    }

    const QVectorPath &vp = qtVectorPathForPath(m_path);
    QVarLengthArray<QSGGeometry::ColoredPoint2D> vertices;
    QVarLengthArray<quint16> indices;

    QTriangleSet ts = qTriangulate(vp, QTransform::fromScale(SCALE, SCALE));
    const int vertexCount = ts.vertices.count() / 2; // just a qreal vector with x,y hence the / 2
    vertices.resize(vertexCount);
    ColoredVertex *vdst = reinterpret_cast<ColoredVertex *>(vertices.data());
    const qreal *vsrc = ts.vertices.constData();
    for (int i = 0; i < vertexCount; ++i)
        vdst[i].set(vsrc[i * 2] / SCALE, vsrc[i * 2 + 1] / SCALE, m_fillColor);

    indices.resize(ts.indices.size());
    quint16 *idst = indices.data();
    if (ts.indices.type() == QVertexIndexVector::UnsignedShort) {
        memcpy(idst, ts.indices.data(), indices.count() * sizeof(quint16));
    } else {
        const quint32 *isrc = (const quint32 *) ts.indices.data();
        for (int i = 0; i < indices.count(); ++i)
            idst[i] = isrc[i];
    }

    g->allocate(vertexCount, indices.count());
    g->setDrawingMode(QSGGeometry::DrawTriangles);
    memcpy(g->vertexData(), vertices.constData(), g->vertexCount() * g->sizeOfVertex());
    memcpy(g->indexData(), indices.constData(), g->indexCount() * g->sizeOfIndex());
}

void QQuickPathRenderer::stroke()
{
    if (!m_rootNode->m_strokeNode)
        return;

    QQuickPathRenderNode *n = m_rootNode->m_strokeNode;
    n->markDirty(QSGNode::DirtyGeometry);

    QSGGeometry *g = &n->m_geometry;
    if (m_path.isEmpty()) {
        g->allocate(0, 0);
        return;
    }

    if (m_needsNewColor && !m_needsNewGeom) {
        ColoredVertex *vdst = reinterpret_cast<ColoredVertex *>(g->vertexData());
        for (int i = 0; i < g->vertexCount(); ++i)
            vdst[i].set(vdst[i].x, vdst[i].y, m_strokeColor);
        return;
    }

    const QVectorPath &vp = qtVectorPathForPath(m_path);
    QVarLengthArray<QSGGeometry::ColoredPoint2D> vertices;

    const QRectF clip(0, 0, m_rootNode->m_item->width(), m_rootNode->m_item->height());
    const qreal inverseScale = 1.0 / SCALE;
    m_stroker.setInvScale(inverseScale);
    if (m_pen.style() == Qt::SolidLine) {
        m_stroker.process(vp, m_pen, clip, 0);
    } else {
        m_dashStroker.setInvScale(inverseScale);
        m_dashStroker.process(vp, m_pen, clip, 0);
        QVectorPath dashStroke(m_dashStroker.points(), m_dashStroker.elementCount(),
                               m_dashStroker.elementTypes(), 0);
        m_stroker.process(dashStroke, m_pen, clip, 0);
    }
    if (!m_stroker.vertexCount()) {
        g->allocate(0, 0);
        return;
    }

    const int vertexCount = m_stroker.vertexCount() / 2; // just a float vector with x,y hence the / 2
    vertices.resize(vertexCount);
    ColoredVertex *vdst = reinterpret_cast<ColoredVertex *>(vertices.data());
    const float *vsrc = m_stroker.vertices();
    for (int i = 0; i < vertexCount; ++i)
        vdst[i].set(vsrc[i * 2], vsrc[i * 2 + 1], m_strokeColor);

    g->allocate(vertexCount, 0);
    g->setDrawingMode(QSGGeometry::DrawTriangleStrip);
    memcpy(g->vertexData(), vertices.constData(), g->vertexCount() * g->sizeOfVertex());
}

QT_END_NAMESPACE
