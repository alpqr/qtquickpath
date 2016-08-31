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

#include "qquicklineitem_p.h"
#include "qquickpathitem_p_p.h"

QT_BEGIN_NAMESPACE

QQuickLineItem::QQuickLineItem(QQuickItem *parent)
    : QQuickItem(parent),
      pd(new QQuickPathItemPrivate),
      m_x1(0),
      m_y1(0),
      m_x2(0),
      m_y2(0)
{
    setFlag(ItemHasContents);
    pd->flags |= QQuickAbstractPathRenderer::RenderNoFill;
}

QQuickLineItem::~QQuickLineItem()
{
    delete pd;
}

QSGNode *QQuickLineItem::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    return pd->updatePaintNode(this, node);
}

qreal QQuickLineItem::x1() const
{
    return m_x1;
}

void QQuickLineItem::setX1(qreal v)
{
    if (m_x1 != v) {
        m_x1 = v;
        emit x1Changed();
        regenerate();
    }
}

qreal QQuickLineItem::y1() const
{
    return m_y1;
}

void QQuickLineItem::setY1(qreal v)
{
    if (m_y1 != v) {
        m_y1 = v;
        emit y1Changed();
        regenerate();
    }
}

qreal QQuickLineItem::x2() const
{
    return m_x2;
}

void QQuickLineItem::setX2(qreal v)
{
    if (m_x2 != v) {
        m_x2 = v;
        emit x2Changed();
        regenerate();
    }
}

qreal QQuickLineItem::y2() const
{
    return m_y2;
}

void QQuickLineItem::setY2(qreal v)
{
    if (m_y2 != v) {
        m_y2 = v;
        emit y2Changed();
        regenerate();
    }
}

QColor QQuickLineItem::color() const
{
    return pd->strokeColor;
}

void QQuickLineItem::setColor(const QColor &color)
{
    if (pd->strokeColor != color) {
        pd->strokeColor = color;
        pd->dirty |= QQuickPathItemPrivate::DirtyStrokeColor;
        emit colorChanged();
        update();
    }
}

qreal QQuickLineItem::lineWidth() const
{
    return pd->strokeWidth;
}

void QQuickLineItem::setLineWidth(qreal w)
{
    if (pd->strokeWidth != w) {
        pd->strokeWidth = w;
        pd->dirty |= QQuickPathItemPrivate::DirtyStrokeWidth;
        emit lineWidthChanged();
        update();
    }
}

void QQuickLineItem::regenerate()
{
    pd->path = QPainterPath();
    pd->path.moveTo(m_x1, m_y1);
    pd->path.lineTo(m_x2, m_y2);
    pd->dirty |= QQuickPathItemPrivate::DirtyPath;
    update();
}

QT_END_NAMESPACE
