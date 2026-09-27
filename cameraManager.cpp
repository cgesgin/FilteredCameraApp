#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QLabel>


class CameraManager : public QMainWindow{

public : CameraManager (QWidget * parent = nullptr): QMainWindow(parent){
        setupIU();
        setupCamera();
    }

private:
    QWidget *central;
    QVBoxLayout *layout;

    QCamera *camera;
    QMediaCaptureSession *session;

    void setupIU(){
        central = new QWidget(this);
        layout = new QVBoxLayout(central);

        setCentralWidget(central);
        setWindowTitle("Camera App");
        resize(700,600);
    }

    void setupCamera(){

        QCameraDevice cameraDevice = QMediaDevices::defaultVideoInput();

        if(cameraDevice.isNull()){
            layout->addWidget(new QLabel("No Camera Avaible on your computer"));
            return;
        }

        camera = new QCamera(cameraDevice);
        session = new QMediaCaptureSession();
        session->setCamera(camera);

        camera->start();

    }
};