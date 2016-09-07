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
        emit xChanged();
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
        emit yChanged();
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
        emit xChanged();
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
        emit yChanged();
        UPDATE_PATH_ITEM();
    }
}

void QQuickPathLineTo::addToPath(QPainterPath *path)
{
    path->lineTo(m_x, m_y);
}

QQuickPathEllipse::QQuickPathEllipse(QObject *parent)
    : QQuickPathCommand(parent),
      m_centerX(0),
      m_centerY(0),
      m_radiusX(0),
      m_radiusY(0)
{
}

qreal QQuickPathEllipse::centerX() const
{
    return m_centerX;
}

void QQuickPathEllipse::setCenterX(qreal v)
{
    if (m_centerX != v) {
        m_centerX = v;
        emit centerXChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathEllipse::centerY() const
{
    return m_centerY;
}

void QQuickPathEllipse::setCenterY(qreal v)
{
    if (m_centerY != v) {
        m_centerY = v;
        emit centerYChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathEllipse::radiusX() const
{
    return m_radiusX;
}

void QQuickPathEllipse::setRadiusX(qreal v)
{
    if (m_radiusX != v) {
        m_radiusX = v;
        emit radiusXChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathEllipse::radiusY() const
{
    return m_radiusY;
}

void QQuickPathEllipse::setRadiusY(qreal v)
{
    if (m_radiusY != v) {
        m_radiusY = v;
        emit radiusYChanged();
        UPDATE_PATH_ITEM();
    }
}

void QQuickPathEllipse::addToPath(QPainterPath *path)
{
    path->addEllipse(QPointF(m_centerX, m_centerY), m_radiusX, m_radiusY);
}

QT_END_NAMESPACE
