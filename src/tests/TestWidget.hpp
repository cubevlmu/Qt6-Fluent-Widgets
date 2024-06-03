#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TestWidget;
}
QT_END_NAMESPACE

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    TestWidget(QWidget *parent = nullptr);
    ~TestWidget();

private:
    Ui::TestWidget *ui;
};