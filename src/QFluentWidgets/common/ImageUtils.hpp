#pragma once

#include <QApplication>
#include <QImage>
#include <QPixmap>
#include <QBuffer>
#include <QIODevice>
#include <QColor>
#include <QFile>
#include <QImageReader>
#include <QFileDialog>
#include <QVector>
#include <QImageWriter>
#include <QtMath>
#include <algorithm>
#include <array>
#include <memory>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace fluent {

    class GaussianBlur {
    public:
        static QPixmap gaussianBlur(const QPixmap& image, double blurRadius = 18, double brightFactor = 1, QSize blurPicSize = QSize());
        static cv::Mat QImageToCvMat(const QImage& image);
        static QImage CvMatToQImage(const cv::Mat& mat);
    };


    class DominantColor {
    public:
        static std::array<int, 3> getDominantColor(const QString& imagePath);

    private:
        static std::vector<QColor> getPalette(const QImage& image);
        static void adjustPaletteValue(std::vector<QColor>& palette);
        static double colorfulness(const QColor& color);
    };

}