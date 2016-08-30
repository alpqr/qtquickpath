/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtNVPR module
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
#include "qquickpathitem_p.h"
#include <QSGFlatColorMaterial>
#include <QtGui/private/qtriangulator_p.h>

QT_BEGIN_NAMESPACE

static const qreal SCALE = 100;

QQuickPathRootRenderNode::QQuickPathRootRenderNode(QQuickPathItem *item)
    : m_item(item)
{
    m_fillNode = new QQuickPathRenderNode;
    appendChildNode(m_fillNode);
    m_strokeNode = new QQuickPathRenderNode;
    appendChildNode(m_strokeNode);
}

QQuickPathRootRenderNode::~QQuickPathRootRenderNode()
{
}

QQuickPathRenderNode::QQuickPathRenderNode()
    : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0, 0)
{
    setGeometry(&m_geometry);
    m_material.reset(new QSGFlatColorMaterial);
    setMaterial(m_material.data());
}

QQuickPathRenderNode::~QQuickPathRenderNode()
{
}

void QQuickPathRenderer::setPath(const QPainterPath &path)
{
    m_path = path;
}

void QQuickPathRenderer::setFillMaterial(const QColor &color)
{
    QQuickPathRenderNode *n = m_rootNode->m_fillNode;
    static_cast<QSGFlatColorMaterial *>(n->m_material.data())->setColor(color);
    n->markDirty(QSGNode::DirtyMaterial);
    // ### these don't really need geometry regeneration in commit()
}

void QQuickPathRenderer::setStrokeMaterial(const QColor &color)
{
    QQuickPathRenderNode *n = m_rootNode->m_strokeNode;
    static_cast<QSGFlatColorMaterial *>(n->m_material.data())->setColor(color);
    n->markDirty(QSGNode::DirtyMaterial);
}

void QQuickPathRenderer::setStrokeWidth(qreal w)
{
    m_strokeWidth = w;
}

void QQuickPathRenderer::fill()
{
    QQuickPathRenderNode *n = m_rootNode->m_fillNode;
    QSGGeometry *g = &n->m_geometry;
    if (m_flags.testFlag(RenderNoFill) || m_path.isEmpty()) {
        g->allocate(0, 0);
        n->markDirty(QSGNode::DirtyGeometry);
        return;
    }

    const QVectorPath &vp = qtVectorPathForPath(m_path);

    QTriangleSet ts = qTriangulate(vp, QTransform::fromScale(SCALE, SCALE));
    m_vertices.resize(ts.vertices.count() / 2);
    QSGGeometry::Point2D *vdst = m_vertices.data();
    const qreal *vdata = ts.vertices.constData();
    for (int i = 0; i < ts.vertices.count() / 2; ++i)
        vdst[i].set(vdata[i * 2] / SCALE, vdata[i * 2 + 1] / SCALE);

    m_indices.resize(ts.indices.size());
    quint16 *idst = m_indices.data();
    if (ts.indices.type() == QVertexIndexVector::UnsignedShort) {
        memcpy(idst, ts.indices.data(), m_indices.count() * sizeof(quint16));
    } else {
        const quint32 *isrc = (const quint32 *) ts.indices.data();
        for (int i = 0; i < m_indices.count(); ++i)
            idst[i] = isrc[i];
    }

    g->allocate(m_vertices.count(), m_indices.count());
    g->setDrawingMode(QSGGeometry::DrawTriangles);
    memcpy(g->vertexData(), m_vertices.constData(), g->vertexCount() * g->sizeOfVertex());
    memcpy(g->indexData(), m_indices.constData(), g->indexCount() * g->sizeOfIndex());
    n->markDirty(QSGNode::DirtyGeometry);
}

void QQuickPathRenderer::stroke()
{
    QQuickPathRenderNode *n = m_rootNode->m_strokeNode;
    QSGGeometry *g = &n->m_geometry;
    if (m_path.isEmpty() || qFuzzyIsNull(m_strokeWidth)) {
        g->allocate(0, 0);
        n->markDirty(QSGNode::DirtyGeometry);
        return;
    }

    const QVectorPath &vp = qtVectorPathForPath(m_path);

    QRectF clip(0, 0, m_rootNode->m_item->width(), m_rootNode->m_item->height());
    QPen pen;
    pen.setWidth(m_strokeWidth);
    const qreal inverseScale = 1 / SCALE;
    m_stroker.setInvScale(inverseScale);
    m_stroker.process(vp, pen, clip, 0);
    if (!m_stroker.vertexCount())
        return;

    g->allocate(m_stroker.vertexCount() / 2, 0);
    g->setDrawingMode(QSGGeometry::DrawTriangleStrip);
    memcpy(g->vertexData(), m_stroker.vertices(), g->vertexCount() * g->sizeOfVertex());
    n->markDirty(QSGNode::DirtyGeometry);
}

void QQuickPathRenderer::setFlags(RenderFlags flags)
{
    m_flags = flags;
}

void QQuickPathRenderer::commit()
{
    fill();
    stroke();
}

QT_END_NAMESPACE
