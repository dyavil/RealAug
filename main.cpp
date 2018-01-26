#include "maindisplay.h"
#include <QApplication>
#include "opencv2/opencv.hpp"
#include "opencv2/video.hpp"
#include "cvtogl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDisplay w;

    w.show();
    return a.exec();
}
