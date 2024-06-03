#pragma once

#include "QFluentWidgets/common/Config.hpp"

#include <regex>
#include <string>
#include <unordered_map>
#include <QSharedPointer>

namespace fluent {

    class StyleSheetBase {
    public:
        virtual ~StyleSheetBase() = default;

        virtual QString path(Theme::Mode theme = Theme::Mode::Auto) const = 0;
        virtual QString content(Theme::Mode theme = Theme::Mode::Auto) const;
        virtual void apply(QWidget* widget, Theme::Mode theme = Theme::Mode::Auto) const;
    };


    class FluentStyleSheet : public StyleSheetBase
    {
    public:
        enum class Type {
            MENU, LABEL, PIVOT, BUTTON, DIALOG, SLIDER, INFO_BAR, SPIN_BOX, TAB_VIEW, TOOL_TIP, CHECK_BOX, COMBO_BOX, FLIP_VIEW, LINE_EDIT, LIST_VIEW, TREE_VIEW, INFO_BADGE, PIPS_PAGER, TABLE_VIEW, CARD_WIDGET, TIME_PICKER, COLOR_DIALOG, MEDIA_PLAYER, SETTING_CARD, TEACHING_TIP, FLUENT_WINDOW, SWITCH_BUTTON, MESSAGE_DIALOG, STATE_TOOL_TIP, CALENDAR_PICKER, FOLDER_LIST_DIALOG, SETTING_CARD_GROUP, EXPAND_SETTING_CARD, NAVIGATION_INTERFACE
        };

    public:
        FluentStyleSheet(Type type);

        virtual QString path(Theme::Mode theme = Theme::Mode::Auto) const override;

    private:
        Type m_type;
    };


    class StyleSheetFile : public StyleSheetBase {
    public:
        // Constructor accepting a file path
        StyleSheetFile(const QString& path);
        // Override the path method to return the file path
        QString path(Theme::Mode theme = Theme::Mode::Auto) const override;

    private:
        QString filePath;
    };


    class CustomStyleSheet : public StyleSheetBase
    {
    public:
        static const char* LIGHT_QSS_KEY; //= "lightCustomQss"
        static const char* DARK_QSS_KEY; // = "darkCustomQss"

        CustomStyleSheet(QWidget* widget);

        virtual QString path(Theme::Mode theme = Theme::Mode::Auto) const override;
        bool operator==(const CustomStyleSheet* other);

        void setCustomStyleSheet(const QString& light, const QString& dark);
        void setLightStyleSheet(const QString& src);
        void setDarkStyleSheet(const QString& src);

        QString lightStyleSheet() const;
        QString darkStyleSheet() const;

        virtual QString content(Theme::Mode theme = Theme::Mode::Auto) const override;

    private:
        QWidget* m_widget;
    };


    class CustomStyleSheetWatcher : public QObject
    {
        Q_GADGET
    public:
        CustomStyleSheetWatcher(QWidget* widget = nullptr);

        virtual bool eventFilter(QObject* watched, QEvent* event) override;
    };


    class DirtyStyleSheetWatcher : public QObject
    {
        Q_GADGET
    public:
        DirtyStyleSheetWatcher(QWidget* widget = nullptr);

        virtual bool eventFilter(QObject* watched, QEvent* event) override;
    };


    class StyleSheetCompose : public StyleSheetBase 
    {
    public:
        StyleSheetCompose(QList<StyleSheetBase*> sheets);
        ~StyleSheetCompose();

        QString content(Theme::Mode theme) const;
        void add(StyleSheetBase* src);
        void remove(StyleSheetBase* src);
        virtual QString path(Theme::Mode theme = Theme::Mode::Auto) const { return ""; };

    private:
        QList<StyleSheetBase*> m_sheets;
    };


    class StyleSheetManager : public QObject
    {
        Q_OBJECT
    public:
        StyleSheetManager(QObject* parent = nullptr);

        void registerWidget(const QString& source, QWidget* widget, bool reset = true);
        void registerWidget(StyleSheetBase* source, QWidget* widget, bool reset = true) {
            registerWidgetInternal(source, widget, reset);
        }

        QMap<QWidget*, StyleSheetCompose*> items() const {
            return widgets;
        }

        StyleSheetCompose* source(QWidget* widget) const;

    public slots:
        void deregister(QObject* widget);

    private:
        QMap<QWidget*, StyleSheetCompose*> widgets;

        void registerWidgetInternal(StyleSheetBase* source, QWidget* widget, bool reset);
    };
    Q_GLOBAL_STATIC(StyleSheetManager, styleSheetManager);


    class QssTemplate
    {
    public:
        explicit QssTemplate(const QString& templateStr);

        QString safeSubstitute(const std::unordered_map<QString, QString>& mappings) const;
    private:
        QString templateStr;
    };


    enum class ThemeColor {
        PRIMARY,
        DARK_1,
        DARK_2,
        DARK_3,
        LIGHT_1,
        LIGHT_2,
        LIGHT_3
    };


    class ThemeColorHelper {
    public:
        static QString name(ThemeColor color);

        static QColor toQColor(ThemeColor color);

    private:
        static QColor getBaseColor();
        static bool isDarkTheme();

        static void adjustDarkTheme(ThemeColor color, float& s, float& v);
        static void adjustLightTheme(ThemeColor color, float& s, float& v);
    };

    QString getStyleSheet(const QString& src, Theme::Mode theme = Theme::Mode::Auto);
    QString getStyleSheet(StyleSheetBase* src, Theme::Mode theme = Theme::Mode::Auto);

    void addStyleSheet(QWidget* widget, const QString& src, Theme::Mode theme = Theme::Mode::Auto, bool reg = true);
    void addStyleSheet(QWidget* widget, StyleSheetBase* src, Theme::Mode theme = Theme::Mode::Auto, bool reg = true);

    void updateStyleSheet(bool lazy = false);
    QSharedPointer<CustomStyleSheet> setCustomStyleSheet(QWidget* widget, const QString& lightQss, const QString& darkQss);

    QColor themeColor();
    QString applyThemeColor(const QString& qss);
    void setTheme(Theme::Mode theme, bool save = false, bool lazy = false);
    void toggleTheme(bool save = false, bool lazy = false);
    void setThemeColor(QColor color, bool save = false, bool lazy = false);
    void setThemeColor(const QString& color, bool save = false, bool lazy = false);
    void setThemeColor(Qt::GlobalColor color, bool save = false, bool lazy = false);

    QString getStyleSheetFromFile(const QString& filePath);
    QString getStyleSheetFromFile(QFile& file);


    void setStyleSheet(QWidget* widget, const QString& styleSheet, Theme::Mode theme = Theme::Mode::Auto, bool reg = true);
    void setStyleSheet(QWidget* widget, StyleSheetBase* styleSheet, Theme::Mode theme = Theme::Mode::Auto, bool reg = true);
}