#include "plywriter.h"


bool validPixel(int i,const cv::Mat& mapped, const cv::Mat& cloud,int x, int y)
{
    bool ok= mapped.at<float>(0,i)>=1.0 && mapped.at<float>(1,i)>=1.0 && mapped.at<float>(0,i) < cloud.cols && mapped.at<float>(1,i)<cloud.rows
            && cloud.at<cv::Vec3f>(y,x)[2]!=10000.0f
            && !std::isinf(cloud.at<cv::Vec3f>(y,x)[0])
            && !std::isinf(cloud.at<cv::Vec3f>(y,x)[1])
            && !std::isinf(cloud.at<cv::Vec3f>(y,x)[2]);
    return ok;
}

//bool writePLYVerticesOnly(const std::string& filename,const cv::Mat& disparityImage,const cv::Mat& img,const cv::Mat& Q,const cv::Mat& rH)
//{
//#define VALID(i,_x,_y) validPixel(i,mapped,_posImage,_x,_y)

//    std::ofstream file(filename.c_str(),std::ios::out | std::ios::binary);

//    if(!file)
//    {
//        std::cerr<<"Can't open file "<<filename<<std::endl;
//        return false;
//    }

//    int i=0;
//    cv::Mat mapped=cv::Mat(3,disparityImage.rows*disparityImage.cols,cv::DataType<float>::type,1.0);

//    if(!rH.empty())
//    {
//        //Only count pixels of disparity image which mapped via rH^(-1) are inside the unrectified image
//        cv::Mat p=cv::Mat(3,disparityImage.rows*disparityImage.cols,cv::DataType<float>::type,1.0);

//        for(float y=0;y<disparityImage.rows;y++)
//        {
//            for(float x=0;x<disparityImage.cols;x++,i++)
//            {
//                p.at<float>(0,i)=x;
//                p.at<float>(1,i)=y;
//            }
//        }
//        cv::Mat rHF;
//        rH.convertTo(rHF,cv::DataType<float>::type);
//        cv::solve(rHF,p,mapped);
//        rHF.release();
//        p.release();
//        for(i=0;i<mapped.cols;i++)
//        {
//            mapped.at<float>(0,i)=mapped.at<float>(0,i)/mapped.at<float>(2,i);
//            mapped.at<float>(1,i)=mapped.at<float>(1,i)/mapped.at<float>(2,i);
//        }
//    }else
//    {
//        mapped.create(2,disparityImage.rows*disparityImage.cols,cv::DataType<float>::type);
//        mapped.setTo(cv::Scalar(1.0));
//    }


//    i=0;

//    cv::Mat _posImage(disparityImage.rows,disparityImage.cols,cv::DataType<cv::Vec3f>::type);
//    //Reproject image point
//    if(type_to_string(disparityImage)!="short" && type_to_string(disparityImage)!="float")
//    {
//        std::cerr<<"Converting disparity image to datatype float"<<std::endl;
//        cv::Mat tmp;
//        disparityImage.convertTo(tmp,CV_32F);
//        cv::reprojectImageTo3D(tmp,_posImage,Q,true);
//    }else
//    {
//        cv::reprojectImageTo3D(disparityImage,_posImage,Q,true);
//    }


////    cv::reprojectImageTo3D(disparityImage,_posImage,Q,true);
////    assert(_posImage.type()==cv::DataType<cv::Vec3f>::type);
//   //std::stringstream verticesBuffer;

//    int counter=0;
//    for(int x=0;x<disparityImage.cols;x++)
//    {
//        for(int y=0;y<disparityImage.rows;y++,i++)
//        {
//            if(validPixel(i,mapped,_posImage,x,y) && i%10==0)//Only those vertices which are valid
//            {
//                counter++;
//            }
//        }
//    }
//    i=0;
//    file << "ply\nformat binary_little_endian 1.0\n"
//    << "element vertex "<< counter << "\nproperty float32 x\nproperty float32 y\nproperty float32 z"
//    << "\nproperty uchar red\nproperty uchar green\nproperty uchar blue\n"
//    << "end_header"<<std::endl;
//    for(int x=0;x<disparityImage.cols;x++)
//    {
//        for(int y=0;y<disparityImage.rows;y++,i++)
//        {
//            if(validPixel(i,mapped,_posImage,x,y) && i%10==0)//Only those vertices which are valid
//            {
//                file.write((char*)&_posImage.at<cv::Vec3f>(y,x),sizeof(float)*3);
//                file.write((char*)&img.at<cv::Vec3b>(y,x),sizeof(char)*3);
//            }
//        }
//    }

//    //Write vertices into file
//    std::cout<<mapped.rows<<std::endl;
//    std::cout<<mapped.cols<<std::endl;

//    file.flush();

//    return true;
//#undef VALID
//}


bool writePLYBinary(const std::string& filename,const cv::Mat& disparityImage,cv::Mat img,const cv::Mat& Q,const cv::Mat& rH)
{
#define VALID(i,_x,_y) validPixel(i,mapped,_posImage,_x,_y)


    std::ofstream file(filename.c_str(),std::ios::binary | std::ios::out);

    if(!file)
    {
        std::cerr<<"Can't open file "<<filename<<std::endl;
        return false;
    }


    int i=0;
    cv::Mat mapped=cv::Mat(3,disparityImage.rows*disparityImage.cols,cv::DataType<float>::type,1.0);

    if(!rH.empty())
    {
        //Only count pixels of disparity image which mapped via rH^(-1) are inside the unrectified image
        cv::Mat p=cv::Mat(3,disparityImage.rows*disparityImage.cols,cv::DataType<float>::type,1.0);

        for(float y=0;y<disparityImage.rows;y++)
        {
            for(float x=0;x<disparityImage.cols;x++,i++)
            {
                p.at<float>(0,i)=x;
                p.at<float>(1,i)=y;
            }
        }
        cv::Mat rHF;
        rH.convertTo(rHF,cv::DataType<float>::type);
        fprintf(stderr,"\nSolving Equation System %ix%i %ix%i %ix%i",rHF.rows,rHF.cols,mapped.rows,mapped.cols,p.rows,p.cols);
        cv::solve(rHF,p,mapped);
        rHF.release();
        p.release();
        for(i=0;i<mapped.cols;i++)
        {
            mapped.at<float>(0,i)=mapped.at<float>(0,i)/mapped.at<float>(2,i);
            mapped.at<float>(1,i)=mapped.at<float>(1,i)/mapped.at<float>(2,i);
        }
    }else
    {
        fprintf(stderr,"\nRectifying Homography is empty");
      mapped.create(2,disparityImage.rows*disparityImage.cols,cv::DataType<float>::type);
      mapped.setTo(cv::Scalar(1.0));
    }


    i=0;
    int noVertices=0;
    int noTriangles=0;

    cv::Mat _posImage(disparityImage.rows,disparityImage.cols,cv::DataType<cv::Vec3f>::type);
    //Reproject image point
    if(type_to_string(disparityImage)!="short" && type_to_string(disparityImage)!="float")
    {
        std::cerr<<"Converting disparity image to datatype float"<<std::endl;
        cv::Mat tmp;
        disparityImage.convertTo(tmp,CV_32F);
        cv::reprojectImageTo3D(tmp,_posImage,Q,true);
    }else
    {
        cv::reprojectImageTo3D(disparityImage,_posImage,Q,true);
    }

    //Write vertices into buffer
    int * indexToVertex = new int[disparityImage.rows*disparityImage.cols];
    memset(indexToVertex,0,disparityImage.rows*disparityImage.cols*sizeof(int));
    float * z=new float[mapped.cols];
    char * vertexBuffer =new char[disparityImage.rows*disparityImage.cols*(4*3+3)];
    for(int x=0;x<disparityImage.cols;x++)
    {
        for(int y=0;y<disparityImage.rows;y++,i++)
        {
            if(validPixel(i,mapped,_posImage,x,y))//Only those vertices which are valid
            {
                char * buffC = vertexBuffer + (noVertices)*(4*3+3) + 4*3;
                float * buffF = (float*) (vertexBuffer + (noVertices)*(4*3+3));
                buffF[0]=_posImage.at<cv::Vec3f>(y,x)[0];
                buffF[1]=_posImage.at<cv::Vec3f>(y,x)[1];
                buffF[2]=_posImage.at<cv::Vec3f>(y,x)[2];
                buffC[0]=img.at<cv::Vec3b>(y,x)[0];
                buffC[1]=img.at<cv::Vec3b>(y,x)[1];
                buffC[2]=img.at<cv::Vec3b>(y,x)[2];
                z[i]=_posImage.at<cv::Vec3f>(y,x)[2];
                indexToVertex[i]=noVertices++; //Keep track of what pixel maps to which vertex
            }
        }
    }

    //Write triangles into a buffer
    i=0;
    char * trianglesBuffer = new char[disparityImage.cols*disparityImage.cols*2*(1+3*4)];
    for(double x=0;x<disparityImage.cols;x++)
    {
        for(double y=0;y<disparityImage.rows;y++,i++)
        {
            if(x<disparityImage.rows-1 && y<disparityImage.cols-1 && VALID(i,x,y)) //Only those vertices which are valid
            {
                const int right_idx=(x+1)*disparityImage.rows + y;
                //first triangle (i,i+1,right_idx+1)
                if(VALID(i+1,x,y+1) && VALID(right_idx+1,x+1,y+1)) //Only those triangles which contain noly valid vertices
                {
                    if(fabs(z[i]-z[i+1])<0.1 && fabs(z[i]-z[right_idx+1])<0.1)
                    {

                        char * buffC= trianglesBuffer +  noTriangles*(1+3*4);
                        int * buffI= (int*)(buffC+1);
                        buffC[0]=3;
                        buffI[0]=indexToVertex[i];
                        buffI[1]=indexToVertex[i+1];
                        buffI[2]=indexToVertex[right_idx+2];
                        if(buffI[0]>=noVertices || buffI[1]>=noVertices || buffI[2]>=noVertices)
                        {
                            std::cout<<"ARRRRGHHHHH "<<buffI[0]<<" "<<buffI[1]<<" "<<buffI[2]<<" "<<noVertices<<" "<<i<<std::endl;
                        }
                        noTriangles++;
                    }
                }
                //second triangle (i,right_idx+1,right_idx)
//                if(VALID(right_idx+1,x+1,y+1) && VALID(right_idx,x+1,y)) //Only those triangles which contain noly valid vertices
//                {
//                    if(fabs(z[right_idx+1]-z[i])<0.1 && fabs(z[right_idx]-z[i])<0.1)
//                    {
//                        char * buffC= trianglesBuffer +  noTriangles*(1+3*4);
//                        int * buffI= (int*)(buffC+1);
//                        buffC[0]=3;
//                        buffI[0]=indexToVertex[i];
//                        buffI[1]=indexToVertex[right_idx+1];
//                        buffI[2]=indexToVertex[right_idx];
//                        noTriangles++;
//                    }
//                }
            }
        }
    }
    delete [] z;
    delete [] indexToVertex;

    file << "ply\nformat binary_little_endian 1.0\n"
    << "element vertex "<< noVertices << "\nproperty float32 x\nproperty float32 y\nproperty float32 z"
    << "\nproperty uchar red\nproperty uchar green\nproperty uchar blue\n"
    << "element face " << noTriangles
    << "\nproperty list uint8 int32 vertex_indices\n"
    << "end_header\n";


    file.write(vertexBuffer,(noVertices)*(sizeof(float)*3+3));
    file.write(trianglesBuffer,(noTriangles)*(1+3*sizeof(int)));
//    noVertices=0;
//    char * buffC = vertexBuffer + (noVertices)*(4*3+3) + 4*3;
//    float * buffF = (float*) (vertexBuffer + (noVertices)*(4*3+3));
//    buffF[0]=100;
//    buffF[1]=0;
//    buffF[2]=0;
//    buffC[0]=255;
//    buffC[1]=0;
//    buffC[2]=0;
//    noVertices++;
//    buffC = vertexBuffer + (noVertices)*(4*3+3) + 4*3;
//    buffF = (float*) (vertexBuffer + (noVertices)*(4*3+3));
//    buffF[0]=0;
//    buffF[1]=100;
//    buffF[2]=0;
//    buffC[0]=0;
//    buffC[1]=0;
//    buffC[2]=255;
//    noVertices++;
//    buffC = vertexBuffer + (noVertices)*(4*3+3) + 4*3;
//    buffF = (float*) (vertexBuffer + (noVertices)*(4*3+3));
//    buffF[0]=0;
//    buffF[1]=0;
//    buffF[2]=100;
//    buffC[0]=0;
//    buffC[1]=255;
//    buffC[2]=0;
//    noVertices++;
//    noTriangles=0;
//    buffC= trianglesBuffer +  noTriangles*(1+3*4);
//    int * buffI= (int*)(buffC+1);
//    buffC[0]=3;
//    buffI[0]=0;
//    buffI[1]=1;
//    buffI[2]=2;
//    noTriangles++;

//    file << "ply\nformat binary_little_endian 1.0\n"
//    << "element vertex "<< 3 << "\nproperty float32 x\nproperty float32 y\nproperty float32 z"
//    << "\nproperty uchar red\nproperty uchar green\nproperty uchar blue\n"
//    << "element face " << 1
//    << "\nproperty list uint8 int32 vertex_indices\n"
//    << "end_header\n";


//    file.write(vertexBuffer,3*(sizeof(float)*3+3));
//    file.write(trianglesBuffer,(1+3*sizeof(int)));



    file.flush();
    file.close();
    delete [] vertexBuffer;
    delete [] trianglesBuffer;
    return true;
#undef VALID
}


bool writePLY(const std::string& filename,const cv::Mat& disparityImage,cv::Mat img,const cv::Mat& Q,const cv::Mat& rH)
{
#define VALID(i,_x,_y) validPixel(i,mapped,_posImage,_x,_y)

    std::ofstream file(filename.c_str());

    if(!file)
    {
        std::cerr<<"Can't open file "<<filename<<std::endl;
        return false;
    }


    int i=0;
    cv::Mat mapped=cv::Mat(3,disparityImage.rows*disparityImage.cols,cv::DataType<float>::type,1.0);

    if(!rH.empty())
    {
        //Only count pixels of disparity image which mapped via rH^(-1) are inside the unrectified image
        cv::Mat p=cv::Mat(3,disparityImage.rows*disparityImage.cols,cv::DataType<float>::type,1.0);

        for(float y=0;y<disparityImage.rows;y++)
        {
            for(float x=0;x<disparityImage.cols;x++,i++)
            {
                p.at<float>(0,i)=x;
                p.at<float>(1,i)=y;
            }
        }
        cv::Mat rHF;
        rH.convertTo(rHF,cv::DataType<float>::type);
        fprintf(stderr,"\nSolving Equation System %ix%i %ix%i %ix%i",rHF.rows,rHF.cols,mapped.rows,mapped.cols,p.rows,p.cols);
        cv::solve(rHF,p,mapped);
        rHF.release();
        p.release();
        for(i=0;i<mapped.cols;i++)
        {
            mapped.at<float>(0,i)=mapped.at<float>(0,i)/mapped.at<float>(2,i);
            mapped.at<float>(1,i)=mapped.at<float>(1,i)/mapped.at<float>(2,i);
        }
    }else
    {
    	fprintf(stderr,"\nRectifying Homography is empty");
      mapped.create(2,disparityImage.rows*disparityImage.cols,cv::DataType<float>::type);
      mapped.setTo(cv::Scalar(1.0));
    }


    i=0;
    int noVertices=0;
    int noTriangles=0;

    cv::Mat _posImage(disparityImage.rows,disparityImage.cols,cv::DataType<cv::Vec3f>::type);
    //Reproject image point
    if(type_to_string(disparityImage)!="short" && type_to_string(disparityImage)!="float")
    {
        std::cerr<<"Converting disparity image to datatype float"<<std::endl;
        cv::Mat tmp;
        disparityImage.convertTo(tmp,CV_32F);
        cv::reprojectImageTo3D(tmp,_posImage,Q,true);
    }else
    {
        cv::reprojectImageTo3D(disparityImage,_posImage,Q,true);
    }
    std::stringstream verticesBuffer;
    std::stringstream trianglesBuffer;

    //Write vertices into buffer
    int * indexToVertex = new int[disparityImage.rows*disparityImage.cols];
    std::cout<<mapped.rows<<std::endl;
    std::cout<<mapped.cols<<std::endl;
    float * z=new float[mapped.cols];

    for(int x=0;x<disparityImage.cols;x++)
    {
        for(int y=0;y<disparityImage.rows;y++,i++)
        {
            if(validPixel(i,mapped,_posImage,x,y))//Only those vertices which are valid
            {
                verticesBuffer<<_posImage.at<cv::Vec3f>(y,x)[0]<<" "<<_posImage.at<cv::Vec3f>(y,x)[1]<<" "<<_posImage.at<cv::Vec3f>(y,x)[2]<<
                                " "<<(int)img.at<cv::Vec3b>(y,x)[0]<<" "<<(int)img.at<cv::Vec3b>(y,x)[1]<<" "<<(int)img.at<cv::Vec3b>(y,x)[2]<<std::endl;
                z[i]=_posImage.at<cv::Vec3f>(y,x)[2];
                indexToVertex[i]=noVertices++; //Keep track of what pixel maps to which vertex
            }
        }
    }

    //Write triangles into a buffer
    i=0;
    bool first=true;
    for(double x=0;x<disparityImage.cols;x++)
    {
        for(double y=0;y<disparityImage.rows;y++,i++)
        {
            if(x<disparityImage.rows-1 && y<disparityImage.cols-1 && VALID(i,x,y)) //Only those vertices which are valid
            {
                const int right_idx=(x+1)*disparityImage.rows + y;
                //first triangle (i,i+1,right_idx+1)
                if(VALID(i+1,x,y+1) && VALID(right_idx+1,x+1,y+1)) //Only those triangles which contain noly valid vertices
                {
                    if(fabs(z[i]-z[i+1])<0.1 && fabs(z[i]-z[right_idx+1])<0.1)
                    {
                        noTriangles++;
                        if(!first)
                        {
                            trianglesBuffer<<std::endl;
                        }else
                        {
                            first=false;
                        }
                        trianglesBuffer<<"3 "<<indexToVertex[i]<<" "<<indexToVertex[i+1]<<" "<<indexToVertex[right_idx+1];
                    }
                }
                //second triangle (i,right_idx+1,right_idx)
                if(VALID(right_idx+1,x+1,y+1) && VALID(right_idx,x+1,y)) //Only those triangles which contain noly valid vertices
                {
                    if(fabs(z[right_idx+1]-z[i])<0.1 && fabs(z[right_idx]-z[i])<0.1)
                    {
                        noTriangles++;
                        if(!first)
                        {
                            trianglesBuffer<<std::endl;
                        }else
                        {
                            first=false;
                        }
                        trianglesBuffer<<"3 "<<indexToVertex[i]<<" "<<indexToVertex[right_idx+1]<<" "<<indexToVertex[right_idx];
                    }
                }
            }
        }
    }
    delete [] z;
    delete [] indexToVertex;

    //Write PLY header
    file << "ply\nformat ascii 1.0\nelement vertex "
         << noVertices << "\nproperty float x\nproperty float y\nproperty float z";

    file << "\nproperty uchar red\nproperty uchar green\nproperty uchar blue";

    file << "\nelement face " << noTriangles
         << "\nproperty list uint8 int32 vertex_indices\nend_header\n";

    //Write vetices buffer to file
    file<<verticesBuffer.str();
    file.flush();
    //Write triangles buffer to file
    file<<trianglesBuffer.str();
    file.flush();
    file.close();
    return true;
#undef VALID
}



