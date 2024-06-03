#include "Translator.hpp"

namespace fluent {

    FluentTranslator::FluentTranslator(const QLocale& locale, QObject* parent)
        : QTranslator(parent)
    {
        load(locale);
    }


    bool FluentTranslator::load(const QLocale& locale) {
        QString translationFile = QString(":/qfluentwidgets/i18n/qfluentwidgets.%1.qm").arg(locale.name());
        return QTranslator::load(translationFile);
    }
    
} // namespace fluent