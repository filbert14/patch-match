#include "pm.h"

/* -- IMAGE RECONSTRUCTION EXAMPLE --
 * Usage: ./reconstruction image_A image_B
 *
 * Description:
 * Visualizes step by step reconstruction
 * of image_A using patches from image_B
 *
 * filbert14
 */

int main(int argc, char* argv[]) {
    std::string A = std::string(argv[1]);
    std::string B = std::string(argv[2]);

    cv::Mat A_mat = pm::LoadImage(A.c_str());
    cv::Mat B_mat = pm::LoadImage(B.c_str());

    pm::ImageMatrix A_matrix = pm::ConvMat2Matrix(A_mat);
    pm::ImageMatrix B_matrix = pm::ConvMat2Matrix(B_mat);

    pm::PatchMap patch_map;
    patch_map.resize(A_matrix.rows(), A_matrix.cols());
    patch_map.fill(true);

    for(int t = -1; t < pm::PM_ITERS; ++t) {
        // Random initialization
        if(t == -1) {
            pm::PatchMatch::GetInstance().Initialize(A_matrix, B_matrix);
        // Iterative refinement
        } else {
            pm::PatchMatch::GetInstance().Iterate(t % 2 == 0);
        }

        pm::ImageMatrix A_reconstructed_matrix = pm::PatchMatch::GetInstance().PatchUp(patch_map);

        cv::Mat A_reconstructed_mat = pm::ConvMatrix2Mat(A_reconstructed_matrix);
        cv::cvtColor(A_reconstructed_mat, A_reconstructed_mat, cv::COLOR_RGB2BGR);

        cv::imshow("A_reconstructed", A_reconstructed_mat);
        cv::waitKey(0);

        if(cv::getWindowProperty("A_reconstructed", cv::WND_PROP_AUTOSIZE) == -1) {
            return 0;
        }
    }

    return 0;
}