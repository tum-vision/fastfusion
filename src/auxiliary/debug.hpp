/*
 * cuda_debug.hpp
 *
 *  Created on: Apr 18, 2012
 *      Author: steinbrf
 */

#ifndef CUDA_DEBUG_HPP_
#define CUDA_DEBUG_HPP_

#include <string>
#include <opencv2/opencv.hpp>
#include <stdio.h>

std::string memoryInfo(long long bytes);

void saveFloatImage
(
		std::string savename,
		float *image,
		int   width,
		int   height,
		int   channels,
		float minimum = 0.0f,
		float maximum = 255.0f
);

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
//);

template <class T>
void saveFloatImageWeighted
(
		std::string savename,
		float *image,
		T *weight,
		int   width,
		int   height,
		int   channels,
		float minimum,
		float maximum
)
{
	cv::Mat outImage(cv::Size(width,height),CV_32FC4);
	if(channels==3){
		for(int x=0;x<width*height;x++){
			((float*)(outImage.data))[4*x+0] = image[3*x+2];
			((float*)(outImage.data))[4*x+1] = image[3*x+1];
			((float*)(outImage.data))[4*x+2] = image[3*x+0];
			((float*)(outImage.data))[4*x+3] = weight[x]==0 ? 0.0f : 255.0f;
		}
	}
	else if(channels==1){
		for(int x=0;x<width*height;x++){
			((float*)(outImage.data))[4*x+0] = ((float*)(outImage.data))[4*x+1]= ((float*)(outImage.data))[4*x+2] = image[x];
			((float*)(outImage.data))[4*x+3] = weight[x]==0 ? 0.0f : 255.0f;
		}
	}
	else{
		fprintf(stderr,"\nERROR: %i-Channel Weighted Debug Image not supported",channels);
	}

	if(maximum<minimum){
		float avg = 0.0f;
		float newmax = -1e6f;
		float newmin = 1e6f;
		for(int x=0;x<width*height;x++){
			for(int y=0;y<3;y++){
				avg += ((float*)(outImage.data))[4*x+y];
				if(newmax < ((float*)(outImage.data))[4*x+y]) newmax = ((float*)(outImage.data))[4*x+y];
				if(newmin > ((float*)(outImage.data))[4*x+y]) newmin = ((float*)(outImage.data))[4*x+y];
			}
		}
		avg /= (float)(width*height*3);
		maximum = newmax;
		if(minimum > 0.0f) {
			minimum = newmin;
		}
		for(int x=0;x<width*height;x++){
			for(int y=0;y<3;y++){
				((float*)(outImage.data))[4*x+y] = (((float*)(outImage.data))[4*x+y]-minimum)*(255.0f/(maximum-minimum));
			}
		}
		fprintf(stderr,"\nDebug Image %s Minimum: %f Maximum: %f Average: %f",savename.c_str(),minimum,maximum,avg);
	}

	cv::imwrite(savename.c_str(),outImage);
}

void showFloatImage
(
		std::string showname,
		float *image,
		int   width,
		int   height,
		int   channels,
		float minimum = 0.0f,
		float maximum = 255.0f
);

void showCharImage(
        std::string showname,
        unsigned char * image,
        int rows,
        int cols
        );


void showShortImage(
        std::string showname,
        unsigned short * image,
        int rows,
        int cols
        );

void saveShortImage(
        std::string savename,
        unsigned short * image,
        int rows,
        int cols
        );


#endif /* CUDA_DEBUG_HPP_ */
