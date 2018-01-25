#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QApplication>
#include <QOpenGLFunctions_2_0>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "cvtogl.h"

class GLView : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLView(QWidget *parent = 0);

    virtual void initializeGL();

    virtual void paintGL();

    virtual void resizeGL(int w, int h);


    CVtoGL drawing;

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );

private:
    float angle;
    QPoint position;
};


#endif // GLVIEW_H
