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

#include "qnvprrendernode_p.h"
#include "qquickpathitem_p.h"
#include <QOpenGLExtraFunctions>

#ifndef QT_NO_OPENGL

QT_BEGIN_NAMESPACE

QNvprRenderNode::QNvprRenderNode(QQuickPathItem *item)
    : m_item(item)
{
}

QNvprRenderNode::~QNvprRenderNode()
{
    releaseResources();
}

void QNvprRenderNode::releaseResources()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    if (m_pp) {
        f->glDeleteProgramPipelines(1, &m_pp);
        m_pp = 0;
    }
}

GLuint pathObj = 42;

void QNvprRenderNode::render(const RenderState *state)
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    // no API yet, just do some hard-coded rendering

    if (!m_pp) {
        if (!m_nvpr.create())
            qFatal("NVPR init failed");

        static const char *fragSrc =
            "#version 310 es\n"
            "precision highp float;\n"
            "out vec4 fragColor;\n"
            "uniform vec4 color;\n"
            "void main() {\n"
            "  fragColor = color;\n"
            "}\n";

        if (!m_nvpr.createFragmentOnlyPipeline(fragSrc, &m_pp, &m_fs))
            qFatal("Failed to create shader program");

        m_colorLoc = f->glGetProgramResourceLocation(m_fs, GL_UNIFORM, "color");
        Q_ASSERT(m_colorLoc >= 0);

        qDebug("nvpr render node inited");
    }

    // from nvpr_basic.c in NVprSDK
    // will be upside-down and out of bounds but nevermind for now

    f->glBindProgramPipeline(m_pp);

    static const GLubyte pathCommands[10] =
      { GL_MOVE_TO_NV, GL_LINE_TO_NV, GL_LINE_TO_NV, GL_LINE_TO_NV,
        GL_LINE_TO_NV, GL_CLOSE_PATH_NV,
        'M', 'C', 'C', 'Z' };  // character aliases
    static const GLshort pathCoords[12][2] =
      { {100, 180}, {40, 10}, {190, 120}, {10, 120}, {160, 10},
        {300,300}, {100,400}, {100,200}, {300,100},
        {500,200}, {500,400}, {300,300} };
    m_nvpr.pathCommands(pathObj, 10, pathCommands, 24, GL_SHORT, pathCoords);
    m_nvpr.pathParameteri(pathObj, GL_PATH_JOIN_STYLE_NV, GL_ROUND_NV);
    m_nvpr.pathParameterf(pathObj, GL_PATH_STROKE_WIDTH_NV, 6.5);

    // f->glDepthMask(true);
    // f->glEnable(GL_DEPTH_TEST);
    // GLfloat slope = -0.05;
    // GLint bias = -1;
    // m_nvpr.pathStencilDepthOffset(slope, bias);
    // m_nvpr.pathCoverDepthFunc(GL_ALWAYS);

    f->glEnable(GL_STENCIL_TEST);
    f->glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    f->glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);

    m_nvpr.matrixLoadIdentity(GL_PATH_PROJECTION_NV);
    m_nvpr.matrixLoadf(GL_PATH_MODELVIEW_NV, state->projectionMatrix()->constData());
    m_nvpr.matrixLoadf(GL_PATH_PROJECTION_NV, matrix()->constData());

    // QMatrix4x4 m = *state->projectionMatrix() * *matrix();
    // m_nvpr.matrixLoadf(GL_PROJECTION, m.constData());
    // m_nvpr.matrixLoadIdentity(GL_MODELVIEW);
//    m_nvpr.matrixLoadf(GL_MODELVIEW, m.constData());

    f->glClearStencil(0);
    f->glStencilMask(~0);
    f->glClear(GL_STENCIL_BUFFER_BIT);

    m_nvpr.stencilFillPath(pathObj, GL_COUNT_UP_NV, 0x1F);

    f->glProgramUniform4f(m_pp, m_colorLoc, 0, 1, 0, 1); // green
    m_nvpr.coverFillPath(pathObj, GL_BOUNDING_BOX_NV);

//    f->glDisable(GL_DEPTH_TEST);
    m_nvpr.stencilStrokePath(pathObj, 0x1, ~0);

//    f->glEnable(GL_DEPTH_TEST);
    f->glProgramUniform4f(m_pp, m_colorLoc, 1, 1, 0, 1); // yellow
    m_nvpr.coverStrokePath(pathObj, GL_CONVEX_HULL_NV);

    f->glBindProgramPipeline(0);
}

QSGRenderNode::StateFlags QNvprRenderNode::changedStates() const
{
    return BlendState | StencilState | DepthState;
}

QSGRenderNode::RenderingFlags QNvprRenderNode::flags() const
{
    return 0;
//    return BoundedRectRendering | DepthAwareRendering;
}

QRectF QNvprRenderNode::rect() const
{
    return QRect(0, 0, m_item->width(), m_item->height());
}

bool QNvprRenderNode::isSupported()
{
    return QNvPathRendering::isSupported();
}

void QNvprPathRenderer::beginSync()
{
}

void QNvprPathRenderer::setPath(const QPainterPath &path)
{
    // ###
}

void QNvprPathRenderer::setFillColor(const QColor &color)
{
}

void QNvprPathRenderer::setStrokeColor(const QColor &color)
{
}

void QNvprPathRenderer::setStrokeWidth(qreal w)
{
}

void QNvprPathRenderer::setFlags(RenderFlags flags)
{
}

void QNvprPathRenderer::setJoinStyle(QQuickPathItem::JoinStyle joinStyle, int miterLimit)
{
}

void QNvprPathRenderer::setCapStyle(QQuickPathItem::CapStyle capStyle)
{
}

void QNvprPathRenderer::setStrokeStyle(QQuickPathItem::StrokeStyle strokeStyle)
{
}

void QNvprPathRenderer::endSync()
{
}

QT_END_NAMESPACE

#endif // QT_NO_OPENGL
