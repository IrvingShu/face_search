#-------------------------------------------------
#
# Project created by QtCreator 2016-08-26T09:52:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gallery.cpp \
    facedetector.cpp \
    classifier.cpp \
    utils.cpp \
    probe.cpp

HEADERS  += mainwindow.h \
    gallery.h \
    facedetector.h \
    classifier.h \
    utils.h \
    probe.h

FORMS    += mainwindow.ui

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_ml \
        -L/home/young/facesearch/build-face-Desktop_Qt_5_5_0_GCC_64bit-Debug/dlib-18.18 -ldlib -lpthread -lX11 -ljpeg \
        -L/home/young/facesearch/build-face-Desktop_Qt_5_5_0_GCC_64bit-Debug/caffe/lib -lcaffe
LIBS += -lglog -lgflags -lprotobuf -lboost_system -lboost_thread  -latlas

INCLUDEPATH += /usr/local/include \
               /home/young/facesearch/build-face-Desktop_Qt_5_5_0_GCC_64bit-Debug \
               /home/young/facesearch/build-face-Desktop_Qt_5_5_0_GCC_64bit-Debug/caffe/include \
              /home/young/facesearch/build-face-Desktop_Qt_5_5_0_GCC_64bit-Debug/caffe/src \
