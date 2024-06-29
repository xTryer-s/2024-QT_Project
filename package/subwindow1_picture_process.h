#ifndef SUBWINDOW1_PICTURE_PROCESS_H
#define SUBWINDOW1_PICTURE_PROCESS_H

#include<opencv2/opencv.hpp>

cv::Mat specialeffect_Saturation(cv::Mat specialeffet_src, int percent);

cv::Mat specialeffect_Lightness(cv::Mat special_effectsrc, float percent);

cv::Mat specialeffect_Contrast(cv::Mat special_effectsrc, int percent);

cv::Mat specialeffect_Sharpen(cv::Mat specialeffect_src, int percent, int type);

cv::Mat specialeffect_HighLight(cv::Mat specialeffect_input, int light);

cv::Mat specialeffect_ColorTemperature(cv::Mat specialeffect_input, int specialeffect_n);

cv::Mat specialeffect_Nostalgic(cv::Mat specialeffect_src);

cv::Mat specialeffect_warmer(cv::Mat specialeffect_src, int specialeffect_warm);

cv::Mat specialeffect_Eclosion(cv::Mat specialeffect_src, cv::Point specialeffect_center, float specialeffect_level);


void specialeffect_add_noise(cv::Mat& image, double noise_factor);

cv::Mat specialeffect_apply_grain(const cv::Mat& src, double noise_factor);

cv::Mat specialeffect_apply_sketch(const cv::Mat& src);


cv::Mat specialeffect_cartoon1(const cv::Mat& src);

cv::Mat specialeffect_embossed(const cv::Mat& src);

//动漫化
cv::Mat specialeffect_cartoon2(const cv::Mat& src);
#endif // SUBWINDOW1_PICTURE_PROCESS_H
