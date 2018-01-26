#include "glview.h"
#include "iostream"
#define FRUSTUM_SIZE 1.1f

GLView::GLView(QWidget *parent) :
    QGLWidget(parent),
    drawing(),
    angle(0.0f)
{
}


void GLView::initializeGL()
{
    for (int i = 0; i < drawing.meshVertices.size(); ++i) {
        vertexArray.push_back(QVector3D(qreal(drawing.meshVertices[i].x), qreal(drawing.meshVertices[i].y), qreal(drawing.meshVertices[i].z)));
    }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glFrontFace(GL_CCW);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.f);
    glDepthFunc(GL_LESS);
}

void GLView::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    cv::Mat tmp;
    cv::cvtColor(drawing.currentImg, tmp, cv::COLOR_RGB2BGR);

    cv::flip(tmp, tmp, 0);
    cv::flip(tmp, tmp, 1);

    QImage imgIn = QImage((uchar*) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);

    //std::cout << imgIn.width() << std::endl;
    QImage textureImg = QGLWidget::convertToGLFormat(imgIn);
    glGenTextures(1, &text);
    glBindTexture(GL_TEXTURE_2D, text);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, textureImg.width(), textureImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImg.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glBindTexture(GL_TEXTURE_2D, text);



    drawing.drawChessBoard(7, 4, 31.0);
    //
    if(drawing.isRead){
        qglColor(Qt::gray);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertexArray.constData());
        glDrawArrays(GL_TRIANGLES, 0, vertexArray.size());
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    update();
}

void GLView::resizeGL(int w, int h)
{
    glViewport(0, 0, 640, 480);

    drawing.initFrustum();
    /*glViewport(0, 0, w, h);
        /*glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();*/

}

void GLView::mouseMoveEvent(QMouseEvent *event)
{
    if( event != NULL ) {
        QPoint pos = event->pos();

        if(event->buttons() == Qt::LeftButton) angle += (pos.x() - position.x());
        std::cout << drawing.worldPoints.size() << std::endl;
        position = pos;
        updateGL();
    }
}

void GLView::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL ) {
    }
}
