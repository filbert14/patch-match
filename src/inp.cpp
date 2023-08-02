#include "inp.h"

namespace inp {

    void Inpainting::Initialize(ImageMatrix& A, ImageMatrix& A_masked) {
        A_ = A;
        A_pyramid_ = ConstructPyramid(A_);

        A_masked_ = A_masked;
        A_masked_pyramid_ = ConstructPyramid(A_masked_);
    }

    std::vector<ImageMatrix> Inpainting::ConstructPyramid(ImageMatrix& I) {
        int l = 2 * PATCH_RADIUS + 1;

        std::vector<ImageMatrix> I_pyramid;
        I_pyramid.push_back(I);

        int min_dim = std::min(I.rows(), I.cols());
        while(min_dim > l) {
            ImageMatrix& I_current = I_pyramid.back();
            cv::Mat I_current_mat = ConvMatrix2Mat(I_current);

            cv::pyrDown(I_current_mat, I_current_mat, {I_current_mat.cols / 2, I_current_mat.rows / 2});
            I_pyramid.push_back(ConvMat2Matrix(I_current_mat));

            min_dim = std::min(I_pyramid.back().rows(), I_pyramid.back().cols());
        }

        I_pyramid.pop_back();
        std::reverse(I_pyramid.begin(), I_pyramid.end());

        return I_pyramid;
    }

    ImageMatrix Inpainting::ThresholdMaskedImage(ImageMatrix& I, Pixel lower_range, Pixel upper_range) {
        cv::Mat I_rgb = ConvMatrix2Mat(I);
        cv::Mat I_hsv; cv::cvtColor(I_rgb, I_hsv, cv::COLOR_RGB2HSV);

        cv::Scalar l {(double) lower_range(0), (double) lower_range(1), (double) lower_range(2)};
        cv::Scalar u {(double) upper_range(0), (double) upper_range(1), (double) upper_range(2)};
        cv::Mat mask; cv::inRange(I_hsv, l, u, mask);

        cv::Mat I_thresholded; cv::bitwise_and(I_rgb, I_rgb, I_thresholded, mask);
        ImageMatrix I_thresholded_matrix = ConvMat2Matrix(I_thresholded);

        return I_thresholded_matrix;
    }

    PatchMap Inpainting::GetPatchMap(ImageMatrix& I_masked) {
        PatchMap patch_map;
        patch_map.resize(I_masked.rows(), I_masked.cols());
        patch_map.fill(false);

        for(int i = 0; i < I_masked.rows(); ++i) {
            for(int j = 0; j < I_masked.cols(); ++j) {
                if(I_masked(i, j) != Pixel {0, 0, 0}) {
                    patch_map(i, j) = true;
                }
            }
        }

        return patch_map;
    }

    std::vector<ImageMatrix> Inpainting::Inpaint() {
        for(int level = 0; level < A_pyramid_.size(); ++level) {
            ImageMatrix& A = A_pyramid_[level];
            ImageMatrix& A_masked = A_masked_pyramid_[level];

            ImageMatrix A_masked_thresholded = ThresholdMaskedImage(A_masked);
            PatchMap patch_map = GetPatchMap(A_masked_thresholded);

            ImageMatrix B = A;
            for(int i = 0; i < B.rows(); ++i) {
                for(int j = 0; j < B.cols(); ++j) {
                    if(patch_map(i, j)) {
                        B(i, j) = {INT_MAX, INT_MAX, INT_MAX};
                    }
                }
            }

            for(int i = 0; i < A.rows(); ++i) {
                for(int j = 0; j < A.cols(); ++j) {
                    if(patch_map(i, j)) {
                        if(level == 0) {
                            cv::Mat A_mat = ConvMatrix2Mat(A);
                            cv::pyrDown(A_mat, A_mat, {A_mat.cols / 2, A_mat.rows / 2});
                            cv::Vec3b pixel = A_mat.at<cv::Vec3b>(0, 0);
                            A(i, j) = {pixel(0), pixel(1), pixel(2)};
                        } else {
                            A(i, j) = A_pyramid_[level - 1](i / 2, j / 2);
                        }
                    }
                }
            }

            PatchMatch::GetInstance().Initialize(A, B);
            PatchMatch::GetInstance().ApproximateNNF();
            A_pyramid_[level] = PatchMatch::GetInstance().PatchUp(patch_map);
        }

        return A_pyramid_;
    }

} // inp