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

#ifndef QQUICKPATHCOMMAND_P_H
#define QQUICKPATHCOMMAND_P_H

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
#include <QPainterPath>

QT_BEGIN_NAMESPACE

class QQUICKPATH_EXPORT QQuickPathCommand : public QObject
{
    Q_OBJECT

public:
    QQuickPathCommand(QObject *parent = nullptr);

    virtual void addToPath(QPainterPath *path) = 0;
};

class QQUICKPATH_EXPORT QQuickPathMoveTo : public QQuickPathCommand
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)

public:
    QQuickPathMoveTo(QObject *parent = nullptr);

    qreal x() const;
    void setX(qreal v);
    qreal y() const;
    void setY(qreal v);

    void addToPath(QPainterPath *path) override;

signals:
    void xChanged();
    void yChanged();

private:
    qreal m_x;
    qreal m_y;
};

class QQUICKPATH_EXPORT QQuickPathLineTo : public QQuickPathCommand
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)

public:
    QQuickPathLineTo(QObject *parent = nullptr);

    qreal x() const;
    void setX(qreal v);
    qreal y() const;
    void setY(qreal v);

    void addToPath(QPainterPath *path) override;

signals:
    void xChanged();
    void yChanged();

private:
    qreal m_x;
    qreal m_y;
};

class QQUICKPATH_EXPORT QQuickPathArcMoveTo : public QQuickPathCommand
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)

public:
    QQuickPathArcMoveTo(QObject *parent = nullptr);

    qreal x() const;
    void setX(qreal v);
    qreal y() const;
    void setY(qreal v);

    qreal width() const;
    void setWidth(qreal w);
    qreal height() const;
    void setHeight(qreal h);

    qreal angle() const;
    void setAngle(qreal v);

    void addToPath(QPainterPath *path) override;

signals:
    void xChanged();
    void yChanged();
    void widthChanged();
    void heightChanged();
    void angleChanged();

private:
    qreal m_x;
    qreal m_y;
    qreal m_width;
    qreal m_height;
    qreal m_angle;
};

class QQUICKPATH_EXPORT QQuickPathArcTo : public QQuickPathCommand
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(qreal arcLength READ arcLength WRITE setArcLength NOTIFY arcLengthChanged)

public:
    QQuickPathArcTo(QObject *parent = nullptr);

    qreal x() const;
    void setX(qreal v);
    qreal y() const;
    void setY(qreal v);

    qreal width() const;
    void setWidth(qreal w);
    qreal height() const;
    void setHeight(qreal h);

    qreal startAngle() const;
    void setStartAngle(qreal v);

    qreal arcLength() const;
    void setArcLength(qreal length);

    void addToPath(QPainterPath *path) override;

signals:
    void xChanged();
    void yChanged();
    void widthChanged();
    void heightChanged();
    void startAngleChanged();
    void arcLengthChanged();

private:
    qreal m_x;
    qreal m_y;
    qreal m_width;
    qreal m_height;
    qreal m_startAngle;
    qreal m_arcLength;
};

class QQUICKPATH_EXPORT QQuickPathCubicTo : public QQuickPathCommand
{
    Q_OBJECT
    Q_PROPERTY(qreal cx1 READ cx1 WRITE setCx1 NOTIFY cx1Changed)
    Q_PROPERTY(qreal cy1 READ cy1 WRITE setCy1 NOTIFY cy1Changed)
    Q_PROPERTY(qreal cx2 READ cx2 WRITE setCx2 NOTIFY cx2Changed)
    Q_PROPERTY(qreal cy2 READ cy2 WRITE setCy2 NOTIFY cy2Changed)
    Q_PROPERTY(qreal ex READ ex WRITE setEx NOTIFY eXChanged)
    Q_PROPERTY(qreal ey READ ey WRITE setEy NOTIFY eYChanged)

public:
    QQuickPathCubicTo(QObject *parent = nullptr);

    qreal cx1() const;
    void setCx1(qreal v);
    qreal cy1() const;
    void setCy1(qreal v);
    qreal cx2() const;
    void setCx2(qreal v);
    qreal cy2() const;
    void setCy2(qreal v);
    qreal ex() const;
    void setEx(qreal w);
    qreal ey() const;
    void setEy(qreal h);

    void addToPath(QPainterPath *path) override;

signals:
    void cx1Changed();
    void cy1Changed();
    void cx2Changed();
    void cy2Changed();
    void eXChanged();
    void eYChanged();

private:
    qreal m_cx1;
    qreal m_cy1;
    qreal m_cx2;
    qreal m_cy2;
    qreal m_ex;
    qreal m_ey;
};

class QQUICKPATH_EXPORT QQuickPathQuadTo : public QQuickPathCommand
{
    Q_OBJECT
    Q_PROPERTY(qreal cx READ cx WRITE setCx NOTIFY cXChanged)
    Q_PROPERTY(qreal cy READ cy WRITE setCy NOTIFY cYChanged)
    Q_PROPERTY(qreal ex READ ex WRITE setEx NOTIFY eXChanged)
    Q_PROPERTY(qreal ey READ ey WRITE setEy NOTIFY eYChanged)

public:
    QQuickPathQuadTo(QObject *parent = nullptr);

    qreal cx() const;
    void setCx(qreal v);
    qreal cy() const;
    void setCy(qreal v);
    qreal ex() const;
    void setEx(qreal w);
    qreal ey() const;
    void setEy(qreal h);

    void addToPath(QPainterPath *path) override;

signals:
    void cXChanged();
    void cYChanged();
    void eXChanged();
    void eYChanged();

private:
    qreal m_cx;
    qreal m_cy;
    qreal m_ex;
    qreal m_ey;
};

class QQUICKPATH_EXPORT QQuickPathClose : public QQuickPathCommand
{
    Q_OBJECT

public:
    QQuickPathClose(QObject *parent = nullptr);

    void addToPath(QPainterPath *path) override;
};

class QQUICKPATH_EXPORT QQuickPathEllipse : public QQuickPathCommand
{
    Q_OBJECT
    Q_PROPERTY(qreal centerX READ centerX WRITE setCenterX NOTIFY centerXChanged)
    Q_PROPERTY(qreal centerY READ centerY WRITE setCenterY NOTIFY centerYChanged)
    Q_PROPERTY(qreal radiusX READ radiusX WRITE setRadiusX NOTIFY radiusXChanged)
    Q_PROPERTY(qreal radiusY READ radiusY WRITE setRadiusY NOTIFY radiusYChanged)

public:
    QQuickPathEllipse(QObject *parent = nullptr);

    qreal centerX() const;
    void setCenterX(qreal v);
    qreal centerY() const;
    void setCenterY(qreal v);

    qreal radiusX() const;
    void setRadiusX(qreal v);
    qreal radiusY() const;
    void setRadiusY(qreal v);

    void addToPath(QPainterPath *path) override;

signals:
    void centerXChanged();
    void centerYChanged();
    void radiusXChanged();
    void radiusYChanged();

private:
    qreal m_centerX;
    qreal m_centerY;
    qreal m_radiusX;
    qreal m_radiusY;
};

class QQUICKPATH_EXPORT QQuickPathRectangle : public QQuickPathCommand
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)

public:
    QQuickPathRectangle(QObject *parent = nullptr);

    qreal x() const;
    void setX(qreal v);
    qreal y() const;
    void setY(qreal v);

    qreal width() const;
    void setWidth(qreal w);
    qreal height() const;
    void setHeight(qreal h);

    void addToPath(QPainterPath *path) override;

signals:
    void xChanged();
    void yChanged();
    void widthChanged();
    void heightChanged();

private:
    qreal m_x;
    qreal m_y;
    qreal m_width;
    qreal m_height;
};

class QQUICKPATH_EXPORT QQuickPathRoundedRectangle : public QQuickPathRectangle
{
    Q_OBJECT
    Q_PROPERTY(qreal radiusX READ radiusX WRITE setRadiusX NOTIFY radiusXChanged)
    Q_PROPERTY(qreal radiusY READ radiusY WRITE setRadiusY NOTIFY radiusYChanged)

public:
    QQuickPathRoundedRectangle(QObject *parent = nullptr);

    qreal radiusX() const;
    void setRadiusX(qreal v);
    qreal radiusY() const;
    void setRadiusY(qreal v);

    void addToPath(QPainterPath *path) override;

signals:
    void radiusXChanged();
    void radiusYChanged();

private:
    qreal m_radiusX;
    qreal m_radiusY;
};

QT_END_NAMESPACE

#endif
