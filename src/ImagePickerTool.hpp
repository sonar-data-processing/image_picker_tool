#ifndef image_picker_tool_ImagePicker_hpp
#define image_picker_tool_ImagePicker_hpp

#include <QtGui>
#include <opencv2/opencv.hpp>
#include "image_picker_tool/ImageViewer.hpp"

namespace image_picker_tool {

class ImagePickerTool : public QWidget {
    Q_OBJECT
public:
    explicit ImagePickerTool(QWidget *parent = 0);
    void loadImage(const cv::Mat& src);
    
private:
    ImageViewer *image_viewer_;
};
    
} /* namespace image_picker_tool */

#endif /* image_picker_tool_ImagePicker_hpp */
