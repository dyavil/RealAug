#ifndef CVTOGL_H
#define CVTOGL_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <QString>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "include/tiny_obj_loader.h"


class CameraInfoLoc{
public:
    CameraInfoLoc(){}
    cv::Mat cameraMat;
    cv::Mat distCoefs;
    std::vector<cv::Point2f> imgPoints;

};

class CVtoGL
{
public:
    CVtoGL();
    void initFrustum();
    void drawChessBoard(int w, int h, float squareWidth);
    void drawObj();
    void render();

    std::vector<cv::Point3f> worldPoints;

    std::vector<cv::Point3f> meshVertices;
    std::vector<cv::Point3f> meshNormales;
    std::vector<cv::Point3f> meshColors;
    void loadObj(QString path, float scale, cv::Point3f trans);

    int ww;
    int hh;
    int imgWidth;
    int imgHeight;
    CameraInfoLoc cm;
};

#endif // CVTOGL_H
