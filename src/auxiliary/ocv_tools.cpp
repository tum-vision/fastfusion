#include <auxiliary/ocv_tools.h>

std::string type_to_string(cv::Mat m)
{
    if(m.type() == cv::DataType<uchar>::type)
    {
        return "uchar";
    }
    if(m.type() == cv::DataType<ushort>::type)
    {
        return "ushort";
    }
    if(m.type() == cv::DataType<short>::type)
    {
        return "short";
    }
    if(m.type() == cv::DataType<char>::type)
    {
        return "char";
    }
    if(m.type() == cv::DataType<float>::type)
    {
        return "float";
    }
    if(m.type() == cv::DataType<double>::type)
    {
        return "double";
    }
    if(m.type() == cv::DataType<uint>::type)
    {
        return "uint";
    }
    if(m.type() == cv::DataType<int>::type)
    {
        return "int";
    }
    if(m.type() == cv::DataType<cv::Vec3f>::type)
    {
        return "float3";
    }
    return "unknown";

}
