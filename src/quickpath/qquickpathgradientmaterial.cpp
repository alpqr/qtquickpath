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
#include <QQuickWindow>
#include <QOpenGLFunctions>
#include <QtQuick/private/qsgtexture_p.h>
#include <QtGui/private/qdrawhelper_p.h>
//#include <QImage>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_OPENGL

// contexts sharing with each other get the same cache instance
class QQuickPathGradientCacheWrapper
{
public:
    QQuickPathGradientCache *get(QOpenGLContext *context)
    {
        return m_resource.value<QQuickPathGradientCache>(context);
    }

private:
    QOpenGLMultiGroupSharedResource m_resource;
};

Q_GLOBAL_STATIC(QQuickPathGradientCacheWrapper, qt_path_gradient_caches)

// let QOpenGLContext manage the lifetime of the cached textures
QQuickPathGradientCache::~QQuickPathGradientCache()
{
    m_cache.clear();
}

void QQuickPathGradientCache::invalidateResource()
{
    m_cache.clear();
}

void QQuickPathGradientCache::freeResource(QOpenGLContext *)
{
    qDeleteAll(m_cache);
    m_cache.clear();
}

// ### borrowed from QtGui. May get replaced with something else later.
static void generateGradientColorTable(const QQuickPathRenderer::GradientDesc &gradient, uint *colorTable, int size, float opacity)
{
    int pos = 0;
    const QGradientStops &s = gradient.stops;
    const bool colorInterpolation = true;

    uint alpha = qRound(opacity * 256);
    uint current_color = ARGB_COMBINE_ALPHA(s[0].second.rgba(), alpha);
    qreal incr = 1.0 / qreal(size);
    qreal fpos = 1.5 * incr;
    colorTable[pos++] = ARGB2RGBA(qPremultiply(current_color));

    while (fpos <= s.first().first) {
        colorTable[pos] = colorTable[pos - 1];
        pos++;
        fpos += incr;
    }

    if (colorInterpolation)
        current_color = qPremultiply(current_color);

    const int sLast = s.size() - 1;
    for (int i = 0; i < sLast; ++i) {
        qreal delta = 1/(s[i+1].first - s[i].first);
        uint next_color = ARGB_COMBINE_ALPHA(s[i + 1].second.rgba(), alpha);
        if (colorInterpolation)
            next_color = qPremultiply(next_color);

        while (fpos < s[i+1].first && pos < size) {
            int dist = int(256 * ((fpos - s[i].first) * delta));
            int idist = 256 - dist;
            if (colorInterpolation)
                colorTable[pos] = ARGB2RGBA(INTERPOLATE_PIXEL_256(current_color, idist, next_color, dist));
            else
                colorTable[pos] = ARGB2RGBA(qPremultiply(INTERPOLATE_PIXEL_256(current_color, idist, next_color, dist)));
            ++pos;
            fpos += incr;
        }
        current_color = next_color;
    }

    Q_ASSERT(s.size() > 0);

    uint last_color = ARGB2RGBA(qPremultiply(ARGB_COMBINE_ALPHA(s[sLast].second.rgba(), alpha)));
    for (;pos < size; ++pos)
        colorTable[pos] = last_color;

    colorTable[size-1] = last_color;
}

QSGTexture *QQuickPathGradientCache::get(const QQuickPathRenderer::GradientDesc &grad)
{
    QSGPlainTexture *tx = m_cache[grad];
    if (!tx) {
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        GLuint id;
        f->glGenTextures(1, &id);
        f->glBindTexture(GL_TEXTURE_2D, id);
        static const uint W = 1024; // texture size is 1024x1
        uint buf[W];
        generateGradientColorTable(grad, buf, W, 1.0f);
//        QImage img(reinterpret_cast<const uchar *>(buf), W, 1, QImage::Format_RGBA8888_Premultiplied);
//        img.save("a.png");
        f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
        tx = new QSGPlainTexture;
        tx->setTextureId(id);
        tx->setHorizontalWrapMode(QSGTexture::Repeat);
        tx->setVerticalWrapMode(QSGTexture::Repeat);
        m_cache[grad] = tx;
    }
    return tx;
}

QSGMaterialType QQuickPathLinearGradientShader::type;

QQuickPathLinearGradientShader::QQuickPathLinearGradientShader()
{
    setShaderSourceFile(QOpenGLShader::Vertex,
                        QStringLiteral(":/qt-project.org/scenegraph/path/shaders/lineargradient.vert"));
    setShaderSourceFile(QOpenGLShader::Fragment,
                        QStringLiteral(":/qt-project.org/scenegraph/path/shaders/lineargradient.frag"));
}

void QQuickPathLinearGradientShader::initialize()
{
    m_opacityLoc = program()->uniformLocation("opacity");
    m_matrixLoc = program()->uniformLocation("matrix");
    m_viewportSizeLoc = program()->uniformLocation("viewportSize");
    m_gradStartLoc = program()->uniformLocation("gradStart");
    m_gradEndLoc = program()->uniformLocation("gradEnd");
}

void QQuickPathLinearGradientShader::updateState(const RenderState &state, QSGMaterial *mat, QSGMaterial *)
{
    QQuickPathLinearGradientMaterial *m = static_cast<QQuickPathLinearGradientMaterial *>(mat);
    if (state.isOpacityDirty())
        program()->setUniformValue(m_opacityLoc, state.opacity());
    if (state.isMatrixDirty())
        program()->setUniformValue(m_matrixLoc, state.combinedMatrix());
    QQuickPathRenderer *r = m->node()->rootNode()->renderer();
    if (r) {
        if (m->node()->dirty() & QQuickPathRenderer::DirtyColor) {
            program()->setUniformValue(m_gradStartLoc, r->fillGradient()->start);
            program()->setUniformValue(m_gradEndLoc, r->fillGradient()->end);
        }
        // the window size may have changed, set the corresponding uniform always
        QQuickWindow *w = m->node()->window();
        const QSize sz = w->size() * w->effectiveDevicePixelRatio();
        const QVector2D viewportSize(sz.width(), sz.height());
        program()->setUniformValue(m_viewportSizeLoc, viewportSize);
        // (re)generate color table and bind the texture
        QSGTexture *tx = qt_path_gradient_caches()->get(QOpenGLContext::currentContext())->get(*r->fillGradient());
        tx->bind();
    }
    m->node()->resetDirty();
}

char const *const *QQuickPathLinearGradientShader::attributeNames() const
{
    static const char *const attr[] = { "vertexCoord", "vertexColor", nullptr };
    return attr;
}

int QQuickPathLinearGradientMaterial::compare(const QSGMaterial *other) const
{
    Q_ASSERT(other && type() == other->type());
    const QQuickPathLinearGradientMaterial *m = static_cast<const QQuickPathLinearGradientMaterial *>(other);

    QQuickPathRenderer *a = node()->rootNode()->renderer();
    QQuickPathRenderer *b = m->node()->rootNode()->renderer();
    if (a == b)
        return 0;
    if (!a)
        return -1;
    if (!b)
        return 1;

    const QQuickPathRenderer::GradientDesc *ga = a->fillGradient();
    const QQuickPathRenderer::GradientDesc *gb = b->fillGradient();
    if (int d = ga->start.x() - gb->start.x())
        return d;
    if (int d = ga->start.y() - gb->start.y())
        return d;
    if (int d = ga->end.x() - gb->end.x())
        return d;
    if (int d = ga->end.y() - gb->end.y())
        return d;

    if (int d = ga->stops.count() - gb->stops.count())
        return d;

    for (int i = 0; i < ga->stops.count(); ++i) {
        if (int d = ga->stops[i].first - gb->stops[i].first)
            return d;
        if (int d = ga->stops[i].second.rgba() - gb->stops[i].second.rgba())
            return d;
    }

    return 0;
}

#endif // QT_NO_OPENGL

QT_END_NAMESPACE
