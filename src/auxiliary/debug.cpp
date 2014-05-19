/*
 * cuda_debug.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: steinbrf
 */

#include <cstdio>
#include <iostream>
#include <sstream>
#include "debug.hpp"


#ifdef CIMGDEBUG
#include "CImg.h"
using namespace cimg_library;
#endif

std::string memoryInfo(long long bytes)
{
	long long kB = 1024, MB = 1048576,
			GB = 1073741824, TB = 1099511627776;
	std::stringstream stream;
	if(bytes/TB) stream << bytes/TB << " TB "; bytes %= TB;
	if(bytes/GB) stream << bytes/GB << " GB "; bytes %= GB;
	if(bytes/MB) stream << bytes/MB << " MB "; bytes %= MB;
	if(bytes/kB) stream << bytes/kB << " kB "; bytes %= kB;
	if(bytes)    stream << bytes    << " B";
	return stream.str();
}

void saveFloatImage
(
		std::string savename,
		float *image,
		int   width,
		int   height,
		int   channels,
		float minimum,
		float maximum
)
{
	cv::Mat outImage(cv::Size(width,height),((channels==3) ? CV_32FC3 : CV_32FC1));
	for(int x=0;x<width*height*channels;x++) ((float*)(outImage.data))[x] = image[x];
	if(channels==3){
		for(int x=0;x<width*height;x++){
			float temp = ((float*)(outImage.data))[x*3];
			((float*)(outImage.data))[x*3] = ((float*)(outImage.data))[x*3+2];
			((float*)(outImage.data))[x*3+2] = temp;
		}
	}

	if(maximum<minimum){
		float avg = 0.0f;
		float newmax = -1e6f;
		float newmin = 1e6f;
		for(int x=0;x<width*height*channels;x++){
			avg += ((float*)(outImage.data))[x];
			if(newmax < ((float*)(outImage.data))[x]) newmax = ((float*)(outImage.data))[x];
			if(newmin > ((float*)(outImage.data))[x]) newmin = ((float*)(outImage.data))[x];
		}
		avg /= (float)(width*height*channels);
		maximum = newmax;
		if(minimum > 0.0f) {
			minimum = newmin;
		}
		fprintf(stderr,"\nDebug Image %s Minimum: %f Maximum: %f Average: %f",savename.c_str(),minimum,maximum,avg);
	}

	cv::imwrite(savename.c_str(),(outImage-minimum)*(255.0f/(maximum-minimum)));
}

//template <class T>
//void saveFloatImageWeighted
//(
//		std::string savename,
//		float *image,
//		T *weight,
//		int   width,
//		int   height,
//		int   channels,
//		float minimum,
//		float maximum
//)
//{
//	cv::Mat outImage(cv::Size(width,height),CV_32FC4);
//	if(channels==3){
//		for(int x=0;x<width*height*3;x+=3){
//			((float*)(outImage.data))[4*x+0] = image[3*x+2];
//			((float*)(outImage.data))[4*x+1] = image[3*x+1];
//			((float*)(outImage.data))[4*x+2] = image[3*x+0];
//			((float*)(outImage.data))[4*x+3] = weight[x]==0 ? 0.0f : 255.0f;
//		}
//	}
//	else if(channels==1){
//		for(int x=0;x<width*height*3;x+=3){
//			((float*)(outImage.data))[4*x+0] = ((float*)(outImage.data))[4*x+1]= ((float*)(outImage.data))[4*x+2] = image[x];
//			((float*)(outImage.data))[4*x+3] = weight[x]==0 ? 0.0f : 255.0f;
//		}
//	}
//	else{
//		fprintf(stderr,"\nERROR: %i-Channel Weighted Debug Image not supported",channels);
//	}
//
//	if(maximum<minimum){
//		float avg = 0.0f;
//		float newmax = -1e6f;
//		float newmin = 1e6f;
//		for(int x=0;x<width*height;x++){
//			for(int y=0;y<3;y++){
//				avg += ((float*)(outImage.data))[4*x+y];
//				if(newmax < ((float*)(outImage.data))[4*x+y]) newmax = ((float*)(outImage.data))[4*x+y];
//				if(newmin > ((float*)(outImage.data))[4*x+y]) newmin = ((float*)(outImage.data))[4*x+y];
//			}
//		}
//		avg /= (float)(width*height*3);
//		maximum = newmax;
//		if(minimum > 0.0f) {
//			minimum = newmin;
//		}
//		for(int x=0;x<width*height;x++){
//			for(int y=0;y<3;y++){
//				((float*)(outImage.data))[4*x+y] = (((float*)(outImage.data))[4*x+y]-minimum)*(255.0f/(maximum-minimum));
//			}
//		}
//		fprintf(stderr,"\nDebug Image %s Minimum: %f Maximum: %f Average: %f",savename.c_str(),minimum,maximum,avg);
//	}
//
//	cv::imwrite(savename.c_str(),outImage);
//}

void showFloatImage
(
		std::string showname,
		float *image,
		int   width,
		int   height,
		int   channels,
		float minimum,
		float maximum
)
{
	cv::Mat outImage(cv::Size(width,height),((channels==3) ? CV_32FC3 : CV_32FC1));
	for(int x=0;x<width*height*channels;x++) ((float*)(outImage.data))[x] = image[x];
	if(channels==3){
		for(int x=0;x<width*height;x++){
			float temp = ((float*)(outImage.data))[x*3];
			((float*)(outImage.data))[x*3] = ((float*)(outImage.data))[x*3+2];
			((float*)(outImage.data))[x*3+2] = temp;
		}
	}

	if(maximum<minimum){
		float avg = 0.0f;
		float newmax = -1e6f;
		float newmin = 1e6f;
		for(int x=0;x<width*height*channels;x++){
			avg += ((float*)(outImage.data))[x];
			if(newmax < ((float*)(outImage.data))[x]) newmax = ((float*)(outImage.data))[x];
			if(newmin > ((float*)(outImage.data))[x]) newmin = ((float*)(outImage.data))[x];
		}
		avg /= (float)(width*height*channels);
		maximum = newmax;
		if(minimum > 0.0f) {
			minimum = newmin;
		}
		fprintf(stderr,"\nDebug Image %s Minimum: %f Maximum: %f Average: %f",showname.c_str(),minimum,maximum,avg);
	}

	cv::imshow(showname.c_str(),(outImage-minimum)*(1.0f/(maximum-minimum)));
}

void showCharImage(std::string showname,
                   unsigned char * image,
                   int rows,
                   int cols)
{
    float * floatImage =new float[rows*cols];
    for(int y=0;y<rows;y++)
    {
        for(int x=0;x<cols;x++)
        {
            floatImage[y*cols+x]=image[y*cols+x];
        }
    }
    showFloatImage(showname,floatImage,cols,rows,1,0.0,-1.0);
    delete [] floatImage;
}

void showShortImage(std::string showname,
        unsigned short *image,
        int rows,
        int cols
        )
{
    float * floatImage =new float[rows*cols];
    for(int y=0;y<rows;y++)
    {
        for(int x=0;x<cols;x++)
        {
            floatImage[y*cols+x]=image[y*cols+x];
        }
    }
    showFloatImage(showname,floatImage,cols,rows,1,0.0,-1.0);
    delete [] floatImage;
}

void saveShortImage(
        std::string savename,
        unsigned short * image,
        int rows,
        int cols
        )
{
    float * floatImage =new float[rows*cols];
    for(int y=0;y<rows;y++)
    {
        for(int x=0;x<cols;x++)
        {
            floatImage[y*cols+x]=image[y*cols+x];
        }
    }
    saveFloatImage(savename,floatImage,cols,rows,1,0.0,-1.0);
    delete floatImage;
}


