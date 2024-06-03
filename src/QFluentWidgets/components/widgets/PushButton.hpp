#pragma once

#include <QPushButton>
#include <QRadioButton>
#include <QToolButton>
#include "QFluentWidgets/common/Icons.hpp"

using namespace fluent;

class Q_WIDGETS_EXPORT PushButton : public QPushButton
{
    friend class PrimaryPushButton;
    friend class ToggleButton;
    friend class HyperlinkButton;
    Q_OBJECT

public:
    explicit PushButton(QWidget* parent = nullptr);

    bool setProperty(const char* name, QVariant&& var);
    QIcon icon() const;

    void setIcon(const QIcon& icon);
    void setIcon(const QString& icon);
    void setIcon(const FluentIcon& icon);

protected:
    virtual void postInit() {}

    virtual void paintEvent(QPaintEvent*) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

    virtual void selfDrawIcon(
        QIcon& icon, 
        QPainter* painter, 
        QRectF rect,
        QIcon::State state = QIcon::Off
    );

private:
    bool m_isPressed = false;
    bool m_isHover = false;
    QIcon m_icon;
    FluentIcon f_icon;
};

//////////////////////////
/// PrimaryPushButton
//////////////////////////

class PrimaryPushButton : public PushButton
{
    Q_OBJECT
public:
    PrimaryPushButton(QWidget* parent = nullptr);

protected:
    virtual void selfDrawIcon(
        QIcon& icon, 
        QPainter* painter, 
        QRectF rect,
        QIcon::State state = QIcon::Off
    ) override;
};

//////////////////////////
/// TransparentPushButton
//////////////////////////

class TransparentPushButton : public PushButton 
{
    Q_OBJECT
public:
    TransparentPushButton(QWidget* parent = nullptr);
};

//////////////////////////
/// ToggleButton
//////////////////////////

class ToggleButton : public PushButton 
{
    Q_OBJECT
public:
    ToggleButton(QWidget* parent = nullptr);

protected:
    virtual void selfDrawIcon(
        QIcon& icon, 
        QPainter* painter, 
        QRectF rect,
        QIcon::State state = QIcon::Off
    ) override;
};
using TogglePushButton = ToggleButton;

////////////////////////////////////
/// TransparentTogglePushButton
////////////////////////////////////

class TransparentTogglePushButton : public TogglePushButton 
{
    Q_OBJECT
public:
    TransparentTogglePushButton(QWidget* widget = nullptr);
};

////////////////////////////////////
/// HyperlinkButton
////////////////////////////////////

class HyperlinkButton : public PushButton
{
    Q_OBJECT
public:
    HyperlinkButton(QWidget* parent = nullptr);

    void setUrl(const QUrl& url);
    void setUrl(const QString& url);
    QUrl getUrl() const;

private slots:
    void onClicked(bool clicked);

protected:
    virtual void selfDrawIcon(
        QIcon& icon, 
        QPainter* painter, 
        QRectF rect,
        QIcon::State state = QIcon::Off
    ) override;

private:
    QUrl m_url;
};

////////////////////////////////////
/// RadioButton
////////////////////////////////////

class RadioButton : public QRadioButton 
{
    Q_OBJECT
public:
    RadioButton(QWidget* parent = nullptr);
    RadioButton(const QString& text, QWidget* parent = nullptr);
};

////////////////////////////////////
/// ToolButton
////////////////////////////////////

class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    ToolButton(QWidget* parent = nullptr);
    ToolButton(FluentIcon& icon, QWidget* parnet = nullptr);
    ToolButton(QIcon& icon, QWidget* widget = nullptr);
    ToolButton(const QString& icon, QWidget* widget = nullptr);

    void setIcon(QIcon& icon);
    void setIcon(FluentIcon& icon);
    void setIcon(const QString& icon);

    QIcon icon();

    bool setProperty(const char *name, const QVariant &&value);

protected: 
    virtual void paintEvent(QPaintEvent*) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

    virtual void selfDrawIcon(
        QIcon& icon, 
        QPainter* painter, 
        QRectF rect,
        QIcon::State state = QIcon::Off
    );

private:
    bool m_isPressed;
    bool m_isHover;

    QIcon m_icon;
    FluentIcon f_icon;
};

////////////////////////////////////
/// TransparentToolButton
////////////////////////////////////

class TransparentToolButton : public ToolButton 
{
    Q_OBJECT
public:
    TransparentToolButton(QWidget* widget = nullptr);
    TransparentToolButton(FluentIcon& icon, QWidget* parnet = nullptr);
    TransparentToolButton(QIcon& icon, QWidget* widget = nullptr);
    TransparentToolButton(const QString& icon, QWidget* widget = nullptr);
};