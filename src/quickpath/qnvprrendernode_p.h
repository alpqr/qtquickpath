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

#ifndef QNVPRRENDERNODE_P_H
#define QNVPRRENDERNODE_P_H

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

#include "qquickabstractpathrenderer_p.h"
#include <qsgrendernode.h>
#include "qnvpr.h"

#ifndef QT_NO_OPENGL

QT_BEGIN_NAMESPACE

class QQuickPathItem;
class QNvprRenderNode;

class QNvprPathRenderer : public QQuickAbstractPathRenderer
{
public:
    QNvprPathRenderer(QNvprRenderNode *rn) : m_node(rn) { }

    void beginSync() override;
    void setPath(const QPainterPath &path) override;
    void setFillColor(const QColor &color) override;
    void setStrokeColor(const QColor &color) override;
    void setStrokeWidth(qreal w) override;
    void setFlags(RenderFlags flags) override;
    void setJoinStyle(QQuickPathItem::JoinStyle joinStyle, int miterLimit) override;
    void setCapStyle(QQuickPathItem::CapStyle capStyle) override;
    void setStrokeStyle(QQuickPathItem::StrokeStyle strokeStyle) override;
    void endSync() override;

private:
    QNvprRenderNode *m_node;
};

class QNvprRenderNode : public QSGRenderNode
{
public:
    QNvprRenderNode(QQuickPathItem *item);
    ~QNvprRenderNode();

    void render(const RenderState *state) override;
    void releaseResources() override;
    StateFlags changedStates() const override;
    RenderingFlags flags() const override;
    QRectF rect() const override;

    static bool isSupported();

private:
    QQuickPathItem *m_item;
    QNvPathRendering m_nvpr;
    GLuint m_pp = 0;
    GLuint m_fs = 0;
    int m_colorLoc;
};

QT_END_NAMESPACE

#endif // QT_NO_OPENGL

#endif
