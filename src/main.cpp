#include "pm.h"
#include "pm_helper.h"

int main(int, char** argv) {
    std::string A = std::string(argv[1]);
    std::string B = std::string(argv[2]);

    cv::Mat A_mat = pm::LoadImage(A.c_str());
    cv::Mat B_mat = pm::LoadImage(B.c_str());

    pm::ImageMatrix A_matrix = pm::ConvMat2Matrix(A_mat);
    pm::ImageMatrix B_matrix = pm::ConvMat2Matrix(B_mat);

    pm::PatchMatch::GetInstance().Initialize(A_matrix, B_matrix, 1);
    pm::PatchMatch::GetInstance().ApproximateNNF(4, 0.5, 1);

    pm::ImageMatrix A_reconstructed_matrix = pm::PatchMatch::GetInstance().Reconstruct();
    cv::Mat A_reconstructed_mat = pm::ConvMatrix2Mat(A_reconstructed_matrix);
    cv::cvtColor(A_reconstructed_mat, A_reconstructed_mat, cv::COLOR_BGR2RGB);

    cv::imshow("A_reconstructed", A_reconstructed_mat);
    cv::waitKey(0);

    return 0;
}