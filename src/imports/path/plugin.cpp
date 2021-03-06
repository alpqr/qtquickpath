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

#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>
#include <QtQuickPath/private/qquickpathitem_p.h>
#include <QtQuickPath/private/qquickpathgradient_p.h>
#include <QtQuickPath/private/qquickpathcommand_p.h>

static void initResources()
{
#ifdef QT_STATIC
    Q_INIT_RESOURCE(qmake_QtQuick_Path)
#endif
}

QT_BEGIN_NAMESPACE

class QmlPathPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    QmlPathPlugin(QObject *parent = 0) : QQmlExtensionPlugin(parent) { initResources(); }
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtQuick.PathItem"));
        qmlRegisterType<QQuickPathItem>(uri, 2, 0, "PathItem");
        qmlRegisterType<QQuickPathGradientStop>(uri, 2, 0, "PathGradientStop");
        qmlRegisterType<QQuickPathGradient>(uri, 2, 0, "PathGradient");
        qmlRegisterType<QQuickPathMoveTo>(uri, 2, 0, "MoveTo");
        qmlRegisterType<QQuickPathLineTo>(uri, 2, 0, "LineTo");
        qmlRegisterType<QQuickPathArcMoveTo>(uri, 2, 0, "ArcMoveTo");
        qmlRegisterType<QQuickPathArcTo>(uri, 2, 0, "ArcTo");
        qmlRegisterType<QQuickPathCubicTo>(uri, 2, 0, "CubicTo");
        qmlRegisterType<QQuickPathQuadTo>(uri, 2, 0, "QuadTo");
        qmlRegisterType<QQuickPathClose>(uri, 2, 0, "ClosePath");
        qmlRegisterType<QQuickPathEllipse>(uri, 2, 0, "PathEllipse");
        qmlRegisterType<QQuickPathRectangle>(uri, 2, 0, "PathRectangle");
        qmlRegisterType<QQuickPathRoundedRectangle>(uri, 2, 0, "PathRoundedRectangle");
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
