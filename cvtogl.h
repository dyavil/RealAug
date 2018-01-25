#ifndef CVTOGL_H
#define CVTOGL_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <iostream>
#include <fstream>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>


class CameraInfoLoc{
public:
    CameraInfoLoc(){}
    cv::Mat cameraMat;
    cv::Mat distCoefs;
    std::vector<cv::Point2f> imgPoints;

};

class MeshFace
{
public:
    MeshFace(cv::Point3i v, cv::Point3i n = cv::Point3i()) : vertices(v), normales(n) {}

    cv::Point3i vertices;
    cv::Point3i normales;
};

class CVtoGL
{
public:
    CVtoGL();
    void initFrustum();
    void drawChessBoard(int w, int h, float squareWidth);
    void loadObj(std::string filename, float scale, cv::Point3f trans);
    void drawObj();
    void render();

    std::vector<cv::Point3f> worldPoints;
    std::vector<cv::Point3f> initworldPoints;

    std::vector<cv::Point3f> meshVertices;
    std::vector<cv::Point3f> meshNormales;
    std::vector<MeshFace> meshFaces;

    int ww;
    int hh;
    int imgWidth;
    int imgHeight;
    CameraInfoLoc cm;
};

#endif // CVTOGL_H
