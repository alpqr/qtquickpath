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
#include "qnvpr.h"
#include <QQuickItem>

#ifndef QT_NO_OPENGL

#include <QOpenGLExtraFunctions>

class QNvprRenderNodePrivate
{
public:
    QQuickItem *item;

    QNvPathRendering nvpr;
    GLuint pp = 0;
    GLuint fs = 0;
    int colorLoc;
};

QNvprRenderNode::QNvprRenderNode(QQuickItem *item)
    : d(new QNvprRenderNodePrivate)
{
    d->item = item;
}

QNvprRenderNode::~QNvprRenderNode()
{
    releaseResources();
    delete d;
}

void QNvprRenderNode::releaseResources()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    if (d->pp) {
        f->glDeleteProgramPipelines(1, &d->pp);
        d->pp = 0;
    }
}

GLuint pathObj = 42;

void QNvprRenderNode::render(const RenderState *state)
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    // no API yet, just do some hard-coded rendering

    if (!d->pp) {
        if (!d->nvpr.create())
            qFatal("NVPR init failed");

        static const char *fragSrc =
            "#version 310 es\n"
            "precision highp float;\n"
            "out vec4 fragColor;\n"
            "uniform vec4 color;\n"
            "void main() {\n"
            "  fragColor = color;\n"
            "}\n";

        if (!d->nvpr.createFragmentOnlyPipeline(fragSrc, &d->pp, &d->fs))
            qFatal("Failed to create shader program");

        d->colorLoc = f->glGetProgramResourceLocation(d->fs, GL_UNIFORM, "color");
        Q_ASSERT(d->colorLoc >= 0);

        qDebug("nvpr render node inited");
    }

    // from nvpr_basic.c in NVprSDK
    // will be upside-down and out of bounds but nevermind for now

    f->glBindProgramPipeline(d->pp);

    static const GLubyte pathCommands[10] =
      { GL_MOVE_TO_NV, GL_LINE_TO_NV, GL_LINE_TO_NV, GL_LINE_TO_NV,
        GL_LINE_TO_NV, GL_CLOSE_PATH_NV,
        'M', 'C', 'C', 'Z' };  // character aliases
    static const GLshort pathCoords[12][2] =
      { {100, 180}, {40, 10}, {190, 120}, {10, 120}, {160, 10},
        {300,300}, {100,400}, {100,200}, {300,100},
        {500,200}, {500,400}, {300,300} };
    d->nvpr.pathCommands(pathObj, 10, pathCommands, 24, GL_SHORT, pathCoords);
    d->nvpr.pathParameteri(pathObj, GL_PATH_JOIN_STYLE_NV, GL_ROUND_NV);
    d->nvpr.pathParameterf(pathObj, GL_PATH_STROKE_WIDTH_NV, 6.5);

    // f->glDepthMask(true);
    // f->glEnable(GL_DEPTH_TEST);
    // GLfloat slope = -0.05;
    // GLint bias = -1;
    // d->nvpr.pathStencilDepthOffset(slope, bias);
    // d->nvpr.pathCoverDepthFunc(GL_ALWAYS);

    f->glEnable(GL_STENCIL_TEST);
    f->glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    f->glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);

    d->nvpr.matrixLoadIdentity(GL_PATH_PROJECTION_NV);
    d->nvpr.matrixLoadf(GL_PATH_MODELVIEW_NV, state->projectionMatrix()->constData());
    d->nvpr.matrixLoadf(GL_PATH_PROJECTION_NV, matrix()->constData());

    // QMatrix4x4 m = *state->projectionMatrix() * *matrix();
    // d->nvpr.matrixLoadf(GL_PROJECTION, m.constData());
    // d->nvpr.matrixLoadIdentity(GL_MODELVIEW);
//    d->nvpr.matrixLoadf(GL_MODELVIEW, m.constData());

    f->glClearStencil(0);
    f->glStencilMask(~0);
    f->glClear(GL_STENCIL_BUFFER_BIT);

    d->nvpr.stencilFillPath(pathObj, GL_COUNT_UP_NV, 0x1F);

    f->glProgramUniform4f(d->pp, d->colorLoc, 0, 1, 0, 1); // green
    d->nvpr.coverFillPath(pathObj, GL_BOUNDING_BOX_NV);

//    f->glDisable(GL_DEPTH_TEST);
    d->nvpr.stencilStrokePath(pathObj, 0x1, ~0);

//    f->glEnable(GL_DEPTH_TEST);
    f->glProgramUniform4f(d->pp, d->colorLoc, 1, 1, 0, 1); // yellow
    d->nvpr.coverStrokePath(pathObj, GL_CONVEX_HULL_NV);

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
    return QRect(0, 0, d->item->width(), d->item->height());
}

bool QNvprRenderNode::isSupported()
{
    return QNvPathRendering::isSupported();
}

#endif // QT_NO_OPENGL
