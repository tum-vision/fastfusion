/*
 * inline_functions.hpp
 *
 *  Created on: Apr 15, 2013
 *      Author: steinbrf
 */

#ifndef INLINE_FUNCTIONS_HPP_
#define INLINE_FUNCTIONS_HPP_

#include "definitions.h"



inline float computeDistanceValue_AoS_h
(
		const float3 &q,
		const float &fx,
		const float &fy,
		const float &cx,
		const float &cy,
		int &imx,
		int &imy,
		const cv::Mat &image
)
{
	imx = (int)floor(q.x/q.z*fx+cx);
	imy = (int)floor(q.y/q.z*fy+cy);
#ifdef DISTANCE_POINT_FAST
	return q.z-image.at<float>(std::max(std::min(imy,image.rows-1),0),
			std::max(std::min(imx,image.cols-1),0));
#endif
#ifdef DISTANCE_POINT_TRUE
	const float length = sqrtf(q.x*q.x+q.y*q.y+q.z*q.z);
	return length - length/q.z* image.at<float>(std::max(std::min(imy,image.rows-1),0),
											std::max(std::min(imx,image.cols-1),0));
#endif
#ifdef DISTANCE_PLANE
	//FIXME: Das funktioniert noch nicht richtig, und ich habe das Gefuehl,
	//in der GPU Variante auch nicht
	const float length = sqrtf(q.x*q.x+q.y*q.y+q.z*q.z);
	float q1z = image.at<float>(std::max(std::min(imy,image.rows-1),0),
			std::max(std::min(imx,image.cols-1),0));
	float q2z = image.at<float>(std::max(std::min(imy,image.rows-1),0),
			std::max(std::min(imx+1,image.cols-1),0));
	float q3z = image.at<float>(std::max(std::min(imy+1,image.rows-1),0),
			std::max(std::min(imx,image.cols-1),0));

	float q1x = q.x/q.z*q1z;
	float q1y = q.y/q.z*q1z;

	float q2x = (imx+1.0f-cx)/fx*q2z;
	float q2y = (imy-cy)/fy*q2z;

	float q3x = (imx-cx)/fx*q3z;
	float q3y = (imy+1.0f-cy)/fy*q3z;

	float nx = (q2y-q1y)*(q3z-q1z)-(q3y-q.y)*(q2z-q1z);
	float ny = (q2z-q1z)*(q3x-q1x)-(q3z-q.z)*(q2x-q1x);
	float nz = (q2x-q1x)*(q3y-q1y)-(q3x-q.x)*(q2y-q1y);
	float norm = sqrtf(nx*nx+ny*ny+nz*nz);
	nx /= norm; ny /= norm; nz /= norm;
//	return fabs(nz) > fabs(nx) && fabs(nz) > fabs(ny) ? (q1x*nx+q1y*ny+q1z*nz)*(q.z/q1z-1.0f) : length - length/q.z* q1z;
	return nz < nx && nz < ny ? (q1x*nx+q1y*ny+q1z*nz)*(q.z/q1z-1.0f) : length - length/q.z* q1z;
//	return (q1x*nx+q1y*ny+q1z*nz)*(q.z/q1z-1.0f);
//	return length - length/q.z* q1z;
#endif
}

inline float processDistanceValue_AoS_h(const float &distance, const float &threshold)
{
#ifdef DISTANCE_TEST
				float sqrtf((x-128)*(x-128)+(y-128)*(y-128)+(z-128)*(z-128))-40;
#endif
#ifdef DISTANCE_NORMAL
				return distance;
#endif
#ifdef DISTANCE_TRUNCATED
				return std::max(-threshold,std::min(threshold,distance));
#endif
}

inline weighttype getDistanceWeight_AoS_variables_h
(
		const float &distance,
		const float &threshold,
		const float &distanceWeightEpsilon,
		const float &distanceWeightSigma
)
{
#ifdef WEIGHT_ONE
	return 1.0f;
#endif
#ifdef WEIGHT_CONSTANT
	return (distance < threshold);
#endif
#ifdef WEIGHT_CONSTANT_NARROW
	return (fabs(distance)<threshold);
#endif
#ifdef WEIGHT_LINEAR
	return (float)(distance<distanceWeightEpsilon) +
			(threshold-distance)/(threshold-distanceWeightEpsilon)*
			(float)(distance>=distanceWeightEpsilon && distance<threshold);
#endif
#ifdef WEIGHT_LINEAR_NARROW
	float distabs = fabs(distance);
					return (float)(distabs<=distanceWeightEpsilon) +
							(threshold-distabs)/(threshold-distanceWeightEpsilon)*
							(float)(distabs>distanceWeightEpsilon && distabs<threshold);
#endif
#ifdef WEIGHT_GAUSS
				float cutoff = expf(-distanceWeightSigma*(threshold-distanceWeightEpsilon)*(threshold-distanceWeightEpsilon));
				return (float)(distance<distanceWeightEpsilon) +
						max((expf(-distanceWeightSigma*(distance-distanceWeightEpsilon)*(distance-distanceWeightEpsilon))-cutoff)/(1.0f-cutoff),DISTANCEMINEXPWEIGHT)
						*(float)(distance>=distanceWeightEpsilon && distance<threshold);
#endif
#ifdef WEIGHT_GAUSS_NARROW
				float distabs = fabs(distance);
				float cutoff = expf(-distanceWeightSigma*(threshold-distanceWeightEpsilon)*(threshold-distanceWeightEpsilon));
				return (float)(distabs<distanceWeightEpsilon) +
						max((expf(-distanceWeightSigma*(distabs-distanceWeightEpsilon)*(distabs-distanceWeightEpsilon))-cutoff)/(1.0f-cutoff),DISTANCEMINEXPWEIGHT)
						*(float)(distabs>=distanceWeightEpsilon && distance<threshold);
#endif
}



inline sidetype leafScaleFunc(const float &depth, const sidetype &referenceBrickLength)
{
	float factor = std::max(depth/REFERENCE_DEPTH,1.0f);
#ifdef SCALE_VARIANCE
	factor *= factor;
	return (sidetype)floor(factor*factor*referenceBrickLength);
//	return referenceBrickLength;
#endif
#ifdef SCALE_DEVIATION
	return (sidetype)floor(factor*factor*referenceBrickLength);
#endif
#ifdef SCALE_LINEAR
	return (sidetype)floor(factor*referenceBrickLength);
#endif
#ifdef SCALE_CONSTANT
	return referenceBrickLength;
#endif
}


#endif /* INLINE_FUNCTIONS_HPP_ */
