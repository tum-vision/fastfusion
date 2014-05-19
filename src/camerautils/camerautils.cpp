/*
 * camerautils.cpp
 *
 *  Created on: Nov 8, 2012
 *      Author: steinbrf
 */

#include "camerautils.hpp"
#include <Eigen/Geometry>

#include <stdio.h>

CameraInfo::CameraInfo(const cv::Mat &projection,int decompositionMethod)
: _decompositionMethod(decompositionMethod)
{
	bool result = false;
	if(!projection.empty()){
		if(decompositionMethod > 1){
			fprintf(stderr,"\nERROR: Invalid Parameter for Projection Matrix Decomposition Method!");
		}
		result = setProjection(projection);
		if(result == false){
			fprintf(stderr,"\nERROR: Could not Create CameraInfo due to errors in the Decomposition Method of %s",
					_decompositionMethod == 0 ? "Brox" : "OpenCV");
		}
	}
	else{
		_intrinsic = cv::Mat::eye(3,3,cv::DataType<double>::type);
		_rotation = cv::Mat::eye(3,3,cv::DataType<double>::type);
		_translation = cv::Mat::zeros(3,1,cv::DataType<double>::type);
	}
}

CameraInfo::CameraInfo(const CameraInfo &info)
{
	this->_rotation = info._rotation.clone();
	this->_translation = info._translation.clone();
	this->_intrinsic = info._intrinsic.clone();
}

CameraInfo::~CameraInfo(){}

bool CameraInfo::setIntrinsic(const cv::Mat &intrinsic)
{
	//TODO: Moeglicher Korrektheitscheck
	_intrinsic = intrinsic.clone();
	return true;
}

bool CameraInfo::setExtrinsic(const cv::Mat &extrinsic)
{
	extrinsic.col(3).rowRange(0,3).copyTo(_translation);
	//TODO: Moeglicher Korrektheitscheck
	extrinsic(cv::Range(0,3),cv::Range(0,3)).copyTo(_rotation);
	return true;
}

bool CameraInfo::setRotation(const cv::Mat &rotation)
{
	//TODO: Moeglicher Korrektheitscheck
	_rotation = rotation.clone();
	return true;
}

bool CameraInfo::setRotationLogarithm(const cv::Mat &rotationLogarithm)
{
	//Rodrigues-Formula, Ma,Soatto, Page 27

	_rotation = cv::Mat::eye(3,3,cv::DataType<double>::type);

	cv::Mat logarithm = rotationLogarithm.clone();

	double length = sqrt(logarithm.at<double>(0,0)*logarithm.at<double>(0,0) +
									     logarithm.at<double>(1,0)*logarithm.at<double>(1,0) +
									     logarithm.at<double>(2,0)*logarithm.at<double>(2,0));
	if(length>0.0){
		cv::Mat hat(3,3,cv::DataType<double>::type);
		hat.at<double>(0,0) = 0.0;
		hat.at<double>(0,1) = -logarithm.at<double>(2,0);
		hat.at<double>(0,2) = logarithm.at<double>(1,0);
		hat.at<double>(1,0) = logarithm.at<double>(2,0);
		hat.at<double>(1,1) = 0.0;
		hat.at<double>(1,2) = -logarithm.at<double>(0,0);
		hat.at<double>(2,0) = -logarithm.at<double>(1,0);
		hat.at<double>(2,1) = logarithm.at<double>(0,0);
		hat.at<double>(2,2) = 0.0;

		_rotation += hat/length*sin(length) + hat*hat/(length*length)*(1.0-cos(length));
	}

	return true;
}

void CameraInfo::setTranslation(const cv::Mat &translation)
{
	_translation = translation.clone();
}

bool CameraInfo::setProjection(const cv::Mat &projection)
{
	bool result = false;
//	if(_decompositionMethod == 0){
////		result = decomposeProjectionMatrixBrox(projection,_intrinsic,_rotation,_translation);
//        std::cerr<<"Invalid decomposition matrix option!"<<std::endl;
//	}
//	else if(_decompositionMethod == 1){
        result = decomposeAndFixProjectionMatrixOpenCV(projection,_intrinsic,_rotation,_translation);
//	}
	return result;
}

cv::Mat CameraInfo::getIntrinsic() const
{
	return _intrinsic.clone();
}

cv::Mat CameraInfo::getExtrinsic() const
{
    cv::Mat result(4,4,cv::DataType<double>::type);
	_rotation.copyTo(result(cv::Range(0,3),cv::Range(0,3)));
    _translation.rowRange(0,3).copyTo(result.col(3).rowRange(0,3));
	result.at<double>(3,0) = result.at<double>(3,1) = result.at<double>(3,2) = 0.0;
	result.at<double>(3,3) = 1.0;
	return result;
}

void CameraInfo::projectTo(CameraInfo other,cv::Mat& A, cv::Mat& b)
{
//    cv::Mat result=cv::Mat::eye(4,4,cv::DataType<double>::type);
//    cv::Mat intInv = _intrinsic.inv();
//    intInv.copyTo(result.rowRange(0,3).colRange(0,3));
//    cv::Mat extInv = getExtrinsicInverse();
//    extInv*=result;
//    result= other.getProjection()*extInv;

//    cv::Mat M=getProjection().rowRange(0,3).colRange(0,3).inv();
//    tmpB=-other.getProjection().rowRange(0,3).colRange(0,3)*M*getProjection().col(3) + other.getProjection().col(3);
//    tmpA=other.getProjection().rowRange(0,3).colRange(0,3)*M;
    cv::Mat tmpA=other.getIntrinsic()*other.getRotation()*getRotation().t()*getIntrinsic().inv();
    cv::Mat tmpb=-other.getIntrinsic()*other.getRotation()*getRotation().t()*getTranslation() + other.getIntrinsic()*other.getTranslation();

    tmpA.convertTo(A,cv::DataType<float>::type);
    tmpb.convertTo(b,cv::DataType<float>::type);

}

void CameraInfo::scale(double scale)
{
   _intrinsic*=scale;
   _intrinsic.at<double>(2,2)=1;
}

cv::Mat CameraInfo::getExtrinsicInverse() const
{
	cv::Mat result(4,4,cv::DataType<double>::type);
	result(cv::Range(0,3),cv::Range(0,3)) = _rotation.t();
	result.col(3).rowRange(0,3) = -_rotation.t()*_translation;
	result.at<double>(3,0) = result.at<double>(3,1) = result.at<double>(3,2) = 0.0;
	result.at<double>(3,3) = 1.0;
	return result;
}


cv::Mat CameraInfo::getProjection() const
{
    cv::Mat result=cv::Mat::eye(4,4,cv::DataType<double>::type);
    _intrinsic.copyTo(result.rowRange(0,3).colRange(0,3));

    cv::Mat extr=getExtrinsic();

    result = result * extr;
	return result;
}

cv::Mat CameraInfo::getRotation() const
{
	return _rotation.clone();
}

void CameraInfo::CalculateRotation( double& x, double& y, double& z, double& w ) const
{

float trace = _rotation.at<double>(0,0) + _rotation.at<double>(1,1) + _rotation.at<double>(2,2) + 1.0f;

if( trace > std::numeric_limits<double>::epsilon() )
{
    float s = 0.5f / sqrtf(trace);
    w = 0.25f / s;
    x = ( _rotation.at<double>(2,1) - _rotation.at<double>(1,2) ) * s;
    y = ( _rotation.at<double>(0,2) - _rotation.at<double>(2,0) ) * s;
    z = ( _rotation.at<double>(1,0) - _rotation.at<double>(0,1) ) * s;
}
else
{
if ( _rotation.at<double>(0,0) > _rotation.at<double>(1,1) && _rotation.at<double>(0,0) > _rotation.at<double>(2,2) )
{
    float s = 2.0f * sqrtf( 1.0f + _rotation.at<double>(0,0) - _rotation.at<double>(1,1) - _rotation.at<double>(2,2) );
    x = 0.25f * s;
    y = (_rotation.at<double>(0,1) + _rotation.at<double>(1,0) ) / s;
    z = (_rotation.at<double>(0,2) + _rotation.at<double>(2,0) ) / s;
    w = (_rotation.at<double>(1,2) - _rotation.at<double>(2,1) ) / s;

}
else if (_rotation.at<double>(1,1) > _rotation.at<double>(2,2))
{
    float s = 2.0f * sqrtf( 1.0f + _rotation.at<double>(1,1) - _rotation.at<double>(0,0) - _rotation.at<double>(2,2) );
    x = (_rotation.at<double>(0,1) + _rotation.at<double>(1,0) ) / s;
    y = 0.25f * s;
    z = (_rotation.at<double>(1,2) + _rotation.at<double>(2,1) ) / s;
    w = (_rotation.at<double>(0,2) - _rotation.at<double>(2,0) ) / s;

}
else
{
    float s = 2.0f * sqrtf( 1.0f + _rotation.at<double>(2,2) - _rotation.at<double>(0,0) - _rotation.at<double>(1,1) );
    x = (_rotation.at<double>(0,2) + _rotation.at<double>(2,0) ) / s;
    y = (_rotation.at<double>(1,2) + _rotation.at<double>(2,1) ) / s;
    z = 0.25f * s;
    w = (_rotation.at<double>(0,1) - _rotation.at<double>(1,0) ) / s;
}
}
}

cv::Mat CameraInfo::getRotationQuaternion() const
{
    cv::Mat q(1,4,cv::DataType<double>::type);
    CalculateRotation(((double*)q.data)[0],
                      ((double*)q.data)[1],
                      ((double*)q.data)[2],
                      ((double*)q.data)[3]);
    return q;
}

cv::Mat CameraInfo::getTranslation() const
{
	return _translation.clone();
}

cv::Mat CameraInfo::getQuaternion() const
{
	cv::Mat result(4,1,cv::DataType<double>::type);
	Eigen::Matrix3d rotation;
	cv::Mat rot2 = getRotation().clone();
	for(int i=0;i<3;i++) for(int j=0;j<3;j++) rotation(i,j) = rot2.at<double>(i,j);
	Eigen::Quaterniond eigenquat(rotation);
	Eigen::Matrix<double,4,1,0,4,1> quat = eigenquat.coeffs();
//	Sophus::SE3 pos;
//	pos.setRotationMatrix(rotation);
//	Eigen::Matrix<double,4,1,0,4,1> quat = pos.unit_quaternion().coeffs();
	for(int i=0;i<4;i++) result.at<double>(i) = quat[i];
//	fprintf(stderr,"\nQuaternion:"); for(int i=0;i<4;i++) fprintf(stderr," %f",result.at<double>(i));
	return result.clone();
}

cv::Mat CameraInfo::getRotationLogarithm() const
{
	//Rodrigues-Formula, Ma,Soatto, Page 27
	double length = acos(0.5*(_rotation.at<double>(0,0)+_rotation.at<double>(1,1)+_rotation.at<double>(2,2)-1.0));
	cv::Mat result(3,1,cv::DataType<double>::type);
	result.at<double>(0,0) = _rotation.at<double>(2,1)-_rotation.at<double>(1,2);
	result.at<double>(1,0) = _rotation.at<double>(0,2)-_rotation.at<double>(2,0);
	result.at<double>(2,0) = _rotation.at<double>(1,0)-_rotation.at<double>(0,1);
	result *= length/(2.0*sin(length));
	return result.clone();
}

//bool CameraInfo::decomposeProjectionMatrixBrox(const cv::Mat& P, cv::Mat& cam, cv::Mat& rot, cv::Mat& trans)
//{
//    if(!(P.type()==cv::DataType<double>::type)){
//    	fprintf(stderr,"\nERROR: Matrix is not of type double");
//    	return false;
//    }
//    cam.create(3,3,P.type());
//    rot.create(3,3,P.type());
//    trans.create(3,1,P.type());
//    P.col(3).rowRange(0,3).copyTo(trans);
//
//    CCamera<double> camera;
//    CMatrix4<double> PMat;
//    for(int i=0;i<3;i++){
//        for(int j=0;j<4;j++){
//            PMat(j,i)=P.at<double>(i,j);
//        }
//    }
//    PMat(0,3)=0.0;
//    PMat(1,3)=0.0;
//    PMat(2,3)=0.0;
//    PMat(3,3)=1.0;
//
//    camera.setProjectionMatrix(PMat);
//    CMatrix4<double> transformation = camera.getWorldTransformationMatrix();
//
//    CMatrix4<double> camMat = camera.getCalibrationMatrix();
//    for(int i=0;i<3;i++){
//			for(int j=0;j<4;j++){
//				if(j<3){
//					rot.at<double>(i,j)=transformation(j,i);
//					cam.at<double>(i,j)=camMat(j,i);
//				}
//				else
//					trans.at<double>(i,0)=transformation(j,i);
//			}
//    }
//
//    return true;
//}

void fixRQ3x3(cv::Mat& R, cv::Mat& Q)
{
    for(int r=0;r<3;r++)
    {
        if(R.at<double>(r,r)<0)
        {
            for(int c=0;c<3;c++)
            {
                R.at<double>(c,r)*=-1;
                Q.at<double>(r,c)*=-1;
            }
        }
    }
    Q=Q*cv::determinant(Q);
}

bool CameraInfo::decomposeAndFixProjectionMatrixOpenCV(const cv::Mat& P, cv::Mat& cam, cv::Mat& rot, cv::Mat& trans)
{
    if(!(P.type()==cv::DataType<double>::type)){
    	fprintf(stderr,"\nERROR: Matrix is not of type double");
    	return false;
    }

    cam.create(3,3,P.type());
    rot.create(3,3,P.type());
    trans.create(3,1,P.type());

    cv::RQDecomp3x3(P.rowRange(0,3).colRange(0,3),cam,rot);
//    std::cout<<"================================"<<std::endl;
    fixRQ3x3(cam,rot);
    cv::Mat tmp=cam*rot;
    tmp.copyTo(P.rowRange(0,3).colRange(0,3));
//    std::cout<<"================================"<<std::endl;
    cv::solve(cam,P.col(3).rowRange(0,3),trans);


    return true;
}


CameraInfo &CameraInfo::operator=(const CameraInfo &info)
{
	this->_rotation = info._rotation.clone();
	this->_translation = info._translation.clone();
	this->_intrinsic = info._intrinsic.clone();
	return *this;
}

Frustum::Frustum(
	float p_lx, float p_ly, float p_lz, float p_lo,
	float p_rx, float p_ry, float p_rz, float p_ro,
	float p_tx, float p_ty, float p_tz, float p_to,
	float p_bx, float p_by, float p_bz, float p_bo,
	float p_fx, float p_fy, float p_fz, float p_fo):
	lx(p_rx), ly(p_ry), lz(p_rz), lo(p_ro),
	rx(p_rx), ry(p_ry), rz(p_rz), ro(p_ro),
	tx(p_tx), ty(p_ty), tz(p_tz), to(p_to),
	bx(p_bx), by(p_by), bz(p_bz), bo(p_bo),
	fx(p_fx), fy(p_fy), fz(p_fz), fo(p_fo)
{}

Frustum::Frustum(const CameraInfo &info, float w, float h, float far)
{
	cv::Mat rot = info.getRotation();
	cv::Mat trans = info.getTranslation();
	cv::Mat intr = info.getIntrinsic();

	double r11 = rot.at<double>(0,0);
	double r12 = rot.at<double>(0,1);
	double r13 = rot.at<double>(0,2);
	double r21 = rot.at<double>(1,0);
	double r22 = rot.at<double>(1,1);
	double r23 = rot.at<double>(1,2);
	double r31 = rot.at<double>(2,0);
	double r32 = rot.at<double>(2,1);
	double r33 = rot.at<double>(2,2);
	double cx = trans.at<double>(0,0);
	double cy = trans.at<double>(1,0);
	double cz = trans.at<double>(2,0);

	double f = intr.at<double>(0,0);
	double aspect_ratio = intr.at<double>(1,1)/f;
	double ox = intr.at<double>(0,2);
	double oy = intr.at<double>(1,2);

	double xl_intr = -ox; double yt_intr = -oy/aspect_ratio;
	double xr_intr = w-ox; double yb_intr = (h-oy)/aspect_ratio;

	//Cross Product:
	// y1*f  - y2*f
	// x2*f  - x1*f
	// x1*y2 - x2*y1

	//Left
	double nx_intr = (yb_intr-yt_intr)*f;
	double ny_intr = 0.0;
	double nz_intr = (yt_intr-yb_intr)*xl_intr;

	lx = r11*nx_intr + r12*ny_intr + r13*nz_intr;
	ly = r21*nx_intr + r22*ny_intr + r23*nz_intr;
	lz = r31*nx_intr + r32*ny_intr + r33*nz_intr;
	double length = sqrt(lx*lx+ly*ly+lz*lz);
	lx /= length; ly /= length; lz /= length;
	lo = lx*cx + ly*cy + lz*cz;

	//Right
	nx_intr = (yt_intr-yb_intr)*f;
	ny_intr = 0.0;
	nz_intr = (yb_intr-yt_intr)*xr_intr;

	rx = r11*nx_intr + r12*ny_intr + r13*nz_intr;
	ry = r21*nx_intr + r22*ny_intr + r23*nz_intr;
	rz = r31*nx_intr + r32*ny_intr + r33*nz_intr;
	length = sqrt(rx*rx+ry*ry+rz*rz);
	rx /= length; ry /= length; rz /= length;
	ro = rx*cx + ry*cy + rz*cz;

	//Top
	nx_intr = 0.0;
	ny_intr = (xr_intr-xl_intr)*f;
	nz_intr = (xl_intr-xr_intr)*yt_intr;

	tx = r11*nx_intr + r12*ny_intr + r13*nz_intr;
	ty = r21*nx_intr + r22*ny_intr + r23*nz_intr;
	tz = r31*nx_intr + r32*ny_intr + r33*nz_intr;
	length = sqrt(tx*tx+ty*ty+tz*tz);
	tx /= length; ty /= length; tz /= length;
	to = tx*cx + ty*cy + tz*cz;

	//Bottom
	nx_intr = 0.0;
	ny_intr = (xl_intr-xr_intr)*f;
	nz_intr = (xr_intr-xl_intr)*yb_intr;

	bx = r11*nx_intr + r12*ny_intr + r13*nz_intr;
	by = r21*nx_intr + r22*ny_intr + r23*nz_intr;
	bz = r31*nx_intr + r32*ny_intr + r33*nz_intr;
	length = sqrt(bx*bx+by*by+bz*bz);
	bx /= length; by /= length; bz /= length;
	bo = bx*cx + by*cy + bz*cz;

	//Far
	nx_intr = 0.0;
	ny_intr = 0.0;
	nz_intr = -far;

	fx = r11*nx_intr + r12*ny_intr + r13*nz_intr;
	fy = r21*nx_intr + r22*ny_intr + r23*nz_intr;
	fz = r31*nx_intr + r32*ny_intr + r33*nz_intr;
	fo = fx*(cx-fx) + fy*(cy-fy) + fz*(cz-fz);
}





