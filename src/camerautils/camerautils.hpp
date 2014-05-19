/*
 * camerautils.hpp
 *
 *  Created on: Nov 8, 2012
 *      Author: steinbrf
 */


#ifndef CAMERAUTILS_HPP_
#define CAMERAUTILS_HPP_

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
//#include "CCamera.h"
//#include <camera/CCamera.h>
//#include <CMatrix4.h>
//#include <cmath>



class CameraInfo
{
public:
	CameraInfo(const cv::Mat &projection = cv::Mat(),int decompositionMethod = 1);
	CameraInfo(const CameraInfo &info);
	~CameraInfo();
	bool setIntrinsic(const cv::Mat &intrinsic);
	bool setExtrinsic(const cv::Mat &extrinsic);
	bool setRotation(const cv::Mat &rotation);
	bool setRotationLogarithm(const cv::Mat &rotationLogarithm);
	void setTranslation(const cv::Mat &translation);
	bool setProjection(const cv::Mat &projection);
	cv::Mat getIntrinsic() const;
	cv::Mat getExtrinsic() const;
	cv::Mat getExtrinsicInverse() const;
	cv::Mat getProjection() const;
	cv::Mat getRotation() const;
  cv::Mat getRotationQuaternion() const;
	cv::Mat getRotationLogarithm() const;
	cv::Mat getTranslation() const;
    void scale(double scale);
	cv::Mat getQuaternion() const;
	CameraInfo &operator=(const CameraInfo &info);
    void projectTo(CameraInfo other, cv::Mat &A, cv::Mat &b);
protected:
	cv::Mat _intrinsic;
	cv::Mat _rotation;
	cv::Mat _translation;
	int _decompositionMethod;
//	bool decomposeProjectionMatrixBrox
//	(const cv::Mat& P, cv::Mat& cam, cv::Mat& rot, cv::Mat& trans);
    bool decomposeAndFixProjectionMatrixOpenCV
	(const cv::Mat& P, cv::Mat& cam, cv::Mat& rot, cv::Mat& trans);
    void CalculateRotation(double& x, double& y, double & z, double& w) const;
};

class Frustum
{
public:
	//Left Normal
	float lx; float ly; float lz;
	//Left Offset
	float lo;
	//Right Normal
	float rx; float ry; float rz;
	//Right Offset
	float ro;
	//Top Normal
	float tx; float ty; float tz;
	//Top Offset
	float to;
	//Bottom Normal
	float bx; float by; float bz;
	//Bottom Offset
	float bo;
	//Far Normal
	float fx; float fy; float fz;
	//Far Offset
	float fo;

	Frustum(
	float p_lx, float p_ly, float p_lz,
	float p_lo,
	float p_rx, float p_ry, float p_rz,
	float p_ro,
	float p_tx, float p_ty, float p_tz,
	float p_to,
	float p_bx, float p_by, float p_bz,
	float p_bo,
	float p_fx, float p_fy, float p_fz,
	float p_fo);
	Frustum(const CameraInfo &info, float w, float h, float far);
	inline bool inside(float px, float py, float pz)
	{
		return
				(px*lx + py*ly + pz*lz >= lo) &&
				(px*rx + py*ry + pz*rz >= ro) &&
				(px*tx + py*ty + pz*tz >= to) &&
				(px*bx + py*by + pz*bz >= bo)
//				&& (px*fx + py*fy + pz*fz >= fo)
				;
	}
};



#endif /* CAMERAUTILS_HPP_ */
