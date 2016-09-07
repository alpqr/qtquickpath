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

#ifndef QQUICKPATHITEM_P_P_H
#define QQUICKPATHITEM_P_P_H

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

#include "qquickpathitem_p.h"
#include "qquickabstractpathrenderer_p.h"

QT_BEGIN_NAMESPACE

class QQuickPathItemPrivate
{
public:
    QQuickPathItemPrivate()
        : renderer(nullptr),
          dirty(0),
          strokeWidth(1),
          strokeColor(Qt::white),
          fillColor(Qt::white),
          flags(0),
          joinStyle(QQuickPathItem::BevelJoin),
          miterLimit(2),
          capStyle(QQuickPathItem::SquareCap),
          strokeStyle(QQuickPathItem::SolidLine),
          dashOffset(0)
    {
        dashPattern << 4 << 2; // 4 * strokeWidth dash followed by 2 * strokeWidth space
    }
    ~QQuickPathItemPrivate() { delete renderer; }

    QSGNode *updatePaintNode(QQuickItem *item, QSGNode *node);

    enum Dirty {
        DirtyPath = 0x01,
        DirtyFillColor = 0x02,
        DirtyStrokeColor = 0x04,
        DirtyStrokeWidth = 0x08,
        DirtyFlags = 0x10,
        DirtyStyle = 0x20
    };

    QPainterPath path;
    QQuickAbstractPathRenderer *renderer;
    int dirty;
    qreal strokeWidth;
    QColor strokeColor;
    QColor fillColor;
    QQuickAbstractPathRenderer::RenderFlags flags;
    QQuickPathItem::JoinStyle joinStyle;
    int miterLimit;
    QQuickPathItem::CapStyle capStyle;
    QQuickPathItem::StrokeStyle strokeStyle;
    qreal dashOffset;
    QVector<qreal> dashPattern;
};

QT_END_NAMESPACE

#endif
