/*
 * geometryfusion_aos.cpp
 *
 *  Created on: May 21, 2013
 *      Author: steinbrf
 */

#include "geometryfusion_aos.hpp"

Fusion_AoS::Fusion_AoS(float offsetX, float offsetY, float offsetZ, bool color,
		float maxX, float maxY, float maxZ, sidetype n)
: _offset(make_float3(offsetX,offsetY,offsetZ)), _n(n), _useColor(color),
  _distanceThreshold(DISTANCETHRESHOLD),
  _distanceWeightSigma(DISTANCEWEIGHTSIGMA), _distanceWeightEpsilon(DISTANCEWEIGHTEPSILON),
  _framesAdded(0),_sumTimeOfAllFrames(0.0), _verbose(true)
{
  float sx = std::isfinite(maxX) && maxX>offsetX ?  (maxX-offsetX) : 0.0;
  float sy = std::isfinite(maxY) && maxY>offsetY ?  (maxY-offsetY) : 0.0;
  float sz = std::isfinite(maxZ) && maxZ>offsetZ ?  (maxZ-offsetZ) : 0.0;
  _scale = std::isfinite(maxX) || std::isfinite(maxY) || std::isfinite(maxZ) ?
  		(sx>sy ? (sx>sz ? sx : sz) : (sy>sz ? sy : sz))/(float)_n : 1.0f;
}

Fusion_AoS::Fusion_AoS(float offsetX, float offsetY, float offsetZ,
		float scale, float distanceThreshold, sidetype n, bool color)
: _offset(make_float3(offsetX,offsetY,offsetZ)), _n(n), _scale(scale),  _useColor(color),
  _distanceThreshold(distanceThreshold),
  _distanceWeightSigma(DISTANCEWEIGHTSIGMA), _distanceWeightEpsilon(DISTANCEWEIGHTEPSILON),
  _framesAdded(0),_sumTimeOfAllFrames(0.0), _verbose(true)
{
	if(_n) fprintf(stderr,"\nWARNING: Fusion is given Size %i at Constructor!",_n);
}
Fusion_AoS::~Fusion_AoS()
{
	if(_verbose) fprintf(stderr,"\nTotal Time for Array of Structs Fusion: %f",
			_sumTimeOfAllFrames/cv::getTickFrequency());
	if(_verbose) fprintf(stderr,"\nAverage Total Time per Frame for Array of Structs Fusion: %f\n\n",
			_sumTimeOfAllFrames/(cv::getTickFrequency()*_framesAdded));
}

float3 Fusion_AoS::offset(){return _offset;}
float Fusion_AoS::size(){return _n*_scale;}
sidetype Fusion_AoS::n(){return _n;}













