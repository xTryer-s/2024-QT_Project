#include "subwindow1_picture_process.h"

#include<opencv2/opencv.hpp>
#include<cmath>
#include<stack>
#include<vector>

using namespace std;
using namespace cv;


#define max2(a,b) (a>b?a:b)
#define max3(a,b,c) (a>b?max2(a,c):max2(b,c))
#define min2(a,b) (a<b?a:b)
#define min3(a,b,c) (a<b?min2(a,c):min2(b,c))

cv::Mat specialeffect_Saturation(cv::Mat specialeffet_src, int percent)
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
cv::Mat specialeffect_Lightness(cv::Mat special_effectsrc, float percent)
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

cv::Mat specialeffect_Contrast(cv::Mat special_effectsrc, int percent)
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


cv::Mat specialeffect_Sharpen(cv::Mat specialeffect_src, int percent, int type)
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

cv::Mat specialeffect_HighLight(cv::Mat specialeffect_input, int light)
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
cv::Mat specialeffect_ColorTemperature(cv::Mat specialeffect_input, int specialeffect_n)
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

// 怀旧色
cv::Mat specialeffect_Nostalgic(cv::Mat specialeffect_src)
{
    CV_Assert(specialeffect_src.channels() == 3);
    int specialeffect_row = specialeffect_src.rows;
    int specialeffect_col = specialeffect_src.cols;
    cv::Mat specialeffect_temp = specialeffect_src.clone();
    for (int i = 0; i < specialeffect_row; ++i)
    {
        uchar *specialeffect_s = specialeffect_src.ptr<uchar>(i);
        uchar *specialeffect_t = specialeffect_temp.ptr<uchar>(i);
        for (int j = 0; j < specialeffect_col; ++j)
        {
            int specialeffect_B = specialeffect_s[3 * j];
            int specialeffect_G = specialeffect_s[3 * j + 1];
            int specialeffect_R = specialeffect_s[3 * j + 2];
            // 怀旧调色
            float newB = 0.272f*specialeffect_R + 0.534f*specialeffect_G + 0.131f*specialeffect_B;
            float newG = 0.349f*specialeffect_R + 0.686f*specialeffect_G + 0.168f*specialeffect_B;
            float newR = 0.393f*specialeffect_R + 0.769f*specialeffect_G + 0.189f*specialeffect_B;
            if (newB < 0)
                newB = 0;
            if (newB > 255)
                newB = 255;
            if (newG < 0)
                newG = 0;
            if (newG > 255)
                newG = 255;
            if (newR < 0)
                newR = 0;
            if (newR > 255)
                newR = 255;
            specialeffect_t[3 * j] = (uchar)newB;
            specialeffect_t[3 * j + 1] = (uchar)newG;
            specialeffect_t[3 * j + 2] = (uchar)newR;
        }
    }
    return specialeffect_temp;
}

cv::Mat specialeffect_warmer(cv::Mat specialeffect_src, int specialeffect_warm)
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

cv::Mat specialeffect_Eclosion(cv::Mat specialeffect_src, cv::Point specialeffect_center, float specialeffect_level)
{
    if (specialeffect_level > 0.9)
        specialeffect_level = 0.9f;
    float specialeffect_diff = (1-specialeffect_level) * (specialeffect_src.rows / 2 * specialeffect_src.rows / 2 + specialeffect_src.cols / 2 * specialeffect_src.cols / 2);
    cv::Mat specialeffect_result = specialeffect_src.clone();
    for (int i = 0; i < specialeffect_result.rows; ++i)
    {
        for (int j = 0; j < specialeffect_result.cols; ++j)
        {
            float specialeffect_dx = float(specialeffect_center.x - j);
            float specialeffect_dy = float(specialeffect_center.y - i);
            float specialeffect_ra = specialeffect_dx * specialeffect_dx + specialeffect_dy * specialeffect_dy;
            float specialeffect_m = ((specialeffect_ra-specialeffect_diff) / specialeffect_diff * 255)>0? ((specialeffect_ra - specialeffect_diff) / specialeffect_diff * 255):0;
            int specialeffect_b = specialeffect_result.at<cv::Vec3b>(i, j)[0];
            int specialeffect_g = specialeffect_result.at<cv::Vec3b>(i, j)[1];
            int specialeffect_r = specialeffect_result.at<cv::Vec3b>(i, j)[2];
            specialeffect_b = (int)(specialeffect_b+ specialeffect_m);
            specialeffect_g = (int)(specialeffect_g + specialeffect_m);
            specialeffect_r = (int)(specialeffect_r + specialeffect_m);
            specialeffect_result.at<cv::Vec3b>(i, j)[0] = (specialeffect_b > 255 ? 255 : (specialeffect_b < 0 ? 0 : specialeffect_b));
            specialeffect_result.at<cv::Vec3b>(i, j)[1] = (specialeffect_g > 255 ? 255 : (specialeffect_g < 0 ? 0 : specialeffect_g));
            specialeffect_result.at<cv::Vec3b>(i, j)[2] = (specialeffect_r > 255 ? 255 : (specialeffect_r < 0 ? 0 : specialeffect_r));
        }
    }
    return specialeffect_result;
}






void specialeffect_add_noise(Mat& image, double noise_factor)
{
    Mat noise = Mat(image.size(), image.type());
    randn(noise, 0, noise_factor);
    image += noise;
}
cv::Mat specialeffect_apply_grain(const Mat& src, double noise_factor)
{
    Mat grainy_image = src.clone();
    specialeffect_add_noise(grainy_image, noise_factor);
    return grainy_image;
}



cv::Mat specialeffect_apply_sketch(const Mat& src)
{
    Mat gray, inv, blur_img, sketch;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    bitwise_not(gray, inv);
    GaussianBlur(inv, blur_img, Size(21, 21), 0);
    bitwise_not(blur_img, blur_img);
    divide(gray, blur_img, sketch, 256.0);
    return sketch;
}



cv::Mat specialeffect_cartoon1(const Mat& src)
{
    Mat gray, edges, color, blur_img, cartoon;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray, 7);
    Laplacian(gray, edges, CV_8U, 5);
    threshold(edges, edges, 80, 255, THRESH_BINARY_INV);
    bilateralFilter(src, color, 9, 75, 75);
    color.copyTo(cartoon, edges);
    return cartoon;
}



cv::Mat specialeffect_embossed(const Mat& src)
{
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

//动漫化
cv::Mat specialeffect_cartoon2(const Mat& src)
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
