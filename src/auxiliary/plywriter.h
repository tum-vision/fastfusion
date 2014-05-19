#ifndef WRITEPLY_H
#define WRITEPLY_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <auxiliary/ocv_tools.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

bool writePLY(const std::string& filename, const cv::Mat &disparity,cv::Mat img, const cv::Mat &Q, const cv::Mat &rH);
bool writePLYBinary(const std::string& filename, const cv::Mat &disparity,cv::Mat img, const cv::Mat &Q, const cv::Mat &rH);
bool writePLYVerticesOnly(const std::string& filename, const cv::Mat &disparity, const cv::Mat &img, const cv::Mat &Q, const cv::Mat &rH);


#endif // WRITEPLY_H
