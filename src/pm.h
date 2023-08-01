#ifndef PM_PM_H
#define PM_PM_H

#include <random>

#include "pm_helper.h"

namespace pm {

    // Default parameters for PatchMatch
    const int    PM_ITERS = 5;
    const size_t PATCH_RADIUS = 1;
    const float  ALPHA = 0.5f;
    const size_t SEARCH_ITERS = 1;

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
        void Initialize(ImageMatrix& A, ImageMatrix& B, size_t patch_radius = PATCH_RADIUS);
        void Propagate(Coordinate& a, bool even);
        void RandomSearch(Coordinate& a, float alpha = ALPHA, size_t search_iters = SEARCH_ITERS);

    // Helper functions
    public:
        void Iterate(bool even, float alpha = ALPHA, size_t search_iters = SEARCH_ITERS);
        void ApproximateNNF(size_t pm_iters = PM_ITERS, float alpha = ALPHA, size_t search_iters = SEARCH_ITERS);
        float CalculateDistance(Coordinate& a, Coordinate& b);

    // Miscellaneous functions
    public:
        ImageMatrix PatchUp(PatchMap& patch_map);

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