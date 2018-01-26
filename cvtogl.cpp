#include "cvtogl.h"

CVtoGL::CVtoGL()
{
    imgHeight = 40;
    imgWidth = 40;
    loadObj("obj/teapot.obj", 0.8, cv::Point3f(80.0, 50.0, 60.0));
}

void CVtoGL::loadObj(QString path, float scale, cv::Point3f trans){
    std::string inputfile = path.toStdString();
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

    if (!err.empty()) { // `err` may contain warning message.
      std::cerr << err << std::endl;
    }

    if (!ret) {
      exit(1);
    }

    std::vector<tinyobj::real_t>::const_iterator result;
    //std::cout << colors.size() << ", " << sf.h*sf.w << std::endl;
    result = std::max_element(attrib.vertices.begin(), attrib.vertices.end());

    //std::cout << *result << std::endl;
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
      // Loop over faces(polygon)
      size_t index_offset = 0;
      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
        unsigned int fv = shapes[s].mesh.num_face_vertices[f];

        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
          tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
          tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
          tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
          meshVertices.push_back(cv::Point3f(vx*scale, vy*scale, vz*scale)+trans);
          tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
          tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
          tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
          meshNormales.push_back(cv::Point3f(nx, ny, nz));
          /*
          tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
          tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];*/
          // Optional: vertex colors
          tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
          tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
          tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
          //std::cout << cv::Point3f(red, green, blue) << std::endl;
          meshColors.push_back(cv::Point3f(red, green, blue));
          /*if(vz > (*result - *result*0.82) ) meshVertices.push_back(colorr);
          else treeColors.push_back(Vector3(0.647059, 0.164706, 0.164706));*/
        }
        index_offset += fv;

        // per-face material
        shapes[s].mesh.material_ids[f];
      }
    }
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
    cv::FileStorage fs("obj/calib.txt",cv::FileStorage::READ);
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
    drawObj();
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


void CVtoGL::drawObj() {
    glColor3ub(150, 150, 150);
    for(unsigned int i = 0; i < meshVertices.size(); i+=3) {
        glBegin(GL_TRIANGLES);

        glNormal3f(meshNormales[i].x, meshNormales[i].y, meshNormales[i].z);
        glVertex3f(meshVertices[i].x, meshVertices[i].y, meshVertices[i].z);
        glNormal3f(meshNormales[i+1].x, meshNormales[i+1].y, meshNormales[i+1].z);
        glVertex3f(meshVertices[i+1].x, meshVertices[i+1].y, meshVertices[i+1].z);
        glNormal3f(meshNormales[i+2].x, meshNormales[i+2].y, meshNormales[i+2].z);
        glVertex3f(meshVertices[i+2].x, meshVertices[i+2].y, meshVertices[i+2].z);

        glEnd();
    }
}


void CVtoGL::render(){
    initFrustum();

    cv::Mat rot, trans, matRot, rotT, pos_camera, point, center, uc, uw;
    cv::solvePnP(worldPoints, cm.imgPoints, cm.cameraMat, cm.distCoefs, rot, trans);
    cv::Rodrigues(rot, matRot);

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

    //drawObj();
}
