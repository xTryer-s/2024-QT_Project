QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SubWindow1.cpp \
    main.cpp \
    mainwindow.cpp \
    subwindow1.cpp \
    subwindow1_picture_process.cpp \
    subwindow2.cpp \
    subwindow3.cpp \
    subwindow4.cpp

HEADERS += \
    SubWindow1.h \
    mainwindow.h \
    subwindow1.h \
    subwindow1_picture_process.h \
    subwindow2.h \
    subwindow2_picture_process.h \
    subwindow3.h \
    subwindow3_gif_process.h \
    subwindow4.h

FORMS += \
    mainwindow.ui \
    subwindow1.ui \
    subwindow2.ui \
    subwindow3.ui \
    subwindow4.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$PWD/"../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/ffmpeg/lib" -lavcodec \
        -L$$PWD/"../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/ffmpeg/lib" -lavdevice \
        -L$$PWD/"../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/ffmpeg/lib" -lavfilter \
        -L$$PWD/"../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/ffmpeg/lib" -lavformat \
        -L$$PWD/"../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/ffmpeg/lib" -lavutil \
        -L$$PWD/"../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/ffmpeg/lib" -lpostproc \
        -L$$PWD/"../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/ffmpeg/lib" -lswresample \
        -L$$PWD/"../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/ffmpeg/lib" -lswscale \

INCLUDEPATH += $$PWD/../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/ffmpeg/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/opencv_4.5.5/opencv/build/x64/vc14/lib/ -lopencv_world455
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/opencv_4.5.5/opencv/build/x64/vc14/lib/ -lopencv_world455d
else:unix: LIBS += -L$$PWD/../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/opencv_4.5.5/opencv/build/x64/vc14/lib/ -lopencv_world455

INCLUDEPATH += $$PWD/../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/opencv_4.5.5/opencv/build/include
DEPENDPATH += $$PWD/../build-package-Desktop_Qt_5_14_2_MSVC2017_64bit-Debug/opencv_4.5.5/opencv/build/include

RESOURCES += \
    pictures.qrc
