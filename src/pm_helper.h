#ifndef PM_PM_HELPER_H
#define PM_PM_HELPER_H

#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>

namespace pm {

    // Type definitions
    typedef Eigen::Vector3i Pixel;
    typedef Eigen::Matrix<Pixel, Eigen::Dynamic, Eigen::Dynamic> ImageMatrix;

    cv::Mat LoadImage(const char* image);

    ImageMatrix ConvMat2Matrix(const cv::Mat& image_mat);
    cv::Mat ConvMatrix2Mat(const ImageMatrix& image_matrix);

} // pm

#endif //PM_PM_HELPER_H