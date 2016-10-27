#include <iostream>
#include <QApplication>
#include "ImagePickerTool.hpp"

using namespace image_picker_tool;

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    const std::string image_path = "/images/gemini-jequitaia.0-1.png";

    cv::Mat img = cv::imread(image_path.c_str(), 1);

    ImagePickerTool image_picker_tool;
    image_picker_tool.loadImage(img);
    image_picker_tool.show();
    
    return app.exec();
}
