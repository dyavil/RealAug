#ifndef MAINDISPLAY_H
#define MAINDISPLAY_H

#include <QMainWindow>
#include "cvtogl.h"

namespace Ui {
class MainDisplay;
}

class MainDisplay : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainDisplay(QWidget *parent = 0);
    ~MainDisplay();

    CVtoGL * getLink();

private:
    Ui::MainDisplay *ui;
};

#endif // MAINDISPLAY_H
