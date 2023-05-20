#ifndef PM_EIGEN_HELPER_H
#define PM_EIGEN_HELPER_H

#include <eigen3/Eigen/Core>

#include "stb_image_helper.h"

namespace pm {

    typedef Eigen::Matrix<unsigned char, 3, 1> PixelRGB8;
    typedef Eigen::Matrix<PixelRGB8, Eigen::Dynamic, Eigen::Dynamic> ImageMatrixRGB8;

    ImageMatrixRGB8 GetImageMatrixRGB8(ImageRGB8& image) {
        ImageMatrixRGB8 image_matrix;
        image_matrix.resize(image.image_height, image.image_width);

        for(int p = 0; p < image.image_width * image.image_height * 3; p += 3) {
            unsigned char r = *(image.image_data + p);
            unsigned char g = *(image.image_data + (p + 1));
            unsigned char b = *(image.image_data + (p + 2));

            int i = p / (image.image_width * 3);
            int j = p % (image.image_width);

            image_matrix(i, j) = PixelRGB8 {r, g, b};
        }

        return image_matrix;
    }

    ImageRGB8 GetImageRGB8(ImageMatrixRGB8& image_matrix) {
        unsigned char* image_data = new unsigned char[image_matrix.rows() * image_matrix.cols() * 3];

        for(int p = 0; p < image_matrix.rows() * image_matrix.cols() * 3; p += 3) {
            int i = p / (image_matrix.cols() * 3);
            int j = p % (image_matrix.cols());

            *(image_data + p) = image_matrix(i,j)[0];
            *(image_data + (p + 1)) = image_matrix(i, j)[1];
            *(image_data + (p + 2)) = image_matrix(i, j)[2];
        }

        ImageRGB8 image {image_matrix.cols(), image_matrix.rows(), 3, image_data};
        return image;
    }

} // pm

#endif //PM_EIGEN_HELPER_H