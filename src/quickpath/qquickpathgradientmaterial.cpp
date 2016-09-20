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

#include "qquickpathgradientmaterial_p.h"
#include "qquickpathrendernode_p.h"
#include <QQuickWindow>

QT_BEGIN_NAMESPACE

QSGMaterialType QQuickPathLinearGradientShader::type;

QQuickPathLinearGradientShader::QQuickPathLinearGradientShader()
{
#ifndef QT_NO_OPENGL
    setShaderSourceFile(QOpenGLShader::Vertex,
                        QStringLiteral(":/qt-project.org/scenegraph/path/shaders/lineargradient.vert"));
    setShaderSourceFile(QOpenGLShader::Fragment,
                        QStringLiteral(":/qt-project.org/scenegraph/path/shaders/lineargradient.frag"));
#endif
}

void QQuickPathLinearGradientShader::initialize()
{
#ifndef QT_NO_OPENGL
    m_opacityLoc = program()->uniformLocation("opacity");
    m_matrixLoc = program()->uniformLocation("matrix");
    m_viewportSizeLoc = program()->uniformLocation("viewportSize");
    m_gradVecLoc = program()->uniformLocation("gradVec");
#endif
}

void QQuickPathLinearGradientShader::updateState(const RenderState &state, QSGMaterial *mat, QSGMaterial *)
{
#ifndef QT_NO_OPENGL
    QQuickPathLinearGradientMaterial *m = static_cast<QQuickPathLinearGradientMaterial *>(mat);
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

char const *const *QQuickPathLinearGradientShader::attributeNames() const
{
    static const char *const attr[] = { "vertexCoord", "vertexColor", nullptr };
    return attr;
}

int QQuickPathLinearGradientMaterial::compare(const QSGMaterial *other) const
{
    // ###
    return 0;
}

QT_END_NAMESPACE
