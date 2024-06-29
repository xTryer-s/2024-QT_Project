#ifndef SUBWINDOW3_H
#define SUBWINDOW3_H

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


namespace Ui {
class SubWindow3;
}

class SubWindow3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubWindow3(QWidget *parent = nullptr);
    ~SubWindow3();

private slots:
    void on_images2gif_button_clicked();

    void on_ExitToMain_button_clicked();

    void on_OutputGIF_button_clicked();

    void on_video2gif_button_clicked();

private:
    Ui::SubWindow3 *ui;
};

#endif // SUBWINDOW3_H
