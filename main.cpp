#include <QApplication>
#include "cameraManager.cpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CameraManager manager;

    manager.show();

    return app.exec();
}
