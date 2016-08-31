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

#include "qquickellipseitem_p.h"
#include "qquickpathitem_p_p.h"

QT_BEGIN_NAMESPACE

QQuickEllipseItem::QQuickEllipseItem(QQuickItem *parent)
    : QQuickItem(parent),
      pd(new QQuickPathItemPrivate),
      m_centerX(0),
      m_centerY(0),
      m_radiusX(0),
      m_radiusY(0)
{
    setFlag(ItemHasContents);
}

QQuickEllipseItem::~QQuickEllipseItem()
{
    delete pd;
}

QSGNode *QQuickEllipseItem::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    return pd->updatePaintNode(this, node);
}

qreal QQuickEllipseItem::centerX() const
{
    return m_centerX;
}

void QQuickEllipseItem::setCenterX(qreal v)
{
    if (m_centerX != v) {
        m_centerX = v;
        emit centerXChanged();
        regenerate();
    }
}

qreal QQuickEllipseItem::centerY() const
{
    return m_centerY;
}

void QQuickEllipseItem::setCenterY(qreal v)
{
    if (m_centerY != v) {
        m_centerY = v;
        emit centerYChanged();
        regenerate();
    }
}

qreal QQuickEllipseItem::radiusX() const
{
    return m_radiusX;
}

void QQuickEllipseItem::setRadiusX(qreal v)
{
    if (m_radiusX != v) {
        m_radiusX = v;
        emit radiusXChanged();
        regenerate();
    }
}

qreal QQuickEllipseItem::radiusY() const
{
    return m_radiusY;
}

void QQuickEllipseItem::setRadiusY(qreal v)
{
    if (m_radiusY != v) {
        m_radiusY = v;
        emit radiusYChanged();
        regenerate();
    }
}

QColor QQuickEllipseItem::fillColor() const
{
    return pd->fillColor;
}

void QQuickEllipseItem::setFillColor(const QColor &color)
{
    if (pd->fillColor != color) {
        pd->fillColor = color;
        pd->dirty |= QQuickPathItemPrivate::DirtyFillColor;
        emit fillColorChanged();
        update();
    }
}

QColor QQuickEllipseItem::borderColor() const
{
    return pd->strokeColor;
}

void QQuickEllipseItem::setBorderColor(const QColor &color)
{
    if (pd->strokeColor != color) {
        pd->strokeColor = color;
        pd->dirty |= QQuickPathItemPrivate::DirtyStrokeColor;
        emit borderColorChanged();
        update();
    }
}

qreal QQuickEllipseItem::borderWidth() const
{
    return pd->strokeWidth;
}

void QQuickEllipseItem::setBorderWidth(qreal w)
{
    if (pd->strokeWidth != w) {
        pd->strokeWidth = w;
        pd->dirty |= QQuickPathItemPrivate::DirtyStrokeWidth;
        emit borderWidthChanged();
        update();
    }
}

void QQuickEllipseItem::regenerate()
{
    pd->path = QPainterPath();
    pd->path.addEllipse(QPointF(m_centerX, m_centerY), m_radiusX, m_radiusY);
    pd->dirty |= QQuickPathItemPrivate::DirtyPath;
    update();
}

QT_END_NAMESPACE
