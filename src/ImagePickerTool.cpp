#include "image_picker_tool/ImagePickerTool.hpp"

namespace image_picker_tool {

ImagePickerTool::ImagePickerTool(QWidget *parent) {
    QVBoxLayout *main_layout = new QVBoxLayout();
    image_viewer_ = new ImageViewer();
    main_layout->addWidget(image_viewer_);
    setLayout(main_layout);
}

void ImagePickerTool::loadImage(const cv::Mat& src) {
    image_viewer_->loadImage(src);
}

} /* namespace image_picker_tool */
