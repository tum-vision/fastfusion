/*
 * geometryfusion.hpp
 *
 *  Created on: May 21, 2013
 *      Author: steinbrf
 */

#ifndef GEOMETRYFUSION_HPP_
#define GEOMETRYFUSION_HPP_


//#include <cuda.h>
#include <opencv2/opencv.hpp>
#include <camerautils/camerautils.hpp>
#include "definitions.h"
#include "mesh.hpp"

//#include <cuda_runtime.h>

class Fusion_AoS
{
public:
	Fusion_AoS(float offsetX, float offsetY, float offsetZ, float scale,
			float distanceThreshold = DISTANCETHRESHOLD,
			sidetype n = 0, bool color = true);
	Fusion_AoS(float offsetX, float offsetY, float offsetZ, bool color = true,
			float maxX = UNENDLICH, float maxY = UNENDLICH, float maxZ = UNENDLICH, sidetype n = SIDELENGTH);
	virtual ~Fusion_AoS();
	virtual int addMap(cv::Mat &image, CameraInfo caminfo,
			std::vector<cv::Mat> rgb = std::vector<cv::Mat>(3)) = 0;
	virtual MeshSeparate getMeshSeparateMarchingCubes(MeshSeparate mesh = MeshSeparate(3)) = 0;
	virtual MeshSeparate getMeshStructural(unsigned int structureType = 0,MeshSeparate mesh = MeshSeparate(4)) = 0;
	float3 offset();
	float size();
	sidetype n();

protected:
	float3 _offset;
	sidetype _n;
	float _scale;
	bool _useColor;
	float _distanceThreshold;
	float _distanceWeightSigma;
	float _distanceWeightEpsilon;
	unsigned int _framesAdded;
	double _sumTimeOfAllFrames;
	bool _verbose;
};


#endif /* GEOMETRYFUSION_HPP_ */
