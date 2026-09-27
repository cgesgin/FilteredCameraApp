#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>

class CameraManager : public QMainWindow{

public : CameraManager (QWidget * parent = nullptr): QMainWindow(parent){
        setupIU();
    }

private:
    QWidget *central;
    QVBoxLayout *layout;

    void setupIU(){
        central = new QWidget(this);
        layout = new QVBoxLayout(central);

        setCentralWidget(central);
        setWindowTitle("Camera App");
        resize(700,600);
    }
};