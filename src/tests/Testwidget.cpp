#include "TestWidget.hpp"
#include "tests/ui_Testwidget.h"

#include <QVBoxLayout>

#include "QFluentWidgets/components/widgets/PushButton.hpp"
#include "QFluentWidgets/common/Icons.hpp"

using namespace fluent;

TestWidget::TestWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestWidget)
{
    ui->setupUi(this);

    FluentIcon icon(FluentIcon::Home);

    QVBoxLayout* box = new QVBoxLayout(this);
    box->setAlignment(Qt::AlignmentFlag::AlignTop | Qt::AlignmentFlag::AlignLeft);
    box->setContentsMargins(10, 10, 10, 10);

    PushButton* btn0 = new PushButton();
    btn0->setText("BasicButton");
    btn0->setIcon(icon);

    box->addWidget(btn0);

    PrimaryPushButton* btn1 = new PrimaryPushButton();
    btn1->setText("PrimaryButton");
    btn1->setIcon(icon);

    box->addWidget(btn1);

    TransparentPushButton* btn2 = new TransparentPushButton();
    btn2->setText("TransparentButton");
    btn2->setIcon(icon);

    box->addWidget(btn2);

    ToggleButton* btn3 = new ToggleButton();
    btn3->setText("ToggleButton");
    btn3->setIcon(icon);

    box->addWidget(btn3);

    TransparentTogglePushButton* btn4 = new TransparentTogglePushButton();
    btn4->setText("TransparentToggleButton");
    btn4->setIcon(icon);

    box->addWidget(btn4);

    HyperlinkButton* btn5 = new HyperlinkButton();
    btn5->setUrl("https://www.bilibili.com");
    btn5->setText("HyperlinkButton");
    btn5->setIcon(icon);

    box->addWidget(btn5);

    RadioButton* btn6 = new RadioButton();
    btn6->setText("RadioButton");
    
    box->addWidget(btn6);

    ToolButton* btn7 = new ToolButton();
    btn7->setText("ToolButton");
    btn7->setIcon(icon);

    box->addWidget(btn7);

    TransparentToolButton* btn8 = new TransparentToolButton();
    btn8->setText("ToolButton");
    btn8->setIcon(icon);

    box->addWidget(btn8);

    setLayout(box);
}


TestWidget::~TestWidget()
{
    delete ui;
}
