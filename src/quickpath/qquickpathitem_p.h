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

#ifndef QQUICKPATHITEM_P_H
#define QQUICKPATHITEM_P_H

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

class QQUICKPATH_EXPORT QQuickPathItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(FillRule fillRule READ fillRule WRITE setFillRule NOTIFY fillRuleChanged)
    Q_PROPERTY(QColor strokeColor READ strokeColor WRITE setStrokeColor NOTIFY strokeColorChanged)
    Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY strokeWidthChanged)
    Q_PROPERTY(JoinStyle joinStyle READ joinStyle WRITE setJoinStyle NOTIFY joinStyleChanged)
    Q_PROPERTY(int miterLimit READ miterLimit WRITE setMiterLimit NOTIFY miterLimitChanged)
    Q_PROPERTY(CapStyle capStyle READ capStyle WRITE setCapStyle NOTIFY capStyleChanged)
    Q_PROPERTY(StrokeStyle strokeStyle READ strokeStyle WRITE setStrokeStyle NOTIFY strokeStyleChanged)

public:
    enum FillRule {
        OddEvenFill = Qt::OddEvenFill,
        WindingFill = Qt::WindingFill
    };
    Q_ENUM(FillRule)

    enum JoinStyle {
        MiterJoin = Qt::MiterJoin,
        BevelJoin = Qt::BevelJoin,
        RoundJoin = Qt::RoundJoin
    };
    Q_ENUM(JoinStyle)

    enum CapStyle {
        FlatCap = Qt::FlatCap,
        SquareCap = Qt::SquareCap,
        RoundCap = Qt::RoundCap
    };
    Q_ENUM(CapStyle)

    enum StrokeStyle {
        SolidLine = Qt::SolidLine,
        DashLine = Qt::DashLine,
        DotLine = Qt::DotLine
    };
    Q_ENUM(StrokeStyle)

    QQuickPathItem(QQuickItem *parent = nullptr);
    ~QQuickPathItem();

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool isEmpty() const;
    Q_INVOKABLE void closeSubPath();

    Q_INVOKABLE void moveTo(qreal x, qreal y);
    Q_INVOKABLE void lineTo(qreal x, qreal y);
    Q_INVOKABLE void arcMoveTo(qreal x, qreal y, qreal w, qreal h, qreal angle);
    Q_INVOKABLE void arcTo(qreal x, qreal y, qreal w, qreal h, qreal startAngle, qreal arcLength);
    Q_INVOKABLE void cubicTo(qreal c1x, qreal c1y, qreal c2x, qreal c2y, qreal endX, qreal endY);
    Q_INVOKABLE void quadTo(qreal cX, qreal cY, qreal endX, qreal endY);

    Q_INVOKABLE void addRect(qreal x, qreal y, qreal w, qreal h);
    Q_INVOKABLE void addRoundedRect(qreal x, qreal y, qreal w, qreal h, qreal xr, qreal yr);
    Q_INVOKABLE void addEllipse(qreal x, qreal y, qreal rx, qreal ry);
    Q_INVOKABLE void addEllipseWithCenter(qreal cx, qreal cy, qreal rx, qreal ry);

    Q_INVOKABLE QPointF currentPosition() const;
    Q_INVOKABLE QRectF boundingRect() const;
    Q_INVOKABLE QRectF controlPointRect() const;

    QColor fillColor() const;
    void setFillColor(const QColor &color);

    FillRule fillRule() const;
    void setFillRule(FillRule fillRule);

    QColor strokeColor() const;
    void setStrokeColor(const QColor &color);

    qreal strokeWidth() const;
    void setStrokeWidth(qreal w);

    JoinStyle joinStyle() const;
    void setJoinStyle(JoinStyle style);

    int miterLimit() const;
    void setMiterLimit(int limit);

    CapStyle capStyle() const;
    void setCapStyle(CapStyle style);

    StrokeStyle strokeStyle() const;
    void setStrokeStyle(StrokeStyle style);

protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

signals:
    void fillColorChanged();
    void fillRuleChanged();
    void strokeColorChanged();
    void strokeWidthChanged();
    void joinStyleChanged();
    void miterLimitChanged();
    void capStyleChanged();
    void strokeStyleChanged();

private:
    QQuickPathItemPrivate *d;
};

QT_END_NAMESPACE

#endif
