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

#ifndef QQUICKPATHGRADIENT_P_H
#define QQUICKPATHGRADIENT_P_H

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

#include <QtQuickPath/qtquickpathglobal.h>
#include <QQuickItem>
#include <QBrush>

QT_BEGIN_NAMESPACE

class QQUICKPATH_EXPORT QQuickPathGradientStop : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal position READ position WRITE setPosition)
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    QQuickPathGradientStop(QObject *parent = nullptr);

    qreal position() const;
    void setPosition(qreal position);

    QColor color() const;
    void setColor(const QColor &color);

private:
    qreal m_position;
    QColor m_color;
};

class QQUICKPATH_EXPORT QQuickPathGradient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QObject> stops READ stops)
    Q_PROPERTY(qreal x1 READ x1 WRITE setX1 NOTIFY x1Changed)
    Q_PROPERTY(qreal y1 READ y1 WRITE setY1 NOTIFY y1Changed)
    Q_PROPERTY(qreal x2 READ x2 WRITE setX2 NOTIFY x2Changed)
    Q_PROPERTY(qreal y2 READ y2 WRITE setY2 NOTIFY y2Changed)
    Q_PROPERTY(SpreadMode spread READ spread WRITE setSpread NOTIFY spreadChanged)
    Q_CLASSINFO("DefaultProperty", "stops")

public:
    enum SpreadMode {
        PadSpread,
        RepeatSpread
    };
    Q_ENUM(SpreadMode)

    QQuickPathGradient(QObject *parent = nullptr);

    QQmlListProperty<QObject> stops();

    QGradientStops sortedGradientStops() const;

    qreal x1() const;
    void setX1(qreal v);
    qreal y1() const;
    void setY1(qreal v);
    qreal x2() const;
    void setX2(qreal v);
    qreal y2() const;
    void setY2(qreal v);

    SpreadMode spread() const;
    void setSpread(SpreadMode mode);

signals:
    void updated();
    void x1Changed();
    void y1Changed();
    void x2Changed();
    void y2Changed();
    void spreadChanged();

private:
    static void appendStop(QQmlListProperty<QObject> *list, QObject *stop);

    QVector<QObject *> m_stops;
    QPointF m_start;
    QPointF m_end;
    SpreadMode m_spread;
};

QT_END_NAMESPACE

#endif
