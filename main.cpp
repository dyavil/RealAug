#include "maindisplay.h"
#include <QApplication>
#include "opencv2/opencv.hpp"
#include "opencv2/video.hpp"
#include "camera_calibration.h"
#include "cvtogl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDisplay w;

    CameraInfo cm;
    FileStorage fs("calib.txt",FileStorage::READ);
    fs["cameraMat"] >> cm.cameraMat;
    fs["distCoeffs"] >> cm.distCoefs;
    fs["imgPoints"] >> cm.imgPoints;

    //std::cout << cm.cameraMat <<std::endl;
    //std::cout << cm.distCoefs << std::endl;
    //std::cout << cm.imgPoints << std::endl;

    /*while(1)
    {
      bool bSuccess = cap.read(frame);

      if (!bSuccess)
      {
        std::cout << "Cannot read the frame from video file" << std::endl;
        break;
      }
      cv::imshow("frame", frame);
      if(cv::waitKey(wait) == 27)
      {
        break;
      }
    }*/

    w.show();

    CVtoGL * gl = w.getLink();
    gl->cm.cameraMat = cm.cameraMat;
    gl->cm.distCoefs = cm.distCoefs;
    gl->cm.imgPoints = cm.imgPoints;
    //std::cout << gl->worldPoints.size() <<std::endl;
    return a.exec();
}
