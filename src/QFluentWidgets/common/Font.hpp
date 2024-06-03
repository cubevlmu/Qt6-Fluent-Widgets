#pragma once

#include <QWidget>
#include <QFont>
#include <QStringList>

namespace fluent {
    
    class Font {
    public:
        static void setFont(QWidget* widget, int fontSize = 14, QFont::Weight weight = QFont::Normal); 
        static QFont getFont(int fontSize = 14, QFont::Weight weight = QFont::Normal);
        static bool loadMSFont();
    };
    
}