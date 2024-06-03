#pragma once

#include <QApplication>
#include <QScrollArea>
#include <QAbstractScrollArea>
#include <QWheelEvent>
#include <QTimer>
#include <QDateTime>
#include <QPoint>
#include <QQueue>
#include <QScrollBar>
#include <cmath>


namespace fluent {

    enum class SmoothMode {
        NO_SMOOTH = 0,
        CONSTANT,
        LINEAR,
        QUADRATIC,
        COSINE
    };


    class MyScrollArea : public QScrollArea {
    public:
        void wheelEvent(QWheelEvent *e) override {
            QAbstractScrollArea::wheelEvent(e);
        }
    };


    class SmoothScroll : public QObject {
        Q_OBJECT

    public:
        SmoothScroll(QScrollArea* widget, Qt::Orientation orient = Qt::Vertical, QObject* parent = nullptr);

        void setSmoothMode(SmoothMode mode);
        void wheelEvent(QWheelEvent* e);

    private slots:
        void smoothMove();

    private:
        int subDelta(int delta, int stepsLeft);

        QScrollArea* widget;
        Qt::Orientation orient;
        int fps;
        int duration;
        int stepsTotal;
        double stepRatio;
        int acceleration;
        QWheelEvent* lastWheelEvent;
        QQueue<qint64> scrollStamps;
        QQueue<QPair<int, int>> stepsLeftQueue;
        QTimer* smoothMoveTimer;
        SmoothMode smoothMode;
    };
}