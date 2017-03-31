#ifndef MYOARMBANDGUI_H
#define MYOARMBANDGUI_H

#include <QWidget>

namespace Ui {
class MyoArmbandGui;
}

class MyoArmbandGui : public QWidget
{
    Q_OBJECT

public:
    explicit MyoArmbandGui(QWidget *parent = 0);
    ~MyoArmbandGui();
//public slots:
    void refreshDisplays(int value);

private:
    Ui::MyoArmbandGui *ui;
};

#endif // MYOARMBANDGUI_H
