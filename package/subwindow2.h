#ifndef SUBWINDOW2_H
#define SUBWINDOW2_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <iostream>
#include <QPoint>


namespace Ui {
class SubWindow2;
}

class SubWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubWindow2(QWidget *parent = nullptr);
    ~SubWindow2();


private slots:
    //void on_picture_choose_clicked();

    //void on_picture_long_clicked();

    void on_pushButton_clicked();

    void on_long_picture_button_clicked();


    void on_picture_combine_modle1_clicked();

    void on_picture_combine_modle_2_clicked();

    void on_output_button_clicked();

    void mousePressEvent(QMouseEvent *event);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_ExitToMain2_button_clicked();

    void on_Rotat_clockwise_button_clicked();

    void on_Revoke_button_clicked();

private:
    Ui::SubWindow2 *ui;
};

#endif // SUBWINDOW2_H
