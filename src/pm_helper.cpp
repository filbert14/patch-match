#include "pm_helper.h"

namespace pm {

    cv::Mat LoadImage(const char* image) {
        cv::Mat image_mat = cv::imread(image, cv::IMREAD_COLOR);
        cv::cvtColor(image_mat, image_mat, cv::COLOR_BGR2RGB);
        return image_mat;
    }

    ImageMatrix ConvMat2Matrix(const cv::Mat& image_mat) {
        ImageMatrix image_matrix;
        image_matrix.resize(image_mat.rows, image_mat.cols);

        for(int i = 0; i < image_matrix.rows(); ++i) {
            for(int j = 0; j < image_matrix.cols(); ++j) {
                cv::Vec3b pixel = image_mat.at<cv::Vec3b>(i, j);
                image_matrix(i, j) = {pixel[0], pixel[1], pixel[2]};
            }
        }

        return image_matrix;
    }

    cv::Mat ConvMatrix2Mat(const ImageMatrix& image_matrix) {
        cv::Mat image_mat(static_cast<int>(image_matrix.rows()), static_cast<int>(image_matrix.cols()), CV_8UC3);

        for(int i = 0; i < image_mat.rows; ++i) {
            for(int j = 0; j < image_mat.cols; ++j) {
                image_mat.at<cv::Vec3b>(i, j) = {static_cast<unsigned char>(image_matrix(i,j)[0]),
                                                        static_cast<unsigned char>(image_matrix(i,j)[1]),
                                                        static_cast<unsigned char>(image_matrix(i,j)[2])};
            }
        }

        return image_mat;
    }

} // pm