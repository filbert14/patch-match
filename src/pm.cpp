#include "pm.h"

namespace pm {

    namespace rgb_8 {

        float PatchMatch::CalculateDistance(Coordinate& a, Coordinate& b) {
            int l = 2 * patch_radius_ + 1;

            Patch at_a = A_padded_.block(a[0], a[1], l, l);
            Patch at_b = B_.block(b[0] - patch_radius_, b[1] - patch_radius_, l, l);

            float mse {0};
            int count {0};

            for(int i = 0; i < at_a.rows(); ++i) {
                for(int j = 0; j < at_a.cols(); ++j) {
                    if(!valid_(a[0] + i, a[1] + j)) { continue; }
                    float y_at_a = 0.2126 * at_a(i, j)[0] + 0.7152 * at_a(i, j)[1] + 0.0722 * at_a(i, j)[2];
                    float y_at_b = 0.2126 * at_b(i, j)[0] + 0.7152 * at_b(i, j)[1] + 0.0722 * at_b(i, j)[2];

                    mse += (y_at_b - y_at_a) * (y_at_b - y_at_a);
                    ++count;
                }
            }

            mse /= count;

            return mse;
        }

        void PatchMatch::Initialize(ImageMatrix& A, ImageMatrix& B, int patch_radius) {
            A_ = A;
            B_ = B;
            patch_radius_ = patch_radius;

            rcg_.Seed();

            A_padded_.resize(A_.rows() + 2 * patch_radius_, A_.cols() + 2 * patch_radius_);
            valid_.resize(A_padded_.rows(), A_padded_.cols());

            nnf_.resize(A_.rows(), A_.cols());
            distance_.resize(A_.rows(), A_.cols());

            A_padded_.fill({0, 0, 0});
            valid_.fill(false);
            A_padded_.block(patch_radius_, patch_radius_, A_.rows(), A_.cols()) = A_;
            valid_.block(patch_radius_, patch_radius_, A_.rows(), A_.cols()).fill(true);

            for(int i = 0; i < A_.rows(); ++i) {
                for(int j = 0; j < A_.cols(); ++j) {
                    Coordinate a = {i, j};
                    Coordinate b = rcg_.GenerateCoordinate({patch_radius_, B_.rows() - patch_radius_ - 1},
                                                           {patch_radius_, B_.cols() - patch_radius_ - 1});
                    nnf_(i, j) = b;
                    distance_(i, j) = CalculateDistance(a, b);
                }
            }
        }

        ImageMatrix PatchMatch::Reconstruct() {
            ImageMatrix A_reconstructed;
            A_reconstructed.resize(A_.rows(), A_.cols());
            for(int i = 0; i < A_.rows(); ++i) {
                for(int j = 0; j < A_.cols(); ++j) {
                    A_reconstructed(i,j) = B_(nnf_(i,j)[0], nnf_(i,j)[1]);
                }
            }
            return A_reconstructed;
        }

    } // rgb_8

} // pm