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

#include "qquickpathitem_p.h"
#include "qnvprrendernode_p.h"
#include "qquickpathrendernode_p.h"
#include <QSGRendererInterface>
#include <QPainterPath>

QT_BEGIN_NAMESPACE

class QQuickPathItemPrivate
{
public:
    QQuickPathItemPrivate()
        : renderer(nullptr),
          dirty(0),
          strokeWidth(0),
          strokeColor(Qt::white),
          fillColor(Qt::white)
    { }
    ~QQuickPathItemPrivate() { delete renderer; }

    enum Dirty {
        DirtyPath = 0x01,
        DirtyStrokeMaterial = 0x02,
        DirtyFillMaterial = 0x04
    };

    QPainterPath path;
    QQuickAbstractPathRenderer *renderer;
    int dirty;
    qreal strokeWidth;
    QColor strokeColor;
    QColor fillColor;
};

QQuickPathItem::QQuickPathItem(QQuickItem *parent)
    : QQuickItem(parent),
      d(new QQuickPathItemPrivate)
{
    setFlag(ItemHasContents);
}

QQuickPathItem::~QQuickPathItem()
{
    delete d;
}

QSGNode *QQuickPathItem::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    if (!node) {
        QSGRendererInterface *ri = window()->rendererInterface();
        if (!ri)
            return nullptr;
        switch (ri->graphicsApi()) {
#ifndef QT_NO_OPENGL
            case QSGRendererInterface::OpenGL:
                if (QNvprRenderNode::isSupported()) {
                    node = new QNvprRenderNode(this);
                    d->renderer = new QNvprPathRenderer(static_cast<QNvprRenderNode *>(node));
                } else {
                    node = new QQuickPathRenderNode(this);
                    d->renderer = new QQuickPathRenderer(static_cast<QQuickPathRenderNode *>(node));
                }
                break;
#endif

            case QSGRendererInterface::Direct3D12:
                node = new QQuickPathRenderNode(this);
                d->renderer = new QQuickPathRenderer(static_cast<QQuickPathRenderNode *>(node));
                break;

            case QSGRendererInterface::Software:
            default:
                qWarning("No path backend for this graphics API yet");
                break;
        }
        d->dirty |= QQuickPathItemPrivate::DirtyPath;
    }

    if (d->dirty & QQuickPathItemPrivate::DirtyPath) {
        d->renderer->setPath(d->path);
        d->dirty &= ~QQuickPathItemPrivate::DirtyPath;
    }
    if (d->dirty & QQuickPathItemPrivate::DirtyFillMaterial) {
        d->renderer->setMaterial(d->fillColor);
        d->dirty &= ~QQuickPathItemPrivate::DirtyFillMaterial;
    }

    return node;
}

void QQuickPathItem::clear()
{
    d->path = QPainterPath();
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

bool QQuickPathItem::isEmpty() const
{
    return d->path.isEmpty();
}

void QQuickPathItem::closeSubPath()
{
    d->path.closeSubpath();
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::moveTo(qreal x, qreal y)
{
    d->path.moveTo(x, y);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::lineTo(qreal x, qreal y)
{
    d->path.lineTo(x, y);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::arcMoveTo(qreal x, qreal y, qreal w, qreal h, qreal angle)
{
    d->path.arcMoveTo(x, y, w, h, angle);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::arcTo(qreal x, qreal y, qreal w, qreal h, qreal startAngle, qreal arcLength)
{
    d->path.arcTo(x, y, w, h, startAngle, arcLength);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::cubicTo(qreal c1x, qreal c1y, qreal c2x, qreal c2y, qreal endX, qreal endY)
{
    d->path.cubicTo(c1x, c1y, c2x, c2y, endX, endY);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::quadTo(qreal cX, qreal cY, qreal endX, qreal endY)
{
    d->path.quadTo(cX, cY, endX, endY);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::addRect(qreal x, qreal y, qreal w, qreal h)
{
    d->path.addRect(x, y, w, h);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::addRoundedRect(qreal x, qreal y, qreal w, qreal h, qreal xr, qreal yr)
{
    d->path.addRoundedRect(x, y, w, h, xr, yr);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::addEllipse(qreal x, qreal y, qreal rx, qreal ry)
{
    d->path.addEllipse(x, y, rx, ry);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

QPointF QQuickPathItem::currentPosition() const
{
    return d->path.currentPosition();
}

QRectF QQuickPathItem::boundingRect() const
{
    return d->path.boundingRect();
}

QRectF QQuickPathItem::controlPointRect() const
{
    return d->path.controlPointRect();
}

QQuickPathItem::FillRule QQuickPathItem::fillRule() const
{
    return FillRule(d->path.fillRule());
}

void QQuickPathItem::setFillRule(FillRule fillRule)
{
    if (d->path.fillRule() != fillRule) {
        d->path.setFillRule(Qt::FillRule(fillRule));
        d->dirty |= QQuickPathItemPrivate::DirtyPath;
        emit fillRuleChanged();
    }
}

QVariant QQuickPathItem::fillMaterial() const
{
    return d->fillColor;
}

void QQuickPathItem::setFillMaterial(const QVariant &material)
{
    if (material.type() != QVariant::Color) {
        qWarning("Unknown fill material");
        return;
    }
    const QColor color = material.value<QColor>();
    if (d->fillColor != color) {
        d->fillColor = color;
        d->dirty |= QQuickPathItemPrivate::DirtyFillMaterial;
        emit fillMaterialChanged();
    }
}

QVariant QQuickPathItem::strokeMaterial() const
{
    return d->strokeColor;
}

void QQuickPathItem::setStrokeMaterial(const QVariant &material)
{
    // ###
}

qreal QQuickPathItem::strokeWidth() const
{
    return 0;
}

void QQuickPathItem::setStrokeWidth(qreal w)
{
}

QT_END_NAMESPACE
