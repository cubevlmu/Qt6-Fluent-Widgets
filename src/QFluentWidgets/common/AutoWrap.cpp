#include "AutoWrap.hpp"

namespace fluent {

    std::unordered_map<QString, int> TextWrap::EAST_ASIAN_WIDTH_TABLE = {
        {"F", 2},
        {"H", 1},
        {"W", 2},
        {"A", 1},
        {"N", 1},
        {"Na", 1}
    };


    int TextWrap::getWidth(const QChar& ch) {
        return EAST_ASIAN_WIDTH_TABLE[QString(ch.unicode())];
    }


    int TextWrap::getTextWidth(const QString& text) {
        int width = 0;
        for (const QChar& ch : text) {
            width += getWidth(ch);
        }
        return width;
    }


    CharType TextWrap::getCharType(const QChar& ch) {
        if (ch.isSpace()) {
            return CharType::SPACE;
        }
        if (getWidth(ch) == 1) {
            return CharType::LATIN;
        }
        return CharType::ASIAN;
    }


    QString TextWrap::processTextWhitespace(const QString& text) {
        QString result = text.simplified();
        result = result.trimmed();
        return result;
    }


    QStringList TextWrap::splitLongToken(const QString& token, int width) {
        QStringList chunks;
        for (int i = 0; i < token.length(); i += width) {
            chunks.append(token.mid(i, width));
        }
        return chunks;
    }


    QStringList TextWrap::tokenizer(const QString& text) {
        QString buffer;
        CharType lastCharType;
        QStringList tokens;

        for (const QChar& ch : text) {
            CharType charType = getCharType(ch);

            if (!buffer.isEmpty() && (charType != lastCharType || charType != CharType::LATIN)) {
                tokens.append(buffer);
                buffer.clear();
            }

            buffer += ch;
            lastCharType = charType;
        }

        tokens.append(buffer);
        return tokens;
    }


    std::pair<QString, bool> TextWrap::wrap(
        const QString& text,
        int width,
        bool once
    ) {
        QStringList lines = text.split(QRegularExpression("\n|\r\n|\r"));
        bool isWrapped = false;
        QStringList wrappedLines;

        for (const QString& line : lines) {
            QString processedLine = processTextWhitespace(line);

            if (getTextWidth(processedLine) > width) {
                auto [wrappedLine, wrapped] = wrapLine(processedLine, width, once);
                wrappedLines.append(wrappedLine);
                isWrapped = wrapped;

                if (once) {
                    wrappedLines.append(text.mid(wrappedLine.length()).trimmed());
                    return { wrappedLines.join("\n"), isWrapped };
                }
            }
            else {
                wrappedLines.append(processedLine);
            }
        }

        return { wrappedLines.join("\n"), isWrapped };
    }


    std::pair<QString, bool> TextWrap::wrapLine(
        const QString& text, 
        int width,
        bool once
    ) {
        QString lineBuffer;
        QStringList wrappedLines;
        int currentWidth = 0;

        for (const QString& token : tokenizer(text)) {
            int tokenWidth = getTextWidth(token);

            if (token == " " && currentWidth == 0) {
                continue;
            }

            if (currentWidth + tokenWidth <= width) {
                lineBuffer += token;
                currentWidth += tokenWidth;

                if (currentWidth == width) {
                    wrappedLines.append(lineBuffer.trimmed());
                    lineBuffer.clear();
                    currentWidth = 0;
                }
            }
            else {
                if (currentWidth != 0) {
                    wrappedLines.append(lineBuffer.trimmed());
                }

                QStringList chunks = splitLongToken(token, width);

                for (const QString& chunk : chunks.mid(0, chunks.size() - 1)) {
                    wrappedLines.append(chunk.trimmed());
                }

                lineBuffer = chunks.last();
                currentWidth = getTextWidth(lineBuffer);
            }
        }

        if (currentWidth != 0) {
            wrappedLines.append(lineBuffer.trimmed());
        }

        if (once) {
            return { wrappedLines.join("\n"), true };
        }

        return { wrappedLines.join("\n"), true };
    }

} // namespace fluent