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
    enum Dirty {
        DirtyGeom = 0x01,
        DirtyColor = 0x02
    };

    QQuickPathRenderer(QQuickItem *item)
        : m_item(item),
          m_rootNode(nullptr),
          m_renderDirty(0)
          { }

    void setRootNode(QQuickPathRootRenderNode *rn);

    void beginSync() override;
    void setPath(const QPainterPath &path) override;
    void setFillColor(const QColor &color, QQuickPathGradient *gradient) override;
    void setStrokeColor(const QColor &color) override;
    void setStrokeWidth(qreal w) override;
    void setFlags(RenderFlags flags) override;
    void setJoinStyle(QQuickPathItem::JoinStyle joinStyle, int miterLimit) override;
    void setCapStyle(QQuickPathItem::CapStyle capStyle) override;
    void setStrokeStyle(QQuickPathItem::StrokeStyle strokeStyle,
                        qreal dashOffset, const QVector<qreal> &dashPattern,
                        bool cosmeticStroke) override;
    void endSync() override;
    void updatePathRenderNode() override;

    struct Color4ub { unsigned char r, g, b, a; };

    struct GradientDesc {
        QGradientStops stops;
        QPointF start;
        QPointF end;
        QQuickPathGradient::SpreadMode spread;
        bool operator==(const GradientDesc &other) const
        {
            return start == other.start && end == other.end && spread == other.spread
                   && stops == other.stops;
        }
    };

    bool isFillGradientActive() const { return m_fillGradientActive; }
    const GradientDesc *fillGradient() const { return &m_fillGradient; }

private:
    void triangulateFill();
    void triangulateStroke();
    void updateFillNode();
    void updateStrokeNode();

    QQuickItem *m_item;
    QQuickPathRootRenderNode *m_rootNode;
    QTriangulatingStroker m_stroker;
    QDashedStrokeProcessor m_dashStroker;

    RenderFlags m_flags;
    QPen m_pen;
    Color4ub m_fillColor;
    Color4ub m_strokeColor;
    QPainterPath m_path;

    QVector<QSGGeometry::ColoredPoint2D> m_fillVertices;
    QVector<quint16> m_fillIndices;
    QVector<QSGGeometry::ColoredPoint2D> m_strokeVertices;

    int m_guiDirty;
    int m_renderDirty;

    bool m_fillGradientActive;
    GradientDesc m_fillGradient;
};

inline uint qHash(const QQuickPathRenderer::GradientDesc &v, uint seed = 0)
{
    uint h = seed;
    h += v.start.x() + v.end.y() + v.spread;
    for (int i = 0; i < 3 && i < v.stops.count(); ++i)
        h += v.stops[i].second.rgba();
    return h;
}

class QQuickPathRenderNode : public QSGGeometryNode
{
public:
    QQuickPathRenderNode(QQuickWindow *window, QQuickPathRootRenderNode *rootNode);
    ~QQuickPathRenderNode();

    enum Material {
        MatSolidColor,
        MatLinearGradient
    };

    void activateMaterial(Material m);

    QQuickWindow *window() const { return m_window; }
    QQuickPathRootRenderNode *rootNode() const { return m_rootNode; }
    int dirty() const { return m_dirty; }
    void resetDirty() { m_dirty = 0; }

private:
    QSGGeometry m_geometry;
    QQuickWindow *m_window;
    QQuickPathRootRenderNode *m_rootNode;
    int m_dirty;
    QSGMaterial *m_material;
    QScopedPointer<QSGMaterial> m_solidColorMaterial;
    QScopedPointer<QSGMaterial> m_linearGradientMaterial;

    friend class QQuickPathRenderer;
};

class QQuickPathRootRenderNode : public QSGNode
{
public:
    QQuickPathRootRenderNode(QQuickWindow *window, bool hasFill, bool hasStroke);
    ~QQuickPathRootRenderNode();

    QQuickPathRenderer *renderer() const { return m_renderer; }

private:
    QQuickPathRenderNode *m_fillNode;
    QQuickPathRenderNode *m_strokeNode;
    QQuickPathRenderer *m_renderer;

    friend class QQuickPathRenderer;
};

QT_END_NAMESPACE

#endif
