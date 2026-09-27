#include <QMainWindow>          // Ana pencereyi oluşturmak için
#include <QVBoxLayout>          // Elemanları dikey olarak sıralamak için
#include <QWidget>              // Temel arayüz elemanı
#include <QCamera>              // Kamerayı kontrol etmek için
#include <QMediaCaptureSession> // Kamera görüntüsünü yönetmek için
#include <QMediaDevices>        // Bilgisayardaki kameraları bulmak için
#include <QLabel>               // Ekranda yazı veya resim göstermek için
#include <QVideoSink>           // Kameradan gelen görüntüleri almak için
#include <QPixmap>              // Görüntüyü QLabel üzerinde göstermek için
#include <QVideoFrame>          // Kameradan gelen bir görüntü karesi
#include <QCheckBox>            // İşaretleme kutusu oluşturmak için
#include <QPushButton>          // Buton oluşturmak için
#include <QFileDialog>          // Dosya kaydetme penceresi için

#include "utilities.cpp"        // Yardımcı fonksiyonları içeriyor


// Kamera uygulamasının ana sınıfı
class CameraManager : public QMainWindow{

public:

    // Program başladığında çalışan constructor
    CameraManager(QWidget * parent = nullptr): QMainWindow(parent){

        setupIU();       // Arayüzü oluştur
        setupCamera();   // Kamerayı ayarla
    }

private:

    // Arayüz elemanları
    QWidget *central;        // Ana widget
    QVBoxLayout *layout;     // Elemanları alt alta yerleştiren layout

    // Kamera ile ilgili değişkenler
    QCamera *camera;                 // Kamera nesnesi
    QMediaCaptureSession *session;   // Kamera görüntü oturumu

    // Görüntü ile ilgili değişkenler
    QLabel *videolabel;      // Kamera görüntüsünü gösterecek alan
    QVideoSink *videoSink;   // Kameradan gelen görüntüleri yakalar

    // Diğer arayüz elemanları
    QCheckBox *bwCheckBox;   // Siyah-beyaz modu kutusu
    QImage currentFrame;     // O anki kamera görüntüsünü tutar

    QPushButton *takePicButton; // Fotoğraf çekme butonu


    // Arayüzü oluşturan fonksiyon
    void setupIU(){

        // Ana widget oluşturuluyor
        central = new QWidget(this);

        // Elemanları dikey olarak yerleştirmek için layout oluşturuluyor
        layout = new QVBoxLayout(central);


        // Kamera görüntüsünün gösterileceği QLabel oluşturuluyor
        videolabel = new QLabel("Camera Feed");

        // Kamera alanının minimum boyutu 640x480 yapılıyor
        videolabel->setMinimumSize(640,480);

        // Kamera görüntüsü gelmeden önce arka plan siyah olacak
        videolabel->setStyleSheet("background-color:black");

        // Görüntüyü ortalamak için
        videolabel->setAlignment(Qt::AlignCenter);


        // Siyah-beyaz modu için checkbox oluşturuluyor
        bwCheckBox = new QCheckBox("Black and White Mode");

        // Fotoğraf çekme butonu oluşturuluyor
        takePicButton = new QPushButton("Take Picture");


        // Kamera görüntüsünü layout'a ekle
        layout->addWidget(videolabel);

        // Siyah-beyaz checkbox'ını layout'a ekle
        layout->addWidget(bwCheckBox);

        // Fotoğraf butonunu layout'a ekle
        layout->addWidget(takePicButton);


        // Oluşturduğumuz widget'ı ana pencereye koy
        setCentralWidget(central);

        // Pencerenin başlığını belirle
        setWindowTitle("Camera App");

        // Pencerenin başlangıç boyutunu belirle
        resize(700,600);
    }


    // Kamerayı bulup çalıştıran fonksiyon
    void setupCamera(){

        // Bilgisayarın varsayılan kamerasını bul
        QCameraDevice cameraDevice =
            QMediaDevices::defaultVideoInput();


        // Eğer kamera bulunamadıysa
        if(cameraDevice.isNull()){

            // Ekrana kamera bulunamadığını belirten mesaj ekle
            layout->addWidget(
                new QLabel("No Camera Avaible on your computer")
                );

            // Kamerayı kurmadan fonksiyondan çık
            return;
        }


        // Bulduğumuz kamera ile QCamera nesnesi oluştur
        camera = new QCamera(cameraDevice);


        // Kamera görüntüsünü yönetmek için session oluştur
        session = new QMediaCaptureSession();


        // Kamerayı session'a bağla
        session->setCamera(camera);


        // Kameradan gelen görüntüleri almak için VideoSink oluştur
        videoSink = new QVideoSink();


        // VideoSink'i session'a bağla
        session->setVideoSink(videoSink);


        // Yeni bir kamera karesi geldiğinde
        // videoFrameChanged fonksiyonunu çalıştır
        connect(
            videoSink,
            &QVideoSink::videoFrameChanged,
            this,
            &CameraManager::videoFrameChanged
            );


        // Take Picture butonuna basıldığında
        // captureImage fonksiyonunu çalıştır
        connect(
            takePicButton,
            &QPushButton::clicked,
            this,
            &CameraManager::captureImage
            );


        // Kamerayı çalıştır
        camera->start();

    }


private slots :


    // Kameradan yeni bir görüntü geldiğinde bu fonksiyon çalışır
    void videoFrameChanged(const QVideoFrame &frame){

        // Gelen görüntü geçerli değilse fonksiyondan çık
        if(!frame.isValid())
            return;


        // Gelen görüntünün bir kopyasını oluştur
        QVideoFrame cloneFrame(frame);


        // Görüntüyü sadece okumak için aç
        cloneFrame.map(QVideoFrame::ReadOnly);


        // VideoFrame'i QImage'e dönüştür
        QImage image = cloneFrame.toImage();


        // Görüntü ile işimiz bitti, belleği serbest bırak
        cloneFrame.unmap();


        // Checkbox işaretliyse görüntüyü siyah-beyaz yap
        // İşaretli değilse normal görüntüyü kullan
        currentFrame =
            bwCheckBox->isChecked()
                ? toGrayscale(image)
                : image;


        // Görüntüyü QLabel üzerinde göster
        videolabel->setPixmap(
            QPixmap::fromImage(currentFrame)
                .scaled(
                    videolabel->size(),          // QLabel'ın boyutuna göre ayarla
                    Qt::KeepAspectRatio,        // Görüntünün oranını bozma
                    Qt::SmoothTransformation    // Görüntüyü daha düzgün ölçekle
                    )
            );
    }


    // Take Picture butonuna basıldığında çalışır
    void captureImage() {


        // Eğer elimizde görüntü yoksa hiçbir şey yapma
        if(currentFrame.isNull())
            return;


        // Kullanıcıdan fotoğrafı kaydedeceği yeri seçmesini iste
        QString path =
            QFileDialog::getSaveFileName(
                this,
                "Save Image",
                "",
                "Images(*png,*jpg)"
                );


        // Kullanıcı uzantı yazmadıysa otomatik olarak PNG ekle
        if(!path.endsWith(".png") &&
            !path.endsWith(".jpg") &&
            !path.endsWith(".jpeg"))

        path += ".png";


        // Kullanıcı bir dosya yolu seçtiyse
        if(!path.isEmpty()){

            // O anki kamera görüntüsünü seçilen yere kaydet
            currentFrame.save(path);
        }
    }
};
