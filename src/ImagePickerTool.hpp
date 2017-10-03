#ifndef image_picker_tool_ImagePickerTool_hpp
#define image_picker_tool_ImagePickerTool_hpp

#include <QtGui>
#include <QMainWindow>
#include <QApplication>
#include <opencv2/opencv.hpp>
#include "ImageDrawer.hpp"

namespace image_picker_tool {

class CustomScrollArea;

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

    void setSelected(int selected_index) {
        image_drawer_->selected_index_ = selected_index;
        image_drawer_->update();
    }

    void removePath(int index) {
        image_drawer_->path_list_.removeAt(index);
        image_drawer_->update();
    }

    int findIndexByUserData(const QVariant& user_data);

signals:
    void pathAppended(QList<QPointF>&, QVariant&);
    void pointChanged(const QList<QPointF>&, const QVariant&, QBool&);
    void pointAppened(const QPointF&, QBool&);

protected:
    virtual bool eventFilter(QObject *obj, QEvent* evt);

private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    bool processScrollKeyEventFilter(QKeyEvent *event);

    QScrollArea *scroll_area_;
    ImageDrawer *image_drawer_;

    friend class CustomScrollArea;

};

class CustomScrollArea : public QScrollArea  {
public:
    CustomScrollArea(ImagePickerTool *i) {
        imagePickerTool = i;
    }

protected:

    void keyPressEvent(QKeyEvent *event) {
        if (!imagePickerTool->processScrollKeyEventFilter(event)) {
            return;
        }

        QScrollArea::keyPressEvent(event);
    }

private:
    ImagePickerTool *imagePickerTool;

};

}

#endif /* image_picker_tool_ImagePickerTool_hpp */
