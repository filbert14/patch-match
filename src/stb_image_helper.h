#ifndef PM_STB_IMAGE_HELPER_H
#define PM_STB_IMAGE_HELPER_H

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "pm.h"

namespace pm {

    void LoadImageRGB(const char* filename, Image& image) {
        int image_width, image_height, image_channels;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, &image_channels, STBI_rgb);

        // @TODO: Do a check here to see if the image has indeed been successfully loaded

        image.image_width = image_width;
        image.image_height = image_height;
        image.image_channels = image_channels;
        image.image_data = image_data;
    }

    // @TODO: Add a function to convert grayscale data to its RGB representation

    void FreeImage(Image& image) {
        stbi_image_free(image.image_data);
    }

} // pm

#endif //PM_STB_IMAGE_HELPER_H