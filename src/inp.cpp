#include "inp.h"

namespace inp {

    void Inpainting::Initialize(ImageMatrix& A, ImageMatrix& A_mask) {
        A_ = A;
        A_pyramid_ = ConstructPyramid(A_);

        A_mask_ = A_mask;
        A_mask_pyramid_ = ConstructPyramid(A_mask_);
    }

    std::vector<ImageMatrix> Inpainting::ConstructPyramid(ImageMatrix& I) {
        int l = 2 * PATCH_RADIUS + 1;

        std::vector<ImageMatrix> I_pyramid;
        I_pyramid.push_back(I);

        int min_dim = std::min(I.rows(), I.cols());
        while(min_dim > l) {
            ImageMatrix& I_current = I_pyramid.back();
            cv::Mat I_downsampled_mat = ConvMatrix2Mat(I_current);
            cv::resize(I_downsampled_mat, I_downsampled_mat, cv::Size(), 0.5f, 0.5f, cv::INTER_NEAREST);
            I_pyramid.push_back(ConvMat2Matrix(I_downsampled_mat));
            min_dim = std::min(I_pyramid.back().rows(), I_pyramid.back().cols());
        }

        I_pyramid.pop_back();
        std::reverse(I_pyramid.begin(), I_pyramid.end());

        return I_pyramid;
    }

    ImageMatrix Inpainting::ThresholdImageMask(ImageMatrix& I_mask, Pixel lower_range, Pixel upper_range) {
        cv::Mat I_mask_rgb = ConvMatrix2Mat(I_mask);
        cv::Mat I_mask_hsv; cv::cvtColor(I_mask_rgb, I_mask_hsv, cv::COLOR_RGB2HSV);

        cv::Scalar l {(double) lower_range(0), (double) lower_range(1), (double) lower_range(2)};
        cv::Scalar u {(double) upper_range(0), (double) upper_range(1), (double) upper_range(2)};
        cv::Mat mask; cv::inRange(I_mask_hsv, l, u, mask);

        cv::Mat I_mask_thresholded; cv::bitwise_and(I_mask_rgb, I_mask_rgb, I_mask_thresholded, mask);
        ImageMatrix I_mask_thresholded_matrix = ConvMat2Matrix(I_mask_thresholded);

        return I_mask_thresholded_matrix;
    }

    PatchMap Inpainting::GetPatchMap(ImageMatrix& I_mask) {
        PatchMap patch_map;
        patch_map.resize(I_mask.rows(), I_mask.cols());
        patch_map.fill(false);

        for(int i = 0; i < I_mask.rows(); ++i) {
            for(int j = 0; j < I_mask.cols(); ++j) {
                if(I_mask(i, j) != Pixel {0, 0, 0}) {
                    patch_map(i, j) = true;
                }
            }
        }

        return patch_map;
    }

    std::vector<ImageMatrix> Inpainting::Inpaint() {
        for(int level = 0; level < A_pyramid_.size(); ++level) {
            ImageMatrix& A = A_pyramid_[level];
            ImageMatrix& A_mask = A_mask_pyramid_[level];

            ImageMatrix A_mask_thresholded = ThresholdImageMask(A_mask);
            PatchMap patch_map = GetPatchMap(A_mask_thresholded);

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