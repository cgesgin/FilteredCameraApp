#include <QImage>

static QImage toGrayscale(const QImage &img){
    return img.convertToFormat(QImage::Format_Grayscale8);
}