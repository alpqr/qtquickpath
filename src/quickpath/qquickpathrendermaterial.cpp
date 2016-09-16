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

#include "qquickpathrendermaterial_p.h"
#include "qquickpathrendernode_p.h"
#include <QQuickWindow>
#include <QSGVertexColorMaterial>

QT_BEGIN_NAMESPACE

QSGMaterial *QQuickPathRenderMaterialFactory::createVertexColor(QQuickWindow *window)
{
    QSGRendererInterface *rif = window->rendererInterface();
    QSGRendererInterface::GraphicsApi api = rif->graphicsApi();

#ifndef QT_NO_OPENGL
    if (api == QSGRendererInterface::OpenGL)
        return new QSGVertexColorMaterial;
#endif

    qWarning("Unsupported api %d", api);
    return nullptr;
}

class QQuickPathRenderLinearGradientShader : public QSGMaterialShader
{
public:
    QQuickPathRenderLinearGradientShader();

    void initialize() override;
    void updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect) override;
    char const *const *attributeNames() const override;

    static QSGMaterialType type;

private:
    int m_opacityLoc;
    int m_matrixLoc;
    int m_viewportSizeLoc;
    int m_gradVecLoc;
};

QSGMaterialType QQuickPathRenderLinearGradientShader::type;

class QQuickPathRenderLinearGradientMaterial : public QSGMaterial
{
public:
    QQuickPathRenderLinearGradientMaterial(QQuickPathRenderNode *node)
        : m_node(node)
    {
        setFlag(Blending);
    }
    QSGMaterialType *type() const override {
        return &QQuickPathRenderLinearGradientShader::type;
    }
    int compare(const QSGMaterial *other) const override {
        // ###
        return 0;
    }
    QSGMaterialShader *createShader() const override {
        return new QQuickPathRenderLinearGradientShader;
    }

    QQuickPathRenderNode *node() const { return m_node; }

private:
    QQuickPathRenderNode *m_node;
};

QQuickPathRenderLinearGradientShader::QQuickPathRenderLinearGradientShader()
{
#ifndef QT_NO_OPENGL
    setShaderSourceFile(QOpenGLShader::Vertex,
                        QStringLiteral(":/qt-project.org/scenegraph/path/shaders/lineargradient.vert"));
    setShaderSourceFile(QOpenGLShader::Fragment,
                        QStringLiteral(":/qt-project.org/scenegraph/path/shaders/lineargradient.frag"));
#endif
}

void QQuickPathRenderLinearGradientShader::initialize()
{
#ifndef QT_NO_OPENGL
    m_opacityLoc = program()->uniformLocation("opacity");
    m_matrixLoc = program()->uniformLocation("matrix");
    m_viewportSizeLoc = program()->uniformLocation("viewportSize");
    m_gradVecLoc = program()->uniformLocation("gradVec");
#endif
}

void QQuickPathRenderLinearGradientShader::updateState(const RenderState &state, QSGMaterial *mat, QSGMaterial *)
{
#ifndef QT_NO_OPENGL
    QQuickPathRenderLinearGradientMaterial *m = static_cast<QQuickPathRenderLinearGradientMaterial *>(mat);
    if (state.isOpacityDirty())
        program()->setUniformValue(m_opacityLoc, state.opacity());
    if (state.isMatrixDirty())
        program()->setUniformValue(m_matrixLoc, state.combinedMatrix());
    if (m->node()->dirty() & QQuickPathRenderer::DirtyColor) {
        QQuickPathRenderer *r = m->node()->rootNode()->renderer();
        if (r) {
            QPointF vec = r->m_fillGradientEnd - r->m_fillGradientStart;
            const QVector2D gradVec(vec.x(), vec.y());
            program()->setUniformValue(m_gradVecLoc, gradVec);
            QQuickWindow *w = m->node()->window();
            const QSize sz = w->size() * w->effectiveDevicePixelRatio();
            const QVector2D viewportSize(sz.width(), sz.height());
            program()->setUniformValue(m_viewportSizeLoc, viewportSize);
        }
    }
    m->node()->resetDirty();
#endif
}

char const *const *QQuickPathRenderLinearGradientShader::attributeNames() const
{
    static const char *const attr[] = { "vertexCoord", "vertexColor", nullptr };
    return attr;
}

QSGMaterial *QQuickPathRenderMaterialFactory::createLinearGradient(QQuickWindow *window, QQuickPathRenderNode *node)
{
    QSGRendererInterface *rif = window->rendererInterface();
    QSGRendererInterface::GraphicsApi api = rif->graphicsApi();

#ifndef QT_NO_OPENGL
    if (api == QSGRendererInterface::OpenGL)
        return new QQuickPathRenderLinearGradientMaterial(node);
#endif

    qWarning("Unsupported api %d", api);
    return nullptr;
}

QT_END_NAMESPACE
