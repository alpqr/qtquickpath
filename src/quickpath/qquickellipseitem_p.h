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

#ifndef QQUICKELLIPSEITEM_P_H
#define QQUICKELLIPSEITEM_P_H

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

class QQUICKPATH_EXPORT QQuickEllipseItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal centerX READ centerX WRITE setCenterX NOTIFY centerXChanged)
    Q_PROPERTY(qreal centerY READ centerY WRITE setCenterY NOTIFY centerYChanged)
    Q_PROPERTY(qreal radiusX READ radiusX WRITE setRadiusX NOTIFY radiusXChanged)
    Q_PROPERTY(qreal radiusY READ radiusY WRITE setRadiusY NOTIFY radiusYChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)

public:
    QQuickEllipseItem(QQuickItem *parent = nullptr);
    ~QQuickEllipseItem();

    qreal centerX() const;
    void setCenterX(qreal v);
    qreal centerY() const;
    void setCenterY(qreal v);

    qreal radiusX() const;
    void setRadiusX(qreal v);
    qreal radiusY() const;
    void setRadiusY(qreal v);

    QColor fillColor() const;
    void setFillColor(const QColor &color);

    QColor borderColor() const;
    void setBorderColor(const QColor &color);

    qreal borderWidth() const;
    void setBorderWidth(qreal w);

protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

signals:
    void centerXChanged();
    void centerYChanged();
    void radiusXChanged();
    void radiusYChanged();
    void fillColorChanged();
    void borderColorChanged();
    void borderWidthChanged();

private:
    void regenerate();

    QQuickPathItemPrivate *pd;
    qreal m_centerX;
    qreal m_centerY;
    qreal m_radiusX;
    qreal m_radiusY;
};

QT_END_NAMESPACE

#endif
