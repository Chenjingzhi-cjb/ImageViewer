#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <opencv2/opencv.hpp>


class ImageProcessor {
public:
    ImageProcessor() = default;

    ~ImageProcessor() = default;

public:
    static void bgrToHsi(cv::Mat &src, cv::Mat &dst) {
        cv::Mat temp(src.size(), src.type());

        for (int r = 0; r < src.rows; ++r) {
            for (int c = 0; c < src.cols; ++c) {
                cv::Vec3b m = src.at<cv::Vec3b>(r, c);
                double B = (double) m[0] / 255;
                double G = (double) m[1] / 255;
                double R = (double) m[2] / 255;

                double H, S, I;

                double den = sqrt((R - G) * (R - G) + (R - B) * (G - B));
                if (den == 0) {
                    H = 0;
                } else {  // den != 0
                    double theta = acos((R - G + R - B) / (2 * den));
                    H = (B <= G) ? (theta / (2 * M_PI)) : (1 - theta / (2 * M_PI));
                }

                double sum = B + G + R;
                if (sum == 0) {
                    S = 0;
                } else {  // sum != 0
                    S = 1 - 3 * std::min(std::min(B, G), R) / sum;
                }

                I = sum / 3.0;

                temp.at<cv::Vec3b>(r, c) = cv::Vec3b((uchar) (H * 255), (uchar) (S * 255), (uchar) (I * 255));
            }
        }

        temp.copyTo(dst);
    }
};


#endif // IMAGE_PROCESSOR_HPP
