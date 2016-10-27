#include "ImageViewer.hpp"

#define SCENE_SIZE INT_MAX

namespace image_picker_tool {

ImageDrawer::ImageDrawer(QWidget *parent)
    : scale_factor_(1.0)
{
}

void ImageDrawer::paintEvent(QPaintEvent *event) {
    if (pixmap_.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.save();
    painter.scale(scale_factor_, scale_factor_);
    painter.drawPixmap(QPoint(), pixmap_);
    painter.restore();
}

void ImageDrawer::setPixmap(const QPixmap& pixmap) {
    pixmap_ = pixmap;
}

void ImageDrawer::adjustSize() {
    if (!pixmap_.isNull()) resize(pixmap_.size());
}

void ImageDrawer::mouseReleaseEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {
    }
}



ImageViewer::ImageViewer()
    : imageDrawer_(new ImageDrawer())
    , scrollArea_(new QScrollArea())
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
    imageDrawer_->setScaleFactor(1.0);
    imageDrawer_->setPixmap(QPixmap::fromImage(image));
    imageDrawer_->adjustSize();
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

    imageDrawer_->setScaleFactor(imageDrawer_->scalefactor() * factor);
    imageDrawer_->resize(imageDrawer_->pixmap()->size() * imageDrawer_->scalefactor());

    adjustScrollBar(scrollArea_->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea_->verticalScrollBar(), factor);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor) {
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

} /* namespace image_picker_tool */
