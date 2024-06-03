#include "StyleSheet.hpp"
#include "QFluentWidgets/common/Config.hpp"
#include "QFluentWidgets/utils/os.hpp"

#include <QWidget>
#include <QPointer>
#include <QEvent>
#include <QDynamicPropertyChangeEvent>
#include <QColor>

namespace fluent {

    QString StyleSheetBase::content(Theme::Mode theme) const {
        return getStyleSheetFromFile(path(theme));
    }


    void StyleSheetBase::apply(QWidget* widget, Theme::Mode theme) const {
        //setStyleSheet(widget, content(theme));
        setStyleSheet(widget, (StyleSheetBase*)this, theme, true);
    }


    /*
        PRIMARY = "ThemeColorPrimary"
        DARK_1 = "ThemeColorDark1"
        DARK_2 = "ThemeColorDark2"
        DARK_3 = "ThemeColorDark3"
        LIGHT_1 = "ThemeColorLight1"
        LIGHT_2 = "ThemeColorLight2"
        LIGHT_3 = "ThemeColorLight3"
    */
    QString applyThemeColor(const QString& qss) {
        static const std::unordered_map<QString, QString> mappings = {
            {"ThemeColorPrimary", ThemeColorHelper::toQColor(ThemeColor::PRIMARY).name()},
            {"ThemeColorDark1", ThemeColorHelper::toQColor(ThemeColor::DARK_1).name()},
            {"ThemeColorDark2", ThemeColorHelper::toQColor(ThemeColor::DARK_2).name()},
            {"ThemeColorDark3", ThemeColorHelper::toQColor(ThemeColor::DARK_3).name()},
            {"ThemeColorLight1", ThemeColorHelper::toQColor(ThemeColor::LIGHT_1).name()},
            {"ThemeColorLight2", ThemeColorHelper::toQColor(ThemeColor::LIGHT_2).name()},
            {"ThemeColorLight3", ThemeColorHelper::toQColor(ThemeColor::LIGHT_3).name()}
        };


        QssTemplate templateObj(qss);
        return templateObj.safeSubstitute(mappings);
    }


    QString getStyleSheet(const QString& src, Theme::Mode theme) {
        return getStyleSheet(new StyleSheetFile(src), theme);
    }


    QString getStyleSheet(StyleSheetBase* src, Theme::Mode theme) {
        return applyThemeColor(src->content(theme));
    }


    void addStyleSheet(
        QWidget* widget,
        const QString& src,
        Theme::Mode theme,
        bool reg
    ) {
        QString qss = "";
        if (reg) {
            styleSheetManager->registerWidget(src, widget, false);
            qss = getStyleSheet(styleSheetManager->source(widget), theme);
        }
        else {
            qss = widget->styleSheet() + '\n' + getStyleSheet(src, theme);
        }

        if (qss.trimmed() != widget->styleSheet().trimmed()) {
            widget->setStyleSheet(qss.trimmed());
        }
    }


    void addStyleSheet(
        QWidget* widget,
        StyleSheetBase* src,
        Theme::Mode theme,
        bool reg
    ) {
        QString qss = "";
        if (reg) {
            styleSheetManager->registerWidget(src, widget, false);
            qss = getStyleSheet(styleSheetManager->source(widget), theme);
        }
        else {
            qss = widget->styleSheet() + '\n' + getStyleSheet(src, theme);
        }

        if (qss.trimmed() != widget->styleSheet().trimmed()) {
            widget->setStyleSheet(qss.trimmed());
        }
    }


    void updateStyleSheet(bool lazy) {
        QList<QWidget*> widget_to_remove;
        auto items = styleSheetManager->items();

        for (auto& widget : items.keys()) {
            try {
                if (!lazy && widget->visibleRegion().isNull())
                {
                    setStyleSheet(widget, items[widget], qconfig->themeMode->value().value<Theme::Mode>());
                }
                else {
                    styleSheetManager->registerWidget(items[widget], widget);
                    widget->setProperty("dirty-qss", true);
                }
            }
            catch (...) {
                widget_to_remove.push_back(widget);
            }
        }

        for (auto* widget : widget_to_remove) {
            styleSheetManager->deregister(widget);
        }
    }

    QSharedPointer<CustomStyleSheet> setCustomStyleSheet(
        QWidget* widget,
        const QString& lightQss,
        const QString& darkQss
    ) {
        QSharedPointer<CustomStyleSheet> ptr;
        ptr.reset(new CustomStyleSheet(widget));
        ptr->setCustomStyleSheet(lightQss, darkQss);
        return ptr;
    }


    QColor themeColor() {
        return ThemeColorHelper::toQColor(ThemeColor::PRIMARY);
    }


    void setTheme(Theme::Mode theme, bool save, bool lazy) {
        qconfig->set(qconfig->themeMode, QVariant::fromValue(theme), save);
        updateStyleSheet(lazy);
        emit qconfig->themeChanged(theme);
    }


    void toggleTheme(bool save, bool lazy) {
        Theme::Mode theme = os::isDarkTheme() ? Theme::Mode::Dark : Theme::Mode::Light;
        setTheme(theme, save, lazy);
    }


    void setThemeColor(QColor color, bool save, bool lazy) {
        qconfig->set(qconfig->themeColor, color, save);
        updateStyleSheet(lazy);
    }


    void setThemeColor(const QString& color, bool save, bool lazy) {
        QColor cc(color);
        setThemeColor(cc, save, lazy);
    }


    void setThemeColor(Qt::GlobalColor color, bool save, bool lazy) {
        setThemeColor(QColor(color), save, lazy);
    }


    QString getStyleSheetFromFile(const QString& filePath) {
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly)) {
            throw std::runtime_error("Unable to open file: " + file.fileName().toStdString());
        }
        QTextStream in(&file);
        return in.readAll();
    }

    QString getStyleSheetFromFile(QFile& file) {
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            throw std::runtime_error("Unable to open file: " + file.fileName().toStdString());
        }
        QTextStream in(&file);
        return in.readAll();
    }


    void setStyleSheet(
        QWidget* widget,
        const QString& src,
        Theme::Mode theme,
        bool reg
    ) {
        if (reg) {
            styleSheetManager->registerWidget(src, widget);
        }

        widget->setStyleSheet(getStyleSheet(src, theme));
    }


    void setStyleSheet(
        QWidget* widget,
        StyleSheetBase* src,
        Theme::Mode theme,
        bool reg
    ) {
        if (reg) {
            styleSheetManager->registerWidget(src, widget);
        }

        widget->setStyleSheet(getStyleSheet(src, theme));

    }



    StyleSheetFile::StyleSheetFile(const QString& path)
        : filePath(path)
    {}


    QString StyleSheetFile::path(Theme::Mode theme) const {
        return filePath;
    }



    StyleSheetCompose::StyleSheetCompose(
        QList<StyleSheetBase*> sheets
    ) : m_sheets(sheets)
    {}


    StyleSheetCompose::~StyleSheetCompose() {
        m_sheets.clear();
    }


    QString StyleSheetCompose::content(Theme::Mode theme) const {
        QString src('\n');
        for (auto& ptr : m_sheets) {
            auto rr = ptr->content(theme);
            src.append(rr);
        }
        return src;
    }


    void StyleSheetCompose::add(StyleSheetBase* src) {
        if (src == this) return;
        if (m_sheets.contains(src)) return;
        m_sheets.push_back(src);
    }


    void StyleSheetCompose::remove(StyleSheetBase* src) {
        if (src == this) return;
        if (!m_sheets.contains(src)) return;
        m_sheets.push_back(src);

        delete src;
    }



    const char* CustomStyleSheet::DARK_QSS_KEY = "darkCustomQss";
    const char* CustomStyleSheet::LIGHT_QSS_KEY = "lightCustomQss";


    CustomStyleSheet::CustomStyleSheet(QWidget* widget) {
        this->m_widget = widget;
    }


    QString CustomStyleSheet::path(Theme::Mode theme) const {
        return "";
    }


    bool CustomStyleSheet::operator==(const CustomStyleSheet* other) {
        return other->m_widget == m_widget;
    }


    void CustomStyleSheet::setCustomStyleSheet(
        const QString& light,
        const QString& dark
    ) {
        setLightStyleSheet(light);
        setDarkStyleSheet(dark);
    }


    void CustomStyleSheet::setLightStyleSheet(const QString& src) {
        m_widget->setProperty(LIGHT_QSS_KEY, src);
    }


    void CustomStyleSheet::setDarkStyleSheet(const QString& src) {
        m_widget->setProperty(DARK_QSS_KEY, src);
    }


    QString CustomStyleSheet::lightStyleSheet() const {
        QVariant data = m_widget->property(LIGHT_QSS_KEY);
        return data.isNull() ? "" : data.toString();
    }


    QString CustomStyleSheet::darkStyleSheet() const {
        QVariant data = m_widget->property(DARK_QSS_KEY);
        return data.isNull() ? "" : data.toString();
    }


    QString CustomStyleSheet::content(Theme::Mode theme) const {
        Theme::Mode tt = theme == Theme::Mode::Auto ? qconfig->themeMode->value().value<Theme::Mode>() : theme;
        return tt == Theme::Mode::Light ? lightStyleSheet() : darkStyleSheet();
    }



    CustomStyleSheetWatcher::CustomStyleSheetWatcher(QWidget* widget)
        : QObject(widget)
    {}


    bool CustomStyleSheetWatcher::eventFilter(QObject* watched, QEvent* event) {
        if (event->type() != QEvent::DynamicPropertyChange) {
            return QObject::eventFilter(watched, event);
        }

        auto* e = static_cast<QDynamicPropertyChangeEvent*>(event);
        std::string temp_name = e->propertyName().toStdString();
        const char* name = temp_name.c_str();

        if (name == CustomStyleSheet::DARK_QSS_KEY || name == CustomStyleSheet::LIGHT_QSS_KEY) {
            //TODO addStyleSheet(obj, CustomStyleSheet(obj))
        }

        return QObject::eventFilter(watched, event);
    }



    DirtyStyleSheetWatcher::DirtyStyleSheetWatcher(QWidget* widget)
        : QObject(widget)
    {}


    bool DirtyStyleSheetWatcher::eventFilter(QObject* watched, QEvent* event) {
        if (event->type() != QEvent::Type::Paint)
            return QObject::eventFilter(watched, event);
        if (auto widget = dynamic_cast<QWidget*>(watched)) {
            if (widget->property("dirty-qss").isNull())
                return QObject::eventFilter(watched, event);

            widget->setProperty("dirty-qss", false);
            //TODO if(styleSheetManager.widgets.constains(widget)) {
            //     widget->setStyleSheet(getStyleSheet(styleSheetManager.source(widget)));
            // }
        }

        return QObject::eventFilter(watched, event);
    }



    StyleSheetManager::StyleSheetManager(QObject* parent) : QObject(parent) {}


    void StyleSheetManager::registerWidget(const QString& source,
        QWidget* widget, bool reset) {
        registerWidgetInternal(new StyleSheetFile(source), widget, reset);
    }

    void StyleSheetManager::deregister(QObject* obj) {
        QWidget* widget = static_cast<QWidget*>(obj);
        if (widget == nullptr) return;

        if (!widgets.contains(widget)) {
            return;
        }
        widgets.remove(widget);
    }


    StyleSheetCompose* StyleSheetManager::source(QWidget* widget) const {
        if (!widgets.contains(widget)) {
            return new StyleSheetCompose(QList<StyleSheetBase*>());
        }
        return widgets[widget];
    }


    void StyleSheetManager::registerWidgetInternal(
        StyleSheetBase* source,
        QWidget* widget,
        bool reset
    ) {
        if (!widgets.contains(widget)) {
            QObject::connect(widget, &QWidget::destroyed, this, &StyleSheetManager::deregister);
            widget->installEventFilter(new CustomStyleSheetWatcher(widget));
            widget->installEventFilter(new DirtyStyleSheetWatcher(widget));
            widgets.insert(widget, new StyleSheetCompose({ source, new CustomStyleSheet(widget) }));
        }

        if (!reset) {
            StyleSheetManager::source(widget)->add(source);
        }
        else {
            widgets[widget] = new StyleSheetCompose({ source, new CustomStyleSheet(widget) });
        }
    }



    QssTemplate::QssTemplate(const QString& templateStr)
        : templateStr(templateStr)
    {}


    QString QssTemplate::safeSubstitute(
        const std::unordered_map<QString, QString>& mappings) const {
        QString result = templateStr;
        for (const auto& pair : mappings) {
            std::regex re("--" + pair.first.toStdString());
            result = QString::fromStdString(std::regex_replace(result.toStdString(), re, pair.second.toStdString()));
        }
        return result;
    }



    QString ThemeColorHelper::name(ThemeColor color) {
        return toQColor(color).name();
    }


    QColor ThemeColorHelper::toQColor(ThemeColor color) {
        QColor baseColor = getBaseColor();
        float h, s, v;
        baseColor.getHsvF(&h, &s, &v);

        if (isDarkTheme()) {
            s *= 0.84;
            v = 1.0;
            adjustDarkTheme(color, s, v);
        }
        else {
            adjustLightTheme(color, s, v);
        }

        return QColor::fromHsvF(h, std::min(s, 1.0f), std::min(v, 1.0f));
    }


    QColor ThemeColorHelper::getBaseColor() {
        return qconfig->themeColor->value().value<QColor>();
    }


    bool ThemeColorHelper::isDarkTheme() {
        return fluent::os::isDarkTheme();
    }


    void ThemeColorHelper::adjustDarkTheme(
        ThemeColor color,
        float& s,
        float& v
    ) {
        switch (color) {
        case ThemeColor::DARK_1:
            v *= 0.9;
            break;
        case ThemeColor::DARK_2:
            s *= 0.977;
            v *= 0.82;
            break;
        case ThemeColor::DARK_3:
            s *= 0.95;
            v *= 0.7;
            break;
        case ThemeColor::LIGHT_1:
            s *= 0.92;
            break;
        case ThemeColor::LIGHT_2:
            s *= 0.78;
            break;
        case ThemeColor::LIGHT_3:
            s *= 0.65;
            break;
        default:
            break;
        }
    }


    void ThemeColorHelper::adjustLightTheme(
        ThemeColor color,
        float& s,
        float& v
    ) {
        switch (color) {
        case ThemeColor::DARK_1:
            v *= 0.75;
            break;
        case ThemeColor::DARK_2:
            s *= 1.05;
            v *= 0.5;
            break;
        case ThemeColor::DARK_3:
            s *= 1.1;
            v *= 0.4;
            break;
        case ThemeColor::LIGHT_1:
            v *= 1.05;
            break;
        case ThemeColor::LIGHT_2:
            s *= 0.75;
            v *= 1.05;
            break;
        case ThemeColor::LIGHT_3:
            s *= 0.65;
            v *= 1.05;
            break;
        default:
            break;
        }
    }

} // namespace fluent
