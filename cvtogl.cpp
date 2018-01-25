#include "cvtogl.h"

CVtoGL::CVtoGL()
{
    imgHeight = 40;
    imgWidth = 40;
    loadObj("teapot.obj", 1, cv::Point3f(80.0, 50.0, 80.0));
}


void CVtoGL::initFrustum(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //std::cout << imgWidth <<", "  <<imgHeight << std::endl;
    cv::Mat p = (cv::Mat_<double>(3,1) << -imgWidth/2, 0, 1);
    cv::Mat q = (cv::Mat_<double>(3,1) << 0, imgHeight/2, 1);
    cv::Mat r = (cv::Mat_<double>(3,1) << imgWidth/2, 0, 1);
    cv::Mat s = (cv::Mat_<double>(3,1) << 0, -imgHeight/2, 1);
    float z0 = 0.1;
    cv::Mat test;
    cv::FileStorage fs("calib.txt",cv::FileStorage::READ);
    fs["cameraMat"] >> test;
    cv::Mat invcam = test.inv();
    p = z0*invcam*p;
    q = z0*invcam*q;
    r = z0*invcam*r;
    s = z0*invcam*s;
    float dw = cv::norm(p, r);
    float dh = cv::norm(q, s);
    //std::cout << p << std::endl;
    //p = cam1*p;
    /*glOrtho(-imgWidth/3.0, imgWidth/3.0,
            -imgHeight/3.0, imgHeight/3.0,
            -0.1, 1000.0);*/
    /*glFrustum(-0.001, 0.001,
            -0.001, 0.001,
            0.0035, 100000.0);*/
    glFrustum(-dw/2.0, dw/2.0,
            -dh/2.0, dh/2.0,
            0.1, 10000.0);
    glMatrixMode(GL_MODELVIEW);
}


void CVtoGL::drawChessBoard(int w, int h, float squareWidth){

    if(worldPoints.empty()){
        ww = 0;

        for (int i = 0; i < w; ++i) {
            ww++;
            hh =0;
            for (int j = 0; j < h; ++j) {
                worldPoints.push_back(cv::Point3f(squareWidth*i, squareWidth*j,0.0));
                initworldPoints.push_back(cv::Point3f(squareWidth*i, squareWidth*j,0.0));
                hh++;
            }
        }

        render();
    }

    int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE;
    cv::VideoCapture cap(0);
    cv::Mat frame;
    std::vector<cv::Point2f> pointBuf;
    cap.read(frame);

    bool found = cv::findChessboardCorners(frame, cv::Size(h, w), pointBuf, chessBoardFlags);
    cv::imshow("frame", frame);
    if(found) {
        cm.imgPoints = pointBuf;

        imgHeight = frame.rows;
        imgWidth = frame.cols;
    }

    render();

    //std::cout << ww << ", " << hh << std::endl;
    for (int i = 0; i < ww-1; ++i) {
        for (int j = 0; j < hh-1; ++j) {
            if(((i+j) %2)) glColor3f(1.0, 1.0, 1.0);
            else glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_QUADS);
            glVertex3f(worldPoints[i*hh+j].x, worldPoints[i*hh+j].y, worldPoints[i*hh+j].z);
            glVertex3f(worldPoints[(i+1)*hh+j].x, worldPoints[(i+1)*hh+j].y, worldPoints[(i+1)*hh+j].z);
            glVertex3f(worldPoints[(i+1)*hh+j+1].x, worldPoints[(i+1)*hh+j+1].y, worldPoints[(i+1)*hh+j+1].z);
            glVertex3f(worldPoints[i*hh+j+1].x, worldPoints[i*hh+j+1].y, worldPoints[i*hh+j+1].z);
            //std::cout << (i+1)*hh+j+1 << ", " << worldPoints.size() << std::endl;
            /*glVertex3f((squareWidth+squareWidth*i), (squareWidth+squareWidth*j),z);
            glVertex3f((squareWidth*i), (squareWidth+squareWidth*j),z);
            glVertex3f((squareWidth*i), (squareWidth*j), z);
            glVertex3f((squareWidth+squareWidth*i), (squareWidth*j), z);*/
            //std::cout << squareWidth+squareWidth*i << squareWidth*j << std::endl;
            glEnd();
        }
    }

}


void CVtoGL::loadObj(std::string filename, float scale, cv::Point3f trans) {
    setlocale(LC_NUMERIC, "C");
    std::ifstream file(filename.c_str());
    std::string line, prefixe;
    while(!file.eof()) {
        getline(file, line);
        prefixe = line.substr(0, 2);

        // Face
        if(prefixe == "f ") {
            std::size_t idx = line.find("//");
            if(idx != std::string::npos) {
                while(idx != std::string::npos) {
                    line.insert(idx+1, "0");
                    idx = line.find("//");
                }
                int v1, v2, v3, t1, t2, t3, n1, n2, n3;
                sscanf(&line[0], "f  %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
                meshFaces.push_back(MeshFace(cv::Point3i(v1-1, v2-1, v3-1), cv::Point3i(n1-1, n2-1, n3-1)));
            } else {
                int v1, v2, v3;
                sscanf(&line[0], "f  %d %d %d", &v1, &v2, &v3);
                meshFaces.push_back(MeshFace(cv::Point3i(v1-1, v2-1, v3-1), cv::Point3i()));
            }
        }
        // Vertice
        else if(prefixe == "v ")
        {
            float x, y, z;
            sscanf(&line[0], "v  %f %f %f", &x, &y, &z);
            meshVertices.push_back(cv::Point3f(scale*x + trans.x, scale*y + trans.y, scale*z + trans.z));
        }
        // Normale
        else if(prefixe == "vn")
        {
            float x, y, z;
            sscanf(&line[0], "vn %f %f %f", &x, &y, &z);
            meshNormales.push_back(cv::Point3f(scale*x + trans.x, scale*y + trans.y, scale*z + trans.z));
        }
    }

    file.close();
}


void CVtoGL::drawObj() {
    glColor3ub(255, 0, 0);

    for(unsigned int i = 0; i < meshFaces.size(); ++i) {
        glBegin(GL_TRIANGLES);

        glNormal3f(meshVertices[meshFaces[i].normales.x].x, meshVertices[meshFaces[i].normales.x].y, meshVertices[meshFaces[i].normales.x].z);
        glVertex3f(meshVertices[meshFaces[i].vertices.x].x, meshVertices[meshFaces[i].vertices.x].y, meshVertices[meshFaces[i].vertices.x].z + 1);

        glNormal3f(meshVertices[meshFaces[i].normales.y].x, meshVertices[meshFaces[i].normales.y].y, meshVertices[meshFaces[i].normales.y].z);
        glVertex3f(meshVertices[meshFaces[i].vertices.y].x, meshVertices[meshFaces[i].vertices.y].y, meshVertices[meshFaces[i].vertices.y].z + 1);

        glNormal3f(meshVertices[meshFaces[i].normales.z].x, meshVertices[meshFaces[i].normales.z].y, meshVertices[meshFaces[i].normales.z].z);
        glVertex3f(meshVertices[meshFaces[i].vertices.z].x, meshVertices[meshFaces[i].vertices.z].y, meshVertices[meshFaces[i].vertices.z].z + 1);

        glEnd();
    }
}


void CVtoGL::render(){
    initFrustum();

    cv::Mat rot, trans, matRot, rotT, pos_camera, point, center, uc, uw;
    cv::solvePnP(initworldPoints, cm.imgPoints, cm.cameraMat, cm.distCoefs, rot, trans);
    cv::Rodrigues(rot, matRot);

    for (unsigned int i = 0; i < initworldPoints.size(); ++i) {
        cv::Mat mat(3,1,CV_64F);
        mat.at<double>(0, 0) = initworldPoints[i].x;
        mat.at<double>(1, 0) = initworldPoints[i].y;
        mat.at<double>(2, 0) = initworldPoints[i].z;
        /*cv::Mat ress = rotf*mat;
        const double *data = ress.ptr<double>(0);
        worldPoints[i] = cv::Point3f(data[0], data[1], data[2]);
        std::cout << ress << initworldPoints[i] <<std::endl;
        worldPoints[i] = rotf*worldPoints[i];*/
    }

    cv::transpose(matRot, rotT);
    pos_camera = -rotT * trans;

    point = cv::Mat::zeros(3, 1, CV_64F);
    point.at<double>(2, 0) = 50.0;
    center = rotT*(point - trans);

    uc = cv::Mat::zeros(3, 1, CV_64F);
    uc.at<double>(1, 0) = -1.0;
    uw = rotT * uc;

    //std::cout << pos_camera.at<double>(1, 0) << pos_camera << std::endl;
    //std::cout << rotf << std::endl;
    //std::cout << (cm.cameraMat.at<double>(0, 2)) << ", " << (cm.cameraMat.at<double>(1, 2)) << std::endl;

    gluLookAt(pos_camera.at<double>(0, 0), pos_camera.at<double>(1, 0), pos_camera.at<double>(2, 0),
              center.at<double>(0, 0), center.at<double>(1, 0), center.at<double>(2, 0),
              uw.at<double>(0, 0), uw.at<double>(1, 0), uw.at<double>(2, 0));

    drawObj();
}
