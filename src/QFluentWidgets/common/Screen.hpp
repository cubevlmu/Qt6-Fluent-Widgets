#pragma once

#include <QApplication>
#include <QCursor>
#include <QScreen>
#include <QRect>

namespace fluent {

    QScreen* getCurrentScreen();
    QRect getCurrentScreenGeometry(bool available = true);

}