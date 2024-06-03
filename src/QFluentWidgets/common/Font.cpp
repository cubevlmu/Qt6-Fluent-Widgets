#include "Font.hpp"

#include <QFontDatabase>
#include <QDebug>

namespace fluent {

    void Font::setFont(QWidget* widget, int fontSize, QFont::Weight weight) {
        widget->setFont(getFont(fontSize, weight));
    }


    QFont Font::getFont(int fontSize, QFont::Weight weight) {
        QFont font;
        QStringList families;
        families << "Segoe UI" << "Microsoft YaHei" << "PingFang SC";
        font.setFamilies(families);
        font.setPixelSize(fontSize);
        font.setWeight(weight);
        return font;
    }


    bool Font::loadMSFont() {
        int fontId = QFontDatabase::addApplicationFont(":/qfluentwidgets/fonts/Segoe UI.ttf");
        if (fontId == -1) {
            qWarning("Failed to load font");
            return false;
        }
        else {
            QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
            qDebug() << "Loaded font family:" << fontFamily;
            return true;
        }
    }
} // namespace fluent
