#include "myoarmbandgui.h"
#include "ui_myoarmbandgui.h"
#include "../myoarmband.h"

using namespace MyoArmbandPlugin;

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
