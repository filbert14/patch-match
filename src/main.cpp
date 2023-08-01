#include "pm.h"

int main(int, char** argv) {
    std::string A = std::string(argv[1]);
    std::string B = std::string(argv[2]);

    cv::Mat A_mat = pm::LoadImage(A.c_str());
    cv::Mat B_mat = pm::LoadImage(B.c_str());

    pm::ImageMatrix A_matrix = pm::ConvMat2Matrix(A_mat);
    pm::ImageMatrix B_matrix = pm::ConvMat2Matrix(B_mat);

    pm::PatchMatch::GetInstance().Initialize(A_matrix, B_matrix);
    pm::PatchMatch::GetInstance().ApproximateNNF();

    pm::PatchMap patch_map;
    patch_map.resize(A_matrix.rows(), A_matrix.cols());
    patch_map.fill(true);

    pm::ImageMatrix A_reconstructed_matrix = pm::PatchMatch::GetInstance().PatchUp(patch_map);
    cv::Mat A_reconstructed_mat = pm::ConvMatrix2Mat(A_reconstructed_matrix);

    cv::cvtColor(A_reconstructed_mat, A_reconstructed_mat, cv::COLOR_BGR2RGB);
    cv::imshow("A_reconstructed", A_reconstructed_mat);
    cv::waitKey(0);

    return 0;
}