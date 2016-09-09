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

#include "qquickpathitem_p_p.h"
#include "qnvprrendernode_p.h"
#include "qquickpathrendernode_p.h"
#include <QSGRendererInterface>
#include <QPainterPath>

QT_BEGIN_NAMESPACE

QQuickPathItem::QQuickPathItem(QQuickItem *parent)
    : QQuickItem(*new QQuickPathItemPrivate, parent)
{
    setFlag(ItemHasContents);
}

QQuickPathItem::~QQuickPathItem()
{
}

void QQuickPathItemPrivate::appendCommand(QQmlListProperty<QObject> *list, QObject *cmd)
{
    QQuickPathCommand *ccmd = qobject_cast<QQuickPathCommand *>(cmd);
    if (!ccmd) {
        qWarning("Cannot add non-QQuickPathCommand derived items to the command list");
        return;
    }
    QQuickPathItem *item = qobject_cast<QQuickPathItem *>(list->object);
    Q_ASSERT(item);
    ccmd->setParent(item);
    QQuickPathItemPrivate::get(item)->commands.append(ccmd);
}

void QQuickPathItemPrivate::handlePathCommandChange()
{
    Q_Q(QQuickPathItem);
    dirty |= QQuickPathItemPrivate::DirtyPath;
    q->updatePath();
}

// renderer lives on the gui thread and all its functions except
// updatePathRenderNode() are invoked on the gui thread.
void QQuickPathItemPrivate::createRenderer()
{
    Q_Q(QQuickPathItem);
    QSGRendererInterface *ri = q->window()->rendererInterface();
    if (!ri)
        return;

    switch (ri->graphicsApi()) {
#ifndef QT_NO_OPENGL
    case QSGRendererInterface::OpenGL:
        renderer = new QQuickPathRenderer(q);
        break;
#endif
    default:
        qWarning("No path backend for this graphics API yet");
        break;
    }
}

// the node lives on the render thread
QSGNode *QQuickPathItemPrivate::createRenderNode()
{
    Q_Q(QQuickPathItem);
    QSGNode *node = nullptr;
    if (!q->window())
        return node;
    QSGRendererInterface *ri = q->window()->rendererInterface();
    if (!ri)
        return node;

    const bool hasFill = fillColor != Qt::transparent;
    const bool hasStroke = !qFuzzyIsNull(strokeWidth) && strokeColor != Qt::transparent;

    switch (ri->graphicsApi()) {
#ifndef QT_NO_OPENGL
    case QSGRendererInterface::OpenGL:
        node = new QQuickPathRootRenderNode(q->window(), hasFill, hasStroke);
        static_cast<QQuickPathRenderer *>(renderer)->setRootNode(static_cast<QQuickPathRootRenderNode *>(node));
        break;
#endif
    default:
        qWarning("No path backend for this graphics API yet");
        break;
    }

    return node;
}

void QQuickPathItemPrivate::sync()
{
    renderer->beginSync();

    if (dirty & QQuickPathItemPrivate::DirtyPath) {
        if (!commands.isEmpty()) {
            path = QPainterPath();
            for (QQuickPathCommand *cmd : qAsConst(commands))
                cmd->addToPath(&path);
        }
        renderer->setPath(path);
    }
    if (dirty & QQuickPathItemPrivate::DirtyFillColor)
        renderer->setFillColor(fillColor, fillGradient);
    if (dirty & QQuickPathItemPrivate::DirtyStrokeColor)
        renderer->setStrokeColor(strokeColor);
    if (dirty & QQuickPathItemPrivate::DirtyStrokeWidth)
        renderer->setStrokeWidth(strokeWidth);
    if (dirty & QQuickPathItemPrivate::DirtyFlags)
        renderer->setFlags(flags);
    if (dirty & QQuickPathItemPrivate::DirtyStyle) {
        renderer->setJoinStyle(joinStyle, miterLimit);
        renderer->setCapStyle(capStyle);
        renderer->setStrokeStyle(strokeStyle, dashOffset, dashPattern, cosmeticStroke);
    }

    renderer->endSync();
    dirty = 0;
}

void QQuickPathItem::updatePolish()
{
    Q_D(QQuickPathItem);

    if (!d->dirty)
        return;

    if (!d->renderer)
        d->createRenderer();

    d->sync();

    update();
}

QSGNode *QQuickPathItem::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    // on the render thread, with the gui thread blocked. we can now
    // safely access gui thread data.

    Q_D(QQuickPathItem);
    if (!node) {
        node = d->createRenderNode();
        d->dirty |= 0xFFFF;
    }

    if (d->renderer)
        d->renderer->updatePathRenderNode();

    return node;
}

void QQuickPathItem::updatePath()
{
    polish();
}

void QQuickPathItem::clear()
{
    Q_D(QQuickPathItem);
    d->path = QPainterPath();
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

bool QQuickPathItem::isEmpty() const
{
    Q_D(const QQuickPathItem);
    return d->path.isEmpty();
}

void QQuickPathItem::closeSubPath()
{
    Q_D(QQuickPathItem);
    d->path.closeSubpath();
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::moveTo(qreal x, qreal y)
{
    Q_D(QQuickPathItem);
    d->path.moveTo(x, y);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::lineTo(qreal x, qreal y)
{
    Q_D(QQuickPathItem);
    d->path.lineTo(x, y);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::arcMoveTo(qreal x, qreal y, qreal w, qreal h, qreal angle)
{
    Q_D(QQuickPathItem);
    d->path.arcMoveTo(x, y, w, h, angle);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::arcTo(qreal x, qreal y, qreal w, qreal h, qreal startAngle, qreal arcLength)
{
    Q_D(QQuickPathItem);
    d->path.arcTo(x, y, w, h, startAngle, arcLength);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::cubicTo(qreal cx1, qreal cy1, qreal cx2, qreal cy2, qreal ex, qreal ey)
{
    Q_D(QQuickPathItem);
    d->path.cubicTo(cx1, cy1, cx2, cy2, ex, ey);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::quadTo(qreal cx, qreal cy, qreal ex, qreal ey)
{
    Q_D(QQuickPathItem);
    d->path.quadTo(cx, cy, ex, ey);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::addRect(qreal x, qreal y, qreal w, qreal h)
{
    Q_D(QQuickPathItem);
    d->path.addRect(x, y, w, h);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::addRoundedRect(qreal x, qreal y, qreal w, qreal h, qreal xr, qreal yr)
{
    Q_D(QQuickPathItem);
    d->path.addRoundedRect(x, y, w, h, xr, yr);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::addEllipse(qreal x, qreal y, qreal w, qreal h)
{
    Q_D(QQuickPathItem);
    d->path.addEllipse(x, y, w, h);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

void QQuickPathItem::addEllipseWithCenter(qreal cx, qreal cy, qreal rx, qreal ry)
{
    Q_D(QQuickPathItem);
    d->path.addEllipse(QPointF(cx, cy), rx, ry);
    d->dirty |= QQuickPathItemPrivate::DirtyPath;
}

QPointF QQuickPathItem::currentPosition() const
{
    Q_D(const QQuickPathItem);
    return d->path.currentPosition();
}

QRectF QQuickPathItem::boundingRect() const
{
    Q_D(const QQuickPathItem);
    return d->path.boundingRect();
}

QRectF QQuickPathItem::controlPointRect() const
{
    Q_D(const QQuickPathItem);
    return d->path.controlPointRect();
}

QQuickPathItem::FillRule QQuickPathItem::fillRule() const
{
    Q_D(const QQuickPathItem);
    return FillRule(d->path.fillRule());
}

void QQuickPathItem::setFillRule(FillRule fillRule)
{
    Q_D(QQuickPathItem);
    if (d->path.fillRule() != Qt::FillRule(fillRule)) {
        d->path.setFillRule(Qt::FillRule(fillRule));
        d->dirty |= QQuickPathItemPrivate::DirtyPath;
        emit fillRuleChanged();
        updatePath();
    }
}

QColor QQuickPathItem::fillColor() const
{
    Q_D(const QQuickPathItem);
    return d->fillColor;
}

void QQuickPathItem::setFillColor(const QColor &color)
{
    Q_D(QQuickPathItem);
    if (d->fillColor != color) {
        d->fillColor = color;
        d->dirty |= QQuickPathItemPrivate::DirtyFillColor;
        emit fillColorChanged();
        updatePath();
    }
}

QQuickPathGradient *QQuickPathItem::fillGradient() const
{
    Q_D(const QQuickPathItem);
    return d->fillGradient;
}

void QQuickPathItem::setFillGradient(QQuickPathGradient *gradient)
{
    Q_D(QQuickPathItem);
    if (d->fillGradient != gradient) {
        if (d->fillGradient)
            disconnect(d->fillGradient, &QQuickPathGradient::updated, this, &QQuickPathItem::updatePath);
        d->fillGradient = gradient;
        if (d->fillGradient)
            connect(d->fillGradient, &QQuickPathGradient::updated, this, &QQuickPathItem::updatePath);
        d->dirty |= QQuickPathItemPrivate::DirtyFillColor;
        updatePath();
    }
}

void QQuickPathItem::resetFillGradient()
{
    setFillGradient(nullptr);
}

QColor QQuickPathItem::strokeColor() const
{
    Q_D(const QQuickPathItem);
    return d->strokeColor;
}

void QQuickPathItem::setStrokeColor(const QColor &color)
{
    Q_D(QQuickPathItem);
    if (d->strokeColor != color) {
        d->strokeColor = color;
        d->dirty |= QQuickPathItemPrivate::DirtyStrokeColor;
        emit strokeColorChanged();
        updatePath();
    }
}

qreal QQuickPathItem::strokeWidth() const
{
    Q_D(const QQuickPathItem);
    return d->strokeWidth;
}

void QQuickPathItem::setStrokeWidth(qreal w)
{
    Q_D(QQuickPathItem);
    if (d->strokeWidth != w) {
        d->strokeWidth = w;
        d->dirty |= QQuickPathItemPrivate::DirtyStrokeWidth;
        emit strokeWidthChanged();
        updatePath();
    }
}

QQuickPathItem::JoinStyle QQuickPathItem::joinStyle() const
{
    Q_D(const QQuickPathItem);
    return d->joinStyle;
}

void QQuickPathItem::setJoinStyle(JoinStyle style)
{
    Q_D(QQuickPathItem);
    if (d->joinStyle != style) {
        d->joinStyle = style;
        d->dirty |= QQuickPathItemPrivate::DirtyStyle;
        emit joinStyleChanged();
        updatePath();
    }
}

int QQuickPathItem::miterLimit() const
{
    Q_D(const QQuickPathItem);
    return d->miterLimit;
}

void QQuickPathItem::setMiterLimit(int limit)
{
    Q_D(QQuickPathItem);
    if (d->miterLimit != limit) {
        d->miterLimit = limit;
        d->dirty |= QQuickPathItemPrivate::DirtyStyle;
        emit miterLimitChanged();
        updatePath();
    }
}

QQuickPathItem::CapStyle QQuickPathItem::capStyle() const
{
    Q_D(const QQuickPathItem);
    return d->capStyle;
}

void QQuickPathItem::setCapStyle(CapStyle style)
{
    Q_D(QQuickPathItem);
    if (d->capStyle != style) {
        d->capStyle = style;
        d->dirty |= QQuickPathItemPrivate::DirtyStyle;
        emit capStyleChanged();
        updatePath();
    }
}

QQuickPathItem::StrokeStyle QQuickPathItem::strokeStyle() const
{
    Q_D(const QQuickPathItem);
    return d->strokeStyle;
}

void QQuickPathItem::setStrokeStyle(StrokeStyle style)
{
    Q_D(QQuickPathItem);
    if (d->strokeStyle != style) {
        d->strokeStyle = style;
        d->dirty |= QQuickPathItemPrivate::DirtyStyle;
        emit strokeStyleChanged();
        updatePath();
    }
}

qreal QQuickPathItem::dashOffset() const
{
    Q_D(const QQuickPathItem);
    return d->dashOffset;
}

void QQuickPathItem::setDashOffset(qreal offset)
{
    Q_D(QQuickPathItem);
    if (d->dashOffset != offset) {
        d->dashOffset = offset;
        d->dirty |= QQuickPathItemPrivate::DirtyStyle;
        emit dashOffsetChanged();
        updatePath();
    }
}

QVector<qreal> QQuickPathItem::dashPattern() const
{
    Q_D(const QQuickPathItem);
    return d->dashPattern;
}

void QQuickPathItem::setDashPattern(const QVector<qreal> &array)
{
    Q_D(QQuickPathItem);
    if (d->dashPattern != array) {
        d->dashPattern = array;
        d->dirty |= QQuickPathItemPrivate::DirtyStyle;
        emit dashPatternChanged();
        updatePath();
    }
}

bool QQuickPathItem::isCosmeticStroke() const
{
    Q_D(const QQuickPathItem);
    return d->cosmeticStroke;
}

void QQuickPathItem::setCosmeticStroke(bool cosmetic)
{
    Q_D(QQuickPathItem);
    if (d->cosmeticStroke != cosmetic) {
        d->cosmeticStroke = cosmetic;
        d->dirty |= QQuickPathItemPrivate::DirtyStyle;
        emit cosmeticStrokeChanged();
        updatePath();
    }
}

QQmlListProperty<QObject> QQuickPathItem::commands()
{
    return QQmlListProperty<QObject>(this, nullptr, &QQuickPathItemPrivate::appendCommand, nullptr, nullptr, nullptr);
}

QT_END_NAMESPACE
