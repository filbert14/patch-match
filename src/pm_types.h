#ifndef PM_PM_TYPES_H
#define PM_PM_TYPES_H

#include <eigen3/Eigen/Core>

namespace pm {

    namespace rgb_8 {

        typedef Eigen::Matrix<unsigned char, 3, 1> Pixel;
        typedef Eigen::Matrix<Pixel, Eigen::Dynamic, Eigen::Dynamic> ImageMatrix;

        struct Image {
            int image_width;
            int image_height;
            int image_channels;
            unsigned char* image_data;
        };

    } // rgb_8

} // pm

#endif //PM_PM_TYPES_H