#ifndef SUBWINDOW4_H
#define SUBWINDOW4_H


#include <opencv2/opencv.hpp>
#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <iostream>
#include <QPoint>
#include <QFileDialog>
#include<QInputDialog>
#include<cstring>
#include <windows.h>

extern "C"   //ffmpeg是采用c语言实现的 c++工程种 导入c语言
{
    //avcodec:编解码(最重要的库)
    #include <libavcodec/avcodec.h>
    //avformat:封装格式处理
    #include <libavformat/avformat.h>
    //swscale:视频像素数据格式转换
    #include <libswscale/swscale.h>
    //avdevice:各种设备的输入输出
    #include <libavdevice/avdevice.h>
    //avutil:工具库（大部分库都需要这个库的支持）
    #include <libavutil/avutil.h>
}
using namespace std;
using namespace cv;

namespace Ui {
class SubWindow4;
}

class SubWindow4 : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubWindow4(QWidget *parent = nullptr);
    ~SubWindow4();

private slots:
    void on_ExitToMain_button_clicked();

    void on_InputVideo_button_clicked();

    void on_separate_musicvideo_button_clicked();

    void on_PlayVideo_button_clicked();

    void on_SetSpeed_button_clicked();

    void on_Set_effect_box_activated(int index);

    void on_CreateNewVideo_button_clicked();

    void on_PlayNewVideo_button_clicked();

    void on_OutputVideo_button_clicked();

private:
    Ui::SubWindow4 *ui;
};

#endif // SUBWINDOW4_H
