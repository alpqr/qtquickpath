/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtNVPR module
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

#ifndef QQUICKABSTRACTPATHRENDERER_P_H
#define QQUICKABSTRACTPATHRENDERER_P_H

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
#include <QPainterPath>
#include <QColor>

QT_BEGIN_NAMESPACE

class QQuickAbstractPathRenderer
{
public:
    virtual ~QQuickAbstractPathRenderer() { }

    enum RenderFlag {
        RenderNoFill = 0x01
    };
    Q_DECLARE_FLAGS(RenderFlags, RenderFlag)

    virtual void beginSync() = 0;
    virtual void setPath(const QPainterPath &path) = 0;
    virtual void setFillColor(const QColor &color) = 0;
    virtual void setStrokeColor(const QColor &color) = 0;
    virtual void setStrokeWidth(qreal w) = 0;
    virtual void setFlags(RenderFlags flags) = 0;
    virtual void setJoinStyle(QQuickPathItem::JoinStyle joinStyle, int miterLimit) = 0;
    virtual void setCapStyle(QQuickPathItem::CapStyle capStyle) = 0;
    virtual void setStrokeStyle(QQuickPathItem::StrokeStyle strokeStyle) = 0;
    virtual void endSync() = 0;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QQuickAbstractPathRenderer::RenderFlags)

QT_END_NAMESPACE

#endif
