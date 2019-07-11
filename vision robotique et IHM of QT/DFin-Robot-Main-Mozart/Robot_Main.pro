#-------------------------------------------------
#
# Project created by QtCreator 2012-07-09T19:07:02
#
#------------------------------------------------
CONFIG += console

QT += serialport
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Robot_Main
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ImageDispose/robot_imagedispose.cpp \
    ImageDispose/image_circle.cpp \
    Strategists/samplestrategist.cpp \
    Strategists/robot_strategist.cpp \
    Strategists/strategy_ball.cpp \
    Strategists/strategy_guard.cpp \
    ImageDispose/hawkseye.cpp \
    saveopen_dialog.cpp \
    Strategists/robot_zap.cpp \
    Strategists/distance.cpp \
    ImageDispose/hyperpoint.cpp \
    Strategists/3v3/goalkeeper3v3.cpp \
    Strategists/3v3/striker3v3.cpp \
    Strategists/strategy_pointdc.cpp

HEADERS  += mainwindow.h \
    imgdisstuff.h \
    ImageDispose/sampleImageDispose.h \
    ImageDispose/Robot_ImageDispose_global.h \
    ImageDispose/robot_imagedispose.h \
    ImageDispose/image_circle.h \
    Strategists/samplestrategist.h \
    Strategists/Robot_Strategist_global.h \
    Strategists/robot_strategist.h \
    strategistsstuff.h \
    Strategists/Strategy_Ball.h \
    Strategists/Strategy_Guard.h \
    ImageDispose/hawkseye.h \
    saveopen_dialog.h \
    Strategists/robot_zap.h \
    Strategists/distance.h \
    ImageDispose/hyperpoint.h \
    Strategists/3v3/goalkeeper3v3.h \
    Strategists/3v3/striker3v3.h \
    Strategists/strategy_pointdc.h

FORMS    += mainwindow.ui \
    saveopen_dialog.ui

#opencv (mingw 2.3.1)

INCLUDEPATH += D:\set_up\openCV\opencv\build\x64\mingw\install\include
LIBS += D:\set_up\openCV\opencv\build\x64\mingw\install\x64\mingw\bin\libopencv_*.dll
