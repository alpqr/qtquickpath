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

#include "qquickpathcommand_p.h"
#include "qquickpathitem_p_p.h"

QT_BEGIN_NAMESPACE

#define UPDATE_PATH_ITEM QQuickPathItemPrivate::get(static_cast<QQuickPathItem *>(parent()))->handlePathCommandChange

QQuickPathCommand::QQuickPathCommand(QObject *parent)
    : QObject(parent)
{
}

QQuickPathMoveTo::QQuickPathMoveTo(QObject *parent)
    : QQuickPathCommand(parent),
      m_x(0),
      m_y(0)
{
}

qreal QQuickPathMoveTo::x() const
{
    return m_x;
}

void QQuickPathMoveTo::setX(qreal v)
{
    if (m_x != v) {
        m_x = v;
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathMoveTo::y() const
{
    return m_y;
}

void QQuickPathMoveTo::setY(qreal v)
{
    if (m_y != v) {
        m_y = v;
        UPDATE_PATH_ITEM();
    }
}

void QQuickPathMoveTo::addToPath(QPainterPath *path)
{
    path->moveTo(m_x, m_y);
}

QQuickPathLineTo::QQuickPathLineTo(QObject *parent)
    : QQuickPathCommand(parent),
      m_x(0),
      m_y(0)
{
}

qreal QQuickPathLineTo::x() const
{
    return m_x;
}

void QQuickPathLineTo::setX(qreal v)
{
    if (m_x != v) {
        m_x = v;
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathLineTo::y() const
{
    return m_y;
}

void QQuickPathLineTo::setY(qreal v)
{
    if (m_y != v) {
        m_y = v;
        UPDATE_PATH_ITEM();
    }
}

void QQuickPathLineTo::addToPath(QPainterPath *path)
{
    path->lineTo(m_x, m_y);
}

QT_END_NAMESPACE
