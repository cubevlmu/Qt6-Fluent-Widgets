#pragma once

#include <QApplication>
#include <QWidget>
#include <qobjectdefs.h>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include <QLineEdit>
#include <QPoint>
#include <QEasingCurve>
#include <QColor>
#include <QVariant>
#include <QObject>
#include <QEvent>


namespace fluent {

    // Enums for FluentAnimation
    enum class FluentAnimationSpeed {
        FAST = 0,
        MEDIUM = 1,
        SLOW = 2
    };


    enum class FluentAnimationType {
        FAST_INVOKE = 0,
        STRONG_INVOKE = 1,
        FAST_DISMISS = 2,
        SOFT_DISMISS = 3,
        POINT_TO_POINT = 4,
        FADE_IN_OUT = 5
    };


    enum class FluentAnimationProperty {
        POSITION,
        SCALE,
        ANGLE,
        OPACITY
    };


    inline const char* getString(FluentAnimationProperty pp) {
        switch (pp) {
        case FluentAnimationProperty::POSITION: return "position";
        case FluentAnimationProperty::SCALE:    return "scale";
        case FluentAnimationProperty::ANGLE:    return "angle";
        case FluentAnimationProperty::OPACITY:  return "opacity";
        }
    }


    class AnimationBase : public QObject {
        Q_OBJECT

    public:
        explicit AnimationBase(QWidget* parent = nullptr);

    protected:
        virtual void onHover(QEnterEvent*) {}
        virtual void onLeave(QEvent*) {}
        virtual void onPress(QMouseEvent*) {}
        virtual void onRelease(QMouseEvent*) {}

        bool eventFilter(QObject* obj, QEvent* e) override;
    };


    class TranslateYAnimation : public AnimationBase {
        Q_OBJECT;
        Q_PROPERTY(float y READ y WRITE setY NOTIFY valueChanged);

    public:
        explicit TranslateYAnimation(QWidget* parent = nullptr, int offset = 2);

        float y() const;
        void setY(float y);

    signals:
        void valueChanged(float);

    protected:
        void onPress(QMouseEvent* e) override;
        void onRelease(QMouseEvent* e) override;

    private:
        float m_y;
        int maxOffset;
        QPropertyAnimation* ani;
    };


    class BackgroundColorObject : public QObject {
        Q_OBJECT;
        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor);

    public:
        explicit BackgroundColorObject(QObject* parent = nullptr);

        QColor backgroundColor() const;
        void setBackgroundColor(const QColor& color);

    private:
        QColor m_backgroundColor;
        QWidget* parentWidget() const;
    };


    class BackgroundAnimationWidget : public QWidget {
        Q_OBJECT

    public:
        explicit BackgroundAnimationWidget(QWidget* parent = nullptr);

    protected:
        bool eventFilter(QObject* obj, QEvent* e) override;
        void mousePressEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void enterEvent(QEnterEvent* e) override; //TODO check it
        void leaveEvent(QEvent* e) override;
        void focusInEvent(QFocusEvent* e) override;

    private:
        bool isHover;
        bool isPressed;
        BackgroundColorObject* bgColorObject;
        QPropertyAnimation* backgroundColorAni;

        QColor normalBackgroundColor() const {
            return QColor(0, 0, 0, 0);
        }

        QColor hoverBackgroundColor() const {
            return normalBackgroundColor();
        }

        QColor pressedBackgroundColor() const {
            return normalBackgroundColor();
        }

        QColor focusInBackgroundColor() const {
            return normalBackgroundColor();
        }

        QColor disabledBackgroundColor() const {
            return normalBackgroundColor();
        }

        void updateBackgroundColor();

        QColor getBackgroundColor() const {
            return bgColorObject->backgroundColor();
        }

        void setBackgroundColor(const QColor& color) {
            bgColorObject->setBackgroundColor(color);
        }

    public:
        Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor);
    };


    class DropShadowAnimation : public QPropertyAnimation {
        Q_OBJECT

    public:
        explicit DropShadowAnimation(QWidget* parent = nullptr, const QColor& normalColor = QColor(0, 0, 0, 0), const QColor& hoverColor = QColor(0, 0, 0, 75));

        void setBlurRadius(int radius) {
            blurRadius = radius;
        }

        void setOffset(int dx, int dy) {
            offset = QPoint(dx, dy);
        }

        void setNormalColor(const QColor& color) {
            normalColor = color;
        }

        void setHoverColor(const QColor& color) {
            hoverColor = color;
        }

    protected:
        bool eventFilter(QObject* obj, QEvent* e) override;

    private slots:
        void onAniFinished();

    private:
        QColor normalColor;
        QColor hoverColor;
        QPoint offset;
        int blurRadius;
        bool isHover;
        QGraphicsDropShadowEffect* shadowEffect;

        QGraphicsDropShadowEffect* createShadowEffect();
    };


    class FluentAnimation : public QPropertyAnimation
    {
        Q_OBJECT
    public:
        explicit FluentAnimation(QWidget* parent = nullptr);

        static QEasingCurve createBezierCurve(qreal x1, qreal y1, qreal x2, qreal y2);
        static QEasingCurve curve();

        void setSpeed(FluentAnimationSpeed speed);
        int speedToDuration(FluentAnimationSpeed speed) const;

        void startAnimation(const QVariant& endValue, const QVariant& startValue = QVariant());

        QVariant value() const;
        void setValue(const QVariant& value);

        template <typename T>
        static FluentAnimation* create(FluentAnimationType aniType, FluentAnimationProperty propertyType,
            FluentAnimationSpeed speed = FluentAnimationSpeed::FAST, const QVariant& value = QVariant(), QWidget* parent = nullptr) {
            T* obj = new T(parent);
            FluentAnimation* ani = new FluentAnimation(parent);

            ani->setSpeed(speed);
            ani->setTargetObject(obj);
            ani->setPropertyName(getString(propertyType));

            if (value.isValid()) {
                ani->setValue(value);
            }

            return ani;
        }
    };


    // Implementation for the custom properties
    class PositionObject : public QObject {
        Q_OBJECT;
        Q_PROPERTY(QPoint position READ position WRITE setPosition NOTIFY positionChanged);

    public:
        explicit PositionObject(QObject* parent = nullptr);

        QPoint position() const;
        void setPosition(const QPoint& pos);

    signals:
        void positionChanged();

    private:
        QPoint m_position;
    };


    class ScaleObject : public QObject {
        Q_OBJECT;
        Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged);

    public:
        explicit ScaleObject(QObject* parent = nullptr);

        float scale() const;
        void setScale(float scale);

    signals:
        void scaleChanged();

    private:
        float m_scale;
    };


    class AngleObject : public QObject {
        Q_OBJECT;
        Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged);

    public:
        explicit AngleObject(QObject* parent = nullptr);

        float angle() const;
        void setAngle(float angle);

    signals:
        void angleChanged();

    private:
        float m_angle;
    };


    class OpacityObject : public QObject {
        Q_OBJECT;
        Q_PROPERTY(float opacity READ opacity WRITE setOpacity NOTIFY opacityChanged);

    public:
        explicit OpacityObject(QObject* parent = nullptr);

        float opacity() const;
        void setOpacity(float opacity);

    signals:
        void opacityChanged();

    private:
        float m_opacity;
    };


}