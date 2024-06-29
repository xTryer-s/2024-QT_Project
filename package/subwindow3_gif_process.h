#ifndef SUBWINDOW3_GIF_PROCESS_H
#define SUBWINDOW3_GIF_PROCESS_H


#include <opencv2/opencv.hpp>
#include <iostream>
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



using namespace cv;
using namespace std;


std::wstring s2ws(const std::string& s) {//将string转成widestring，并且处理中文路径的问题
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    std::wstring r(len, L'\0');
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
    return r;
}

void remove_all_files_in_directory(const std::string& folder_path) {
    std::string search_path = folder_path + "\\*.*";
    std::wstring wsearch_path = s2ws(search_path);
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(wsearch_path.c_str(), &fd);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error finding first file: " << GetLastError() << std::endl;
        return;
    }

    do {
        std::string file_path = folder_path + "\\" + std::string(fd.cFileName, fd.cFileName + wcslen(fd.cFileName));
        std::wstring wfile_path = s2ws(file_path);

        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            // Remove the read-only attribute if set
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
                SetFileAttributes(wfile_path.c_str(), fd.dwFileAttributes & ~FILE_ATTRIBUTE_READONLY);
            }
            if (!::DeleteFile(wfile_path.c_str())) {
                std::cerr << "Error deleting file: " << file_path << " Error: " << GetLastError() << std::endl;
            } else {
                std::cout << "Successfully deleted file: " << file_path << std::endl;
            }
        }
    } while (::FindNextFile(hFind, &fd));

    ::FindClose(hFind);
}

void create_video_segment(const string& ffmpeg_path_,const string& input_video_path, const string& output_video_path, int start_time, int finish_time, int fps) {

    string command = ffmpeg_path_ + " -ss " + to_string(start_time) + " -t " + to_string(finish_time-start_time) + " -i " + input_video_path + " -c:v libx264 -r " + to_string(fps) + " -y " + output_video_path;
    system(command.c_str());
}

void convert_video_to_gif(const string& ffmpeg_path_,const string& temp_video_path, const string& gif_path, int fps) {


    string command = ffmpeg_path_ + " -i " + temp_video_path + " -vf \"fps=" + to_string(fps) + "\" -y " + gif_path;
    system(command.c_str());
}

void save_images_to_temp_dir(const string& temp_dir_path,const vector<string>& image_paths) {
    for (size_t i = 0; i < image_paths.size(); ++i) {
        Mat img = imread(image_paths[i]);
        if (img.empty()) {
            cerr << "Could not open or find the image: " << image_paths[i] << endl;
            return;
        }
        string output_path = temp_dir_path + "\\frame_" + to_string(i) + ".bmp";
        imwrite(output_path, img);
    }
}

void convert_images_to_gif(const string& ffmpeg_path_,const string& temp_dir_path,const string& gif_path, int fps) {

    string command = ffmpeg_path_ + " -framerate " + to_string(fps) + " -i " + temp_dir_path + "\\frame_%d.bmp -y " + gif_path;
    system(command.c_str());
}


#endif // SUBWINDOW3_GIF_PROCESS_H
