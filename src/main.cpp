#include "pm.h"

// 09/06/2023
// Sample code that visualizes the process of
// reconstructing image A from the patches of image B
// for the class presentation

int    pm_iters = 5;
size_t patch_radius = 1;
float  alpha = 0.5f;
size_t search_iters = 1;

int main(int, char** argv) {
    std::string A = std::string(argv[1]);
    std::string B = std::string(argv[2]);

    cv::Mat A_mat = pm::LoadImage(A.c_str());
    cv::Mat B_mat = pm::LoadImage(B.c_str());

    pm::ImageMatrix A_matrix = pm::ConvMat2Matrix(A_mat);
    pm::ImageMatrix B_matrix = pm::ConvMat2Matrix(B_mat);

    for(int t = -1; t < pm_iters; ++t) {
        if(t == -1) {
            pm::PatchMatch::GetInstance().Initialize(A_matrix, B_matrix, patch_radius);
        } else {
            pm::PatchMatch::GetInstance().Iterate(t % 2 == 0, alpha, search_iters);
        }

        pm::ImageMatrix A_reconstructed_matrix = pm::PatchMatch::GetInstance().Reconstruct();

        cv::Mat A_reconstructed_mat = pm::ConvMatrix2Mat(A_reconstructed_matrix);
        cv::cvtColor(A_reconstructed_mat, A_reconstructed_mat, cv::COLOR_BGR2RGB);

        cv::imshow("A_reconstructed", A_reconstructed_mat);
        cv::waitKey(0);
    }

    return 0;
}