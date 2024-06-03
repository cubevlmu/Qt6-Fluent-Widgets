#include "Animations.hpp"

namespace fluent {


    AnimationBase::AnimationBase(QWidget* parent)
        : QObject(parent)
    {
        parent->installEventFilter(this);
    }


    bool AnimationBase::eventFilter(QObject* obj, QEvent* e)
    {
        if (obj == parent()) {
            switch (e->type()) {
            case QEvent::MouseButtonPress:
                onPress(static_cast<QMouseEvent*>(e));
                break;
            case QEvent::MouseButtonRelease:
                onRelease(static_cast<QMouseEvent*>(e));
                break;
            case QEvent::Enter:
                onHover(static_cast<QEnterEvent*>(e));
                break;
            case QEvent::Leave:
                onLeave(e);
                break;
            default:
                break;
            }
        }
        return QObject::eventFilter(obj, e);
    }



    TranslateYAnimation::TranslateYAnimation(QWidget* parent, int offset)
        : AnimationBase(parent), m_y(0), maxOffset(offset)
    {
        ani = new QPropertyAnimation(this, "y", this);
    }


    float TranslateYAnimation::y() const {
        return m_y;
    }


    void TranslateYAnimation::setY(float y) {
        m_y = y;
        ((QWidget*)parent())->update();
        emit valueChanged(y);
    }


    void TranslateYAnimation::onPress(QMouseEvent* e) {
        ani->setEndValue(maxOffset);
        ani->setEasingCurve(QEasingCurve::OutQuad);
        ani->setDuration(150);
        ani->start();
    }


    void TranslateYAnimation::onRelease(QMouseEvent* e) {
        ani->setEndValue(0);
        ani->setEasingCurve(QEasingCurve::OutElastic);
        ani->setDuration(500);
        ani->start();
    }



    BackgroundColorObject::BackgroundColorObject(QObject* parent)
        : QObject(parent), m_backgroundColor(Qt::transparent)
    {}


    QColor BackgroundColorObject::backgroundColor() const {
        return m_backgroundColor;
    }


    void BackgroundColorObject::setBackgroundColor(const QColor& color) {
        m_backgroundColor = color;
        if (parentWidget())
            parentWidget()->update();
    }


    QWidget* BackgroundColorObject::parentWidget() const {
        return qobject_cast<QWidget*>(parent());
    }



    BackgroundAnimationWidget::BackgroundAnimationWidget(QWidget* parent)
        : QWidget(parent), isHover(false), isPressed(false),
        bgColorObject(new BackgroundColorObject(this)),
        backgroundColorAni(new QPropertyAnimation(bgColorObject, "backgroundColor", this))
    {
        backgroundColorAni->setDuration(120);
        installEventFilter(this);
    }


    bool BackgroundAnimationWidget::eventFilter(QObject* obj, QEvent* e) {
        if (obj == this) {
            switch (e->type()) {
            case QEvent::EnabledChange:
                updateBackgroundColor();
                break;
            default:
                break;
            }
        }
        return QWidget::eventFilter(obj, e);
    }


    void BackgroundAnimationWidget::mousePressEvent(QMouseEvent* e) {
        isPressed = true;
        updateBackgroundColor();
        QWidget::mousePressEvent(e);
    }


    void BackgroundAnimationWidget::mouseReleaseEvent(QMouseEvent* e) {
        isPressed = false;
        updateBackgroundColor();
        QWidget::mouseReleaseEvent(e);
    }


    void BackgroundAnimationWidget::enterEvent(QEnterEvent* e) { //TODO check it
        isHover = true;
        updateBackgroundColor();
    }


    void BackgroundAnimationWidget::leaveEvent(QEvent* e) {
        isHover = false;
        updateBackgroundColor();
    }


    void BackgroundAnimationWidget::focusInEvent(QFocusEvent* e) {
        QWidget::focusInEvent(e);
        updateBackgroundColor();
    }


    void BackgroundAnimationWidget::updateBackgroundColor() {
        QColor color;
        if (!isEnabled()) {
            color = disabledBackgroundColor();
        }
        else if (dynamic_cast<QLineEdit*>(this) && hasFocus()) {
            color = focusInBackgroundColor();
        }
        else if (isPressed) {
            color = pressedBackgroundColor();
        }
        else if (isHover) {
            color = hoverBackgroundColor();
        }
        else {
            color = normalBackgroundColor();
        }

        backgroundColorAni->stop();
        backgroundColorAni->setEndValue(color);
        backgroundColorAni->start();
    }


    DropShadowAnimation::DropShadowAnimation(
        QWidget* parent,
        const QColor& normalColor,
        const QColor& hoverColor
    ) : QPropertyAnimation(parent), normalColor(normalColor), hoverColor(hoverColor),
        offset(0, 0), blurRadius(38), isHover(false)
    {
        shadowEffect = new QGraphicsDropShadowEffect(this);
        shadowEffect->setColor(normalColor);
        parent->installEventFilter(this);
    }


    bool DropShadowAnimation::eventFilter(QObject* obj, QEvent* e) {
        if (obj == parent() && ((QWidget*)parent())->isEnabled()) {
            switch (e->type()) {
            case QEvent::Enter:
                isHover = true;
                if (state() != QPropertyAnimation::Running)
                    ((QWidget*)parent())->setGraphicsEffect(createShadowEffect());
                setEndValue(hoverColor);
                start();
                break;
            case QEvent::Leave:
            case QEvent::MouseButtonPress:
                isHover = false;
                if (((QWidget*)parent())->graphicsEffect()) {
                    connect(this, &QPropertyAnimation::finished, this, &DropShadowAnimation::onAniFinished);
                    setEndValue(normalColor);
                    start();
                }
                break;
            default:
                break;
            }
        }
        return QPropertyAnimation::eventFilter(obj, e);
    }


    QGraphicsDropShadowEffect* DropShadowAnimation::createShadowEffect() {
        shadowEffect = new QGraphicsDropShadowEffect(this);
        shadowEffect->setOffset(offset);
        shadowEffect->setBlurRadius(blurRadius);
        shadowEffect->setColor(normalColor);

        setTargetObject(shadowEffect);
        setStartValue(shadowEffect->color());
        setPropertyName("color");
        setDuration(150);

        return shadowEffect;
    }


    void DropShadowAnimation::onAniFinished() {
        disconnect(this, &QPropertyAnimation::finished, this, &DropShadowAnimation::onAniFinished);
        shadowEffect = nullptr;
        ((QWidget*)parent())->setGraphicsEffect(nullptr);
    }



    FluentAnimation::FluentAnimation(QWidget* parent)
        : QPropertyAnimation(parent) {
        setSpeed(FluentAnimationSpeed::FAST);
        setEasingCurve(curve());
    }


    QEasingCurve FluentAnimation::createBezierCurve(
        qreal x1,
        qreal y1,
        qreal x2,
        qreal y2
    ) {
        QEasingCurve curve(QEasingCurve::BezierSpline);
        curve.addCubicBezierSegment(QPointF(x1, y1), QPointF(x2, y2), QPointF(1, 1));
        return curve;
    }


    QEasingCurve FluentAnimation::curve() {
        return createBezierCurve(0, 0, 1, 1);
    }


    void FluentAnimation::setSpeed(FluentAnimationSpeed speed) {
        setDuration(speedToDuration(speed));
    }


    int FluentAnimation::speedToDuration(FluentAnimationSpeed speed) const {
        return 100;
    }


    void FluentAnimation::startAnimation(
        const QVariant& endValue,
        const QVariant& startValue
    ) {
        stop();

        if (startValue.isNull()) {
            setStartValue(value());
        }
        else {
            setStartValue(startValue);
        }

        setEndValue(endValue);
        start();
    }


    QVariant FluentAnimation::value() const {
        return targetObject()->property("value");
    }


    void FluentAnimation::setValue(const QVariant& value) {
        targetObject()->setProperty("value", value);
    }


    PositionObject::PositionObject(QObject* parent)
        : QObject(parent)
    {}


    QPoint PositionObject::position() const {
        return m_position;
    }


    void PositionObject::setPosition(const QPoint& pos) {
        if (m_position != pos) {
            m_position = pos;
            emit positionChanged();
        }
    }



    ScaleObject::ScaleObject(QObject* parent)
        : QObject(parent)
    {}


    float ScaleObject::scale() const {
        return m_scale;
    }


    void ScaleObject::setScale(float scale) {
        if (!qFuzzyCompare(m_scale, scale)) {
            m_scale = scale;
            emit scaleChanged();
        }
    }



    AngleObject::AngleObject(QObject* parent)
        : QObject(parent)
    {}


    float AngleObject::angle() const {
        return m_angle;
    }


    void AngleObject::setAngle(float angle) {
        if (!qFuzzyCompare(m_angle, angle)) {
            m_angle = angle;
            emit angleChanged();
        }
    }



    OpacityObject::OpacityObject(QObject* parent)
        : QObject(parent), m_opacity(1.0f)
    {}


    float OpacityObject::opacity() const {
        return m_opacity;
    }


    void OpacityObject::setOpacity(float opacity) {
        if (!qFuzzyCompare(m_opacity, opacity)) {
            m_opacity = opacity;
            emit opacityChanged();
        }
    }

} // namespace fluent