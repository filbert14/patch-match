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

        void PatchMatch::Propagate(Coordinate& a, bool even) {
            int i = a[0];
            int j = a[1];

            if(even) {
                Coordinate right = {i, j == A_.cols() - 1 ? A_.cols() - 1 : j + 1};
                Coordinate down = {i == A_.rows() - 1 ? A_.rows() - 1 : i + 1, j};

                float distance_current = distance_(i, j);
                float distance_right = distance_(right[0], right[1]);
                float distance_down = distance_(down[0], down[1]);

                std::vector<float> d = {distance_current, distance_right, distance_down};
                int v = std::distance(std::begin(d), std::min_element(std::begin(d), std::end(d)));

                if(v == 1) {
                    nnf_(i, j) = nnf_(right[0], right[1]);
                    distance_(i, j) = CalculateDistance(a, nnf_(i, j));
                } else if(v == 2) {
                    nnf_(i, j) = nnf_(down[0], down[1]);
                    distance_(i, j) = CalculateDistance(a, nnf_(i,j));
                }
            }

            else {
                Coordinate left = {i, j == 0 ? 0 : j - 1};
                Coordinate up = {i == 0 ? 0 : i - 1, j};

                float distance_current = distance_(i, j);
                float distance_left = distance_(left[0], left[1]);
                float distance_up = distance_(up[0], up[1]);

                std::vector<float> d = {distance_current, distance_left, distance_up};
                int v = std::distance(std::begin(d), std::min_element(std::begin(d), std::end(d)));

                if(v == 1) {
                    nnf_(i, j) = nnf_(left[0], left[1]);
                    distance_(i, j) = CalculateDistance(a, nnf_(i, j));
                } else if(v == 2) {
                    nnf_(i, j) = nnf_(up[0], up[1]);
                    distance_(i, j) = CalculateDistance(a, nnf_(i, j));
                }
            }
        }

        void PatchMatch::RandomSearch(Coordinate& a, float alpha, size_t iters) {
            int i = a[0];
            int j = a[1];

            int search_radius_rows = B_.rows();
            int search_radius_cols = B_.cols();
            int k = 0;

            Coordinate b = nnf_(i, j);

            while(search_radius_rows > 0 && search_radius_cols > 0) {
                search_radius_rows *= std::pow(alpha, k);
                search_radius_cols *= std::pow(alpha, k);
                ++k;

                Range search_row_range = {b[0] - search_radius_rows < patch_radius_ ? patch_radius_ : b[0] - search_radius_rows,
                                          b[0] + search_radius_rows > B_.rows() - patch_radius_ - 1 ? B_.rows() - patch_radius_ - 1 : b[0] + search_radius_rows};
                Range search_col_range = {b[1] - search_radius_cols < patch_radius_ ? patch_radius_ : b[1] - search_radius_cols,
                                          b[1] + search_radius_cols > B_.cols() - patch_radius_ - 1 ? B_.cols() - patch_radius_ - 1 : b[1] + search_radius_cols};

                for(int t = 0; t < iters; ++t) {
                    Coordinate b_rand = rcg_.GenerateCoordinate(search_row_range, search_col_range);

                    if(float distance = CalculateDistance(a, b_rand); distance < distance_(i, j)) {
                        nnf_(i, j) = b_rand;
                        distance_(i, j) = distance;
                    }
                }
            }
        }

        void PatchMatch::ApproximateNNF(size_t pm_iters, float alpha, size_t search_iters) {
            for(int t = 0; t < pm_iters; ++t) {
                if(t % 2 == 0) {
                    for(int i = 0; i < A_.rows(); ++i) {
                        for(int j = 0; j < A_.cols(); ++j) {
                            Coordinate a {i, j};
                            Propagate(a, true);
                            RandomSearch(a, alpha, search_iters);
                        }
                    }
                } else {
                    for(int i = A_.rows() - 1; i > -1; --i) {
                        for(int j = A_.cols() - 1; j > -1; --j) {
                            Coordinate a {i, j};
                            Propagate(a, false);
                            RandomSearch(a, alpha, search_iters);
                        }
                    }
                }
            }
        }

    } // rgb_8

} // pm