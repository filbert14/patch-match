#include "inp.h"

/* -- IMAGE INPAINTING EXAMPLE --
 * Usage: ./inpainting image_A image_A_masked
 *
 * Description:
 * Visualization of patch-based image inpainting by
 * coarse-to-fine gradual resizing,
 * sped up by PatchMatch
 *
 * filbert14
 */

int main(int argc, char* argv[]) {
    std::string A = std::string(argv[1]);
    std::string A_masked = std::string(argv[2]);

    cv::Mat A_mat = pm::LoadImage(A.c_str());
    cv::Mat A_masked_mat = pm::LoadImage(A_masked.c_str());

    pm::ImageMatrix A_matrix = pm::ConvMat2Matrix(A_mat);
    pm::ImageMatrix A_masked_matrix = pm::ConvMat2Matrix(A_masked_mat);

    inp::Inpainting::GetInstance().Initialize(A_matrix, A_masked_matrix);
    std::vector<pm::ImageMatrix> A_inpainted_pyramid = inp::Inpainting::GetInstance().Inpaint();

    for(const auto& A_inpainted : A_inpainted_pyramid) {
        cv::Mat A_inpainted_mat = pm::ConvMatrix2Mat(A_inpainted);
        cv::cvtColor(A_inpainted_mat, A_inpainted_mat, cv::COLOR_RGB2BGR);

        cv::imshow("A_inpainted_mat", A_inpainted_mat);
        cv::waitKey(0);

        if(cv::getWindowProperty("A_inpainted_mat", cv::WND_PROP_AUTOSIZE) == -1) {
            return 0;
        }
    }
}