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

#include "qquickpathitem_p.h"
#include "qnvprrendernode_p.h"
#include <QSGRendererInterface>

QQuickPathItem::QQuickPathItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents);
}

QSGNode *QQuickPathItem::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    QQuickPathRenderNode *n = static_cast<QQuickPathRenderNode *>(node);

    if (!n) {
        QSGRendererInterface *ri = window()->rendererInterface();
        if (!ri)
            return nullptr;
        switch (ri->graphicsApi()) {
            case QSGRendererInterface::OpenGL:
#ifndef QT_NO_OPENGL
                if (QNvprRenderNode::isSupported()) {
                    n = new QNvprRenderNode(this);
                    break;
                }
#endif
            case QSGRendererInterface::Direct3D12:
            case QSGRendererInterface::Software:
            default:
                qWarning("No path backend for this graphics API yet");
                break;
        }
    }

    return n;
}
