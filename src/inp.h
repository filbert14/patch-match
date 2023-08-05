#ifndef PM_INP_H
#define PM_INP_H

#include <limits.h>

#include "pm.h"

namespace inp {

    using namespace pm;

    // Default hole color
    // https://stackoverflow.com/questions/22588146/tracking-white-color-using-python-opencv
    const int SENSITIVITY = 180;
    const Pixel WHITE_HSV_LOWER_RANGE = {0, 0, 255 - SENSITIVITY};
    const Pixel WHITE_HSV_UPPER_RANGE = {255, SENSITIVITY, 255};

    // Inpainting (by coarse-to-fine gradual resizing)
    // Represents an instance of the inpainting algorithm
    class Inpainting {
    public:
        static Inpainting& GetInstance() {
            static Inpainting inp;
            return inp;
        }

    private:
        Inpainting() {}
        Inpainting(const Inpainting&) = delete;
        void operator = (const Inpainting&) = delete;

    // Helper functions
    public:
        void Initialize(ImageMatrix& A, ImageMatrix& A_mask);
        std::vector<ImageMatrix> ConstructPyramid(ImageMatrix& I);
        ImageMatrix ThresholdImageMask(ImageMatrix& I_mask, Pixel lower_range = WHITE_HSV_LOWER_RANGE, Pixel upper_range = WHITE_HSV_UPPER_RANGE);
        PatchMap GetPatchMap(ImageMatrix& I_mask);

    // Core algorithm
    public:
        std::vector<ImageMatrix> Inpaint();

    private:
        ImageMatrix A_;
        std::vector<ImageMatrix> A_pyramid_;

        ImageMatrix A_mask_;
        std::vector<ImageMatrix> A_mask_pyramid_;
    };

} // inp

#endif //PM_INP_H