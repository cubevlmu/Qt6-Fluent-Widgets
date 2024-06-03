#include "SmoothScroll.hpp"

namespace fluent {


    SmoothScroll::SmoothScroll(
        QScrollArea* widget,
        Qt::Orientation orient,
        QObject* parent
    ) : QObject(parent), widget(widget), orient(orient), fps(60), duration(400),
        stepsTotal(0), stepRatio(1.5), acceleration(1), lastWheelEvent(nullptr), smoothMode(SmoothMode::LINEAR)
    {
        smoothMoveTimer = new QTimer(widget);
        connect(smoothMoveTimer, &QTimer::timeout, this, &SmoothScroll::smoothMove);
    }


    void SmoothScroll::setSmoothMode(SmoothMode mode) {
        smoothMode = mode;
    }


    void SmoothScroll::wheelEvent(QWheelEvent* e) {
        int delta = e->angleDelta().y() != 0 ? e->angleDelta().y() : e->angleDelta().x();
        if (smoothMode == SmoothMode::NO_SMOOTH || std::abs(delta) % 120 != 0) {
            ((MyScrollArea*)widget)->wheelEvent(e);
            return;
        }

        qint64 now = QDateTime::currentMSecsSinceEpoch();
        scrollStamps.enqueue(now);
        while (now - scrollStamps.head() > 500) {
            scrollStamps.dequeue();
        }

        double accelerationRatio = std::min(scrollStamps.size() / 15.0, 1.0);
        lastWheelEvent = e;

        stepsTotal = fps * duration / 1000;
        delta *= stepRatio;
        if (acceleration > 0) {
            delta += delta * acceleration * accelerationRatio;
        }

        stepsLeftQueue.enqueue(QPair<int, int>(delta, stepsTotal));
        smoothMoveTimer->start(1000 / fps);
    }


    int SmoothScroll::subDelta(int delta, int stepsLeft) {
        double m = stepsTotal / 2.0;
        double x = std::abs(stepsTotal - stepsLeft - m);

        double res = 0;
        switch (smoothMode) {
        case SmoothMode::NO_SMOOTH:
            res = 0;
            break;
        case SmoothMode::CONSTANT:
            res = delta / stepsTotal;
            break;
        case SmoothMode::LINEAR:
            res = 2 * delta / stepsTotal * (m - x) / m;
            break;
        case SmoothMode::QUADRATIC:
            res = 3 / 4.0 / m * (1 - x * x / m / m) * delta;
            break;
        case SmoothMode::COSINE:
            res = (std::cos(x * M_PI / m) + 1) / (2 * m) * delta;
            break;
        }

        return res;
    }


    void SmoothScroll::smoothMove() {
        int totalDelta = 0;
        for (auto& pair : stepsLeftQueue) {
            totalDelta += subDelta(pair.first, pair.second);
            pair.second -= 1;
        }

        while (!stepsLeftQueue.isEmpty() && stepsLeftQueue.head().second == 0) {
            stepsLeftQueue.dequeue();
        }

        QPoint p;
        QScrollBar* bar;
        if (orient == Qt::Vertical) {
            p = QPoint(0, std::round(totalDelta));
            bar = widget->verticalScrollBar();
        }
        else {
            p = QPoint(std::round(totalDelta), 0);
            bar = widget->horizontalScrollBar();
        }
        /*
        QWheelEvent(const QPointF &pos, const QPointF &globalPos, QPoint pixelDelta, QPoint angleDelta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::ScrollPhase phase, bool inverted, Qt::MouseEventSource source =
        */

        QWheelEvent e(
            lastWheelEvent->position(), 
            lastWheelEvent->globalPosition(), 
            QPoint(), 
            p, 
            lastWheelEvent->buttons(),
            Qt::NoModifier,
            lastWheelEvent->phase(),
            lastWheelEvent->inverted()
        ); //TODO test it
        QApplication::sendEvent(bar, &e);

        if (stepsLeftQueue.isEmpty()) {
            smoothMoveTimer->stop();
        }
    }

} // namespace fluent