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
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CCW);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glColor3f(1.0, 1.0, 0.0);
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    drawing.drawChessBoard(7, 4, 31.0);

    update();
}

void GLView::resizeGL(int w, int h)
{
    glViewport(0, 0, 640, 480);
    drawing.initFrustum();

    /*glMatrixMode(GL_PROJECTION);

    glViewport(0, 0, w, h);

    glOrtho(-FRUSTUM_SIZE, FRUSTUM_SIZE,
            -FRUSTUM_SIZE, FRUSTUM_SIZE,
            -FRUSTUM_SIZE, FRUSTUM_SIZE);

    glMatrixMode(GL_MODELVIEW);*/
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
