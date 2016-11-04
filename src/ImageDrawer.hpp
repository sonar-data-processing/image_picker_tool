#ifndef image_picker_tool_ImageDrawer_hpp
#define image_picker_tool_ImageDrawer_hpp

#include <cfloat>
#include <QtGui>

namespace image_picker_tool {

struct PathElement {
    PathElement(const QList<QPointF>& path)
        : path(path)
        , userData(QString(""))
    {
    }

    PathElement(const QList<QPointF>& path, const QVariant userData)
        : path(path)
        , userData(userData)
    {
    }

    QList<QPointF> path;
    QVariant userData;
};

class ImagePickerTool;



class ImageDrawer : public QWidget {
    Q_OBJECT

public:
    ImageDrawer(QWidget *parent = 0);
    void setPixmap(const QPixmap& pixmap);
    void adjustSize();

    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    const QPixmap* pixmap() const  {
        return &pixmap_;
    }

    const double scalefactor() const {
        return scale_factor_;
    }

    void setScaleFactor(double scale_factor) {
        scale_factor_ = scale_factor;
    }

    const QList<PathElement > pathList() {
        return path_list_;
    }

    void savePath();

signals:
    void pathAppended(QList<QPointF>&, QVariant&);
    void pointChanged(const QList<QPointF>&, const QVariant&);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:

    static const int kPointRadius = 5;
    static const int kLineWidth = 2;

    enum PickerMode {
        kNewPathMode = 0,
        kEditPointMode,
        kAddPointMode
    };

    float pointDifference(const QPointF& pt0, const QPointF& pt1) {
        QPointF dt = pt1 - pt0;
        return sqrt(dt.x() * dt.x() + dt.y() * dt.y());
    }

    virtual void drawPointList(QPainter& painter, const QList<QPointF>& point_list, qreal radius, QBrush brush = QBrush(Qt::red));
    virtual void drawPath(QPainter& painter, const QList<QPointF>& points, QBrush brush = QBrush(Qt::red), bool close_path = false);
    virtual void drawPathList(QPainter& painter, const QList<PathElement>& path_list, QBrush brush = QBrush(Qt::red));
    void checkLastPoint(QList<QPointF>& point_list);
    int findMinimumPointDifference(const QList<QPointF>& list, const QPointF& pt, float& difference);
    bool findNearPoint(const QPointF& point, int& path_index, int& point_index);

    QPixmap pixmap_;
    QList<QPointF> point_list_;
    QList<PathElement> path_list_;

    double scale_factor_;
    bool is_left_button_pressed_;
    QPoint mouse_pos_;
    qint64 last_double_click_time_;

    int edit_path_index_;
    int edit_point_index_;

    PickerMode current_mode_;

    friend class ImagePickerTool;
};

} /* namespace image_picker_tool */


#endif /* ImageDrawer_hpp */
