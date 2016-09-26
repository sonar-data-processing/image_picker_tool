#include "image_picker_tool/ImageViewer.hpp"

#define SCENE_SIZE INT_MAX

namespace image_picker_tool {
    
ImageDrawer::ImageDrawer(QWidget *parent) {
    
}

void ImageDrawer::paintEvent(QPaintEvent *event) {
    if (pixmap_.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.save();
    QSize pixmap_size = pixmap_.size();

    pixmap_size.scale(size(), Qt::KeepAspectRatio);
    QPixmap scale_pixmap =  pixmap_.scaled(pixmap_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(QPoint(), scale_pixmap);
    painter.restore();
}

void ImageDrawer::setPixmap(const QPixmap& pixmap) {
    pixmap_ = pixmap;
}

void ImageDrawer::adjustSize() {
    if (!pixmap_.isNull()) resize(pixmap_.size());
}

void ImageDrawer::mouseReleaseEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton){
        printf("Point position: (%0.2f, %0.2f)\n", event.localPos().x(), event.localPos().y());
    }
}



ImageViewer::ImageViewer()
    : imageDrawer_(new ImageDrawer())
    , scrollArea_(new QScrollArea())
    , scaleFactor_(1.0)
{
    imageDrawer_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    scrollArea_->setBackgroundRole(QPalette::Dark);
    scrollArea_->setWidget(imageDrawer_);
    scrollArea_->viewport()->installEventFilter(this);
    setCentralWidget(scrollArea_);
}

void ImageViewer::loadImage(const cv::Mat& img) {
    CV_Assert(img.depth() == CV_8U && img.channels() == 3);
    QImage image = QImage((const uchar*)img.data,img.cols,img.rows, static_cast<int>(img.step), QImage::Format_RGB888);
    image = image.rgbSwapped();
    imageDrawer_->setPixmap(QPixmap::fromImage(image));
    imageDrawer_->adjustSize();
    scaleFactor_ = 1.0;
}

bool ImageViewer::eventFilter(QObject *obj, QEvent* evt) {

    if (evt->type() == QEvent::Wheel) {
        QWheelEvent *event = dynamic_cast<QWheelEvent*>(evt);

        if (event->modifiers() & Qt::ControlModifier) {
            scaleImage((event->delta() > 0) ? 1.1 : 1.0 / 1.1);
            event->ignore();
            return true;
        }
    }

    return false;
}

void ImageViewer::scaleImage(double factor) {
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor_ *= factor;
    imageDrawer_->resize(imageDrawer_->pixmap()->size() * scaleFactor_);
    adjustScrollBar(scrollArea_->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea_->verticalScrollBar(), factor);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor) {
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

} /* namespace image_picker_tool */
