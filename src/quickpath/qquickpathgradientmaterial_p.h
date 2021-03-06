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

#ifndef QQUICKPATHGRADIENTMATERIAL_P_H
#define QQUICKPATHGRADIENTMATERIAL_P_H

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
#include <qsgmaterial.h>
#include <QtGui/private/qopenglcontext_p.h>
#include "qquickpathrendernode_p.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_OPENGL

class QSGTexture;
class QSGPlainTexture;

class QQuickPathGradientCache : public QOpenGLSharedResource
{
public:
    QQuickPathGradientCache(QOpenGLContext *context)
        : QOpenGLSharedResource(context->shareGroup())
    {
    }
    ~QQuickPathGradientCache();

    void invalidateResource() override;
    void freeResource(QOpenGLContext *) override;

    QSGTexture *get(const QQuickPathRenderer::GradientDesc &grad);

private:
    QHash<QQuickPathRenderer::GradientDesc, QSGPlainTexture *> m_cache;
};

class QQuickPathLinearGradientShader : public QSGMaterialShader
{
public:
    QQuickPathLinearGradientShader();

    void initialize() override;
    void updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect) override;
    char const *const *attributeNames() const override;

    static QSGMaterialType type;

private:
    int m_opacityLoc;
    int m_matrixLoc;
    int m_viewportSizeLoc;
    int m_gradStartLoc;
    int m_gradEndLoc;
};

class QQuickPathLinearGradientMaterial : public QSGMaterial
{
public:
    QQuickPathLinearGradientMaterial(QQuickPathRenderNode *node)
        : m_node(node)
    {
        setFlag(Blending);
    }

    QSGMaterialType *type() const override
    {
        return &QQuickPathLinearGradientShader::type;
    }

    int compare(const QSGMaterial *other) const override;

    QSGMaterialShader *createShader() const override
    {
        return new QQuickPathLinearGradientShader;
    }

    QQuickPathRenderNode *node() const { return m_node; }

private:
    QQuickPathRenderNode *m_node;
};

#endif // QT_NO_OPENGL

QT_END_NAMESPACE

#endif
