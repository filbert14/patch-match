#ifndef PM_PM_H
#define PM_PM_H

#include <random>

#include "pm_helper.h"

namespace pm {

    // RandomCoordinateGenerator
    // Generates a random coordinate within a given range
    // using a Mersenne Twister pseudo-random generator
    class RandomCoordinateGenerator {
    public:
        void Seed() {
            gen.seed(std::random_device()());
        }

        Coordinate GenerateCoordinate(Range row_range, Range col_range) {
            std::uniform_int_distribution<int> row_distr {row_range.first, row_range.second};
            std::uniform_int_distribution<int> col_distr {col_range.first, col_range.second};
            return {row_distr(gen), col_distr(gen)};
        }

    private:
        std::mt19937 gen;
    };

    // PatchMatch
    // Represents an instance of the PatchMatch algorithm
    class PatchMatch {
    public:
        static PatchMatch& GetInstance() {
            static PatchMatch pm;
            return pm;
        }

    private:
        PatchMatch() {}
        PatchMatch(const PatchMatch&) = delete;
        void operator = (const PatchMatch&) = delete;

    // Core algorithm
    public:
        void Initialize(ImageMatrix& A, ImageMatrix& B, size_t patch_radius);
        void Propagate(Coordinate& a, bool even);
        void RandomSearch(Coordinate& a, float alpha, size_t search_iters);

    public:
        void Iterate(bool even, float alpha, size_t search_iters);
        float CalculateDistance(Coordinate& a, Coordinate& b);
        ImageMatrix Reconstruct();

    // Internal data structures
    private:
        ImageMatrix A_;
        ImageMatrix B_;
        ImageMatrix A_padded_;

        Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> valid_;
        Eigen::Matrix<Coordinate, Eigen::Dynamic, Eigen::Dynamic> nnf_;
        Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> distance_;

        int patch_radius_;

        RandomCoordinateGenerator rcg_;
    };

} // pm

#endif //PM_PM_H