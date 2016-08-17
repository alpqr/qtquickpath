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

#include "qnvpr.h"

#ifndef QT_NO_OPENGL

#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include "qnvpr_p.h"

QT_BEGIN_NAMESPACE

QNvPathRendering::QNvPathRendering()
    : d(new QNvPathRenderingPrivate(this))
{
}

QNvPathRendering::~QNvPathRendering()
{
    delete d;
}

/*!
   \return a recommended QSurfaceFormat suitable for GL_NV_path_rendering on top
   of OpenGL 4.3 or OpenGL ES 3.1.
 */
QSurfaceFormat QNvPathRendering::format()
{
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setStencilBufferSize(8);
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        fmt.setVersion(4, 3);
        fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    } else if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGLES) {
        fmt.setVersion(3, 1);
    }
    return fmt;
}

/*!
    Initializes using the current OpenGL context.
 */
bool QNvPathRendering::create()
{
    return d->resolve();
}

/*!
    Creates a program pipeline consisting of a separable fragment shader program.

    This is essential for using NVPR with OpenGL ES 3.1+ since normal,
    GLES2-style programs would not work without a vertex shader.

    \note \a fragmentShaderSource should be a \c{version 310 es} shader since
    this works both on desktop and embedded NVIDIA drivers, thus avoiding the
    need to fight GLSL and GLSL ES differences.

    The pipeline object is stored into \a pipeline, the fragment shader program
    into \a program.

    Use QOpenGLExtraFunctions to set uniforms, bind the pipeline, etc.

    \return \c false on failure in which case the error log is printed on the
    debug output. \c true on success.
 */
bool QNvPathRendering::createFragmentOnlyPipeline(const char *fragmentShaderSource, GLuint *pipeline, GLuint *program)
{
    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    if (!ctx)
        return false;

    QOpenGLExtraFunctions *f = ctx->extraFunctions();
    *program = f->glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragmentShaderSource);
    GLint status = 0;
    f->glGetProgramiv(*program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint len = 0;
        f->glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &len);
        if (len) {
            QByteArray s;
            s.resize(len);
            f->glGetProgramInfoLog(*program, s.count(), nullptr, s.data());
            qWarning("Failed to create separable shader program:\n%s", s.constData());
        }
        return false;
    }

    f->glGenProgramPipelines(1, pipeline);
    f->glUseProgramStages(*pipeline, GL_FRAGMENT_SHADER_BIT, *program);
    f->glActiveShaderProgram(*pipeline, *program);

    f->glValidateProgramPipeline(*pipeline);
    status = 0;
    f->glGetProgramPipelineiv(*pipeline, GL_VALIDATE_STATUS, &status);
    if (!status) {
        GLint len = 0;
        f->glGetProgramPipelineiv(*pipeline, GL_INFO_LOG_LENGTH, &len);
        if (len) {
            QByteArray s;
            s.resize(len);
            f->glGetProgramPipelineInfoLog(*pipeline, s.count(), nullptr, s.data());
            qWarning("Program pipeline validation failed:\n%s", s.constData());
        }
        return false;
    }

    return true;
}

#define PROC(type, name) reinterpret_cast<type>(ctx->getProcAddress(#name))

bool QNvPathRenderingPrivate::resolve()
{
    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    if (!ctx) {
        qWarning("QtNVPR: No OpenGL context");
        return false;
    }

    if (!ctx->hasExtension(QByteArrayLiteral("GL_NV_path_rendering"))) {
        qWarning("QtNVPR: GL_NV_path_rendering not supported");
        return false;
    }

    // Do not check for DSA as the string may not be exposed on ES
    // drivers, yet the functions we need are resolvable.
#if 0
    if (!ctx->hasExtension(QByteArrayLiteral("GL_EXT_direct_state_access"))) {
        qWarning("QtNVPR: GL_EXT_direct_state_access not supported");
        return false;
    }
#endif

    if (ctx->format().stencilBufferSize() < 8)
        qWarning("QtNVPR: Stencil buffer not present?");

    q->genPaths = PROC(PFNGLGENPATHSNVPROC, glGenPathsNV);
    q->deletePaths = PROC(PFNGLDELETEPATHSNVPROC, glDeletePathsNV);
    q->isPath = PROC(PFNGLISPATHNVPROC, glIsPathNV);
    q->pathCommands = PROC(PFNGLPATHCOMMANDSNVPROC, glPathCommandsNV);
    q->pathCoords = PROC(PFNGLPATHCOORDSNVPROC, glPathCoordsNV);
    q->pathSubCommands = PROC(PFNGLPATHSUBCOMMANDSNVPROC, glPathSubCommandsNV);
    q->pathSubCoords = PROC(PFNGLPATHSUBCOORDSNVPROC, glPathSubCoordsNV);
    q->pathString = PROC(PFNGLPATHSTRINGNVPROC, glPathStringNV);
    q->pathGlyphs = PROC(PFNGLPATHGLYPHSNVPROC, glPathGlyphsNV);
    q->pathGlyphRange = PROC(PFNGLPATHGLYPHRANGENVPROC, glPathGlyphRangeNV);
    q->weightPaths = PROC(PFNGLWEIGHTPATHSNVPROC, glWeightPathsNV);
    q->copyPath = PROC(PFNGLCOPYPATHNVPROC, glCopyPathNV);
    q->interpolatePaths = PROC(PFNGLINTERPOLATEPATHSNVPROC, glInterpolatePathsNV);
    q->transformPath = PROC(PFNGLTRANSFORMPATHNVPROC, glTransformPathNV);
    q->pathParameteriv = PROC(PFNGLPATHPARAMETERIVNVPROC, glPathParameterivNV);
    q->pathParameteri = PROC(PFNGLPATHPARAMETERINVPROC, glPathParameteriNV);
    q->pathParameterfv = PROC(PFNGLPATHPARAMETERFVNVPROC, glPathParameterfvNV);
    q->pathParameterf = PROC(PFNGLPATHPARAMETERFNVPROC, glPathParameterfNV);
    q->pathDashArray = PROC(PFNGLPATHDASHARRAYNVPROC, glPathDashArrayNV);
    q->pathStencilFunc = PROC(PFNGLPATHSTENCILFUNCNVPROC, glPathStencilFuncNV);
    q->pathStencilDepthOffset = PROC(PFNGLPATHSTENCILDEPTHOFFSETNVPROC, glPathStencilDepthOffsetNV);
    q->stencilFillPath = PROC(PFNGLSTENCILFILLPATHNVPROC, glStencilFillPathNV);
    q->stencilStrokePath = PROC(PFNGLSTENCILSTROKEPATHNVPROC, glStencilStrokePathNV);
    q->stencilFillPathInstanced = PROC(PFNGLSTENCILFILLPATHINSTANCEDNVPROC, glStencilFillPathInstancedNV);
    q->stencilStrokePathInstanced = PROC(PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC, glStencilStrokePathInstancedNV);
    q->pathCoverDepthFunc = PROC(PFNGLPATHCOVERDEPTHFUNCNVPROC,  glPathCoverDepthFuncNV);
    q->pathColorGen = PROC(PFNGLPATHCOLORGENNVPROC, glPathColorGenNV);
    q->pathTexGen = PROC(PFNGLPATHTEXGENNVPROC, glPathTexGenNV);
    q->pathFogGen = PROC(PFNGLPATHFOGGENNVPROC, glPathFogGenNV);
    q->coverFillPath = PROC(PFNGLCOVERFILLPATHNVPROC, glCoverFillPathNV);
    q->coverStrokePath = PROC(PFNGLCOVERSTROKEPATHNVPROC, glCoverStrokePathNV);
    q->coverFillPathInstanced = PROC(PFNGLCOVERFILLPATHINSTANCEDNVPROC, glCoverFillPathInstancedNV);
    q->coverStrokePathInstanced = PROC(PFNGLCOVERSTROKEPATHINSTANCEDNVPROC, glCoverStrokePathInstancedNV);
    q->getPathParameteriv = PROC(PFNGLGETPATHPARAMETERIVNVPROC, glGetPathParameterivNV);
    q->getPathParameterfv = PROC(PFNGLGETPATHPARAMETERFVNVPROC, glGetPathParameterfvNV);
    q->getPathCommands = PROC(PFNGLGETPATHCOMMANDSNVPROC, glGetPathCommandsNV);
    q->getPathCoords = PROC(PFNGLGETPATHCOORDSNVPROC, glGetPathCoordsNV);
    q->getPathDashArray = PROC(PFNGLGETPATHDASHARRAYNVPROC, glGetPathDashArrayNV);
    q->getPathMetrics = PROC(PFNGLGETPATHMETRICSNVPROC, glGetPathMetricsNV);
    q->getPathMetricRange = PROC(PFNGLGETPATHMETRICRANGENVPROC, glGetPathMetricRangeNV);
    q->getPathSpacing = PROC(PFNGLGETPATHSPACINGNVPROC, glGetPathSpacingNV);
    q->getPathColorgeniv = PROC(PFNGLGETPATHCOLORGENIVNVPROC, glGetPathColorGenivNV);
    q->getPathColorgenfv = PROC(PFNGLGETPATHCOLORGENFVNVPROC, glGetPathColorGenfvNV);
    q->getPathTexGeniv = PROC(PFNGLGETPATHTEXGENIVNVPROC, glGetPathTexGenivNV);
    q->getPathTexGenfv = PROC(PFNGLGETPATHTEXGENFVNVPROC, glGetPathTexGenfvNV);
    q->isPointInFillPath = PROC(PFNGLISPOINTINFILLPATHNVPROC, glIsPointInFillPathNV);
    q->isPointInStrokePath = PROC(PFNGLISPOINTINSTROKEPATHNVPROC, glIsPointInStrokePathNV);
    q->getPathLength = PROC(PFNGLGETPATHLENGTHNVPROC, glGetPathLengthNV);
    q->getPointAlongPath = PROC(PFNGLPOINTALONGPATHNVPROC, glPointAlongPathNV);
    q->matrixLoad3x2f = PROC(PFNGLMATRIXLOAD3X2FNVPROC, glMatrixLoad3x2fNV);
    q->matrixLoad3x3f = PROC(PFNGLMATRIXLOAD3X3FNVPROC, glMatrixLoad3x3fNV);
    q->matrixLoadTranspose3x3f = PROC(PFNGLMATRIXLOADTRANSPOSE3X3FNVPROC, glMatrixLoadTranspose3x3fNV);
    q->matrixMult3x2f = PROC(PFNGLMATRIXMULT3X2FNVPROC, glMatrixMult3x2fNV);
    q->matrixMult3x3f = PROC(PFNGLMATRIXMULT3X3FNVPROC, glMatrixMult3x3fNV);
    q->matrixMultTranspose3x3f = PROC(PFNGLMATRIXMULTTRANSPOSE3X3FNVPROC, glMatrixMultTranspose3x3fNV);
    q->stencilThenCoverFillPath = PROC(PFNGLSTENCILTHENCOVERFILLPATHNVPROC, glStencilThenCoverFillPathNV);
    q->stencilThenCoverStrokePath = PROC(PFNGLSTENCILTHENCOVERSTROKEPATHNVPROC, glStencilThenCoverStrokePathNV);
    q->stencilThenCoverFillPathInstanced = PROC(PFNGLSTENCILTHENCOVERFILLPATHINSTANCEDNVPROC, glStencilThenCoverFillPathInstancedNV);
    q->stencilThenCoverStrokePathInstanced = PROC(PFNGLSTENCILTHENCOVERSTROKEPATHINSTANCEDNVPROC, glStencilThenCoverStrokePathInstancedNV);
    q->pathGlyphIndexRange = PROC(PFNGLPATHGLYPHINDEXRANGENVPROC, glPathGlyphIndexRangeNV);
    q->pathGlyphIndexArray = PROC(PFNGLPATHGLYPHINDEXARRAYNVPROC, glPathGlyphIndexArrayNV);
    q->pathMemoryGlyphIndexArray = PROC(PFNGLPATHMEMORYGLYPHINDEXARRAYNVPROC, glPathMemoryGlyphIndexArrayNV);
    q->programPathFragmentInputGen = PROC(PFNGLPROGRAMPATHFRAGMENTINPUTGENNVPROC, glProgramPathFragmentInputGenNV);
    q->getProgramResourcefv = PROC(PFNGLGETPROGRAMRESOURCEFVNVPROC, glGetProgramResourcefvNV);

    q->matrixLoadf = PROC(PFNGLMATRIXLOADFEXTPROC, glMatrixLoadfEXT);
    q->matrixLoadIdentity = PROC(PFNGLMATRIXLOADIDENTITYEXTPROC, glMatrixLoadIdentityEXT);

    return q->genPaths != nullptr // base path rendering ext
        && q->programPathFragmentInputGen != nullptr // updated path rendering ext
        && q->matrixLoadf != nullptr // direct state access ext
        && q->matrixLoadIdentity != nullptr;
}

QT_END_NAMESPACE

#endif // QT_NO_OPENGL
