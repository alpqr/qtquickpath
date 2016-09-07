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

#ifndef QQUICKLINEITEM_P_H
#define QQUICKLINEITEM_P_H

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

QT_BEGIN_NAMESPACE

class QQuickPathItemPrivate;

class QQUICKPATH_EXPORT QQuickLineItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal x1 READ x1 WRITE setX1 NOTIFY x1Changed)
    Q_PROPERTY(qreal y1 READ y1 WRITE setY1 NOTIFY y1Changed)
    Q_PROPERTY(qreal x2 READ x2 WRITE setX2 NOTIFY x2Changed)
    Q_PROPERTY(qreal y2 READ y2 WRITE setY2 NOTIFY y2Changed)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)

public:
    QQuickLineItem(QQuickItem *parent = nullptr);
    ~QQuickLineItem();

    qreal x1() const;
    void setX1(qreal v);
    qreal y1() const;
    void setY1(qreal v);
    qreal x2() const;
    void setX2(qreal v);
    qreal y2() const;
    void setY2(qreal v);

    QColor color() const;
    void setColor(const QColor &color);

    qreal lineWidth() const;
    void setLineWidth(qreal w);

protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

signals:
    void x1Changed();
    void y1Changed();
    void x2Changed();
    void y2Changed();
    void colorChanged();
    void lineWidthChanged();

private:
    Q_DISABLE_COPY(QQuickLineItem)
    Q_DECLARE_PRIVATE(QQuickPathItem)

    void regenerate();

    qreal m_x1, m_y1, m_x2, m_y2;
};

QT_END_NAMESPACE

#endif
