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

#ifndef QQUICKPATHRENDERNODE_P_H
#define QQUICKPATHRENDERNODE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of a number of Qt sources files.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qquickabstractpathrenderer_p.h"
#include <qsgnode.h>
#include <qsggeometry.h>
#include <QtGui/private/qtriangulatingstroker_p.h>

QT_BEGIN_NAMESPACE

class QQuickPathItem;
class QQuickPathRootRenderNode;

class QQuickPathRenderer : public QQuickAbstractPathRenderer
{
public:
    QQuickPathRenderer(QQuickPathRootRenderNode *rn) : m_rootNode(rn) { }

    void beginSync() override;
    void setPath(const QPainterPath &path) override;
    void setFillColor(const QColor &color) override;
    void setStrokeColor(const QColor &color) override;
    void setStrokeWidth(qreal w) override;
    void setFlags(RenderFlags flags) override;
    void setJoinStyle(QQuickPathItem::JoinStyle joinStyle, int miterLimit) override;
    void setCapStyle(QQuickPathItem::CapStyle capStyle) override;
    void setStrokeStyle(QQuickPathItem::StrokeStyle strokeStyle,
                        qreal dashOffset, const QVector<qreal> &dashPattern) override;
    void endSync() override;

    struct Color4ub { unsigned char r, g, b, a; };

private:
    void fill();
    void stroke();

    QQuickPathRootRenderNode *m_rootNode;
    QPainterPath m_path;
    QTriangulatingStroker m_stroker;
    QDashedStrokeProcessor m_dashStroker;
    RenderFlags m_flags;
    QPen m_pen;
    Color4ub m_fillColor;
    Color4ub m_strokeColor;
    uint m_needsNewGeom : 1;
    uint m_needsNewColor : 1;
};

class QQuickPathRenderNode : public QSGGeometryNode
{
public:
    QQuickPathRenderNode(QQuickItem *item);
    ~QQuickPathRenderNode();

private:
    QSGGeometry m_geometry;
    QScopedPointer<QSGMaterial> m_material;

    friend class QQuickPathRenderer;
};

class QQuickPathRootRenderNode : public QSGNode
{
public:
    QQuickPathRootRenderNode(QQuickItem *item, bool hasFill, bool hasStroke);
    ~QQuickPathRootRenderNode();

private:
    QQuickItem *m_item;
    QQuickPathRenderNode *m_fillNode;
    QQuickPathRenderNode *m_strokeNode;

    friend class QQuickPathRenderer;
};

QT_END_NAMESPACE

#endif
