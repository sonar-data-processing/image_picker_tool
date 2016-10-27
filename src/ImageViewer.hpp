#ifndef image_picker_tool_ImageViewer_hpp
#define image_picker_tool_ImageViewer_hpp

#include <memory>
#include <iostream>
#include <QtGui>
#include <QMainWindow>
#include <QApplication>
#include <opencv2/opencv.hpp>

namespace image_picker_tool {

class ImageDrawer : public QWidget {
    Q_OBJECT

public:
    ImageDrawer(QWidget *parent = 0);
    void setPixmap(const QPixmap& pixmap);
    void adjustSize();

    void mouseReleaseEvent(QMouseEvent * event);

    const QPixmap* pixmap() const  {
        return &pixmap_;
    }

    const double scalefactor() const {
        return scale_factor_;
    }

    void setScaleFactor(double scale_factor) {
        scale_factor_ = scale_factor;
    }

protected:
    virtual void paintEvent(QPaintEvent *event);

private:

    QPixmap pixmap_;
    std::vector<QPointF> points_;
    double scale_factor_;
};

class ImageViewer : public QMainWindow {
    Q_OBJECT

public:
    explicit ImageViewer();
    void loadImage(const cv::Mat& source);

protected:
    virtual bool eventFilter(QObject *obj, QEvent* evt);

private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QScrollArea *scrollArea_;
    ImageDrawer *imageDrawer_;
};

}

#endif /* image_picker_tool_ImageViewer_hpp */
