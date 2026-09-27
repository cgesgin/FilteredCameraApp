#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QLabel>
#include <QVideoSink>
#include <QPixmap>
#include <QVideoFrame>
#include <QCheckBox>

#include "utilities.cpp"

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

    QLabel *videolabel;
    QVideoSink *videoSink;

    QCheckBox *bwCheckBox;
    QImage currentFrame;

    void setupIU(){
        central = new QWidget(this);
        layout = new QVBoxLayout(central);

        videolabel = new QLabel("Camera Feed");
        videolabel->setMinimumSize(640,480);
        videolabel->setStyleSheet("background-color:black");
        videolabel->setAlignment(Qt::AlignCenter);

        bwCheckBox = new QCheckBox("Black and White Mode");

        layout->addWidget(videolabel);
        layout->addWidget(bwCheckBox);

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

        videoSink =new QVideoSink();
        session->setVideoSink(videoSink);

        connect(videoSink,&QVideoSink::videoFrameChanged,this,&CameraManager::videoFrameChanged);

        camera->start();

    }

private slots :
    void videoFrameChanged(const QVideoFrame &frame){

        if(!frame.isValid())
            return;
        QVideoFrame cloneFrame(frame);
        cloneFrame.map(QVideoFrame::ReadOnly);
        QImage image = cloneFrame.toImage();

        cloneFrame.unmap();

        currentFrame = bwCheckBox->isChecked() ? toGrayscale(image): image;

        videolabel->setPixmap(QPixmap::fromImage(currentFrame).scaled(videolabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
};