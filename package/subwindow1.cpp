#include "subwindow1.h"
#include "ui_subwindow1.h"
#include "subwindow1_picture_process.h"



#include <qpainter.h>
#include<opencv2/opencv.hpp>
#include<random>
#include<cmath>
#include<stack>
#include<vector>

using namespace cv;



Mat origin_img;//原图
Mat former_Matimg;//上一步图
Mat display_Matimg;//修改图 mat形式
QImage display_Qimg;//修改图 qimg形式
std::stack<Mat> revoke_ps_img_history;

SubWindow1::SubWindow1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SubWindow1)
{
    ui->setupUi(this);
}

SubWindow1::~SubWindow1()
{
    delete ui;
}



int origin_display_picture_width=1920;
int origin_display_picture_height=1080;


void SubWindow1::on_Rotate_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);

    bool input_angle_ok;
    int input_rotate_angle = QInputDialog::getInt(this, "Input Rotate Angle", "Please Input a Angle:", 0, 0, 360, 1, &input_angle_ok);
    if(input_angle_ok)
    {
        Mat rotate_img;
        Point2f rotate_center((display_Matimg.cols - 1) / 2.0, (display_Matimg.rows - 1) / 2.0);
        Mat rotate_matrix = getRotationMatrix2D(rotate_center, input_rotate_angle,1);
        warpAffine(display_Matimg, rotate_img, rotate_matrix,display_Matimg.size());

        display_Matimg=rotate_img.clone();
        Mat tmp_img;
        cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
  display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

        //         ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));

        if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
        {
            ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
            std::cout<<"type1"<<std::endl;
            ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
        }
        else
        {
            std::cout<<"type2"<<std::endl;
            ui->picture_display->resize(display_Qimg.size());
            ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
            std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
        }
    }
    else
    {
        std::cerr << "Error:Error Input" << std::endl;
        return;
    }
}

void SubWindow1::on_Backtoorigin_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);

    display_Matimg=origin_img.clone();

    Mat tmp_img;
    cvtColor(origin_img,tmp_img,COLOR_BGR2RGB);
  display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }
}

void SubWindow1::on_Flip_Vertical_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);
    int input_fliptype=0;
    Mat flip_img;
    flip(display_Matimg,flip_img,input_fliptype);

    display_Matimg=flip_img.clone();

    Mat tmp_img;
    cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
    display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }
}

void SubWindow1::on_Flip_Horizontal_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);
    int input_fliptype=1;
    Mat flip_img;
    flip(display_Matimg,flip_img,input_fliptype);

    display_Matimg=flip_img.clone();

    Mat tmp_img;
    cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
    display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }
}

void SubWindow1::on_Brightness_down_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);
    Mat brightness_bar_src = display_Matimg.clone();
    Mat brightness_bar_dst = Mat::zeros(brightness_bar_src.size(), brightness_bar_src.type());
    Mat brightness_bar_operator_mat = Mat::zeros(brightness_bar_src.size(), brightness_bar_src.type());

    if (brightness_bar_src.channels() == 1)brightness_bar_operator_mat = Scalar(5);
    else if (brightness_bar_src.channels() == 3)brightness_bar_operator_mat = Scalar(5,5,5);

    subtract(brightness_bar_src, brightness_bar_operator_mat, brightness_bar_dst);


    display_Matimg=brightness_bar_dst.clone();

    Mat tmp_img;
    cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
    display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }

}

void SubWindow1::on_Brightness_up_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);
    Mat brightness_bar_src = display_Matimg.clone();
    Mat brightness_bar_dst = Mat::zeros(brightness_bar_src.size(), brightness_bar_src.type());
    Mat brightness_bar_operator_mat = Mat::zeros(brightness_bar_src.size(), brightness_bar_src.type());

    if (brightness_bar_src.channels() == 1)brightness_bar_operator_mat = Scalar(5);
    else if (brightness_bar_src.channels() == 3)brightness_bar_operator_mat = Scalar(5,5,5);

    add(brightness_bar_src, brightness_bar_operator_mat, brightness_bar_dst);


    display_Matimg=brightness_bar_dst.clone();

    Mat tmp_img;
    cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
    display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }
}

void SubWindow1::on_RevokeOperation_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    if(revoke_ps_img_history.empty())display_Matimg=origin_img.clone();
    else
    {
        display_Matimg=revoke_ps_img_history.top();
        revoke_ps_img_history.pop();
    }


    Mat tmp_img;
    cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
    display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }

}

void SubWindow1::on_GaussianBlur_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);

    bool input_blur_ok;
    Mat blur_img;
    int input_blur_ratio = QInputDialog::getInt(this, "Input Blur Ratio", "Please Input Blur Ratio:", 0, 0, 200, 1, &input_blur_ok);
    if(input_blur_ok)
    {
        GaussianBlur(display_Matimg, blur_img, Size(21, 21),input_blur_ratio, 0);
    }
    else
    {
        std::cerr << "Error:Error Input" << std::endl;
        return;
    }

    display_Matimg=blur_img.clone();
    Mat tmp_img;
    cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
  display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }

}

void SubWindow1::on_Output_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    QString outputFilePath = QFileDialog::getSaveFileName(this, "保存图片", "", "Image Files (*.png *.jpg *.jpeg)");
        if (outputFilePath.isEmpty()) {
            std::cerr<<"Your OutPutFilePath is Error~"<<std::endl;
        }
        // 保存图像文件
        if (!cv::imwrite(outputFilePath.toStdString(), display_Matimg)) {
            std::cerr<< "Failed to save image file"<<std::endl;
        }

}



void SubWindow1::on_Contrast_up_button_clicked()
{

    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);

    Mat contrast_bar_src = display_Matimg.clone();
    Mat contrast_bar_dst = Mat::zeros(display_Matimg.size(), display_Matimg.type());
    Mat contrast_bar_operator_mat = Mat::zeros(contrast_bar_src.size(), contrast_bar_dst.type());


    double contrast = 1.1;
    addWeighted(contrast_bar_src, contrast, contrast_bar_operator_mat, 0.0, 0, contrast_bar_dst);

    display_Matimg=contrast_bar_dst.clone();

    Mat tmp_img;
    cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
    display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }

}

void SubWindow1::on_Contrast_down_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);

    Mat contrast_bar_src = display_Matimg.clone();
    Mat contrast_bar_dst = Mat::zeros(display_Matimg.size(), display_Matimg.type());
    Mat contrast_bar_operator_mat = Mat::zeros(contrast_bar_src.size(), contrast_bar_dst.type());


    double contrast = 0.909;
    addWeighted(contrast_bar_src, contrast, contrast_bar_operator_mat, 0.0, 0, contrast_bar_dst);
    display_Matimg=contrast_bar_dst.clone();

    Mat tmp_img;
    cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
     display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }
}

void SubWindow1::on_actionOutput_Picture_triggered()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    QString outputFilePath = QFileDialog::getSaveFileName(this, "保存图片", "", "Image Files (*.png *.jpg *.jpeg)");
        if (outputFilePath.isEmpty()) {
            std::cerr<<"Your OutPutFilePath is Error~"<<std::endl;
        }
        // 保存图像文件
        if (!cv::imwrite(outputFilePath.toStdString(), display_Matimg)) {
            std::cerr<< "Failed to save image file"<<std::endl;
        }

}

cv::Mat show_eclosion_img;
Point eclosion_point;
bool eclosion_flag=false;
int eclosion_degree;

void Eclosion_handle_mouse(int event, int x, int y, int, void*) {
    if (event == EVENT_LBUTTONDOWN) {
        eclosion_point=Point(x,y);
        bool elcosion_degree_flag=false;
        eclosion_degree = QInputDialog::getInt(nullptr, "Input Eclosion Degree", "Please Set Your Eclosion Degree:", 3, 3, 10, 1, &elcosion_degree_flag);
        if(elcosion_degree_flag==true)
        {
            eclosion_flag=true;
        }
    }
}



bool specialeffect_flag=false;
Mat specialeffect_origin_img;
void SubWindow1::on_SpecialEffects_comboBox_activated(int index)
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    std::cout<<display_Matimg.channels()<<std::endl;
    if(display_Matimg.channels()!=3 && specialeffect_flag==false)
    {
        QMessageBox::StandardButton SpecialEffect_Warning;
        SpecialEffect_Warning=QMessageBox::information(nullptr,"SpecialEffect_Warning",
                                                 "You Can Only Apply SpecialEffects to 3dims image."
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);

    if(specialeffect_flag==false)
    {
        specialeffect_origin_img=display_Matimg.clone();
        specialeffect_flag=true;
    }
    else display_Matimg=specialeffect_origin_img.clone();

    Mat special_effect_img;
    if(index==0)
    {
        return;
    }
    else if(index==1)
    {
        special_effect_img=display_Matimg.clone();
    }
    else if(index==2)
    {
         cvtColor(display_Matimg, special_effect_img, COLOR_BGRA2GRAY);
    }
    else if(index==3)
    {
        cv::Mat tmp10_con = specialeffect_Contrast(display_Matimg, 25);

        cv::Mat tmp10_lig = specialeffect_Lightness(tmp10_con, -30);

        cv::Mat tmp10_hig = specialeffect_HighLight(tmp10_lig, 20);

        cv::Mat tmp10_sha = specialeffect_Sharpen(tmp10_hig, 50, 0);

        cv::Mat tmp10_sat = specialeffect_Saturation(tmp10_sha, 20);

        cv::Mat tmp10_col = specialeffect_ColorTemperature(tmp10_sat, -20);

        special_effect_img=tmp10_col.clone();
    }
    else if(index==4)
    {
       cv::Mat tmp11_img=specialeffect_warmer(display_Matimg,30);

       special_effect_img=tmp11_img.clone();
    }
    else if(index==5)
    {
        cv::Mat tmp12_img=specialeffect_Nostalgic(display_Matimg);

        special_effect_img=tmp12_img.clone();
    }
    else if(index==6)
    {
        show_eclosion_img=display_Matimg.clone();

        eclosion_flag=false;
        // 创建窗口
        namedWindow("Choose Eclosion Center", WINDOW_AUTOSIZE);

        // 设置鼠标回调函数
        setMouseCallback("Choose Eclosion Center", Eclosion_handle_mouse);

        while (true)
        {
            imshow("Choose Eclosion Center", show_eclosion_img);
            int key=waitKey(1);
            if (eclosion_flag==true)
            {
                cv::Mat tmp12_img=specialeffect_Eclosion(display_Matimg,eclosion_point,(float)eclosion_degree/(float)10);
                special_effect_img=tmp12_img.clone();
                break;
            }
            else if(key==27)
            {
                eclosion_flag=false;
                return;
            }
        }


       destroyWindow("Choose Eclosion Center");

    }
    else if(index==7)
    {
        cv::Mat tmp14_img=specialeffect_embossed(display_Matimg);

        special_effect_img=tmp14_img.clone();
    }
    else if(index==8)
    {
        cv::Mat tmp15_img=specialeffect_cartoon2(display_Matimg);

        special_effect_img=tmp15_img.clone();
    }
    else if(index==9)
    {
        cv::Mat tmp16_img=specialeffect_apply_sketch(display_Matimg);

        special_effect_img=tmp16_img.clone();
    }

    else if(index==10)
    {
        cvtColor(display_Matimg, special_effect_img, COLOR_BGR2HSV);
    }
    else if(index==11)
    {
        Canny(display_Matimg,special_effect_img, 25,75);
    }
    else if(index==12)
    {
        cvtColor(display_Matimg,special_effect_img,COLOR_BGR2HLS_FULL);//满饱和
    }
    else if(index==13)
    {
        cvtColor(display_Matimg,special_effect_img,COLOR_BGR2Lab);
    }
    else if(index==14)
    {
        cvtColor(display_Matimg,special_effect_img,COLOR_BGR2Luv);
    }
    else if(index==15)
    {
        cvtColor(display_Matimg,special_effect_img,COLOR_BGR2XYZ);
    }
    else if(index==16)
    {
        cvtColor(display_Matimg,special_effect_img,COLOR_BGR2YUV);
    }



    display_Matimg=special_effect_img.clone();

    Mat tmp_img;
    cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
  display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }
}




bool crop_first_point_flag=false;
Point crop_first_point; // 第一个点
bool crop_second_point_flag=false;
Point crop_second_point; // 第二个点

bool crop_agree_flag=false;

Scalar crop_color = Scalar(0, 0, 255); // 标记颜色

Mat crop_img;
Mat crop_img_origin;

// 鼠标回调函数
void crop_handle_mouse(int event, int x, int y, int, void*) {
    if (event == EVENT_LBUTTONDOWN && crop_first_point_flag==false) {
        crop_first_point=Point(x,y);
        circle(crop_img,crop_first_point, 10, Scalar(0, 0, 255), FILLED);//画圈标记
        crop_first_point_flag=true;

    }
    else if (event == EVENT_LBUTTONDOWN && crop_first_point_flag==true) {
        crop_second_point=Point(x,y);
        circle(crop_img,crop_second_point, 10, Scalar(0, 0, 255), FILLED);//画圈标记
        crop_second_point_flag=true;

    }
    else if (event == EVENT_RBUTTONDOWN) {
        crop_first_point_flag=false;
        crop_second_point_flag=false;
        crop_img=crop_img_origin.clone();
    }
}

void SubWindow1::on_Crop_button_clicked()
{

    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    former_Matimg=display_Matimg.clone();
    revoke_ps_img_history.push(former_Matimg);


    crop_first_point_flag=false;
    crop_second_point_flag=false;
    crop_agree_flag=false;

    crop_img = display_Matimg.clone();
    crop_img_origin = display_Matimg.clone();
    crop_first_point_flag=false;
    crop_second_point_flag=false;
    // 创建窗口
    namedWindow("Crop(Press ESC to Cancel)", WINDOW_FREERATIO);

    // 设置鼠标回调函数
    setMouseCallback("Crop(Press ESC to Cancel)", crop_handle_mouse);


    while (true) {
        imshow("Crop(Press ESC to Cancel)", crop_img);

        int key = waitKey(1);
        if (key == 27) { // 按下ESC键退出
            break;
        }

        if(crop_agree_flag)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            int crop_x1=crop_first_point.x;
            int crop_y1=crop_first_point.y;
            int crop_x2=crop_second_point.x;
            int crop_y2=crop_second_point.y;

            int crop_height=abs(crop_y1-crop_y2);
            int crop_width=abs(crop_x1-crop_x2);

            int crop_x_final=min(crop_x1,crop_x2);
            int crop_y_final=min(crop_y1,crop_y2);

             Rect roi_(crop_x_final, crop_y_final, crop_width,crop_height);
             Mat final_crop_img=crop_img_origin(roi_);

             if(final_crop_img.cols%2==1)
             {
                 cv::Size target_size(final_crop_img.cols+1,final_crop_img.rows);
                 cv::Mat resized_image;
                 cv::resize(final_crop_img,resized_image,target_size);
                 final_crop_img=resized_image.clone();
             }


             display_Matimg=final_crop_img.clone();

             Mat tmp_img;
             cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
             display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

             if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
             {
                 ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
                 std::cout<<"type1"<<std::endl;
                 ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
             }
             else
             {
                 std::cout<<"type2"<<std::endl;
                 ui->picture_display->resize(display_Qimg.size());
                 ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
                 std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
             }
             break;
        }



        if(crop_first_point_flag && crop_second_point_flag)
        {


            int crop_x1=crop_first_point.x;
            int crop_y1=crop_first_point.y;
            int crop_x2=crop_second_point.x;
            int crop_y2=crop_second_point.y;

            line(crop_img, Point(crop_x1,crop_y1), Point(crop_x1,crop_y2), Scalar(255,0,0), 5, LINE_AA); // 画线
            line(crop_img, Point(crop_x2,crop_y1), Point(crop_x2,crop_y2), Scalar(255,0,0), 5, LINE_AA);
            line(crop_img, Point(crop_x1,crop_y1), Point(crop_x2,crop_y1), Scalar(255,0,0), 5, LINE_AA);
            line(crop_img, Point(crop_x1,crop_y2), Point(crop_x2,crop_y2), Scalar(255,0,0), 5, LINE_AA);



            imshow("Crop(Press ESC to Cancel)", crop_img);

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, "Are You Sure?", "Click Yes if you want to crop this way.",
                                         QMessageBox::Yes | QMessageBox::No);



           if (reply == QMessageBox::Yes)
           {
               crop_agree_flag=true;
           }
           else
           {
               crop_first_point_flag=false;
               crop_second_point_flag=false;
               crop_img=crop_img_origin.clone();
           }

        }
    }


    destroyWindow("Crop(Press ESC to Cancel)");
}




int freedraw_colorPalette_length=512;

Mat hsv_freedraw_colorPalette(freedraw_colorPalette_length, freedraw_colorPalette_length, CV_8UC3);//调色板
Mat bgr_freedraw_colorPalette;//调色板

bool freedraw_drawing = false; // 是否正在绘画
bool freedraw_saving_flag=false;

Point freedraw_prev_point; // 前一个点
int freedraw_thickness = 2; // 线条粗细
Scalar freedraw_color = Scalar(255, 0, 0); // 线条颜色

Mat freedraw_img;
Mat freedraw_img_origin;

bool freedraw_erasing = false; // 是否启用橡皮擦
int freedraw_mosaic = 0; // 马赛克模式
std::stack<Mat> freedraw_history; // 用于存储绘画历史

// 随机数生成器
std::random_device freedraw_rd;
std::mt19937 gen(freedraw_rd());
std::uniform_int_distribution<> freedraw_dis(0, 255);

bool freedraw_colorpalette_flag=false;



void apply_mosaic1(Mat& image, Rect roi,int mosaic_size)
{
    for (int y = roi.y; y < roi.y + roi.height; y += mosaic_size) {
        for (int x = roi.x; x < roi.x + roi.width; x += mosaic_size) {
            Scalar random_color(freedraw_dis(gen), freedraw_dis(gen), freedraw_dis(gen));
            rectangle(image, Point(x, y), Point(x + mosaic_size, y + mosaic_size), random_color, FILLED);
        }
    }
}


void apply_mosaic2(Mat& image, Rect roi,int mosaic_size)
{
    for (int y = roi.y; y < roi.y + roi.height; y += mosaic_size) {
        for (int x = roi.x; x < roi.x + roi.width; x += mosaic_size) {
            Scalar random_color(freedraw_dis(gen), freedraw_dis(gen), freedraw_dis(gen));
            rectangle(image, Point(x, y), Point(x + mosaic_size, y + mosaic_size), random_color, FILLED);
        }
    }
}





// 鼠标回调函数
void freedraw_colorpalette_handle_mouse(int event, int x, int y, int, void*)
{
    if(event== EVENT_LBUTTONDOWN)
    {
        freedraw_colorpalette_flag=true;
        freedraw_color=Scalar(bgr_freedraw_colorPalette.at<cv::Vec3b>(y, x));
        //

    }

}

// 鼠标回调函数
void freedraw_handle_mouse(int event, int x, int y, int, void*) {
    if (event == EVENT_LBUTTONDOWN) {
        freedraw_drawing = true;
        freedraw_prev_point = Point(x, y);
        freedraw_history.push(freedraw_img.clone());
    }
    else if (event == EVENT_MOUSEMOVE && freedraw_drawing) {
        Point current_point(x, y);
        if (freedraw_erasing) {
            //line(img, prev_point, current_point, Scalar(0, 0, 0), thickness, LINE_AA); // 用黑色模拟橡皮擦
            int half_thickness = freedraw_thickness / 2;
            int start_x = max(0, x - half_thickness);
            int start_y = max(0, y - half_thickness);
            int end_x = min(freedraw_img.cols, x + half_thickness);
            int end_y = min(freedraw_img.rows, y + half_thickness);

            // 计算实际的ROI宽度和高度
            int roi_width = end_x - start_x;
            int roi_height = end_y - start_y;

            if (roi_width > 0 && roi_height > 0) {
                Rect roi(Point(start_x, start_y), Size(roi_width, roi_height));
                freedraw_img_origin(roi).copyTo(freedraw_img(roi));
            }
        }
        else if(freedraw_mosaic==1)
        {
            int start_x = max(0, min(freedraw_prev_point.x, x));
            int start_y = max(0, min(freedraw_prev_point.y, y));
            int end_x = min(freedraw_img.cols, max(freedraw_prev_point.x, x));
            int end_y = min(freedraw_img.rows, max(freedraw_prev_point.y, y));

            // 计算实际的ROI宽度和高度
            int roi_width = end_x - start_x;
            int roi_height = end_y - start_y;

            if (roi_width > 0 && roi_height > 0) {
                Rect roi(Point(start_x, start_y), Size(roi_width, roi_height));
                Scalar freedraw_mosaic_average=mean(freedraw_img_origin(roi));

                freedraw_img(roi).setTo(freedraw_mosaic_average);
            }
        }
        else if (freedraw_mosaic==2) {
            // 确保不超出边界
            int start_x = max(0, min(freedraw_prev_point.x, x));
            int start_y = max(0, min(freedraw_prev_point.y, y));
            int end_x = min(freedraw_img.cols, max(freedraw_prev_point.x, x));
            int end_y = min(freedraw_img.rows, max(freedraw_prev_point.y, y));

            // 计算实际的ROI宽度和高度
            int roi_width = end_x - start_x;
            int roi_height = end_y - start_y;

            if (roi_width > 0 && roi_height > 0) {
                Rect roi(Point(start_x, start_y), Size(roi_width, roi_height));
                apply_mosaic2(freedraw_img, roi, freedraw_thickness); // 使用当前画笔粗细作为马赛克块大小
            }
        }
        else {
            line(freedraw_img, freedraw_prev_point, current_point, freedraw_color, freedraw_thickness, LINE_AA); // 画线
        }
        freedraw_prev_point = current_point;
    }
    else if (event == EVENT_LBUTTONUP) {
        freedraw_drawing = false;
    }
}








// 键盘事件处理函数
void freedraw_handle_key(int key)
{
    switch (key) {
    case 'r':
        // 创建窗口 调色板
        namedWindow("Free Drawing-ColorPalette(Press ESC To Cancel)", WINDOW_NORMAL);
        // 设置鼠标回调函数
        setMouseCallback("Free Drawing-ColorPalette(Press ESC To Cancel)", freedraw_colorpalette_handle_mouse);

        while (true)
        {
            imshow("Free Drawing-ColorPalette(Press ESC To Cancel)", bgr_freedraw_colorPalette);
            int freedraw_key = waitKey(1);
            if (freedraw_key == 27 || freedraw_colorpalette_flag==true)
            { // 按下ESC键退出 选择完毕退出
                break;
            }
        }

        freedraw_colorpalette_flag=false;
        destroyWindow("Free Drawing-ColorPalette(Press ESC To Cancel)");

        freedraw_erasing = false;
        freedraw_mosaic = 0;
        break;

    case 'w':
        freedraw_color=Scalar(255,255,255);
    case '+':
        if (freedraw_thickness < 50) freedraw_thickness++; // 增加线条粗细
        break;
    case '-':
        if (freedraw_thickness > 1) freedraw_thickness--; // 减少线条粗细
        break;
    case 'e':
        freedraw_erasing = !freedraw_erasing; // 切换橡皮擦模式
        freedraw_mosaic = 0;
        break;
    case 'c':
        freedraw_img = freedraw_img_origin.clone(); // 清空画布
        while (!freedraw_history.empty()) {
            freedraw_history.pop(); // 清空历史记录
        }
        break;
    case 'z':
        if (!freedraw_history.empty()) {
            freedraw_img = freedraw_history.top().clone(); // 恢复上一状态
            freedraw_history.pop();
        }
        break;
    case 'm':
        freedraw_mosaic = (freedraw_mosaic+1)%3;
        freedraw_erasing = false;
        break;
    case 's':

        QMessageBox::StandardButton freedraw_reply;
        freedraw_reply = QMessageBox::question(nullptr, "Are You Sure?", "Click Yes if you want to draw this way.",
                                     QMessageBox::Yes | QMessageBox::No);

       if (freedraw_reply == QMessageBox::Yes)
       {
         freedraw_saving_flag=true;
       }
        break;
    case 'h':
//        reply = QMessageBox::question(nullptr, "Are You Sure?", "Click Yes if you want to draw this way.",
//                                     QMessageBox::Yes | QMessageBox::No);
        QMessageBox::StandardButton freedraw_helper;
        freedraw_helper=QMessageBox::information(nullptr,"Freedraw Helper",
                                                 "Use Your Keyboard:\n"
                                                 "R:Change The Color of Your Brush\n"
                                                 "W:Use White Brush\n"
                                                 "+/-:Change The Size of Your Brush\n"
                                                 "E:Use Eraser\n"
                                                 "M:Use Mosaic\n"
                                                 "C:Clean Your Canvas\n"
                                                 "Z:Revoke The Previous Step\n"
                                                 "ESC:Quit FreeDraw Without Saving\n"
                                                 "S:Quit FreeDraw With Saving"
                                                 );
        break;
    }
}


void SubWindow1::on_FreeDraw_button_clicked()
{

    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }


     former_Matimg=display_Matimg.clone();
     revoke_ps_img_history.push(former_Matimg);

     freedraw_img_origin=display_Matimg.clone();
     freedraw_img=display_Matimg.clone();


     for (int i = 0; i < 512; ++i) {
          for (int j = 0; j < 512; ++j)
          {

              hsv_freedraw_colorPalette.at<cv::Vec3b>(i, j) = cv::Vec3b(i*180/freedraw_colorPalette_length, 200, j/(freedraw_colorPalette_length/256));
          }
      }

      cvtColor(hsv_freedraw_colorPalette,bgr_freedraw_colorPalette,COLOR_HSV2BGR);//初始化调色盘


      freedraw_saving_flag=false;

     // 创建窗口
     namedWindow("Free Drawing(Press H To Get Help)", WINDOW_AUTOSIZE);

     // 设置鼠标回调函数
     setMouseCallback("Free Drawing(Press H To Get Help)", freedraw_handle_mouse);

     while (true) {
         imshow("Free Drawing(Press H To Get Help)", freedraw_img);
         int freedraw_key = waitKey(1);
         if (freedraw_key == 27 || freedraw_saving_flag==true) { // 按下ESC键退出

             if(freedraw_saving_flag==true)
             {
                 display_Matimg=freedraw_img.clone();
                 specialeffect_flag=false;//绘画后重置

                 Mat tmp_img;
                 cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
                 display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

                 if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
                 {
                     ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
                     std::cout<<"type1"<<std::endl;
                     ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
                 }
                 else
                 {
                     std::cout<<"type2"<<std::endl;
                     ui->picture_display->resize(display_Qimg.size());
                     ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
                     std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
                 }
             }
             break;
         }
         else if (freedraw_key != -1) {
             freedraw_handle_key(freedraw_key); // 处理键盘事件
         }
     }

    destroyWindow("Free Drawing(Press H To Get Help)");
}







int addtext_colorPalette_length=512;

Mat hsv_addtext_colorPalette(addtext_colorPalette_length, addtext_colorPalette_length, CV_8UC3);//调色板
Mat bgr_addtext_colorPalette;//调色板



int addtext_thickness = 2; // 线条粗细
Scalar addtext_color = Scalar(255, 0, 0); // 线条颜色

Point addtext_point;//添加文字的左上角

Mat addtext_img;
Mat addtext_img_origin;

std::stack<Mat> addtext_history; // 用于存储绘画历史

bool addtext_saving_flag=false;
bool addtext_colorpalette_flag=false;


void addtext_colorpalette_handle_mouse(int event, int x, int y, int, void*)
{
    if(event== EVENT_LBUTTONDOWN)
    {
        addtext_colorpalette_flag=true;
        addtext_color=Scalar(bgr_addtext_colorPalette.at<cv::Vec3b>(y, x));


    }


}

int myPutText(cv::Mat &tmp_addtext_img, QString tmp_addtext_text, QPoint tmp_addtext_org, QFont tmp_addtext_font, QPen tmp_addtext_pen)
{
    QImage::Format tmp_addtext_imgFormat;

    switch (tmp_addtext_img.channels()) {
    case 1:
        tmp_addtext_imgFormat = QImage::Format_Grayscale8;
        break;
    case 3:
        tmp_addtext_imgFormat = QImage::Format_RGB888; // 没有BGR格式，因此要注意一下pen的color
        break;
    default:
        return -1;
        break;
    }

        // 直接共享了Mat的内存
    QImage tmp_addtext_tmpImg(tmp_addtext_img.data, tmp_addtext_img.cols, tmp_addtext_img.rows, tmp_addtext_img.step, tmp_addtext_imgFormat);

    QPainter tmp_addtext_painter(&tmp_addtext_tmpImg);
    tmp_addtext_painter.setPen(tmp_addtext_pen);
    tmp_addtext_painter.setFont(tmp_addtext_font);
    tmp_addtext_painter.drawText(tmp_addtext_org, tmp_addtext_text);
    return 0;
}


void addtext_handle_mouse(int event, int x, int y, int, void*) {
    if (event == EVENT_LBUTTONDOWN) {
        addtext_point = Point(x, y);
        bool addtext_thickness_ok;
        bool addtext_text_ok;
        QString addtext_string=QInputDialog::getText(nullptr,"Input Text","Please Input Your Text:",QLineEdit::Normal,"Text",&addtext_text_ok);
        int addtext_thickness = QInputDialog::getInt(nullptr, "Input Text Size", "Please Set Your Text Size:", 32, 32, 256, 1, &addtext_thickness_ok);

        if(addtext_thickness_ok && addtext_text_ok)
        {
            QPen addtext_pen;
            addtext_pen.setColor(QColor(addtext_color[0],addtext_color[1],addtext_color[2]));
            QFont addtext_font;
            addtext_font.setPixelSize(addtext_thickness);
            QPoint addtext_qpoint=QPoint(addtext_point.x,addtext_point.y);

            addtext_history.push(addtext_img.clone());
            myPutText(addtext_img,addtext_string,addtext_qpoint, addtext_font,addtext_pen);

        }


    }
}




// 键盘事件处理函数
void addtext_handle_key(int key)
{
    switch (key) {
    case 'r':
        // 创建窗口 调色板
        namedWindow("AddText-ColorPalette(Press ESC To Cancel)", WINDOW_NORMAL);
        // 设置鼠标回调函数
        setMouseCallback("AddText-ColorPalette(Press ESC To Cancel)", addtext_colorpalette_handle_mouse);

        while (true)
        {
            imshow("AddText-ColorPalette(Press ESC To Cancel)", bgr_addtext_colorPalette);
            int addtext_key = waitKey(1);
            if (addtext_key == 27 || addtext_colorpalette_flag==true)
            { // 按下ESC键退出 选择完毕退出
                break;
            }
        }

        addtext_colorpalette_flag=false;
        destroyWindow("AddText-ColorPalette(Press ESC To Cancel)");
        break;
    case 'w':
        addtext_color=Scalar(255,255,255);
        break;

    case 'c':
        addtext_img = addtext_img_origin.clone(); // 清空画布
        while (!addtext_history.empty()) {
            addtext_history.pop(); // 清空历史记录
        }
        break;
    case 'z':
        if (!addtext_history.empty()) {
            addtext_img = addtext_history.top().clone(); // 恢复上一状态
            addtext_history.pop();
        }
        break;
    case 's':

        QMessageBox::StandardButton addtext_reply;
        addtext_reply = QMessageBox::question(nullptr, "Are You Sure?", "Click Yes if you want to save this way.",
                                     QMessageBox::Yes | QMessageBox::No);

       if (addtext_reply == QMessageBox::Yes)
       {
         addtext_saving_flag=true;
       }
        break;
    case 'h':
//        reply = QMessageBox::question(nullptr, "Are You Sure?", "Click Yes if you want to draw this way.",
//                                     QMessageBox::Yes | QMessageBox::No);
        QMessageBox::StandardButton addtext_helper;
        addtext_helper=QMessageBox::information(nullptr,"AddText Helper",
                                                 "Use Your Keyboard:\n"
                                                 "R:Change The Color of Your Text\n"
                                                 "W:Use White Color\n"
                                                 "C:Clean Your Canvas\n"
                                                 "Z:Revoke The Previous Step\n"
                                                 "ESC:Quit AddText Without Saving\n"
                                                 "S:Quit AddText With Saving"
                                                 );
        break;
    }
}

void SubWindow1::on_AddText_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }


     former_Matimg=display_Matimg.clone();
     revoke_ps_img_history.push(former_Matimg);

     addtext_img_origin=display_Matimg.clone();
     addtext_img=display_Matimg.clone();


     for (int i = 0; i < 512; ++i) {
          for (int j = 0; j < 512; ++j)
          {

              hsv_addtext_colorPalette.at<cv::Vec3b>(i, j) = cv::Vec3b(i*180/addtext_colorPalette_length, 200, j/(addtext_colorPalette_length/256));
          }
      }

      cvtColor(hsv_addtext_colorPalette,bgr_addtext_colorPalette,COLOR_HSV2BGR);//初始化调色盘


      addtext_saving_flag=false;



     for (int i = 0; i < 512; ++i) {
          for (int j = 0; j < 512; ++j)
          {

              hsv_addtext_colorPalette.at<cv::Vec3b>(i, j) = cv::Vec3b(i*180/addtext_colorPalette_length, 200, j/(addtext_colorPalette_length/256));
          }
      }

      cvtColor(hsv_addtext_colorPalette,bgr_addtext_colorPalette,COLOR_HSV2BGR);//初始化调色盘


     // 创建窗口
     namedWindow("AddText(Press H To Get Help)", WINDOW_AUTOSIZE);

     // 设置鼠标回调函数
     setMouseCallback("AddText(Press H To Get Help)", addtext_handle_mouse);

     while (true) {
         imshow("AddText(Press H To Get Help)", addtext_img);
         int addtext_key = waitKey(1);
         if (addtext_key == 27 || addtext_saving_flag==true) { // 按下ESC键退出

             if(addtext_saving_flag==true)
             {
                 display_Matimg=addtext_img.clone();
                 specialeffect_flag=false;//绘画后重置

                 Mat tmp_img;
                 cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
                 display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

                 if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
                 {
                     ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
                     std::cout<<"type1"<<std::endl;
                     ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
                 }
                 else
                 {
                     std::cout<<"type2"<<std::endl;
                     ui->picture_display->resize(display_Qimg.size());
                     ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
                     std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
                 }
             }
             break;
         }
         else if (addtext_key != -1) {
             addtext_handle_key(addtext_key); // 处理键盘事件
         }
     }

    destroyWindow("AddText(Press H To Get Help)");
}



bool stick_first_point_flag=false;
Point stick_first_point; // 第一个点
bool stick_second_point_flag=false;
Point stick_second_point; // 第二个点

bool stick_agree_flag=false;

Scalar stick_color = Scalar(0, 0, 255); // 标记颜色

Mat stick_img;
Mat stick_img_origin;



// 鼠标回调函数
void stick_handle_mouse(int event, int x, int y, int, void*) {
    if (event == EVENT_LBUTTONDOWN && stick_first_point_flag==false) {
        stick_first_point=Point(x,y);
        circle(stick_img,stick_first_point, 6, Scalar(0, 0, 255), FILLED);//画圈标记
        stick_first_point_flag=true;

    }
    else if (event == EVENT_LBUTTONDOWN && stick_first_point_flag==true) {
        stick_second_point=Point(x,y);
        circle(stick_img,stick_second_point, 6, Scalar(0, 0, 255), FILLED);//画圈标记
        stick_second_point_flag=true;

    }
    else if (event == EVENT_RBUTTONDOWN) {
        stick_first_point_flag=false;
        stick_second_point_flag=false;
        stick_img=stick_img_origin.clone();
    }
}


void SubWindow1::on_Stickpicture_button_clicked()
{
    if(display_Matimg.empty())
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }


     former_Matimg=display_Matimg.clone();
     revoke_ps_img_history.push(former_Matimg);


      QString input_stickimg_path = QFileDialog::getOpenFileName(this, "贴图", "", "Images (*.png *.jpg *.jpeg)");
      if (input_stickimg_path.isNull())
      {
          std::cerr << "Error:No Input" << std::endl;
          return;
      }
      cv::Mat load_stickimage = cv::imread(input_stickimg_path.toStdString());

      if(load_stickimage.cols%2==1)//图像预处理
      {
          cv::Size target_size(load_stickimage.cols+1,load_stickimage.rows);
          cv::Mat resized_image;
          cv::resize(load_stickimage,resized_image,target_size);
          load_stickimage=resized_image.clone();
      }


      stick_agree_flag=false;
      stick_img = display_Matimg.clone();
      stick_img_origin = display_Matimg.clone();

      stick_first_point_flag=false;
      stick_second_point_flag=false;

      // 创建窗口
      namedWindow("Stick(Press ESC to Cancel)", WINDOW_FREERATIO);

      // 设置鼠标回调函数
      setMouseCallback("Stick(Press ESC to Cancel)", stick_handle_mouse);


      while (true) {
          imshow("Stick(Press ESC to Cancel)", stick_img);

          int key = waitKey(1);
          if (key == 27) { // 按下ESC键退出
              break;
          }

          if(stick_agree_flag)//确认贴图
          {
              std::this_thread::sleep_for(std::chrono::seconds(1));

              int stick_x1=stick_first_point.x;
              int stick_y1=stick_first_point.y;
              int stick_x2=stick_second_point.x;
              int stick_y2=stick_second_point.y;

              int stick_height=abs(stick_y1-stick_y2);
              int stick_width=abs(stick_x1-stick_x2);

              int stick_x_final=min(stick_x1,stick_x2);
              int stick_y_final=min(stick_y1,stick_y2);

              Mat resized_stick_picture;
              //cv::resize(load_image,resized_image,target_size);
              cv::resize(load_stickimage,resized_stick_picture,Size(stick_width,stick_height));

              Rect stick_roi(stick_x_final,stick_y_final,resized_stick_picture.cols,resized_stick_picture.rows);
              Mat final_stick_img;

              final_stick_img=stick_img_origin.clone();
              resized_stick_picture.copyTo(final_stick_img(stick_roi));


               display_Matimg=final_stick_img.clone();

               Mat tmp_img;
               cvtColor(display_Matimg,tmp_img,COLOR_BGR2RGB);
               display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

               if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
               {
                   ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
                   std::cout<<"type1"<<std::endl;
                   ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
               }
               else
               {
                   std::cout<<"type2"<<std::endl;
                   ui->picture_display->resize(display_Qimg.size());
                   ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
                   std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
               }
               break;
          }

          if(stick_first_point_flag && stick_second_point_flag)
          {


              int stick_x1=stick_first_point.x;
              int stick_y1=stick_first_point.y;
              int stick_x2=stick_second_point.x;
              int stick_y2=stick_second_point.y;

              int stick_height=abs(stick_y1-stick_y2);
              int stick_width=abs(stick_x1-stick_x2);

              int stick_x_final=min(stick_x1,stick_x2);
              int stick_y_final=min(stick_y1,stick_y2);

              Mat resized_stick_picture;
              //cv::resize(load_image,resized_image,target_size);

              cv::Size stick_target_size(stick_width,stick_height);
              cv::resize(load_stickimage,resized_stick_picture,stick_target_size);

              Rect stick_roi(stick_x_final,stick_y_final,resized_stick_picture.cols,resized_stick_picture.rows);

              Mat final_stick_img;

              final_stick_img=stick_img.clone();
              resized_stick_picture.copyTo(final_stick_img(stick_roi));


              stick_img=final_stick_img.clone();

              imshow("Stick(Press ESC to Cancel)", stick_img);

              QMessageBox::StandardButton reply;
              reply = QMessageBox::question(nullptr, "Are You Sure?", "Click Yes if you want to stick this way.",
                                           QMessageBox::Yes | QMessageBox::No);



             if (reply == QMessageBox::Yes)
             {
                 stick_agree_flag=true;
             }
             else
             {
                 stick_first_point_flag=false;
                 stick_second_point_flag=false;
                 stick_img=stick_img_origin.clone();
             }

          }
      }


      destroyWindow("Stick(Press ESC to Cancel)");




}



void SubWindow1::on_ExitToMain_button_clicked()
{
    this->hide();
    parentWidget()->show();
}

void SubWindow1::on_Input_button_clicked()
{
    specialeffect_flag=false;
    crop_first_point_flag=false;
    crop_second_point_flag=false;
    crop_agree_flag=false;
    freedraw_drawing=false;
    freedraw_saving_flag=false;
    freedraw_erasing=false;
    freedraw_colorpalette_flag=false;
    addtext_saving_flag=false;
    addtext_colorpalette_flag=false;
    while(!revoke_ps_img_history.empty())
    {
        revoke_ps_img_history.pop();
    }

    while(!addtext_history.empty())
    {
        addtext_history.pop();
    }
    while(!freedraw_history.empty())
    {
        freedraw_history.pop();
    }



    QString imagePath = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg)");
    if (imagePath.isNull())
    {
        std::cerr << "Error:No Input" << std::endl;
        return;
    }
    cv::Mat load_image = cv::imread(imagePath.toStdString());
//    cv::imshow("load_image",load_image);
//    waitKey(0);

    if(load_image.cols%2==1)
    {
        cv::Size target_size(load_image.cols+1,load_image.rows);
        cv::Mat resized_image;
        cv::resize(load_image,resized_image,target_size);
        load_image=resized_image.clone();
    }


    std::cout<<load_image.cols<<" "<<load_image.rows<<std::endl;

    Mat tmp_img;
    origin_img=load_image.clone();
    display_Matimg=load_image.clone();

    cvtColor(origin_img,tmp_img,COLOR_BGR2RGB);



    display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }
}

void SubWindow1::on_actionInput_Picture_triggered()
{
    specialeffect_flag=false;
    crop_first_point_flag=false;
    crop_second_point_flag=false;
    crop_agree_flag=false;
    freedraw_drawing=false;
    freedraw_saving_flag=false;
    freedraw_erasing=false;
    freedraw_colorpalette_flag=false;
    addtext_saving_flag=false;
    addtext_colorpalette_flag=false;
    while(!revoke_ps_img_history.empty())
    {
        revoke_ps_img_history.pop();
    }

    while(!addtext_history.empty())
    {
        addtext_history.pop();
    }
    while(!freedraw_history.empty())
    {
        freedraw_history.pop();
    }


    //全局变量初始化
    QString imagePath = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg)");
    if (imagePath.isNull())
    {
        std::cerr << "Error:No Input" << std::endl;
        return;
    }
    cv::Mat load_image = cv::imread(imagePath.toStdString());
//    cv::imshow("load_image",load_image);
//    waitKey(0);

    if(load_image.cols%2==1)
    {
        cv::Size target_size(load_image.cols+1,load_image.rows);
        cv::Mat resized_image;
        cv::resize(load_image,resized_image,target_size);
        load_image=resized_image.clone();
    }


    std::cout<<load_image.cols<<" "<<load_image.rows<<std::endl;

    Mat tmp_img;
    origin_img=load_image.clone();
    display_Matimg=load_image.clone();

    cvtColor(origin_img,tmp_img,COLOR_BGR2RGB);



    display_Qimg=QImage((const unsigned char*)(tmp_img.data),tmp_img.cols,tmp_img.rows,tmp_img.step,QImage::Format_RGB888);

    if(tmp_img.cols>=origin_display_picture_width || tmp_img.rows>=origin_display_picture_height)
    {
        ui->picture_display->resize(QSize(origin_display_picture_width,origin_display_picture_height));
        std::cout<<"type1"<<std::endl;
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg.scaled(ui->picture_display->size())));
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->picture_display->resize(display_Qimg.size());
        ui->picture_display->setPixmap(QPixmap::fromImage(display_Qimg));
        std::cout<<ui->picture_display->width()<<" "<<ui->picture_display->height()<<std::endl;
    }
}


