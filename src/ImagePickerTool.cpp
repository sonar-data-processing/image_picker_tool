#include "ImagePickerTool.hpp"

#define SCENE_SIZE INT_MAX

namespace image_picker_tool {

ImagePickerTool::ImagePickerTool()
    : image_drawer_(new ImageDrawer())
    , scroll_area_(new QScrollArea())
{
    image_drawer_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    scroll_area_->setBackgroundRole(QPalette::Dark);
    scroll_area_->setWidget(image_drawer_);
    scroll_area_->viewport()->installEventFilter(this);

    connect(image_drawer_, SIGNAL(pathAppended(image_picker_tool::PathElement&)), this, SIGNAL(pathAppended(image_picker_tool::PathElement&)));
    connect(image_drawer_, SIGNAL(pointChanged(image_picker_tool::PathElement&)), this, SIGNAL(pointChanged(image_picker_tool::PathElement&)));

    setCentralWidget(scroll_area_);
}

void ImagePickerTool::loadImage(const cv::Mat& img) {
    CV_Assert(img.depth() == CV_8U && img.channels() == 3);
    QImage image = QImage((const uchar*)img.data,img.cols,img.rows, static_cast<int>(img.step), QImage::Format_RGB888);
    image = image.rgbSwapped();
    image_drawer_->setScaleFactor(1.0);
    image_drawer_->setPixmap(QPixmap::fromImage(image));
    image_drawer_->adjustSize();
    image_drawer_->update();
}

bool ImagePickerTool::eventFilter(QObject *obj, QEvent* evt) {

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

void ImagePickerTool::scaleImage(double factor) {
    Q_ASSERT(imageLabel->pixmap());

    image_drawer_->setScaleFactor(image_drawer_->scalefactor() * factor);
    image_drawer_->resize(image_drawer_->pixmap()->size() * image_drawer_->scalefactor());

    adjustScrollBar(scroll_area_->horizontalScrollBar(), factor);
    adjustScrollBar(scroll_area_->verticalScrollBar(), factor);
}

void ImagePickerTool::adjustScrollBar(QScrollBar *scrollBar, double factor) {
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImagePickerTool::saveCurrentPath() {
    image_drawer_->savePath();
}

void ImagePickerTool::removeLastPoint() {
    if (image_drawer_->current_mode_ == ImageDrawer::kAddPointMode &&
         !image_drawer_->point_list_.isEmpty()){

        image_drawer_->point_list_.pop_back();

        if (image_drawer_->point_list_.isEmpty()) {
            image_drawer_->current_mode_ = ImageDrawer::kNewPathMode;
        }

        image_drawer_->update();
    }
}

void ImagePickerTool::removeLastPath() {
    if (!image_drawer_->path_list_.isEmpty()) {
        image_drawer_->path_list_.pop_back();
        image_drawer_->update();
    }
}

void ImagePickerTool::clearPaths() {
    image_drawer_->path_list_.clear();
    image_drawer_->update();
}

void ImagePickerTool::appendPaths(const QList<PathElement>& path_list) {
    image_drawer_->path_list_.append(path_list);
}

} /* namespace image_picker_tool */
