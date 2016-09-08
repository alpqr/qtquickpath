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

QQuickPathArcMoveTo::QQuickPathArcMoveTo(QObject *parent)
    : QQuickPathCommand(parent),
      m_x(0),
      m_y(0),
      m_width(0),
      m_height(0),
      m_angle(0)
{
}

qreal QQuickPathArcMoveTo::x() const
{
    return m_x;
}

void QQuickPathArcMoveTo::setX(qreal v)
{
    if (m_x != v) {
        m_x = v;
        emit xChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathArcMoveTo::y() const
{
    return m_y;
}

void QQuickPathArcMoveTo::setY(qreal v)
{
    if (m_y != v) {
        m_y = v;
        emit yChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathArcMoveTo::width() const
{
    return m_width;
}

void QQuickPathArcMoveTo::setWidth(qreal w)
{
    if (m_width != w) {
        m_width = w;
        emit widthChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathArcMoveTo::height() const
{
    return m_height;
}

void QQuickPathArcMoveTo::setHeight(qreal h)
{
    if (m_height != h) {
        m_height = h;
        emit heightChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathArcMoveTo::angle() const
{
    return m_angle;
}

void QQuickPathArcMoveTo::setAngle(qreal v)
{
    if (m_angle != v) {
        m_angle = v;
        emit angleChanged();
        UPDATE_PATH_ITEM();
    }
}

void QQuickPathArcMoveTo::addToPath(QPainterPath *path)
{
    path->arcMoveTo(m_x, m_y, m_width, m_height, m_angle);
}

QQuickPathArcTo::QQuickPathArcTo(QObject *parent)
    : QQuickPathCommand(parent),
      m_x(0),
      m_y(0),
      m_width(0),
      m_height(0),
      m_startAngle(0),
      m_arcLength(0)
{
}

qreal QQuickPathArcTo::x() const
{
    return m_x;
}

void QQuickPathArcTo::setX(qreal v)
{
    if (m_x != v) {
        m_x = v;
        emit xChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathArcTo::y() const
{
    return m_y;
}

void QQuickPathArcTo::setY(qreal v)
{
    if (m_y != v) {
        m_y = v;
        emit yChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathArcTo::width() const
{
    return m_width;
}

void QQuickPathArcTo::setWidth(qreal w)
{
    if (m_width != w) {
        m_width = w;
        emit widthChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathArcTo::height() const
{
    return m_height;
}

void QQuickPathArcTo::setHeight(qreal h)
{
    if (m_height != h) {
        m_height = h;
        emit heightChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathArcTo::startAngle() const
{
    return m_startAngle;
}

void QQuickPathArcTo::setStartAngle(qreal v)
{
    if (m_startAngle != v) {
        m_startAngle = v;
        emit startAngleChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathArcTo::arcLength() const
{
    return m_arcLength;
}

void QQuickPathArcTo::setArcLength(qreal v)
{
    if (m_arcLength != v) {
        m_arcLength = v;
        emit arcLengthChanged();
        UPDATE_PATH_ITEM();
    }
}

void QQuickPathArcTo::addToPath(QPainterPath *path)
{
    path->arcTo(m_x, m_y, m_width, m_height, m_startAngle, m_arcLength);
}

QQuickPathCubicTo::QQuickPathCubicTo(QObject *parent)
    : QQuickPathCommand(parent),
      m_cx1(0),
      m_cy1(0),
      m_cx2(0),
      m_cy2(0),
      m_ex(0),
      m_ey(0)
{
}

qreal QQuickPathCubicTo::cx1() const
{
    return m_cx1;
}

void QQuickPathCubicTo::setCx1(qreal v)
{
    if (m_cx1 != v) {
        m_cx1 = v;
        emit cx1Changed();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathCubicTo::cy1() const
{
    return m_cy1;
}

void QQuickPathCubicTo::setCy1(qreal v)
{
    if (m_cy1 != v) {
        m_cy1 = v;
        emit cy1Changed();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathCubicTo::cx2() const
{
    return m_cx2;
}

void QQuickPathCubicTo::setCx2(qreal v)
{
    if (m_cx2 != v) {
        m_cx2 = v;
        emit cx2Changed();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathCubicTo::cy2() const
{
    return m_cy2;
}

void QQuickPathCubicTo::setCy2(qreal v)
{
    if (m_cy2 != v) {
        m_cy2 = v;
        emit cy2Changed();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathCubicTo::ex() const
{
    return m_ex;
}

void QQuickPathCubicTo::setEx(qreal v)
{
    if (m_ex != v) {
        m_ex = v;
        emit eXChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathCubicTo::ey() const
{
    return m_ey;
}

void QQuickPathCubicTo::setEy(qreal v)
{
    if (m_ey != v) {
        m_ey = v;
        emit eYChanged();
        UPDATE_PATH_ITEM();
    }
}

void QQuickPathCubicTo::addToPath(QPainterPath *path)
{
    path->cubicTo(m_cx1, m_cy1, m_cx2, m_cy2, m_ex, m_ey);
}

QQuickPathQuadTo::QQuickPathQuadTo(QObject *parent)
    : QQuickPathCommand(parent),
      m_cx(0),
      m_cy(0),
      m_ex(0),
      m_ey(0)
{
}

qreal QQuickPathQuadTo::cx() const
{
    return m_cx;
}

void QQuickPathQuadTo::setCx(qreal v)
{
    if (m_cx != v) {
        m_cx = v;
        emit cXChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathQuadTo::cy() const
{
    return m_cy;
}

void QQuickPathQuadTo::setCy(qreal v)
{
    if (m_cy != v) {
        m_cy = v;
        emit cYChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathQuadTo::ex() const
{
    return m_ex;
}

void QQuickPathQuadTo::setEx(qreal v)
{
    if (m_ex != v) {
        m_ex = v;
        emit eXChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathQuadTo::ey() const
{
    return m_ey;
}

void QQuickPathQuadTo::setEy(qreal v)
{
    if (m_ey != v) {
        m_ey = v;
        emit eYChanged();
        UPDATE_PATH_ITEM();
    }
}

void QQuickPathQuadTo::addToPath(QPainterPath *path)
{
    path->quadTo(m_cx, m_cy, m_ex, m_ey);
}

QQuickPathClose::QQuickPathClose(QObject *parent)
    : QQuickPathCommand(parent)
{
}

void QQuickPathClose::addToPath(QPainterPath *path)
{
    path->closeSubpath();
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

QQuickPathRectangle::QQuickPathRectangle(QObject *parent)
    : QQuickPathCommand(parent),
      m_x(0),
      m_y(0),
      m_width(0),
      m_height(0)
{
}

qreal QQuickPathRectangle::x() const
{
    return m_x;
}

void QQuickPathRectangle::setX(qreal v)
{
    if (m_x != v) {
        m_x = v;
        emit xChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathRectangle::y() const
{
    return m_y;
}

void QQuickPathRectangle::setY(qreal v)
{
    if (m_y != v) {
        m_y = v;
        emit yChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathRectangle::width() const
{
    return m_width;
}

void QQuickPathRectangle::setWidth(qreal w)
{
    if (m_width != w) {
        m_width = w;
        emit widthChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathRectangle::height() const
{
    return m_height;
}

void QQuickPathRectangle::setHeight(qreal h)
{
    if (m_height != h) {
        m_height = h;
        emit heightChanged();
        UPDATE_PATH_ITEM();
    }
}

void QQuickPathRectangle::addToPath(QPainterPath *path)
{
    path->addRect(m_x, m_y, m_width, m_height);
}

QQuickPathRoundedRectangle::QQuickPathRoundedRectangle(QObject *parent)
    : QQuickPathRectangle(parent),
      m_radiusX(0),
      m_radiusY(0)
{
}

qreal QQuickPathRoundedRectangle::radiusX() const
{
    return m_radiusX;
}

void QQuickPathRoundedRectangle::setRadiusX(qreal v)
{
    if (m_radiusX != v) {
        m_radiusX = v;
        emit radiusXChanged();
        UPDATE_PATH_ITEM();
    }
}

qreal QQuickPathRoundedRectangle::radiusY() const
{
    return m_radiusY;
}

void QQuickPathRoundedRectangle::setRadiusY(qreal v)
{
    if (m_radiusY != v) {
        m_radiusY = v;
        emit radiusYChanged();
        UPDATE_PATH_ITEM();
    }
}

void QQuickPathRoundedRectangle::addToPath(QPainterPath *path)
{
    path->addRoundedRect(x(), y(), width(), height(), m_radiusX, m_radiusY);
}

QT_END_NAMESPACE
