#include "ImageDrawer.hpp"

namespace image_picker_tool {

ImageDrawer::ImageDrawer(QWidget *parent)
    : scale_factor_(1.0)
    , is_left_button_pressed_(false)
    , last_double_click_time_(0)
    , current_mode_(kNewPathMode)
    , edit_path_index_(-1)
    , edit_point_index_(-1)
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

    if (!point_list_.isEmpty()) {
        drawPath(painter, point_list_);
        drawPointList(painter, point_list_, kPointRadius);
        if (is_left_button_pressed_ &&
            mouse_pos_ != QPoint(-1, -1)) {
            painter.save();
            painter.setPen(QPen(QBrush(Qt::gray), 2));
            const QPointF& last_point = point_list_.last();
            painter.drawLine(last_point * scale_factor_, mouse_pos_);
            painter.restore();
        }
    }

    if (!path_list_.isEmpty()) {
        drawPathList(painter, path_list_, QBrush(Qt::green));
    }

    if (edit_path_index_ != -1 && edit_point_index_ != -1) {
        painter.save();
        painter.setPen(QPen(Qt::blue, 2));
        painter.setBrush(Qt::green);
        float radius = kPointRadius + 3;
        painter.drawEllipse(path_list_[edit_path_index_].path[edit_point_index_] * scale_factor_, radius, radius);
        painter.restore();
    }

}

void ImageDrawer::drawPointList(QPainter& painter, const QList<QPointF>& point_list, qreal radius, QBrush brush) {
    painter.save();
    painter.setPen(QPen(brush, 1));
    painter.setBrush(brush);
    QList<QPointF>::const_iterator it;
    for (it = point_list.begin(); it != point_list.end(); it++) {
        painter.drawEllipse((*it) * scale_factor_, radius, radius);
    }
    painter.restore();
}

void ImageDrawer::drawPath(QPainter& painter, const QList<QPointF>& points, QBrush brush, bool close_path) {
    painter.save();
    painter.setPen(QPen(brush, kLineWidth));
    if (points.length() >= 2) {
        QPainterPath path;

        for (size_t i = 0; i < points.length()-1; i++) {
            path.moveTo(points.at(i)*scale_factor_);
            path.lineTo(points.at(i+1)*scale_factor_);
        }

        if (close_path) {
            path.moveTo(points.last()*scale_factor_);
            path.lineTo(points.first()*scale_factor_);
        }

        painter.drawPath(path);
    }
    painter.restore();
}

void ImageDrawer::drawPathList(QPainter& painter, const QList<PathElement>& path_list, QBrush brush) {
    QList<PathElement>::const_iterator it;
    for (it = path_list.begin(); it != path_list.end(); it++) {
        drawPath(painter, (*it).path, brush, true);
        drawPointList(painter, (*it).path, kPointRadius, brush);
    }
}

void ImageDrawer::setPixmap(const QPixmap& pixmap) {
    pixmap_ = pixmap;
}

void ImageDrawer::adjustSize() {
    if (!pixmap_.isNull()) resize(pixmap_.size());
}

void ImageDrawer::mouseReleaseEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {

        qint64 dt = QDateTime::currentMSecsSinceEpoch() - last_double_click_time_;

        if (dt > 250) {
            is_left_button_pressed_ = false;
            mouse_pos_ = QPoint(-1, -1);

            if (current_mode_ == kAddPointMode) {
                double xf = event->pos().x() / scale_factor_;
                double yf = event->pos().y() / scale_factor_;
                QPointF point = QPointF(xf, yf);
                float point_difference;
                int index = findMinimumPointDifference(point_list_, point, point_difference);

                if(point_difference > kPointRadius || index == -1) {
                    point_list_ << point;
                }
            }
            else if (current_mode_ == kEditPointMode) {
                PathElement path_element = path_list_[edit_path_index_];
                edit_path_index_ = -1;
                edit_point_index_ = -1;
                current_mode_ = kNewPathMode;
                emit pointChanged(path_element);
            }

            update();
        }
    }
}

void ImageDrawer::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        is_left_button_pressed_ = true;
        mouse_pos_ = event->pos();

        if (current_mode_ == kNewPathMode) {
            if (path_list_.size() > 0) {
                double xf = event->pos().x() / scale_factor_;
                double yf = event->pos().y() / scale_factor_;
                QPointF point = QPointF(xf, yf);

                edit_path_index_ = -1;
                edit_point_index_ = -1;

                if (findNearPoint(point, edit_path_index_, edit_point_index_)) {
                    current_mode_ = kEditPointMode;
                    update();
                    return;
                }

            }
            current_mode_ = kAddPointMode;
        }

        update();
    }
}

void ImageDrawer::mouseMoveEvent(QMouseEvent *event) {

    if ((event->buttons() & Qt::LeftButton) && is_left_button_pressed_) {
        mouse_pos_ = event->pos();

        if (current_mode_ == kEditPointMode) {

            if (edit_path_index_ != -1 && edit_point_index_ != -1) {
                double xf = event->pos().x() / scale_factor_;
                double yf = event->pos().y() / scale_factor_;
                path_list_[edit_path_index_].path[edit_point_index_] = QPointF(xf, yf);
            }
        }

        update();
    }
}

void ImageDrawer::mouseDoubleClickEvent(QMouseEvent * event) {
    if (event->buttons() == Qt::LeftButton) {
        if (point_list_.size() > 2 && current_mode_ == kAddPointMode) {
            last_double_click_time_ = QDateTime::currentMSecsSinceEpoch();
            checkLastPoint(point_list_);
            savePath();
        }
    }
}

void ImageDrawer::checkLastPoint(QList<QPointF>& point_list) {
    QPointF first = point_list.first();
    QPointF last = point_list.last();
    if (first != last && pointDifference(first, last) < kPointRadius) {
        point_list.pop_back();
    }
}

void ImageDrawer::savePath() {
    if (point_list_.size() > 2) {
        path_list_ << PathElement(point_list_);
        point_list_.clear();
        current_mode_ = kNewPathMode;
        update();
        emit pathAppended(path_list_.last());
    }
}


int ImageDrawer::findMinimumPointDifference(const QList<QPointF>& list, const QPointF& pt, float& minimumDifference) {
    int minimumIndex = -1;
    minimumDifference = FLT_MAX;
    for (size_t index = 0; index < list.size(); index++) {
        float difference = pointDifference(pt, list.value(index));
        if (difference < minimumDifference) {
            minimumDifference = difference;
            minimumIndex = index;
        }
    }
    return minimumIndex;
}

bool ImageDrawer::findNearPoint(const QPointF& point, int& path_index, int& point_index) {
    for (size_t current_path_index = 0;
          current_path_index < path_list_.size();
          current_path_index++) {

        float point_difference;
        int index = findMinimumPointDifference(path_list_[current_path_index].path, point, point_difference);

        if (point_difference < (kPointRadius+2) && index != -1) {
            path_index = current_path_index;
            point_index = index;
            return true;
        }
    }

    return false;
}

} /*  namespace image_picker_tool */
