#include "myoarmbandgui.h"
#include "ui_myoarmbandgui.h"
#include "../myoarmband.h"


MyoArmbandGui::MyoArmbandGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyoArmbandGui)
{
    ui->setupUi(this);

}

MyoArmbandGui::~MyoArmbandGui()
{
    delete ui;
}

void MyoArmbandGui::refreshDisplays(int value)
{
}
