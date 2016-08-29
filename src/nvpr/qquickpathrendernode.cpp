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

QQuickPathRenderNode::QQuickPathRenderNode(QQuickPathItem *item)
    : m_item(item),
      m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0, 0)
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
    QSGGeometry *g = &m_node->m_geometry;

    if (path.isEmpty()) {
        g->allocate(0, 0);
        m_node->markDirty(QSGNode::DirtyGeometry);
        return;
    }

    const qreal scale = 100;

    QTriangleSet ts = qTriangulate(path, QTransform::fromScale(scale, scale));
    qDebug() << ts.vertices;

    m_vertices.resize(ts.vertices.size() / 2);
    QSGGeometry::Point2D *vdst = m_vertices.data();
    const qreal *vdata = ts.vertices.constData();
    for (int i = 0; i < ts.vertices.size() / 2; ++i)
        vdst[i].set(vdata[i * 2] / scale, vdata[i * 2 + 1] / scale);

    m_indices.resize(ts.indices.size());
    quint16 *idst = m_indices.data();
    if (ts.indices.type() == QVertexIndexVector::UnsignedShort) {
        memcpy(idst, ts.indices.data(), m_indices.size() * sizeof(quint16));
    } else {
        const quint32 *isrc = (const quint32 *) ts.indices.data();
        for (int i = 0; i < m_indices.size(); ++i)
            idst[i] = isrc[i];
    }

    g->allocate(m_vertices.count(), m_indices.count());
    g->setDrawingMode(QSGGeometry::DrawTriangles);
    memcpy(g->vertexData(), m_vertices.constData(), g->vertexCount() * g->sizeOfVertex());
    memcpy(g->indexData(), m_indices.constData(), g->indexCount() * g->sizeOfIndex());
    m_node->markDirty(QSGNode::DirtyGeometry);
}

void QQuickPathRenderer::setMaterial(const QColor &color)
{
    static_cast<QSGFlatColorMaterial *>(m_node->m_material.data())->setColor(color);
    m_node->markDirty(QSGNode::DirtyMaterial);
}

QT_END_NAMESPACE
