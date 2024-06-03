#include "Screen.hpp"

namespace fluent {


    QScreen* getCurrentScreen() {
        // Get the position of the cursor
        QPoint cursorPos = QCursor::pos();

        // Iterate through all screens and return the one that contains the cursor position
        for (QScreen* screen : QApplication::screens()) {
            if (screen->geometry().contains(cursorPos)) {
                return screen;
            }
        }

        return nullptr;
    }


    QRect getCurrentScreenGeometry(bool available) {
        // Get the current screen or the primary screen if the current screen is not available
        QScreen* screen = getCurrentScreen();
        if (!screen) {
            screen = QApplication::primaryScreen();
        }

        // Return a default QRect if no screen is found (this should not happen)
        if (!screen) {
            return QRect(0, 0, 1920, 1080);
        }

        // Return the available geometry or the full geometry of the screen
        return available ? screen->availableGeometry() : screen->geometry();
    }
    
} // namespace fluent