#ifndef PM_INP_H
#define PM_INP_H

#include <limits.h>

#include "pm.h"

namespace inp {

    using namespace pm;

    // Default hole color
    const int SENSITIVITY = 15;
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
        void Initialize(ImageMatrix& A, ImageMatrix& A_masked);
        std::vector<ImageMatrix> ConstructPyramid(ImageMatrix& I);
        ImageMatrix ThresholdMaskedImage(ImageMatrix& I, Pixel lower_range = WHITE_HSV_LOWER_RANGE, Pixel upper_range = WHITE_HSV_UPPER_RANGE);
        PatchMap GetPatchMap(ImageMatrix& I_masked);

    // Core algorithm
    public:
        std::vector<ImageMatrix> Inpaint();

    private:
        ImageMatrix A_;
        std::vector<ImageMatrix> A_pyramid_;

        ImageMatrix A_masked_;
        std::vector<ImageMatrix> A_masked_pyramid_;
    };

} // inp

#endif //PM_INP_H