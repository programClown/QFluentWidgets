#include "MainWindow.h"

#include <QDebug>
#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <FramelessHelper/Core/private/framelessconfig_p.h>
#include <clocale>
#include "shared/log.h"

// This example demonstrates easy, cross-platform usage of OpenGL ES 3.0 functions via
// QOpenGLExtraFunctions in an application that works identically on desktop platforms
// with OpenGL 3.3 and mobile/embedded devices with OpenGL ES 3.0.

// The code is always the same, with the exception of two places: (1) the OpenGL context
// creation has to have a sufficiently high version number for the features that are in
// use, and (2) the shader code's version directive is different.

FRAMELESSHELPER_USE_NAMESPACE

int main(int argc, char *argv[])
{
    std::setlocale(LC_ALL, "en_US.UTF-8");

    Log::setup(FRAMELESSHELPER_STRING_LITERAL("openglwidget"));

    // Not necessary, but better call this function, before the construction
    // of any Q(Core|Gui)Application instances.
    FramelessHelper::Widgets::initialize();

    const auto application = std::make_unique<QApplication>(argc, argv);

    // Must be called after QGuiApplication has been constructed, we are using
    // some private functions from QPA which won't be available until there's
    // a QGuiApplication instance.
    FramelessHelper::Core::setApplicationOSThemeAware();

    FramelessConfig::instance()->set(Global::Option::EnableBlurBehindWindow);
    FramelessConfig::instance()->set(Global::Option::DisableLazyInitializationForMicaMaterial);

    QSurfaceFormat fmt = {};
    fmt.setDepthBufferSize(24);

    // Request OpenGL 3.3 core or OpenGL ES 3.0.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        qDebug() << "Requesting OpenGL 3.3 core context ...";
        fmt.setVersion(3, 3);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    } else {
        qDebug() << "Requesting OpenGL ES 3.0 context ...";
        fmt.setVersion(3, 0);
    }

    QSurfaceFormat::setDefaultFormat(fmt);

    const auto mainWindow = std::make_unique<MainWindow>();
    mainWindow->waitReady();
    mainWindow->show();

    return QCoreApplication::exec();
}
