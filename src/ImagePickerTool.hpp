#ifndef image_picker_tool_ImagePickerTool_hpp
#define image_picker_tool_ImagePickerTool_hpp

#include <QtGui>
#include <QMainWindow>
#include <QApplication>
#include <opencv2/opencv.hpp>
#include "ImageDrawer.hpp"

namespace image_picker_tool {

class ImagePickerTool : public QMainWindow {
    Q_OBJECT

public:
    explicit ImagePickerTool();
    void loadImage(const cv::Mat& source);
    void appendPaths(const QList<QList<QPointF> >& path_list, const QList<QVariant>& user_data_list);

    void saveCurrentPath();
    void removeLastPoint();
    void removeLastPath();
    void clearPaths();

    const ImageDrawer* imageDrawer() const {
        return image_drawer_;
    }

signals:
    void pathAppended(QList<QPointF>&, QVariant&);
    void pointChanged(const QList<QPointF>&, const QVariant&);

protected:
    virtual bool eventFilter(QObject *obj, QEvent* evt);

private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QScrollArea *scroll_area_;
    ImageDrawer *image_drawer_;

};

}

#endif /* image_picker_tool_ImagePickerTool_hpp */
