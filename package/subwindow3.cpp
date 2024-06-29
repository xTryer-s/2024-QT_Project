#include "subwindow3.h"
#include "ui_subwindow3.h"
#include "subwindow3_gif_process.h"

SubWindow3::SubWindow3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SubWindow3)
{
    ui->setupUi(this);
}

SubWindow3::~SubWindow3()
{
    delete ui;
}


string ffmpeg_path = "ffmpeg\\bin\\ffmpeg.exe";//需要修改

string original_video_address;
vector<string> images_address;

int input_gif_fps = 1;
int ori_video_starttime=0;
int ori_video_finishtime=1;


string temp_imgs2gif_address="TempFiles_Images2GIF";// 临时文件夹（用于存储转化成gif的临时图片）
std::wstring wide_imgs2gif_address= s2ws(temp_imgs2gif_address);


string temp_video2gif_address="TempFiles_Video2GIF"; //临时文件路径用于存储处理后的无音频视频
string temp_video_path = temp_video2gif_address + "\\temp_video.mp4";
string temp_audio_path = temp_video2gif_address + "\\temp_audio.wav";


std::wstring wide_video2gif_address= s2ws(temp_video2gif_address);

QStringList inputimages_fileNames;
QString inputvideo_filename;

int which_gif_mode=0;





void SubWindow3::on_images2gif_button_clicked()
{

    remove_all_files_in_directory(temp_imgs2gif_address);

    which_gif_mode=1;
    inputimages_fileNames = QFileDialog::getOpenFileNames(this, "选择图片用于制作GIF动画", "", "Images (*.png *.jpg *.jpeg)");

    if (inputimages_fileNames.isEmpty())
    {
        std::cerr << "Error:No Input" << std::endl;
        return;
    }

    for(int i=0;i<inputimages_fileNames.size();i++)
    {
        images_address.push_back(inputimages_fileNames[i].toStdString());

        std::cout<<inputimages_fileNames[i].toStdString()<<std::endl;
    }



    if (CreateDirectoryW(wide_imgs2gif_address.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
        std::cout << "Directory created successfully: " << temp_imgs2gif_address<< std::endl;
    }
    else {
        std::cerr << "Failed to create directory: " << temp_imgs2gif_address << std::endl;
    }


    // 保存图片到临时目录

    save_images_to_temp_dir(temp_imgs2gif_address,images_address);


}




void SubWindow3::on_video2gif_button_clicked()
{
    remove_all_files_in_directory(temp_video2gif_address);


    which_gif_mode=2;
    inputvideo_filename=QFileDialog::getOpenFileName(this, "选择视频", "", "Video(*.mp4 *.avi)");

    if (inputvideo_filename.isEmpty())
    {
        std::cerr << "Error:No Input" << std::endl;
        return;
    }

    original_video_address=inputvideo_filename.toStdString();
}




void SubWindow3::on_ExitToMain_button_clicked()
{
    this->hide();
    parentWidget()->show();

    which_gif_mode=0;
}

void SubWindow3::on_OutputGIF_button_clicked()
{
    if(which_gif_mode==0)return;



    if(which_gif_mode==2)
    {
//        bool input_starttime_ok;
//        ori_video_starttime = QInputDialog::getInt(nullptr, "Input StartTime", "Please Set Your StartTime:", 0, 0, 200, 1, &input_starttime_ok);

//        bool input_finishtime_ok;
//        ori_video_finishtime = QInputDialog::getInt(nullptr, "Input FinishTime", "Please Set Your StartTime:", 0, 0, 200, 1, &input_finishtime_ok);

//        if(!(input_starttime_ok && input_finishtime_ok))
//        {
//            std::cerr<<"input time error~"<<std::endl;
//            which_gif_mode=0;
//            return;
//        }

        if (CreateDirectoryW(wide_video2gif_address.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
            std::cout << "Directory created successfully: " << temp_video2gif_address<< std::endl;
        }
        else {
            std::cerr << "Failed to create directory: " << temp_video2gif_address << std::endl;
        }


        VideoCapture cap(original_video_address);
        if (!cap.isOpened())
        {
            cerr << "Error: Could not open the video!" << endl;
            return;
        }
        double fps = cap.get(CAP_PROP_FPS);

        int wait_time = static_cast<int>(1000 / fps);

        int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
        int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
        int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v'); // 使用 mp4v 编码器


        cerr<<temp_video_path<<endl;

        VideoWriter output_video(temp_video_path, fourcc, fps, Size(frame_width, frame_height));

        if (!output_video.isOpened())
        {
            cerr << "Error: Could not open the output video file for write!" << endl;
            return;
        }

        int start_flag=0;
        int finish_flag=0;
        int pause_flag=0;

        QMessageBox::StandardButton ChooseDuration_helper;
        ChooseDuration_helper=QMessageBox::information(nullptr,"Get A Video Part",
                                                 "Use Your Keyboard:\n"
                                                 "S:Set Start Time\n"
                                                 "F:Set Finish Time\n"
                                                 );

        while (true)
        {
            Mat frame;
            bool isSuccess = cap.read(frame);
            if (!isSuccess)
            {
                cout << "Video processing completed" << endl;
                break;
            }
            //显示视频
            imshow("Show Video", frame);
            char key = (char)waitKey(pause_flag==1 ? 0 : wait_time);

            if (key == 27)
            { // ESC退出
                cout << "ESC" << endl;
                break;
            }
            else if (key == 's')
            {
                ori_video_starttime = cap.get(CAP_PROP_POS_MSEC) / 1000.0;
                cout << "Start time recorded: " << ori_video_starttime << " seconds" << endl;
                start_flag=1;
             }
            else if (key == 'f' && start_flag==1)
            {
                ori_video_finishtime = cap.get(CAP_PROP_POS_MSEC) / 1000.0;
                cout << "End time recorded: " << ori_video_starttime << " seconds" << endl;
                finish_flag=1;
             }
            else if(key=='p')
            {
                pause_flag=1-pause_flag;
            }

            if(start_flag==1 && finish_flag==1)
            {
                break;
            }
        }


        // 释放视频捕获和写入对象
        cap.release();
        output_video.release();
        destroyAllWindows();

        if(!(start_flag==1 && finish_flag==1))
        {
            QMessageBox::StandardButton no_duration_warning;
            no_duration_warning=QMessageBox::information(nullptr,"Unavailable Time",
                                                     "Please Choose Start and End Time"
                                                     );
            return;

        }
    }

    bool input_fps_ok;
    input_gif_fps = QInputDialog::getInt(nullptr, "Input FPS", "Please Set Your GIF FPS:", 1, 1, 2000, 5, &input_fps_ok);

    if(input_fps_ok)
    {
        QString outputFilePath = QFileDialog::getSaveFileName(this, "保存GIF","","GIF(*.gif)");
            if (outputFilePath.isEmpty())
            {
                std::cerr<<"Your OutPutFilePath is Error~"<<std::endl;
            }

        if(which_gif_mode==1)
        {
            // 将图片转换为 GIF
            convert_images_to_gif(ffmpeg_path,temp_imgs2gif_address,outputFilePath.toStdString(),input_gif_fps);

            remove_all_files_in_directory(temp_imgs2gif_address);
        }
        else if(which_gif_mode==2)
        {
            create_video_segment(ffmpeg_path,original_video_address, temp_video_path, ori_video_starttime, ori_video_finishtime, input_gif_fps);

            convert_video_to_gif(ffmpeg_path,temp_video_path, outputFilePath.toStdString(), input_gif_fps);

            remove_all_files_in_directory(temp_video2gif_address);
        }
    }
    else
    {
        std::cerr<<"Error Input FPS~"<<std::endl;
    }




    which_gif_mode=0;

}
