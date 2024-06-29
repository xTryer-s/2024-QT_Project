#ifndef SUBWINDOW1_H
#define SUBWINDOW1_H

#include <QMainWindow>
#include<opencv2/opencv.hpp>
#include <QFileDialog>
#include<QInputDialog>
#include <QMessageBox>

namespace Ui {
class SubWindow1;
}

class SubWindow1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubWindow1(QWidget *parent = nullptr);
    ~SubWindow1();

private slots:
    void on_actionInput_Picture_triggered();

    void on_Rotate_button_clicked();

    void on_Backtoorigin_button_clicked();

    void on_Flip_Vertical_button_clicked();


    void on_Flip_Horizontal_button_clicked();


    void on_Brightness_down_button_clicked();

    void on_Brightness_up_button_clicked();

    void on_RevokeOperation_button_clicked();

    void on_GaussianBlur_button_clicked();

    void on_Output_button_clicked();


    void on_Contrast_up_button_clicked();

    void on_Contrast_down_button_clicked();

    void on_actionOutput_Picture_triggered();

    void on_SpecialEffects_comboBox_activated(int index);

    void on_Crop_button_clicked();

    void on_FreeDraw_button_clicked();


    void on_ExitToMain_button_clicked();

    void on_Input_button_clicked();

    void on_AddText_button_clicked();

    void on_Stickpicture_button_clicked();

private:
    Ui::SubWindow1 *ui;

};

#endif // SUBWINDOW1_H
