#ifndef PM_EIGEN_HELPER_H
#define PM_EIGEN_HELPER_H

#include <eigen3/Eigen/Core>

#include "stb_image_helper.h"
#include "pm_types.h"

namespace pm {

    namespace rgb_8 {

        ImageMatrix GetImageMatrix(Image& image) {
            ImageMatrix image_matrix;
            image_matrix.resize(image.image_height, image.image_width);

            for(int p = 0; p < image.image_width * image.image_height * 3; p += 3) {
                unsigned char r = *(image.image_data + p);
                unsigned char g = *(image.image_data + (p + 1));
                unsigned char b = *(image.image_data + (p + 2));

                int i = p / (image.image_width * 3);
                int j = p % (image.image_width);

                image_matrix(i, j) = Pixel {r, g, b};
            }

            return image_matrix;
        }

        Image GetImage(ImageMatrix& image_matrix) {
            unsigned char* image_data = new unsigned char[image_matrix.rows() * image_matrix.cols() * 3];

            for(int p = 0; p < image_matrix.rows() * image_matrix.cols() * 3; p += 3) {
                int i = p / (image_matrix.cols() * 3);
                int j = p % (image_matrix.cols());

                *(image_data + p) = image_matrix(i, j)[0];
                *(image_data + (p + 1)) = image_matrix(i, j)[1];
                *(image_data + (p + 2)) = image_matrix(i, j)[2];
            }

            Image image{static_cast<int>(image_matrix.cols()), static_cast<int>(image_matrix.rows()), 3, image_data};
            return image;
        }

    } // rgb_8

} // pm

#endif //PM_EIGEN_HELPER_H