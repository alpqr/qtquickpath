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

#include "qquickpathgradient_p.h"

QT_BEGIN_NAMESPACE

QQuickPathGradientStop::QQuickPathGradientStop(QObject *parent)
    : QObject(parent),
      m_position(0),
      m_color(Qt::black)
{
}

qreal QQuickPathGradientStop::position() const
{
    return m_position;
}

void QQuickPathGradientStop::setPosition(qreal position)
{
    if (m_position != position) {
        m_position = position;
        if (QQuickPathGradient *grad = qobject_cast<QQuickPathGradient *>(parent()))
            emit grad->updated();
    }
}

QColor QQuickPathGradientStop::color() const
{
    return m_color;
}

void QQuickPathGradientStop::setColor(const QColor &color)
{
    if (m_color != color) {
        m_color = color;
        if (QQuickPathGradient *grad = qobject_cast<QQuickPathGradient *>(parent()))
            emit grad->updated();
    }
}

QQuickPathGradient::QQuickPathGradient(QObject *parent)
    : QObject(parent)
{
}

void QQuickPathGradient::appendStop(QQmlListProperty<QObject> *list, QObject *stop)
{
    QQuickPathGradientStop *sstop = qobject_cast<QQuickPathGradientStop *>(stop);
    if (!sstop) {
        qWarning("Gradient stop list only supports QQuickPathGradientStop elements");
        return;
    }
    QQuickPathGradient *grad = qobject_cast<QQuickPathGradient *>(list->object);
    Q_ASSERT(grad);
    sstop->setParent(grad);
    grad->m_stops.append(sstop);
}

QQmlListProperty<QObject> QQuickPathGradient::stops()
{
    return QQmlListProperty<QObject>(this, nullptr, &QQuickPathGradient::appendStop, nullptr, nullptr, nullptr);
}

QT_END_NAMESPACE
