#include "tests/TestWidget.hpp"

#include <QApplication>
#include "QFluentWidgets/common/Font.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!fluent::Font::loadMSFont()) {
        return -1;
    }

    TestWidget w;
    w.show();
    return a.exec();
}
