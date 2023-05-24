#ifndef PM_PM_HELPER_H
#define PM_PM_HELPER_H

#include <string>

#include "stb_image.h"
#include <eigen3/Eigen/Core>

namespace pm {

    namespace rgb_8 {

        typedef std::pair<int, int> Range;
        typedef Eigen::Matrix<int, 3, 1> Pixel;
        typedef Eigen::Vector2i Coordinate;
        typedef Eigen::Matrix<Pixel, Eigen::Dynamic, Eigen::Dynamic> ImageMatrix, Patch;

        struct Image {
            int image_width;
            int image_height;
            int image_channels;
            unsigned char* image_data;
        };

        void LoadImage(const char* filename, Image& image);
        void FreeImage(Image& image);

        ImageMatrix GetImageMatrix(Image& image);
        Image GetImage(ImageMatrix& image_matrix);

    } // rgb_8

} // pm

#endif //PM_PM_HELPER_H