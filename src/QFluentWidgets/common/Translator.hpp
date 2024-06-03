#pragma once

#include <QTranslator>
#include <QLocale>

namespace fluent {

    class FluentTranslator : public QTranslator {
    public:
        explicit FluentTranslator(const QLocale& locale = QLocale(), QObject* parent = nullptr);
        bool load(const QLocale& locale);
    };
}