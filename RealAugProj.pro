#-------------------------------------------------
#
# Project created by QtCreator 2018-01-10T09:55:01
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_features2d -lopencv_videoio -lopencv_calib3d -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lGLU


TARGET = RealAugProj
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        maindisplay.cpp \
        glview.cpp \
        cvtogl.cpp \
        include/tiny_obj_loader.cc

HEADERS += \
        maindisplay.h \
        glview.h \
        cvtogl.h \
        include/tiny_obj_loader.h

FORMS += \
        maindisplay.ui

obj.path   = $${OUT_PWD}/obj
obj.files  = obj/*
INSTALLS    += obj

QMAKE_CXXFLAGS += -std=c++11
