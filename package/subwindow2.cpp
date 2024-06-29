#include "subwindow2.h"
#include "ui_subwindow2.h"

#include <opencv2/opencv.hpp>
#include "subwindow2_picture_process.h"
#include<stack>

SubWindow2::SubWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SubWindow2)
{
    ui->setupUi(this);
}

SubWindow2::~SubWindow2()
{
    delete ui;
}

bool subwindow2_input_img_flag=false;

int subwindow2_origin_display_picture_width=900;
int subwindow2_origin_display_picture_height=1200;

int subwindow2_target_pictrue_width=900;
int subwindow2_target_pictrue_height=1200;


QImage subwindow2_display_Qimg;
cv::Mat subwindow2_under_picture;//底图
QImage subwindow2_origin_Qimg;
std::stack<QImage> subwindow2_revoke_ps_img_history;



QImage subwindow2_former_display_Qimg;


bool subwindow2_flag1 = false;
bool subwindow2_flag2 = false;
QPoint subwindow2_firstClickPos; // 第一次点击的坐标
QPoint subwindow2_secondClickPos; // 第二次点击的坐标



Mat longPicture(const vector<string>& imageFilenames) {
    int n = imageFilenames.size(); // 图片数量
    // 加载所有图片并计算最大宽度和总高度
    vector<Mat> images;
    int maxWidth = 0;
    int maxHeight = 0;
    for (const auto& filename : imageFilenames) {
        cv::Mat img = cv::imread(filename);
        if (img.empty()) {
            cout << "无法加载图片: " << filename << std::endl;
            return Mat();
        }

        if(img.cols%2==1)
        {
            cv::Size target_size(img.cols+1,img.rows);
            cv::Mat resized_image;
            cv::resize(img,resized_image,target_size);
            img=resized_image.clone();
        }

        images.push_back(img);
        maxWidth = std::max(maxWidth, img.cols);
        maxHeight = std::max(maxHeight, img.rows);
        //totalHeight += img.rows;
    }
    cv::Mat result(maxHeight*n, maxWidth, CV_8UC3,cv::Scalar(0, 0, 0));
    // 将每张图片复制到新Mat对象的相应位置
    int offsetY = 0;
    for (const auto& img : images) {
         cv::Mat temp_picture;
         int tmp_to_width=maxWidth;
         int tmp_to_height=(maxWidth/img.cols)*img.rows;
         cv::resize(img, temp_picture, cv::Size(tmp_to_width,tmp_to_height), 0, 0, cv::INTER_LINEAR);
        temp_picture.copyTo(result(Rect(0, offsetY,tmp_to_width,tmp_to_height)));
        offsetY += maxHeight;
    }
    return result; // 返回拼接后的图片
}

cv::Mat stitchImages(const std::vector<std::string>& imagePaths, const std::vector<std::array<int, 4>>& positions) {
    // 确保图片数量和位置数量匹配
    if (imagePaths.size() != positions.size()) {
        throw std::runtime_error("Number of images and positions must match.");
    }
    // 初始化目标图片
    cv::Mat result(subwindow2_target_pictrue_height, subwindow2_target_pictrue_width, CV_8UC3, cv::Scalar(0, 0, 0));

    // 遍历每张图片
    for (size_t i = 0; i < imagePaths.size(); ++i) {
        // 读取图片
        cv::Mat image = cv::imread(imagePaths[i]);
        if (image.empty()) {
            throw std::runtime_error("Error loading image: " + imagePaths[i]);
        }

        // 获取目标位置和大小
        int x = positions[i][0]*(subwindow2_target_pictrue_width/300);
        int y = positions[i][1]*(subwindow2_target_pictrue_height/400);
        int targetWidth = positions[i][2]*(subwindow2_target_pictrue_width/300);
        int targetHeight = positions[i][3]*(subwindow2_target_pictrue_height/400);

        // 确保位置不会越界
        if (x + targetWidth > result.cols || y + targetHeight > result.rows) {
            throw std::runtime_error("Position and size of an image exceed the target image size.");
        }

        // 调整图片大小
        cv::Mat resizedImage;
        cv::resize(image, resizedImage, cv::Size(targetWidth, targetHeight), 0, 0, cv::INTER_LINEAR);
        // 将图片放置到结果图片上
        resizedImage.copyTo(result(cv::Rect(x, y, targetWidth, targetHeight)));
    }
    return result;
}


void SubWindow2::on_long_picture_button_clicked()
{
    if(!subwindow2_input_img_flag)
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }

    subwindow2_former_display_Qimg=subwindow2_display_Qimg.copy();
    subwindow2_revoke_ps_img_history.push(subwindow2_display_Qimg.copy());

    result =longPicture(imageFilenames);
//    cv::imshow("Result", result);
//    cv::waitKey(0);

    /*cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    display_Qimg=QImage((const unsigned char*)(result.data),result.cols,result.rows,QImage::Format_RGB888);*/

    subwindow2_under_picture = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    subwindow2_display_Qimg=QImage((const unsigned char*)(result.data),result.cols,result.rows,QImage::Format_RGB888);
    subwindow2_origin_Qimg=subwindow2_display_Qimg.copy();

    if(subwindow2_display_Qimg.width()>=subwindow2_origin_display_picture_width || subwindow2_display_Qimg.height()>=subwindow2_origin_display_picture_height)
    {
        float change_ratio1=(float)subwindow2_display_Qimg.width()/(float)subwindow2_origin_display_picture_width;
        float change_ratio2=(float)subwindow2_display_Qimg.height()/(float)subwindow2_origin_display_picture_height;
        if(change_ratio1>change_ratio2)//宽变化比例更大
        {
            int tmp_to_show_width=subwindow2_target_pictrue_width;
            int tmp_to_show_height=subwindow2_display_Qimg.height()/change_ratio1;
            if(tmp_to_show_width%2==1)tmp_to_show_width--;
            ui->label->resize(QSize(tmp_to_show_width,tmp_to_show_height));
            std::cout<<"type11"<<std::endl;
            ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));
        }
        else
        {
            int tmp_to_show_width=subwindow2_display_Qimg.width()/change_ratio2;
            int tmp_to_show_height=subwindow2_target_pictrue_height;
            if(tmp_to_show_width%2==1)tmp_to_show_width--;
            ui->label->resize(QSize(tmp_to_show_width,tmp_to_show_height));
            std::cout<<"type12"<<std::endl;
            ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));
        }
    }
    else
    {
        std::cout<<"type2"<<std::endl;
        ui->label->resize(subwindow2_display_Qimg.size());
        ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg));
        std::cout<<ui->label->width()<<" "<<ui->label->height()<<std::endl;
    }
}

void SubWindow2::on_picture_combine_modle1_clicked()
{
    if(!subwindow2_input_img_flag)
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }


    subwindow2_former_display_Qimg=subwindow2_display_Qimg.copy();
    subwindow2_revoke_ps_img_history.push(subwindow2_display_Qimg.copy());

    int pic_num = imageFilenames.size();
    int n =1;
    if (pic_num == 1){
        result = cv::imread(imageFilenames[0]);
        cv::resize(result, result, cv::Size(subwindow2_target_pictrue_width,subwindow2_target_pictrue_height), 0, 0, cv::INTER_LINEAR);
    }
    if (pic_num == 2){
        if (n == 1) { result = stitchImages(imageFilenames, modle21); }
        else { result = stitchImages(imageFilenames, modle22); }
        }
    if (pic_num == 3){
        if (n == 1) { result = stitchImages(imageFilenames, modle31); }
        else { result = stitchImages(imageFilenames, modle32); }
        }
    if (pic_num == 4) {
        if (n == 1) { result = stitchImages(imageFilenames, modle41); }
        else { result = stitchImages(imageFilenames, modle42); }
    }
    if (pic_num == 5) {
        if (n == 1) { result = stitchImages(imageFilenames, modle51); }
        else { result = stitchImages(imageFilenames, modle52); }
    }
    if (pic_num == 6) {
        if (n == 1) { result = stitchImages(imageFilenames, modle61); }
        else { result = stitchImages(imageFilenames, modle62); }
    }
    if (pic_num == 7) {
        if (n == 1) { result = stitchImages(imageFilenames, modle71); }
        else { result = stitchImages(imageFilenames, modle72); }
    }
    if (pic_num == 8) {
        if (n == 1) { result = stitchImages(imageFilenames, modle81); }
        else { result = stitchImages(imageFilenames, modle82); }
    }
    if (pic_num == 9) {
        if (n == 1) { result = stitchImages(imageFilenames, modle91); }
        else { result = stitchImages(imageFilenames, modle92); }
    }
    subwindow2_under_picture = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    subwindow2_display_Qimg=QImage((const unsigned char*)(result.data),result.cols,result.rows,QImage::Format_RGB888);
    subwindow2_origin_Qimg=subwindow2_display_Qimg.copy();

    ui->label->resize(QSize(subwindow2_target_pictrue_width,subwindow2_target_pictrue_height));
    ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));
}

void SubWindow2::on_picture_combine_modle_2_clicked()
{
    if(!subwindow2_input_img_flag)
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }


    subwindow2_former_display_Qimg=subwindow2_display_Qimg.copy();
    subwindow2_revoke_ps_img_history.push(subwindow2_display_Qimg.copy());

    int pic_num = imageFilenames.size();
    int n =2;
    if (pic_num == 1){
        result = cv::imread(imageFilenames[0]);
        cv::resize(result, result, cv::Size(subwindow2_target_pictrue_width, subwindow2_target_pictrue_height), 0, 0, cv::INTER_LINEAR);

    }
    if (pic_num == 2){
        if (n == 1) { result = stitchImages(imageFilenames, modle21); }
        else { result = stitchImages(imageFilenames, modle22); }
        }
    if (pic_num == 3){
        if (n == 1) { result = stitchImages(imageFilenames, modle31); }
        else { result = stitchImages(imageFilenames, modle32); }
        }
    if (pic_num == 4) {
        if (n == 1) { result = stitchImages(imageFilenames, modle41); }
        else { result = stitchImages(imageFilenames, modle42); }
    }
    if (pic_num == 5) {
        if (n == 1) { result = stitchImages(imageFilenames, modle51); }
        else { result = stitchImages(imageFilenames, modle52); }
    }
    if (pic_num == 6) {
        if (n == 1) { result = stitchImages(imageFilenames, modle61); }
        else { result = stitchImages(imageFilenames, modle62); }
    }
    if (pic_num == 7) {
        if (n == 1) { result = stitchImages(imageFilenames, modle71); }
        else { result = stitchImages(imageFilenames, modle72); }
    }
    if (pic_num == 8) {
        if (n == 1) { result = stitchImages(imageFilenames, modle81); }
        else { result = stitchImages(imageFilenames, modle82); }
    }
    if (pic_num == 9) {
        if (n == 1) { result = stitchImages(imageFilenames, modle91); }
        else { result = stitchImages(imageFilenames, modle92); }
    }
    subwindow2_under_picture = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    subwindow2_display_Qimg=QImage((const unsigned char*)(result.data),result.cols,result.rows,QImage::Format_RGB888);
    subwindow2_origin_Qimg=subwindow2_display_Qimg.copy();

    ui->label->resize(QSize(subwindow2_target_pictrue_width,subwindow2_target_pictrue_height));
    ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));
}

void SubWindow2::on_output_button_clicked()
{
    if(!subwindow2_input_img_flag)
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }


    QString filePath = QFileDialog::getSaveFileName(this, "选择保存位置", "", "Image Files (*.png *.xpm *.jpg)");
    if (filePath.isEmpty()) {
       std::cout<<"No Input!"<<std::endl;
       return;
    }
    if (!subwindow2_display_Qimg.save(filePath)) {
       QMessageBox::critical(this, "错误", "无法保存图片到指定位置！");
    }
}





bool subwindow2_stick_flag=false;
bool subwindow2_havestick_flag=false;

cv::Mat up_image;//贴纸
cv::Mat under_image;//截取对应的底图
cv::Mat whiteCanvas;//白色蒙版
cv::Mat orient_up_image;
int num = 0;
int num1 = 0;

bool tmpstick_process=false;
bool tmpstick_already_colorchange=false;

void SubWindow2::mousePressEvent(QMouseEvent *event)
{
    if(tmpstick_process==true)return;
    if (event->button() == Qt::LeftButton && subwindow2_stick_flag==true) // 检查是否是左键点击
    {
        tmpstick_process=true;

        if (!subwindow2_flag1) // 如果没有第一次点击
        {
            subwindow2_flag1 = true; // 标记为已点击
            subwindow2_firstClickPos = event->pos();
            qDebug() << subwindow2_firstClickPos;
        }
        else if (subwindow2_flag1 && !subwindow2_flag2) // 如果有第一次点击但没有第二次点击
        {


            subwindow2_flag2 = true; // 标记为已点击
            subwindow2_secondClickPos = event->pos(); // 记录坐标
            qDebug()<< subwindow2_secondClickPos;


//            orient_up_image = up_image.clone();

//            int target_width = abs(subwindow2_secondClickPos.x()-subwindow2_firstClickPos.x())*(subwindow2_target_pictrue_width/subwindow2_origin_display_picture_width);
//            int target_height = abs(subwindow2_secondClickPos.y()-subwindow2_firstClickPos.y())*(subwindow2_target_pictrue_height/subwindow2_origin_display_picture_height);
//            if(num == 0){
//                cv::resize(orient_up_image, orient_up_image,cv::Size(target_width, target_height),0,0,cv::INTER_LINEAR);
//                cv::resize(up_image, up_image,cv::Size(target_width, target_height),0,0,cv::INTER_LINEAR);
//                whiteCanvas = cv::Mat::zeros(up_image.rows, up_image.cols, CV_8UC3);
//                whiteCanvas.setTo(cv::Scalar(255, 255, 255));
//            }

//            cv::Rect roi(subwindow2_firstClickPos.x()*(subwindow2_target_pictrue_width/subwindow2_origin_display_picture_width), subwindow2_firstClickPos.y()*(subwindow2_target_pictrue_height/subwindow2_origin_display_picture_height), target_width, target_height);
//            if(num1 == 0){
//                cv::cvtColor(subwindow2_under_picture, subwindow2_under_picture, cv::COLOR_BGR2RGB);
//                num1++;
//            }
//            if(num==0)
//            {
//                under_image = subwindow2_under_picture(roi).clone();
//                num++;
//            }
//            up_image.copyTo(subwindow2_under_picture(roi));


//            subwindow2_display_Qimg=QImage((const unsigned char*)(subwindow2_under_picture.data),subwindow2_under_picture.cols,subwindow2_under_picture.rows,QImage::Format_RGB888);


//            subwindow2_havestick_flag=true;


//            ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));

            //分割线




            cv::Mat tmpstick_up_image;//贴纸
            cv::Mat tmpstick_under_image;//截取对应的底图
            cv::Mat tmpstick_whiteCanvas;//白色蒙版
            cv::Mat tmpstick_orient_up_image;
            QImage tmpstick_subwindow2_display_Qimg;


            int tmpstick_num = 0;
            int tmpstick_num1 = 0;
            cv::Mat tmpstick_subwindow2_under_picture=subwindow2_under_picture.clone();


            tmpstick_orient_up_image = up_image.clone();
            tmpstick_up_image=up_image.clone();

            int tmpstick_target_width = abs(subwindow2_secondClickPos.x()-subwindow2_firstClickPos.x())*(subwindow2_target_pictrue_width/subwindow2_origin_display_picture_width);
            int tmpstick_target_height = abs(subwindow2_secondClickPos.y()-subwindow2_firstClickPos.y())*(subwindow2_target_pictrue_height/subwindow2_origin_display_picture_height);
            if(tmpstick_num == 0){
                cv::resize(tmpstick_orient_up_image, tmpstick_orient_up_image,cv::Size(tmpstick_target_width, tmpstick_target_height),0,0,cv::INTER_LINEAR);
                cv::resize(tmpstick_up_image, tmpstick_up_image,cv::Size(tmpstick_target_width, tmpstick_target_height),0,0,cv::INTER_LINEAR);
                tmpstick_whiteCanvas = cv::Mat::zeros(tmpstick_up_image.rows, tmpstick_up_image.cols, CV_8UC3);
                tmpstick_whiteCanvas.setTo(cv::Scalar(255, 255, 255));
            }
            cv::Rect tmpstick_roi(subwindow2_firstClickPos.x()*(subwindow2_target_pictrue_width/subwindow2_origin_display_picture_width), subwindow2_firstClickPos.y()*(subwindow2_target_pictrue_height/subwindow2_origin_display_picture_height), tmpstick_target_width, tmpstick_target_height);
            if(tmpstick_num1 == 0 && tmpstick_already_colorchange==false){
                cv::cvtColor(tmpstick_subwindow2_under_picture, tmpstick_subwindow2_under_picture, cv::COLOR_BGR2RGB);
                tmpstick_num1++;
            }
            if(tmpstick_num==0) {
                tmpstick_under_image = tmpstick_subwindow2_under_picture(tmpstick_roi).clone();
                tmpstick_num++;}
            tmpstick_up_image.copyTo(tmpstick_subwindow2_under_picture(tmpstick_roi));

            tmpstick_subwindow2_display_Qimg=QImage((const unsigned char*)(tmpstick_subwindow2_under_picture.data),tmpstick_subwindow2_under_picture.cols,tmpstick_subwindow2_under_picture.rows,QImage::Format_RGB888);

            ui->label->resize(QSize(subwindow2_target_pictrue_width,subwindow2_target_pictrue_height));
            ui->label->setPixmap(QPixmap::fromImage(tmpstick_subwindow2_display_Qimg.scaled(ui->label->size())));

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, "Are You Sure?", "Click Yes if you want to stick this way.",
                                         QMessageBox::Yes | QMessageBox::No);


           if (reply == QMessageBox::Yes)
           {
               orient_up_image = up_image.clone();

               int target_width = abs(subwindow2_secondClickPos.x()-subwindow2_firstClickPos.x())*(subwindow2_target_pictrue_width/subwindow2_origin_display_picture_width);
               int target_height = abs(subwindow2_secondClickPos.y()-subwindow2_firstClickPos.y())*(subwindow2_target_pictrue_height/subwindow2_origin_display_picture_height);
               if(num == 0){
                   cv::resize(orient_up_image, orient_up_image,cv::Size(target_width, target_height),0,0,cv::INTER_LINEAR);
                   cv::resize(up_image, up_image,cv::Size(target_width, target_height),0,0,cv::INTER_LINEAR);
                   whiteCanvas = cv::Mat::zeros(up_image.rows, up_image.cols, CV_8UC3);
                   whiteCanvas.setTo(cv::Scalar(255, 255, 255));
               }


               cv::Rect roi(subwindow2_firstClickPos.x()*(subwindow2_target_pictrue_width/subwindow2_origin_display_picture_width), subwindow2_firstClickPos.y()*(subwindow2_target_pictrue_height/subwindow2_origin_display_picture_height), target_width, target_height);
               if(num1 == 0){
                   cv::cvtColor(subwindow2_under_picture, subwindow2_under_picture, cv::COLOR_BGR2RGB);
                   num1++;
                   tmpstick_already_colorchange=true;
               }
               if(num==0) {
                   under_image = subwindow2_under_picture(roi).clone();
                   num++;}
               up_image.copyTo(subwindow2_under_picture(roi));



               subwindow2_display_Qimg=QImage((const unsigned char*)(subwindow2_under_picture.data),subwindow2_under_picture.cols,subwindow2_under_picture.rows,QImage::Format_RGB888);


               subwindow2_havestick_flag=true;


               ui->label->resize(QSize(subwindow2_target_pictrue_width,subwindow2_target_pictrue_height));
               ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));


           }
           else
           {
               subwindow2_flag1=false;
               subwindow2_flag2=false;


               subwindow2_display_Qimg=subwindow2_former_display_Qimg.copy();

               ui->label->resize(QSize(subwindow2_target_pictrue_width,subwindow2_target_pictrue_height));
               ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));

               subwindow2_stick_flag=false;
               subwindow2_havestick_flag=false;
           }
        }
    }
    else if (event->button()==Qt::RightButton && subwindow2_stick_flag==true)//右键取消选择
    {
        tmpstick_process=true;
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Are You Sure?", "Click Yes if you want to cancel click.",
                                     QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            subwindow2_stick_flag=false;

            subwindow2_flag1=false;
            subwindow2_flag2=false;

            subwindow2_display_Qimg=subwindow2_former_display_Qimg.copy();

            ui->label->resize(QSize(subwindow2_target_pictrue_width,subwindow2_target_pictrue_height));
            ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));
        }
    }

    tmpstick_process=false;

}



void SubWindow2::on_pushButton_2_clicked()
{
    if(!subwindow2_input_img_flag)
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }


    subwindow2_stick_flag=true;

    subwindow2_former_display_Qimg=subwindow2_display_Qimg.copy();
    subwindow2_revoke_ps_img_history.push(subwindow2_display_Qimg.copy());


    subwindow2_flag1 = false;
    subwindow2_flag2 = false;
    num=0;

    QString fileName = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    up_image = cv::imread(fileName.toStdString());//加载贴图
    cv::cvtColor(up_image, up_image, cv::COLOR_BGR2RGB);

}

void SubWindow2::on_pushButton_3_clicked()
{
    if(!subwindow2_input_img_flag)
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }


    if(subwindow2_havestick_flag==false)
    {
        std::cerr<<"No Correspond Stick."<<std::endl;
        return;
    }

    subwindow2_former_display_Qimg=subwindow2_display_Qimg.copy();
    subwindow2_revoke_ps_img_history.push(subwindow2_display_Qimg.copy());

    cv::Point2f center(up_image.cols / 2.0, up_image.rows / 2.0);
        double angle = 15; // 角度为15度
        double scale = 0.8;
        cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);
        cv::warpAffine(orient_up_image, orient_up_image, rot_mat, up_image.size());//旋转原图
        up_image = orient_up_image.clone();
        cv::warpAffine(whiteCanvas, whiteCanvas, rot_mat, up_image.size());//同样的旋转白布
        cv::Mat temp = cv::Mat::zeros(orient_up_image.size(), orient_up_image.type());
        for (int y = 0; y < whiteCanvas.rows; ++y) {
                for (int x = 0; x < whiteCanvas.cols; ++x) {
                    cv::Vec3b pixel = whiteCanvas.at<cv::Vec3b>(y, x);
                    if ((pixel[0] == 0) && (pixel[1] == 0) && (pixel[2] == 0)) {

                        temp.at<cv::Vec3b>(y, x) = under_image.at<cv::Vec3b>(y, x);
                    }
                    else
                    {
                        temp.at<cv::Vec3b>(y, x) = orient_up_image.at<cv::Vec3b>(y, x);
                    }
                }
            }
        up_image=temp.clone();


        //
        if(subwindow2_flag1&&subwindow2_flag2){
        int target_width = abs(subwindow2_secondClickPos.x()-subwindow2_firstClickPos.x())*(subwindow2_target_pictrue_width/subwindow2_origin_display_picture_width);
        int target_height = abs(subwindow2_secondClickPos.y()-subwindow2_firstClickPos.y())*(subwindow2_target_pictrue_height/subwindow2_origin_display_picture_height);
        if(num == 0){
            cv::resize(orient_up_image, orient_up_image,cv::Size(target_width, target_height),0,0,cv::INTER_LINEAR);
            cv::resize(up_image, up_image,cv::Size(target_width, target_height),0,0,cv::INTER_LINEAR);
            whiteCanvas = cv::Mat::zeros(up_image.rows, up_image.cols, CV_8UC3);
            whiteCanvas.setTo(cv::Scalar(255, 255, 255));
        }
        cv::Rect roi(subwindow2_firstClickPos.x()*(subwindow2_target_pictrue_width/subwindow2_origin_display_picture_width), subwindow2_firstClickPos.y()*(subwindow2_target_pictrue_height/subwindow2_origin_display_picture_height), target_width, target_height);
        if(num1 == 0){
            cv::cvtColor(subwindow2_under_picture, subwindow2_under_picture, cv::COLOR_BGR2RGB);
            num1++;
        }
        if(num==0) {
            under_image = subwindow2_under_picture(roi).clone();
            num++;}
        up_image.copyTo(subwindow2_under_picture(roi));
        subwindow2_display_Qimg=QImage((const unsigned char*)(subwindow2_under_picture.data),subwindow2_under_picture.cols,subwindow2_under_picture.rows,QImage::Format_RGB888);

        ui->label->resize(QSize(subwindow2_target_pictrue_width,subwindow2_target_pictrue_height));
        ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));}
}

void SubWindow2::on_ExitToMain2_button_clicked()
{
    this->hide();
    parentWidget()->show();
}

void SubWindow2::on_Rotat_clockwise_button_clicked()
{
    if(!subwindow2_input_img_flag)
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }


    if(subwindow2_havestick_flag==false)
    {
        std::cerr<<"No Correspond Stick."<<std::endl;
        return;
    }

    subwindow2_former_display_Qimg=subwindow2_display_Qimg.copy();
    subwindow2_revoke_ps_img_history.push(subwindow2_display_Qimg.copy());

    cv::Point2f center(up_image.cols / 2.0, up_image.rows / 2.0);
        double angle = 345; // 角度为15度
        double scale = 0.8;
        cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);
        cv::warpAffine(orient_up_image, orient_up_image, rot_mat, up_image.size());//旋转原图
        up_image = orient_up_image.clone();
        cv::warpAffine(whiteCanvas, whiteCanvas, rot_mat, up_image.size());//同样的旋转白布
        cv::Mat temp = cv::Mat::zeros(orient_up_image.size(), orient_up_image.type());
        for (int y = 0; y < whiteCanvas.rows; ++y) {
                for (int x = 0; x < whiteCanvas.cols; ++x) {
                    cv::Vec3b pixel = whiteCanvas.at<cv::Vec3b>(y, x);
                    if ((pixel[0] == 0) && (pixel[1] == 0) && (pixel[2] == 0)) {

                        temp.at<cv::Vec3b>(y, x) = under_image.at<cv::Vec3b>(y, x);
                    }
                    else
                    {
                        temp.at<cv::Vec3b>(y, x) = orient_up_image.at<cv::Vec3b>(y, x);
                    }
                }
            }
        up_image=temp.clone();

        //
        if(subwindow2_flag1&&subwindow2_flag2){
        int target_width = abs(subwindow2_secondClickPos.x()-subwindow2_firstClickPos.x())*(subwindow2_target_pictrue_width/subwindow2_origin_display_picture_width);
        int target_height = abs(subwindow2_secondClickPos.y()-subwindow2_firstClickPos.y())*(subwindow2_target_pictrue_height/subwindow2_origin_display_picture_height);
        if(num == 0){
            cv::resize(orient_up_image, orient_up_image,cv::Size(target_width, target_height),0,0,cv::INTER_LINEAR);
            cv::resize(up_image, up_image,cv::Size(target_width, target_height),0,0,cv::INTER_LINEAR);
            whiteCanvas = cv::Mat::zeros(up_image.rows, up_image.cols, CV_8UC3);
            whiteCanvas.setTo(cv::Scalar(255, 255, 255));
        }
        cv::Rect roi(subwindow2_firstClickPos.x()*(subwindow2_target_pictrue_width/subwindow2_origin_display_picture_width), subwindow2_firstClickPos.y()*(subwindow2_target_pictrue_height/subwindow2_origin_display_picture_height), target_width, target_height);
        if(num1 == 0){
            cv::cvtColor(subwindow2_under_picture, subwindow2_under_picture, cv::COLOR_BGR2RGB);
            num1++;
        }
        if(num==0) {
            under_image = subwindow2_under_picture(roi).clone();
            num++;}
        up_image.copyTo(subwindow2_under_picture(roi));
        subwindow2_display_Qimg=QImage((const unsigned char*)(subwindow2_under_picture.data),subwindow2_under_picture.cols,subwindow2_under_picture.rows,QImage::Format_RGB888);

        ui->label->resize(QSize(subwindow2_target_pictrue_width,subwindow2_target_pictrue_height));
        ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));}
}

void SubWindow2::on_Revoke_button_clicked()
{
    if(!subwindow2_input_img_flag)
    {
        QMessageBox::StandardButton no_picture_warning;
        no_picture_warning=QMessageBox::information(nullptr,"No Image",
                                                 "Please Input A image first~"
                                                 );
        return;
    }


    if(subwindow2_revoke_ps_img_history.empty())
    {
      subwindow2_display_Qimg=subwindow2_origin_Qimg.copy();
    }
    else
    {
        subwindow2_display_Qimg=subwindow2_revoke_ps_img_history.top();
        subwindow2_revoke_ps_img_history.pop();
    }

    ui->label->resize(QSize(subwindow2_target_pictrue_width,subwindow2_target_pictrue_height));
    ui->label->setPixmap(QPixmap::fromImage(subwindow2_display_Qimg.scaled(ui->label->size())));
}


void SubWindow2::on_pushButton_clicked()
{
    subwindow2_input_img_flag=false;

    num=0;
    num1=0;
    subwindow2_flag1 = false;
    subwindow2_flag2 = false;

    tmpstick_process=false;
    tmpstick_already_colorchange=false;
    subwindow2_havestick_flag=false;
    subwindow2_stick_flag=false;
    while(!subwindow2_revoke_ps_img_history.empty())
    {
        subwindow2_revoke_ps_img_history.pop();
    }
    //全部变量在重新导入图片后初始化


    QStringList fileNames = QFileDialog::getOpenFileNames(this, "选择图片(1-9)", "", "Images (*.png *.jpg *.jpeg)");
    if (fileNames.isEmpty())
    {
        std::cerr << "Error:No Input" << std::endl;
        return;
    }
    else if (fileNames.size()>9)
    {
        std::cerr << "Error:Input Out Of Range" << std::endl;
        return;
    }
    subwindow2_input_img_flag=true;

    imageFilenames.clear();
    for (const QString &fileName : fileNames) {
        imageFilenames.push_back(fileName.toStdString());
    }
}
