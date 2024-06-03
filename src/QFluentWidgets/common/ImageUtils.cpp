#include "ImageUtils.hpp"

namespace fluent {


    QPixmap GaussianBlur::gaussianBlur(
        const QPixmap& image,
        double blurRadius,
        double brightFactor,
        QSize blurPicSize
    ) {
        QImage qImage = image.toImage();

        if (!blurPicSize.isEmpty()) {
            QSize newSize = qImage.size().scaled(blurPicSize, Qt::KeepAspectRatio);
            qImage = qImage.scaled(newSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }

        cv::Mat mat = QImageToCvMat(qImage);

        if (mat.channels() == 1) {
            cv::cvtColor(mat, mat, cv::COLOR_GRAY2BGR);
        }

        for (int i = 0; i < 3; ++i) {
            cv::Mat channel;
            cv::extractChannel(mat, channel, i);
            cv::GaussianBlur(channel, channel, cv::Size(0, 0), blurRadius);
            cv::multiply(channel, brightFactor, channel);
            cv::insertChannel(channel, mat, i);
        }

        QImage blurredImage = CvMatToQImage(mat);

        return QPixmap::fromImage(blurredImage);
    }


    cv::Mat GaussianBlur::QImageToCvMat(const QImage& image) {
        switch (image.format()) {
        case QImage::Format_RGB32:
            return cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        case QImage::Format_RGB888:
            return cv::Mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        default:
            QImage convertedImage = image.convertToFormat(QImage::Format_RGB888);
            return cv::Mat(convertedImage.height(), convertedImage.width(), CV_8UC3, const_cast<uchar*>(convertedImage.bits()), convertedImage.bytesPerLine());
        }
    }


    QImage GaussianBlur::CvMatToQImage(const cv::Mat& mat) {
        switch (mat.type()) {
        case CV_8UC4:
            return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
        case CV_8UC3:
            return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped();
        case CV_8UC1:
            return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        default:
            return QImage();
        }
    }


    std::array<int, 3> DominantColor::getDominantColor(const QString& imagePath) {
        if (imagePath.startsWith(':')) {
            return { 24, 24, 24 };
        }

        QImage image(imagePath);
        if (image.isNull()) {
            return { 24, 24, 24 };
        }

        std::vector<QColor> palette = getPalette(image);
        adjustPaletteValue(palette);

        auto it = std::remove_if(palette.begin(), palette.end(), [](const QColor& color) {
            float h, s, v;
            color.getHsvF(&h, &s, &v);
            return h < 0.02;
            });

        palette.erase(it, palette.end());
        if (palette.size() > 5) {
            palette.resize(5);
        }

        std::sort(palette.begin(), palette.end(), [](const QColor& a, const QColor& b) {
            return colorfulness(a) > colorfulness(b);
            });

        return { palette[0].red(), palette[0].green(), palette[0].blue() };
    }


    std::vector<QColor> DominantColor::getPalette(const QImage& image) {
        std::vector<QColor> palette;
        cv::Mat mat = GaussianBlur::QImageToCvMat(image);
        cv::Mat lab;
        cv::cvtColor(mat, lab, cv::COLOR_BGR2Lab);

        std::vector<cv::Mat> channels;
        cv::split(lab, channels);

        cv::Mat hist;
        int histSize[] = { 16, 16, 16 };
        float lranges[] = { 0, 256 };
        float aranges[] = { 0, 256 };
        float branges[] = { 0, 256 };
        const float* ranges[] = { lranges, aranges, branges };
        int channelsArr[] = { 0, 1, 2 };

        cv::calcHist(&lab, 1, channelsArr, cv::Mat(), hist, 3, histSize, ranges, true, false);

        cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());

        for (int l = 0; l < 16; ++l) {
            for (int a = 0; a < 16; ++a) {
                for (int b = 0; b < 16; ++b) {
                    float binVal = hist.at<float>(l, a, b);
                    if (binVal > 0) {
                        int L = cvRound(l * 256 / 16);
                        int A = cvRound(a * 256 / 16);
                        int B = cvRound(b * 256 / 16);

                        cv::Mat color(1, 1, CV_8UC3, cv::Scalar(L, A, B));
                        cv::cvtColor(color, color, cv::COLOR_Lab2BGR);

                        QColor qColor(color.data[2], color.data[1], color.data[0]);
                        palette.push_back(qColor);
                    }
                }
            }
        }

        return palette;
    }


    void DominantColor::adjustPaletteValue(std::vector<QColor>& palette) {
        for (auto& color : palette) {
            float h, s, v;
            color.getHsvF(&h, &s, &v);
            double factor = 1.0;

            if (v > 0.9) {
                factor = 0.8;
            }
            else if (v > 0.8) {
                factor = 0.9;
            }
            else if (v > 0.7) {
                factor = 0.95;
            }

            v *= factor;
            color.setHsvF(h, s, v);
        }
    }

    
    double DominantColor::colorfulness(const QColor& color) {
        double rg = std::abs(color.red() - color.green());
        double yb = std::abs(0.5 * (color.red() + color.green()) - color.blue());

        double rgMean = (rg + rg) / 2.0;
        double ybMean = (yb + yb) / 2.0;

        double rgStd = std::sqrt(std::pow(rg - rgMean, 2.0));
        double ybStd = std::sqrt(std::pow(yb - ybMean, 2.0));

        double stdRoot = std::sqrt(rgStd * rgStd + ybStd * ybStd);
        double meanRoot = std::sqrt(rgMean * rgMean + ybMean * ybMean);

        return stdRoot + 0.3 * meanRoot;
    }
} // namespace fluent