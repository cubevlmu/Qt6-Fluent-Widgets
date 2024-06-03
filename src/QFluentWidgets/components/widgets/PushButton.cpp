#include "PushButton.hpp"

#include "QFluentWidgets/common/StyleSheet.hpp"
#include "QFluentWidgets/common/Font.hpp"
#include "QFluentWidgets/common/Icons.hpp"
#include <QFluentWidgets/utils/OS.hpp>
#include <QDesktopServices>

#include <QPainter>
#include <QPaintEvent>
#include <QSize>
#include <QPaintDevice>
#include <QStyleOption>

#include <QDebug>

using namespace fluent;

PushButton::PushButton(QWidget* parent)
    : QPushButton(parent), m_icon(), f_icon(FluentIcon::Nil)
{
    this->setAttribute(Qt::WA_StyledBackground);
    postInit();

    FluentStyleSheet sheet(FluentStyleSheet::Type::BUTTON);
    sheet.apply(this);

    setIconSize(QSize(16, 16));
    setIcon(m_icon);

    Font::setFont(this);
}


bool PushButton::setProperty(const char* name, QVariant&& var) {
    if (strcmp(name, "icon")) {
        return QPushButton::setProperty(name, var);
    }

    setIcon(var.value<QIcon>());
    return true;
}


QIcon PushButton::icon() const {
    return m_icon;
}


void PushButton::setIcon(const QIcon& icon) {
    setProperty("hasIcon", !icon.isNull());
    setStyle(QApplication::style());
    m_icon = icon;
    update();
}


void PushButton::setIcon(const QString& icon) {
    setIcon(QIcon(icon));
}


void PushButton::setIcon(const FluentIcon& icon) {
    f_icon = icon;
    setIcon(icon.icon());
}


void PushButton::paintEvent(QPaintEvent* e)
{
    QPushButton::paintEvent(e);
    if (m_icon.isNull()) return;

    QPainter p;
    p.begin(this);

    p.setRenderHint(QPainter::RenderHint::Antialiasing, true);
    p.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, true);

    if (!isEnabled()) {
        p.setOpacity(0.3628);
    }
    else if (m_isPressed) {
        p.setOpacity(0.786);
    }

    auto size = iconSize();
    int x = 0;
    auto y = (height() - size.height()) / 2;
    auto mw = minimumSizeHint().width();
    if (mw > 0) {
        x = 12 + (width() - mw) / 2;
    }
    else {
        x = 12;
    }

    if (isRightToLeft()) {
        x = width() - size.width() - x;
    }

    selfDrawIcon(m_icon, &p, QRectF(x, y, size.width(), size.height()));
}


void PushButton::mousePressEvent(QMouseEvent* event) {
    m_isPressed = true;
    QPushButton::mousePressEvent(event);
}


void PushButton::mouseReleaseEvent(QMouseEvent* event) {
    m_isPressed = false;
    QPushButton::mouseReleaseEvent(event);
}


void PushButton::enterEvent(QEnterEvent* event) {
    m_isHover = true;
    update();
}


void PushButton::leaveEvent(QEvent* event) {
    m_isHover = false;
    update();
}


void PushButton::selfDrawIcon(
    QIcon& icon,
    QPainter* painter,
    QRectF rect,
    QIcon::State state
) {
    drawIcon(icon, painter, rect, state);
}

//////////////////////////
/// PrimaryPushButton
//////////////////////////

PrimaryPushButton::PrimaryPushButton(QWidget* parent)
    : PushButton(parent)
{

}


void PrimaryPushButton::selfDrawIcon(
    QIcon& icon,
    QPainter* painter,
    QRectF rect,
    QIcon::State state
) {
    if (!f_icon.isNull()) {
        Theme::Mode tt = os::isDarkTheme() ? Theme::Mode::Light : Theme::Mode::Dark;
        m_icon = f_icon.icon(tt);
    }
    else if (isEnabled()) {
        painter->setOpacity(fluent::os::isDarkTheme() ? 0.786 : 0.9);
        if (!f_icon.isNull())
            m_icon = f_icon.icon(Theme::Mode::Dark);
    }

    PushButton::selfDrawIcon(icon, painter, rect, state);
}

//////////////////////////
/// TransparentPushButton
//////////////////////////

TransparentPushButton::TransparentPushButton(QWidget* parent)
    : PushButton(parent)
{}

//////////////////////////
/// ToggleButton
//////////////////////////

ToggleButton::ToggleButton(QWidget* parent)
    : PushButton(parent)
{
    setCheckable(true);
    setChecked(false);
}


void ToggleButton::selfDrawIcon(
    QIcon& icon,
    QPainter* painter,
    QRectF rect,
    QIcon::State state
) {
    if (!isChecked()) {
        if (!f_icon.isNull()) {
            Theme::Mode tt = os::isDarkTheme() ? Theme::Mode::Dark : Theme::Mode::Light;
            m_icon = f_icon.icon(tt);
        }
        PushButton::selfDrawIcon(icon, painter, rect);
        return;
    }

    if (!f_icon.isNull()) {
        Theme::Mode tt = os::isDarkTheme() ? Theme::Mode::Light : Theme::Mode::Dark;
        m_icon = f_icon.icon(tt);
    }
    else if (isEnabled()) {
        painter->setOpacity(fluent::os::isDarkTheme() ? 0.786 : 0.9);
        if (!f_icon.isNull())
            m_icon = f_icon.icon(Theme::Mode::Dark);
    }

    PushButton::selfDrawIcon(icon, painter, rect, QIcon::On);
}

////////////////////////////////////
/// TransparentTogglePushButton
////////////////////////////////////

TransparentTogglePushButton::TransparentTogglePushButton(QWidget* widget)
    : TogglePushButton(widget)
{ }

////////////////////////////////////
/// HyperlinkButton
////////////////////////////////////

HyperlinkButton::HyperlinkButton(QWidget* parent)
    : PushButton(parent), m_url()
{
    setCursor(Qt::PointingHandCursor);
    QObject::connect(this, &QPushButton::clicked, this, &HyperlinkButton::onClicked);
}


void HyperlinkButton::setUrl(const QUrl& url)
{
    m_url = url;
}


void HyperlinkButton::setUrl(const QString& url)
{
    m_url = QUrl(url);
}


QUrl HyperlinkButton::getUrl() const
{
    return m_url;
}


void HyperlinkButton::selfDrawIcon(QIcon& icon, QPainter* painter, QRectF rect, QIcon::State state)
{
    if (!f_icon.isNull() && isEnabled()) {
        m_icon = f_icon.icon(Theme::Mode::Auto, themeColor());
    } else if(!isEnabled()) {
        painter->setOpacity(isDarkTheme() ? 0.786 : 0.9);
        if(!f_icon.isNull()) {
            m_icon = f_icon.icon(Theme::Mode::Dark);
        }
    }

    PushButton::selfDrawIcon(icon, painter, rect, state);
    //TODO url
}


void HyperlinkButton::onClicked(bool clicked) {
    if(!m_url.isValid()) return;
    QDesktopServices::openUrl(m_url);
}

////////////////////////////////////
/// RadioButton
////////////////////////////////////

RadioButton::RadioButton(QWidget* parent)
    : QRadioButton(parent)
{
    FluentStyleSheet sheet(FluentStyleSheet::Type::BUTTON);
    sheet.apply(this);
}


RadioButton::RadioButton(const QString& text, QWidget* parent)
    : RadioButton(parent)
{
    setText(text);
}

////////////////////////////////////
/// ToolButton
////////////////////////////////////

ToolButton::ToolButton(QWidget* parent)
    : QToolButton(parent), m_icon(), f_icon(FluentIcon::IconType::Nil)
{
    FluentStyleSheet sheet(FluentStyleSheet::Type::BUTTON);
    sheet.apply(this);

    setIconSize(QSize(16, 16));
    setIcon(m_icon);
    
    Font::setFont(this);
}


ToolButton::ToolButton(FluentIcon& icon, QWidget* parnet)
    : ToolButton(parnet)
{
    setIcon(f_icon);
}


ToolButton::ToolButton(QIcon& icon, QWidget* widget)
    : ToolButton(widget)
{
    setIcon(icon);
}


ToolButton::ToolButton(const QString& icon, QWidget* widget)
    : ToolButton(widget)
{
    setIcon(icon);
}


void ToolButton::setIcon(QIcon& icon)
{
    m_icon = icon;
}


void ToolButton::setIcon(FluentIcon& icon)
{
    f_icon = icon;
}


void ToolButton::setIcon(const QString& icon)
{
    m_icon = QIcon(icon);
}


QIcon ToolButton::icon()
{
    if(!f_icon.isNull()) return f_icon.icon();
    return m_icon;
}


bool ToolButton::setProperty(const char* name, const QVariant&& value)
{
    if(strcmp(name, "icon")) {
        return QToolButton::setProperty(name, value);
    }

    //TODO FluentIcon f_i = value.value<FluentIcon>();
    // if(!f_i.isNull()) setIcon(f_i);

    QIcon q_i = value.value<QIcon>();
    if (!q_i.isNull()) setIcon(q_i);

    QString q_s = value.value<QString>();
    if (!q_s.isEmpty()) setIcon(q_s);

    return true;
}


void ToolButton::paintEvent(QPaintEvent* e)
{
    QToolButton::paintEvent(e);
    if (!f_icon.isNull())
        m_icon = f_icon.icon();
    if (m_icon.isNull()) return;

    QPainter p;
    p.begin(this);

    p.setRenderHint(QPainter::RenderHint::Antialiasing, true);
    p.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, true);

    if (!isEnabled()) {
        p.setOpacity(0.43);
    }
    else if (m_isPressed) {
        p.setOpacity(0.63);
    }

    auto size = iconSize();
    int x = (height() - size.height()) / 2;
    int y = (width() - size.width()) / 2;
    
    selfDrawIcon(m_icon, &p, QRectF(x, y, size.width(), size.height()));
}


void ToolButton::mousePressEvent(QMouseEvent* event)
{
    m_isPressed = true;
    QToolButton::mousePressEvent(event);
}


void ToolButton::mouseReleaseEvent(QMouseEvent* event)
{
    m_isPressed = false;
    QToolButton::mouseReleaseEvent(event);
}


void ToolButton::enterEvent(QEnterEvent* event)
{
    m_isHover = true;
    QToolButton::enterEvent(event);
}


void ToolButton::leaveEvent(QEvent* event)
{
    m_isHover = false;
    QToolButton::leaveEvent(event);
}


void ToolButton::selfDrawIcon(QIcon& icon, QPainter* painter, QRectF rect, QIcon::State state)
{
    drawIcon(icon, painter, rect, state);
}

////////////////////////////////////
/// TransparentToolButton
////////////////////////////////////

TransparentToolButton::TransparentToolButton(QWidget* widget)
    : ToolButton(widget)
{ }


TransparentToolButton::TransparentToolButton(FluentIcon& icon, QWidget* parnet)
    : ToolButton(icon, parnet)
{ }


TransparentToolButton::TransparentToolButton(QIcon& icon, QWidget* widget)
    : ToolButton(icon, widget)
{ }


TransparentToolButton::TransparentToolButton(const QString& icon, QWidget* widget)
    : ToolButton(icon, widget)
{ }