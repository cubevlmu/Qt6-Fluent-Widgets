#pragma once

#include <QApplication>
#include <QWidget>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QRegularExpression>
#include <QChar>
#include <unordered_map>

namespace fluent {

    enum class CharType {
        SPACE,
        ASIAN,
        LATIN
    };
    

    class TextWrap {
    public:
        static std::unordered_map<QString, int> EAST_ASIAN_WIDTH_TABLE;

        static int getWidth(const QChar& ch);
        static int getTextWidth(const QString& text);
        static CharType getCharType(const QChar& ch);
        static QString processTextWhitespace(const QString& text);
        static QStringList splitLongToken(const QString& token, int width);
        static QStringList tokenizer(const QString& text);

        static std::pair<QString, bool> wrap(const QString& text, int width, bool once = true);

    private:
        static std::pair<QString, bool> wrapLine(const QString& text, int width, bool once);
    };
}