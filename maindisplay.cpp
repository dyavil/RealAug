#include "maindisplay.h"
#include "ui_maindisplay.h"

MainDisplay::MainDisplay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDisplay)
{
    ui->setupUi(this);
}

MainDisplay::~MainDisplay()
{
    delete ui;
}

CVtoGL * MainDisplay::getLink(){
    return &(ui->widget->drawing);
}
