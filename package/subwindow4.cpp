#include "subwindow4.h"
#include "ui_subwindow4.h"

SubWindow4::SubWindow4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SubWindow4)
{
    ui->setupUi(this);
}

SubWindow4::~SubWindow4()
{
    delete ui;
}

void SubWindow4::on_ExitToMain_button_clicked()
{
    this->hide();
    parentWidget()->show();

}


#define max2(a,b) (a>b?a:b)
#define max3(a,b,c) (a>b?max2(a,c):max2(b,c))
#define min2(a,b) (a<b?a:b)
#define min3(a,b,c) (a<b?min2(a,c):min2(b,c))

cv::Mat video_Saturation(cv::Mat specialeffet_src, int percent)
{
    float Increment = percent* 1.0f / 100;
    cv::Mat specialeffet_temp = specialeffet_src.clone();
    int specialeffet_srcrow = specialeffet_src.rows;
    int specialeffet_srccol = specialeffet_src.cols;
    for (int i = 0; i < specialeffet_srcrow; ++i)
    {
        uchar *t = specialeffet_temp.ptr<uchar>(i);
        uchar *s = specialeffet_src.ptr<uchar>(i);
        for (int j = 0; j < specialeffet_srccol; ++j)
        {
            uchar b = s[3 * j];
            uchar g = s[3 * j + 1];
            uchar r = s[3 * j + 2];
            float max = max3(r, g, b);
            float min = min3(r, g, b);
            float delta, value;
            float L, S, alpha;
            delta = (max - min) / 255;
            if (delta == 0)
                continue;
            value = (max + min) / 255;
            L = value / 2;
            if (L < 0.5)
                S = delta / value;
            else
                S = delta / (2 - value);
            if (Increment >= 0)
            {
                if ((Increment + S) >= 1)
                    alpha = S;
                else
                    alpha = 1 - Increment;
                alpha = 1 / alpha - 1;
                t[3 * j + 2] =static_cast<uchar>( r + (r - L * 255) * alpha);
                t[3 * j + 1] = static_cast<uchar>(g + (g - L * 255) * alpha);
                t[3 * j] = static_cast<uchar>(b + (b - L * 255) * alpha);
            }
            else
            {
                alpha = Increment;
                t[3 * j + 2] = static_cast<uchar>(L * 255 + (r - L * 255) * (1 + alpha));
                t[3 * j + 1] = static_cast<uchar>(L * 255 + (g - L * 255) * (1 + alpha));
                t[3 * j] = static_cast<uchar>(L * 255 + (b - L * 255) * (1 + alpha));
            }
        }
    }
    return specialeffet_temp;
}
cv::Mat video_Lightness(cv::Mat special_effectsrc, float percent)
{
    float alpha = percent / 100;
    alpha = max(-1.f, min(1.f, alpha));
    cv::Mat special_effecttemp = special_effectsrc.clone();
    int special_effectsrcrow = special_effectsrc.rows;
    int special_effectsrccol = special_effectsrc.cols;
    for (int i = 0; i < special_effectsrcrow; ++i)
    {
        uchar *t = special_effecttemp.ptr<uchar>(i);
        uchar *s = special_effectsrc.ptr<uchar>(i);
        for (int j = 0; j < special_effectsrccol; ++j)
        {
            uchar b = s[3 * j];
            uchar g = s[3 * j + 1];
            uchar r = s[3 * j + 2];
            if (alpha >= 0)
            {
                t[3 * j + 2] = static_cast<uchar>(r*(1 - alpha) + 255 * alpha);
                t[3 * j + 1] = static_cast<uchar>(g*(1 - alpha) + 255 * alpha);
                t[3 * j] = static_cast<uchar>(b*(1 - alpha) + 255 * alpha);
            }
            else
            {
                t[3 * j + 2] = static_cast<uchar>(r*(1 + alpha));
                t[3 * j + 1] = static_cast<uchar>(g*(1 + alpha));
                t[3 * j] = static_cast<uchar>(b*(1 + alpha));
            }
        }
    }
    return special_effecttemp;
}

cv::Mat video_Contrast(cv::Mat special_effectsrc, int percent)
{
    float alpha = percent / 100.f;
    alpha = max(-1.f, min(1.f, alpha));
    cv::Mat special_effecttemp = special_effectsrc.clone();
    int special_effectsrcrow = special_effectsrc.rows;
    int special_effectsrccol = special_effectsrc.cols;
    int thresh = 127;
    for (int i = 0; i < special_effectsrcrow; ++i)
    {
        uchar *t = special_effecttemp.ptr<uchar>(i);
        uchar *s = special_effectsrc.ptr<uchar>(i);
        for (int j = 0; j < special_effectsrccol; ++j)
        {
            uchar b = s[3 * j];
            uchar g = s[3 * j + 1];
            uchar r = s[3 * j + 2];
            int newb, newg, newr;
            if (alpha == 1)
            {
                t[3 * j + 2] = r > thresh ? 255 : 0;
                t[3 * j + 1] = g > thresh ? 255 : 0;
                t[3 * j] = b > thresh ? 255 : 0;
                continue;
            }
            else if (alpha >= 0)
            {
                newr = static_cast<int>(thresh + (r - thresh) / (1 - alpha));
                newg = static_cast<int>(thresh + (g - thresh) / (1 - alpha));
                newb = static_cast<int>(thresh + (b - thresh) / (1 - alpha));
            }
            else {
                newr = static_cast<int>(thresh + (r - thresh) * (1 + alpha));
                newg = static_cast<int>(thresh + (g - thresh) * (1 + alpha));
                newb = static_cast<int>(thresh + (b - thresh) * (1 + alpha));

            }
            newr = max(0, min(255, newr));
            newg = max(0, min(255, newg));
            newb = max(0, min(255, newb));
            t[3 * j + 2] = static_cast<uchar>(newr);
            t[3 * j + 1] = static_cast<uchar>(newg);
            t[3 * j] = static_cast<uchar>(newb);
        }
    }
    return special_effecttemp;
}


cv::Mat video_Sharpen(cv::Mat specialeffect_src, int percent, int type)
{
    cv::Mat specialeffect_result;
    cv::Mat specialeffect_s = specialeffect_src.clone();
    cv::Mat kernel;
    switch (type)
    {
    case 0:
        kernel = (Mat_<int>(3, 3) <<
            0, -1, 0,
            -1, 4, -1,
            0, -1, 0
            );
        break;
    case 1:
        kernel = (Mat_<int>(3, 3) <<
            -1, -1, -1,
            -1, 8, -1,
            -1, -1, -1
            );
        break;
    default:
        kernel = (Mat_<int>(3, 3) <<
            0, -1, 0,
            -1, 4, -1,
            0, -1, 0
            );
        break;
    }
    filter2D(specialeffect_s, specialeffect_s, specialeffect_s.depth(), kernel);
    specialeffect_result = specialeffect_src + specialeffect_s * 0.01 * percent;
    return specialeffect_result;
}

cv::Mat video_HighLight(cv::Mat specialeffect_input, int light)
{
    // 生成灰度图
    cv::Mat specialeffect_gray = cv::Mat::zeros(specialeffect_input.size(), CV_32FC1);
    cv::Mat specialeffect_f = specialeffect_input.clone();
    specialeffect_f.convertTo(specialeffect_f, CV_32FC3);
    std::vector<cv::Mat> specialeffect_pics;
    split(specialeffect_f, specialeffect_pics);
    specialeffect_gray = 0.299f*specialeffect_pics[2] + 0.587*specialeffect_pics[1] + 0.114*specialeffect_pics[0];
    specialeffect_gray = specialeffect_gray / 255.f;

    // 确定高光区
    cv::Mat specialeffect_thresh = cv::Mat::zeros(specialeffect_gray.size(), specialeffect_gray.type());
    specialeffect_thresh = specialeffect_gray.mul(specialeffect_gray);
    // 取平均值作为阈值
    Scalar specialeffect_t = mean(specialeffect_thresh);
    cv::Mat specialeffect_mask = cv::Mat::zeros(specialeffect_gray.size(), CV_8UC1);
    specialeffect_mask.setTo(255, specialeffect_thresh >= specialeffect_t[0]);

    // 参数设置
    int specialeffect_max = 4;
    float specialeffect_bright = light / 100.0f / specialeffect_max;
    float specialeffect_mid = 1.0f + specialeffect_max * specialeffect_bright;

    // 边缘平滑过渡
    cv::Mat specialeffect_midrate = cv::Mat::zeros(specialeffect_input.size(), CV_32FC1);
    cv::Mat specialeffect_brightrate = cv::Mat::zeros(specialeffect_input.size(), CV_32FC1);
    for (int i = 0; i < specialeffect_input.rows; ++i)
    {
        uchar *m = specialeffect_mask.ptr<uchar>(i);
        float *specialeffect_th = specialeffect_thresh.ptr<float>(i);
        float *specialeffect_mi = specialeffect_midrate.ptr<float>(i);
        float *specialeffect_br = specialeffect_brightrate.ptr<float>(i);
        for (int j = 0; j < specialeffect_input.cols; ++j)
        {
            if (m[j] == 255)
            {
                specialeffect_mi[j] = specialeffect_mid;
                specialeffect_br[j] = specialeffect_bright;
            }
            else {
                specialeffect_mi[j] = (specialeffect_mid - 1.0f) / specialeffect_t[0] * specialeffect_th[j] + 1.0f;
                specialeffect_br[j] = (1.0f / specialeffect_t[0] * specialeffect_th[j])*specialeffect_bright;
            }
        }
    }

    // 高光提亮，获取结果图
    cv::Mat specialeffect_result = cv::Mat::zeros(specialeffect_input.size(), specialeffect_input.type());
    for (int i = 0; i < specialeffect_input.rows; ++i)
    {
        float *specialeffect_mi = specialeffect_midrate.ptr<float>(i);
        float *specialeffect_br = specialeffect_brightrate.ptr<float>(i);
        uchar *specialeffect_in = specialeffect_input.ptr<uchar>(i);
        uchar *specialeffect_r = specialeffect_result.ptr<uchar>(i);
        for (int j = 0; j < specialeffect_input.cols; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                float specialeffect_temp = pow(float(specialeffect_in[3 * j + k]) / 255.f, 1.0f / specialeffect_mi[j])*(1.0 / (1 - specialeffect_br[j]));
                if (specialeffect_temp > 1.0f)
                    specialeffect_temp = 1.0f;
                if (specialeffect_temp < 0.0f)
                    specialeffect_temp = 0.0f;
                uchar utemp = uchar(255*specialeffect_temp);
                specialeffect_r[3 * j + k] = utemp;
            }

        }
    }
    return specialeffect_result;
}

// 色温调节
cv::Mat video_ColorTemperature(cv::Mat specialeffect_input, int specialeffect_n)
{
    cv::Mat specialeffect_result = specialeffect_input.clone();
    int specialeffect_row = specialeffect_input.rows;
    int specialeffect_col = specialeffect_input.cols;
    int specialeffect_level = specialeffect_n/2;
    for (int i = 0; i < specialeffect_row; ++i)
    {
        uchar* specialeffect_a = specialeffect_input.ptr<uchar>(i);
        uchar* specialeffect_r = specialeffect_result.ptr<uchar>(i);
        for (int j = 0; j < specialeffect_col; ++j)
        {
            int specialeffect_R,specialeffect_G,specialeffect_B;
            // R通道
            specialeffect_R = specialeffect_a[j * 3 + 2];
            specialeffect_R = specialeffect_R + specialeffect_level;
            if (specialeffect_R > 255) {
                specialeffect_r[j * 3 + 2] = 255;
            }
            else if (specialeffect_R < 0) {
                specialeffect_r[j * 3 + 2] = 0;
            }
            else {
                specialeffect_r[j * 3 + 2] = specialeffect_R;
            }
            // G通道
            specialeffect_G = specialeffect_a[j * 3 + 1];
            specialeffect_G = specialeffect_G + specialeffect_level;
            if (specialeffect_G > 255) {
                specialeffect_r[j * 3 + 1] = 255;
            }
            else if (specialeffect_G < 0) {
                specialeffect_r[j * 3 + 1] = 0;
            }
            else {
                specialeffect_r[j * 3 + 1] = specialeffect_G;
            }
            // B通道
            specialeffect_B = specialeffect_a[j * 3];
            specialeffect_B = specialeffect_B - specialeffect_level;
            if (specialeffect_B > 255) {
                specialeffect_r[j * 3] = 255;
            }
            else if (specialeffect_B < 0) {
                specialeffect_r[j * 3] = 0;
            }
            else {
                specialeffect_r[j * 3] = specialeffect_B;
            }
        }
    }
    return specialeffect_result;
}

//暖色调
cv::Mat video_warmer(cv::Mat specialeffect_src, int specialeffect_warm)
{
    cv::Mat specialeffect_result = specialeffect_src.clone();
    int specialeffect_row = specialeffect_src.rows;
    int specialeffect_col = specialeffect_src.cols;
    int specialeffect_level = specialeffect_warm/2;
    for (int i = 0; i < specialeffect_row; ++i)
    {
        uchar* specialeffect_a = specialeffect_src.ptr<uchar>(i);
        uchar* specialeffect_r = specialeffect_result.ptr<uchar>(i);
        for (int j = 0; j < specialeffect_col; ++j)
        {
            int specialeffect_R,specialeffect_G,specialeffect_B;
            // R通道
            specialeffect_R = specialeffect_a[j * 3 + 2];
            specialeffect_R = specialeffect_R + specialeffect_level;
            if (specialeffect_R > 255) {
                specialeffect_r[j * 3 + 2] = 255;
            }
            else if (specialeffect_R < 0) {
                specialeffect_r[j * 3 + 2] = 0;
            }
            else {
                specialeffect_r[j * 3 + 2] = specialeffect_R;
            }
            // G通道
            specialeffect_G = specialeffect_a[j * 3 + 1];
            specialeffect_G = specialeffect_G + specialeffect_level;
            if (specialeffect_G > 255) {
                specialeffect_r[j * 3 + 1] = 255;
            }
            else if (specialeffect_G < 0) {
                specialeffect_r[j * 3 + 1] = 0;
            }
            else {
                specialeffect_r[j * 3 + 1] = specialeffect_G;
            }
            // B通道
            specialeffect_B = specialeffect_a[j * 3];
            specialeffect_B = specialeffect_B - specialeffect_level;
            if (specialeffect_B > 255) {
                specialeffect_r[j * 3] = 255;
            }
            else if (specialeffect_B < 0) {
                specialeffect_r[j * 3] = 0;
            }
            else {
                specialeffect_r[j * 3] = specialeffect_B;
            }
        }
    }
    return specialeffect_result;
}


//素描化
cv::Mat video_apply_sketch(const Mat& src)
{
    Mat gray, inv, blur_img, sketch,final_sketch;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    bitwise_not(gray, inv);
    GaussianBlur(inv, blur_img, Size(21, 21), 0);
    bitwise_not(blur_img, blur_img);
    divide(gray, blur_img, sketch, 256.0);

    cvtColor(sketch,final_sketch, COLOR_GRAY2BGR);

    return final_sketch;
}

//怀旧风
Mat video_apply_sepia(const Mat& src) {
    Mat sepia_image = Mat::zeros(src.size(), src.type());

    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            Vec3b color = src.at<Vec3b>(y, x);
            float B = color[0];
            float G = color[1];
            float R = color[2];
            float newB = 0.272f * R + 0.534f * G + 0.131f * B;
            float newG = 0.349f * R + 0.686f * G + 0.168f * B;
            float newR = 0.393f * R + 0.769f * G + 0.189f * B;
            sepia_image.at<Vec3b>(y, x)[0] = saturate_cast<uchar>(newB);
            sepia_image.at<Vec3b>(y, x)[1] = saturate_cast<uchar>(newG);
            sepia_image.at<Vec3b>(y, x)[2] = saturate_cast<uchar>(newR);
        }
    }
    return sepia_image;
}

//颗粒化
void video_add_noise(Mat& image, double noise_factor) {
    Mat noise = Mat(image.size(), image.type());
    randn(noise, 0, noise_factor);
    image += noise;
}
Mat video_apply_grain(const Mat& src, double noise_factor) {
    Mat grainy_image = src.clone();
    video_add_noise(grainy_image, noise_factor);
    return grainy_image;
}

//动漫化
Mat video_cartoonize(const Mat& src)
{

    Mat tmp_dst=src.clone();

    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);

    Mat blurred;
    medianBlur(gray, blurred, 7);

    Mat edges;
    adaptiveThreshold(blurred, edges, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 2);

    Mat color;
    bilateralFilter(src, color, 9, 300, 300);

    tmp_dst = Scalar::all(0);
    color.copyTo(tmp_dst, edges);

    return tmp_dst;
}

//浮雕化
Mat video_apply_fd(const Mat& src) {
    Mat gray, embossed;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    embossed = Mat::zeros(gray.size(), gray.type());
    for (int y = 1; y < gray.rows; y++) {
        for (int x = 1; x < gray.cols; x++) {
            int current_pixel = gray.at<uchar>(y, x);
            int previous_pixel = gray.at<uchar>(y - 1, x - 1);
            int new_pixel = current_pixel - previous_pixel + 128;
            embossed.at<uchar>(y, x) = saturate_cast<uchar>(new_pixel);
        }
    }
    Mat embossed_bgr;
    cvtColor(embossed, embossed_bgr, COLOR_GRAY2BGR);
    return embossed_bgr;
}


wstring s2ws4(const std::string& s1)
{//将string转成widestring，并且处理中文路径的问题
    int len1;
    int slength1 = (int)s1.length() + 1;
    len1 = MultiByteToWideChar(CP_ACP, 0, s1.c_str(), slength1, 0, 0);
    std::wstring r(len1, L'\0');
    MultiByteToWideChar(CP_ACP, 0, s1.c_str(), slength1, &r[0], len1);
    return r;
}

void remove_all_files_in_directory4(const std::string& folder_path)
{
    std::string search_path = folder_path + "\\*.*";
    std::wstring wsearch_path = s2ws4(search_path);
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(wsearch_path.c_str(), &fd);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error finding first file: " << GetLastError() << std::endl;
        return;
    }

    do {
        std::string file_path = folder_path + "\\" + std::string(fd.cFileName, fd.cFileName + wcslen(fd.cFileName));
        std::wstring wfile_path = s2ws4(file_path);

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


string ffmpeg_path4 = "ffmpeg\\bin\\ffmpeg.exe";

QString inputvideo_filename4;//待处理video地址
string original_video_address4;

string final_output_address4;//video输出地址

string screenshots_address="ScreenShots";//截图输出地址
wstring wide_screenshots_address= s2ws4(screenshots_address);

string workshop_path="WorkShop";
wstring wide_workshop_path= s2ws4(workshop_path);

string workshop_video_path4="WorkShop\\workshop_tmpvideo.mp4";  // 临时文件路径用于存储处理后的无音频视频
string workshop_audio_path4="WorkShop\\workshop_tmpaudio.wav";	// 临时文件路径用于存储音频
string workshop_output_path4="WorkShop\\workshop_created_video.mp4";

int have_newvideo_flag=0;


double special_playspeed4=1.0;
int screenshots_num4 = 0;//已经截图的数量

int gifdelay4 = 100;//单位ms
int choose4; //选择的特效


int have_video_input=0;


void SubWindow4::on_InputVideo_button_clicked()
{
    inputvideo_filename4=QFileDialog::getOpenFileName(this, "选择视频", "", "Video(*.mp4 *.avi)");

    if (inputvideo_filename4.isEmpty())
    {
        std::cerr << "Error:No Input" << std::endl;
        return;
    }

    if (CreateDirectoryW(wide_screenshots_address.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
    {
        remove_all_files_in_directory4(screenshots_address);
    }


    have_video_input=1;

    screenshots_num4=0;

    have_newvideo_flag=0;

    original_video_address4=inputvideo_filename4.toStdString();
}



void SubWindow4::on_separate_musicvideo_button_clicked()
{
    if(have_video_input==0)
    {
        QMessageBox::StandardButton no_video_warning;
        no_video_warning=QMessageBox::information(nullptr,"No Video Input",
                                                 "Please Input A Video first~"
                                                 );
        return;
    }

    // 打开文件夹选择对话框
    QString separate_folderPath = QFileDialog::getExistingDirectory(this, "Choose FolderPath", "");

    if (separate_folderPath.isEmpty())
    {
        cerr<<"OutputPath Error~"<<endl;
        return;
    }

    string separate_video_path=separate_folderPath.toStdString()+"//Separated_Video.mp4";
    string separate_audio_path=separate_folderPath.toStdString()+"//Separated_Audio.wav";


    VideoCapture cap(original_video_address4);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open the video!" << endl;
        return;
    }

    double fps = cap.get(CAP_PROP_FPS);
    double new_fps = fps;

    int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v'); // 使用 mp4v 编码器



    VideoWriter output_video(separate_video_path, fourcc, new_fps, Size(frame_width, frame_height));

    if (!output_video.isOpened()) {
        cerr << "Error: Could not open the output video file for write!" << endl;
        return;
    }

    cerr<<"Video Separating..."<<endl;

    while (true)
    {
        Mat frame;
        bool isSuccess = cap.read(frame);
        if (!isSuccess)
        {
            cout << "Video processing completed" << endl;
            break;
        }
        output_video.write(frame);

    }

    // 释放视频捕获和写入对象
    cap.release();
    output_video.release();

    cerr<<"Video Separated Successfully!"<<endl;

    string ffmpeg_command1 = ffmpeg_path4 + " -y -i " + original_video_address4 + " -filter:a \"atempo=" + to_string(1) + "\" " + separate_audio_path;//提取原vdo中的音频到temp_audio
    system(ffmpeg_command1.c_str());
}

void SubWindow4::on_PlayVideo_button_clicked()
{
    if(have_video_input==0)
    {
        QMessageBox::StandardButton no_video_warning;
        no_video_warning=QMessageBox::information(nullptr,"No Video Input",
                                                 "Please Input A Video first~"
                                                 );
        return;
    }

    if (CreateDirectoryW(wide_screenshots_address.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
    {
        std::cout << "Directory created successfully: " << screenshots_address<< std::endl;
    }
    else {
        std::cerr << "Failed to create directory: " << screenshots_address << std::endl;
    }


    VideoCapture cap(original_video_address4);
    if (!cap.isOpened())
    {
        cerr << "Error: Could not open the video!" << endl;
        return;
    }
    double fps = cap.get(CAP_PROP_FPS);
    int wait_time = static_cast<int>(1000 / fps);
    bool paused = false;
    screenshots_num4=0;

    QMessageBox::StandardButton videoplay_helper;
    videoplay_helper=QMessageBox::information(nullptr,"VideoPlay Helper",
                                             "Use Your Keyboard:\n"
                                             "ESC:Turn Off The Video Playing\n"
                                             "W:ScreenShot\n"
                                             );

    while (true) {
        Mat frame;
        if (!paused) {
            bool isSuccess = cap.read(frame);
            if (!isSuccess)
            {
                cout << "Video Finished" << endl;
                break;
            }
        }
            imshow("Show Video", frame);
            char key = (char)waitKey(paused ? 0 : wait_time);
            if (key == 27) { // ESC退出
                cout << "ESC" << endl;
                break;
            }
            else if (key == 'w')
            { // w截图
                screenshots_num4++;
                imwrite(screenshots_address + "\\screenshot" + to_string(screenshots_num4) + ".png", frame);
                cout << "ScreenShot No." << screenshots_num4 << " Image in "<<screenshots_address << endl;
            }
    }

    destroyAllWindows();



}


void SubWindow4::on_SetSpeed_button_clicked()
{
    if(have_video_input==0)
    {
        QMessageBox::StandardButton no_video_warning;
        no_video_warning=QMessageBox::information(nullptr,"No Video Input",
                                                 "Please Input A Video first~"
                                                 );
        return;
    }

    bool input_playspeed_ok;
    double tmp_inputplayspeed4;
    tmp_inputplayspeed4 = QInputDialog::getDouble(nullptr, "Input Speed", "Please Set Your Video Speed:", 1.0, 0.5, 64, 1, &input_playspeed_ok);

    if(input_playspeed_ok)
    {
        special_playspeed4=tmp_inputplayspeed4;
        ui->playspeed_label->setText(QString::number(tmp_inputplayspeed4));
    }
    else
    {
        cerr<<"Input Speed Error~"<<endl;
        return;
    }
}

void SubWindow4::on_Set_effect_box_activated(int index)
{
    choose4=index;
}

void SubWindow4::on_CreateNewVideo_button_clicked()
{
    if(have_video_input==0)
    {
        QMessageBox::StandardButton no_video_warning;
        no_video_warning=QMessageBox::information(nullptr,"No Video Input",
                                                 "Please Input A Video first~"
                                                 );
        return;
    }

    if (CreateDirectoryW(wide_workshop_path.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
    {
        std::cout << "Directory created successfully: " << workshop_path<< std::endl;
    }
    else {
        std::cerr << "Failed to create directory: " << workshop_path << std::endl;
    }

    string create_newvideo_source=original_video_address4;

    VideoCapture cap(create_newvideo_source);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open the video!" << endl;
        return;
    }
    double fps = cap.get(CAP_PROP_FPS);
    double new_fps = fps * special_playspeed4;

    int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v'); // 使用 mp4v 编码器



    VideoWriter output_video(workshop_video_path4, fourcc, new_fps, Size(frame_width, frame_height));

    if (!output_video.isOpened())
    {
        cerr << "Error: Could not open the output video file for write!" << endl;
        return;
    }



     cout<<"Creating New Video....."<<endl;

    while (true)
    {
        Mat frame, new_frame;

            bool isSuccess = cap.read(frame);
            int framecnt = 0;
            if (!isSuccess) {
                cout << "Video processing completed" << endl;
                break;
            }

            if (special_playspeed4 < 1)
            {
                for (int i = 1; i <= 1 / (special_playspeed4); i++)
                {
                    if (choose4 <=1)new_frame = frame.clone();
                    else if (choose4 == 2)
                    {
                        Mat tmp_newframe;
                        cvtColor(frame, tmp_newframe, COLOR_BGR2GRAY);
                        cvtColor(tmp_newframe, new_frame, COLOR_GRAY2BGR);
                    }
                    else if (choose4 == 3) new_frame = video_apply_sepia(frame);
                    else if (choose4 == 4) new_frame=  video_cartoonize(frame);
                    else if (choose4 == 5) new_frame=video_apply_sketch(frame);
                    else if (choose4 == 6)new_frame = video_apply_grain(frame, 40);
                    else if (choose4 == 7) new_frame = video_apply_fd(frame);
                    else if(choose4==8)new_frame=video_warmer(frame,30);
                    else if(choose4==9)
                    {
                        cv::Mat tmp8_con = video_Contrast(frame, 25);

                        cv::Mat tmp8_lig = video_Lightness(tmp8_con, -30);

                        cv::Mat tmp8_hig = video_HighLight(tmp8_lig, 20);

                        cv::Mat tmp8_sha = video_Sharpen(tmp8_hig, 50, 0);

                        cv::Mat tmp8_sat = video_Saturation(tmp8_sha, 20);

                        cv::Mat tmp8_col = video_ColorTemperature(tmp8_sat, -20);

                        new_frame=tmp8_col.clone();
                    }





                    output_video.write(new_frame);
                }
            }
            else if (framecnt % static_cast<int>(special_playspeed4) == 0)
            {
                if (choose4 <=1)new_frame = frame.clone();
                else if (choose4 == 2)
                {
                    Mat tmp_newframe;
                    cvtColor(frame, tmp_newframe, COLOR_BGR2GRAY);
                    cvtColor(tmp_newframe, new_frame, COLOR_GRAY2BGR);
                }
                else if (choose4 == 3) new_frame = video_apply_sepia(frame);
                else if (choose4 == 4) new_frame=  video_cartoonize(frame);
                else if (choose4 == 5)
                {
                    new_frame=video_apply_sketch(frame);
                }
                else if (choose4 == 6) new_frame = video_apply_grain(frame, 40);
                else if (choose4 == 7) new_frame = video_apply_fd(frame);
                else if(choose4==8)new_frame=video_warmer(frame,30);
                else if(choose4==9)
                {
                    cv::Mat tmp8_con = video_Contrast(frame, 25);

                    cv::Mat tmp8_lig = video_Lightness(tmp8_con, -30);

                    cv::Mat tmp8_hig = video_HighLight(tmp8_lig, 20);

                    cv::Mat tmp8_sha = video_Sharpen(tmp8_hig, 50, 0);

                    cv::Mat tmp8_sat = video_Saturation(tmp8_sha, 20);

                    cv::Mat tmp8_col = video_ColorTemperature(tmp8_sat, -20);

                    new_frame=tmp8_col.clone();
                }



                output_video.write(new_frame);

            }
            else
            {
                cap.grab(); // 跳过帧
            }
            framecnt++;

    }

    // 释放视频捕获和写入对象
    cap.release();
    output_video.release();

    string ffmpeg_command1 = ffmpeg_path4 + " -y -i " + create_newvideo_source + " -filter:a \"atempo=" + to_string(special_playspeed4) + "\" " + workshop_audio_path4;//提取原vdo中的音频到temp_audio
    string ffmpeg_command2 = ffmpeg_path4 + " -y -i " + workshop_video_path4 + " -i " + workshop_audio_path4 + " -c:v copy -c:a aac -strict experimental " + workshop_output_path4;//将temp_radio和temp_audio合并
    system(ffmpeg_command1.c_str());
    system(ffmpeg_command2.c_str());

    cout<<"Create New Video Successfully!"<<endl;
    have_newvideo_flag=1;
}

void SubWindow4::on_PlayNewVideo_button_clicked()
{
    if(have_newvideo_flag==0)
    {
        QMessageBox::StandardButton no_video_warning;
        no_video_warning=QMessageBox::information(nullptr,"No New Video",
                                                 "Please Creat A New Video first~"
                                                 );
        return;
    }

    if (CreateDirectoryW(wide_screenshots_address.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
    {
        std::cout << "Directory created successfully: " << screenshots_address<< std::endl;
    }
    else {
        std::cerr << "Failed to create directory: " << screenshots_address << std::endl;
    }


    VideoCapture cap(workshop_output_path4);
    if (!cap.isOpened())
    {
        cerr << "Error: Could not open the video!" << endl;
        return;
    }
    double fps = cap.get(CAP_PROP_FPS);
    int wait_time = static_cast<int>(1000 / fps);
    bool paused = false;

    QMessageBox::StandardButton videoplay_helper;
    videoplay_helper=QMessageBox::information(nullptr,"VideoPlay Helper",
                                             "Use Your Keyboard:\n"
                                             "ESC:Turn Off The Video Playing\n"
                                             "W:ScreenShot\n"
                                             );

    while (true) {
        Mat frame;
        if (!paused)
        {
            bool isSuccess = cap.read(frame);
            if (!isSuccess)
            {
                cout << "Video Finished" << endl;
                break;
            }
        }
            imshow("Show Video", frame);
            char key = (char)waitKey(paused ? 0 : wait_time);
            if (key == 27) { // ESC退出
                cout << "ESC" << endl;
                break;
            }
            else if (key == 'w')
            { // w截图
                screenshots_num4++;
                imwrite(screenshots_address + "\\screenshot" + to_string(screenshots_num4) + ".png", frame);
                cout << "ScreenShot No." << screenshots_num4 << " Image in "<<screenshots_address << endl;
            }
    }

    destroyAllWindows();

}

void SubWindow4::on_OutputVideo_button_clicked()
{
    if(have_newvideo_flag==0)
    {
        QMessageBox::StandardButton no_video_warning;
        no_video_warning=QMessageBox::information(nullptr,"No New Video",
                                                 "Please Creat A New Video first~"
                                                 );
        return;
    }

    if(have_newvideo_flag)
    {
            QString outputFilePath = QFileDialog::getSaveFileName(this, "保存视频","","Video(*.mp4)");
            if (outputFilePath.isEmpty())
            {
                std::cerr<<"Your OutPutFilePath is Error~"<<std::endl;
            }

            string ffmpeg_command2 = ffmpeg_path4 + " -y -i " + workshop_video_path4 + " -i " + workshop_audio_path4 + " -c:v copy -c:a aac -strict experimental " + outputFilePath.toStdString();//将temp_radio和temp_audio合并
            system(ffmpeg_command2.c_str());
            cout<<"Out Put Successfully!"<<endl;

    }
    else
    {

        cerr<<"No New Video"<<endl;
    }
}
