
#include <opencv2/opencv.hpp> 
#include <cmath>
#include <iostream>

using namespace cv;
using namespace std;

void binaryDilation(Mat& inputImage, Mat& outputImage, int kernelSize);
void binaryErosion(Mat& inputImage, Mat& outputImage, int kernelSize);
void binaryClosing(Mat& inputImage, Mat& outputImage, int kernelSize);
void binaryOpening(Mat& inputImage, Mat& outputImage, int kernelSize);

void grayscaleErosion(Mat& inputImage, Mat& outputImage, int kernelSize);
void grayscaleDilation(Mat& inputImage, Mat& outputImage, int kernelSize);
void grayscaleClosing(Mat& inputImage, Mat& outputImage, int kernelSize);
void grayscaleOpening(Mat& inputImage, Mat& outputImage, int kernelSize);
void multiscaleMorphologicalGradient(Mat& inputImage, Mat& outputImage, int minKernelSize, int maxKernelSize);

void show_image(const Mat input_img, Mat& output_img, string filter_name);
void sobelOperator(cv::Mat& inputImage, cv::Mat& outputImage);

int main()
{
    setlocale(LC_ALL, "Russian");
    // �������� ����������� 
    Mat input_image = imread("myImg1.jpg");

    if (input_image.empty())
    {
        cout << "������ �������� �����" << endl;
        return -1;
    }

    Mat output_image(input_image.rows, input_image.cols, CV_8UC1, WINDOW_GUI_NORMAL);

    Mat grayscaleImage;
    cvtColor(input_image, grayscaleImage, COLOR_BGR2GRAY);

    Mat binaryImage;
    threshold(grayscaleImage, binaryImage, 128, 255, THRESH_BINARY);

    Mat binResult = binaryImage.clone();
    Mat monoResult = grayscaleImage.clone();
    show_image(input_image, binResult, "�������� �������� �����������");
    show_image(input_image, monoResult, "�������� ����������� �����������");

    int input_key;
    int apertureSize;

    while (true)
    {
        cout << "�������� ������:\n"
            "1) ������ �������� ������\n"
            "2) ������ �������� ���������\n"
            "3) �������� �������� (��������)\n"
            "4) �������� �������� (��������)\n"
            "5) ������ ����������� ������\n"
            "6) ������ ����������� ���������\n"
            "7) �������� �������� (�����������)\n"
            "8) �������� �������� (�����������)\n"
            "9) �������� ��������� ��������\n"
            "10) ��������������� ��������������� ��������\n"
            "0) �����\n";
        cin >> input_key;

        switch (input_key)
        {
        case 1:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            binaryErosion(binaryImage, binResult, apertureSize);
            show_image(binaryImage, binResult, "������ �������� ������");
            break;
        }
        case 2:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            binaryDilation(binaryImage, binResult, apertureSize);
            show_image(binaryImage, binResult, "������ �������� ���������");
            break;
        }
        case 3:
        {
            Mat binaryClosedImage;
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            binaryClosing(binaryImage, binaryClosedImage, apertureSize);
            show_image(binaryImage, binaryClosedImage, "�������� �������� (��������)");
            break;
        }
        case 4:
        {
            Mat binaryOpenedImage;
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            binaryOpening(binaryImage, binaryOpenedImage, apertureSize);
            show_image(binaryImage, binaryOpenedImage, "�������� �������� (��������)");
            break;
        }
        case 5:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            grayscaleErosion(grayscaleImage, monoResult, apertureSize);
            show_image(grayscaleImage, monoResult, "������ ����������� ������");
            break;
        }
        case 6:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            grayscaleDilation(grayscaleImage, monoResult, apertureSize);
            show_image(grayscaleImage, monoResult, "������ ����������� ���������");
            break;
        }
        case 7:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            grayscaleClosing(grayscaleImage, monoResult, apertureSize);
            show_image(grayscaleImage, monoResult, "�������� �������� (�����������)");
            break;
        }
        case 8:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            grayscaleOpening(grayscaleImage, monoResult, apertureSize);
            show_image(grayscaleImage, monoResult, "�������� �������� (�����������)");
            break;
        }
        case 9:
        {
            cv::Mat contoursImage;
            sobelOperator(grayscaleImage, contoursImage);
            show_image(grayscaleImage, contoursImage, "�������� ��������� ��������");
            break;
        }
        case 10:
        {
            Mat multiscaleGradientImage;
            int minKernelSize = 3;  // ����������� ������ ������������ ��������
            int maxKernelSize = 15; // ������������ ������ ������������ ��������
            multiscaleMorphologicalGradient(grayscaleImage, multiscaleGradientImage, minKernelSize, maxKernelSize);
            show_image(grayscaleImage, multiscaleGradientImage, "��������������� ��������������� ��������");
            break;
        }
        case 0:
            return 0;
        default:
            cout << "������� �������!\n ���������� �����\n";
            break;
        }
    }
    return 0;
}

void show_image(const Mat input_img, Mat& output_img, string filter_name)
{
    imshow("�������� �����������", input_img);
    waitKey(0);
    imshow(filter_name, output_img);
    waitKey(0);
    destroyAllWindows();
    imwrite("D:/����/" + filter_name + ".jpg", output_img);
}

// ������� �������� ���������
void binaryDilation(cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize)
{
    outputImage = inputImage.clone();

    int border = kernelSize / 2;

    for (int y = border; y < inputImage.rows - border; ++y) {
        for (int x = border; x < inputImage.cols - border; ++x) {
            bool hasWhitePixel = false;

            for (int ky = -border; ky <= border; ++ky) {
                for (int kx = -border; kx <= border; ++kx) {
                    if (inputImage.at<uchar>(y + ky, x + kx) == 255) {
                        hasWhitePixel = true;
                        break;
                    }
                }
                if (hasWhitePixel) {
                    break;
                }
            }

            outputImage.at<uchar>(y, x) = (hasWhitePixel) ? 255 : 0;
        }
    }
}

// ������� �������� ������
void binaryErosion(cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize)
{
    outputImage = inputImage.clone();

    int border = kernelSize / 2;

    for (int y = border; y < inputImage.rows - border; ++y) {
        for (int x = border; x < inputImage.cols - border; ++x) {
            bool allWhitePixels = true;

            for (int ky = -border; ky <= border; ++ky) {
                for (int kx = -border; kx <= border; ++kx) {
                    if (inputImage.at<uchar>(y + ky, x + kx) == 0) {
                        allWhitePixels = false;
                        break;
                    }
                }
                if (!allWhitePixels) {
                    break;
                }
            }

            outputImage.at<uchar>(y, x) = (allWhitePixels) ? 255 : 0;
        }
    }
}

// 5
void grayscaleErosion(Mat& inputImage, Mat& outputImage, int kernelSize)
{
    outputImage = inputImage.clone(); // ������� ����� �������� �����������

    int border = kernelSize / 2;

    for (int y = border; y < inputImage.rows - border; ++y) {
        for (int x = border; x < inputImage.cols - border; ++x) {
            // ���������� �������, ����������� �����
            cv::Rect regionOfInterest(x - border, y - border, kernelSize, kernelSize);

            // ��������� ����������� �������� � �������
            double minVal;
            minMaxLoc(inputImage(regionOfInterest), &minVal);

            // ������������� ������� ������� ������ ������������ ��������
            outputImage.at<uchar>(y, x) = static_cast<uchar>(minVal);
        }
    }
}

// 6
void grayscaleDilation(Mat& inputImage, Mat& outputImage, int kernelSize)
{
    outputImage = inputImage.clone(); // ������� ����� �������� �����������

    int border = kernelSize / 2;

    for (int y = border; y < inputImage.rows - border; ++y) {
        for (int x = border; x < inputImage.cols - border; ++x) {
            // ���������� �������, ����������� �����
            cv::Rect regionOfInterest(x - border, y - border, kernelSize, kernelSize);

            // ��������� ������������ �������� � �������
            double maxVal;
            cv::minMaxLoc(inputImage(regionOfInterest), nullptr, &maxVal);

            // ������������� ������� ������� ������ ������������� ��������
            outputImage.at<uchar>(y, x) = static_cast<uchar>(maxVal);
        }
    }
}

void binaryClosing(Mat& inputImage, Mat& outputImage, int kernelSize)
{
    Mat temp;
    binaryErosion(inputImage, temp, kernelSize);
    binaryDilation(temp, outputImage, kernelSize);
}

void binaryOpening(Mat& inputImage, Mat& outputImage, int kernelSize)
{
    Mat temp;
    binaryDilation(inputImage, temp, kernelSize);
    binaryErosion(temp, outputImage, kernelSize);
}

void grayscaleClosing(cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize)
{
    cv::Mat temp;
    grayscaleErosion(inputImage, temp, kernelSize);
    grayscaleDilation(temp, outputImage, kernelSize);
}

void grayscaleOpening(cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize)
{
    cv::Mat temp;
    grayscaleDilation(inputImage, temp, kernelSize);
    grayscaleErosion(temp, outputImage, kernelSize);
}

/*
��������������� ��������������� �������� ������������ �������������
��������� �������� ����������� ��������� ��� ��������� ��������
����������� �� ������ ������� ��������.
� ������ ������, �� ��������� ��������������� ��������������� ��������
� �������������� �������� ������ � ���������.
*/

void multiscaleMorphologicalGradient(Mat& inputImage, Mat& outputImage, int minKernelSize, int maxKernelSize)
{
    // �������� ��������� ����������� ���� �� �������, ��� � �������
    outputImage = cv::Mat::zeros(inputImage.size(), CV_8UC1);

    // ���������� ���������������� ���������������� ���������
    for (int kernelSize = minKernelSize; kernelSize <= maxKernelSize; ++kernelSize) {
        cv::Mat eroded, dilated;

        // ������
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
        cv::erode(inputImage, eroded, element);

        // ���������
        cv::dilate(inputImage, dilated, element);

        // ���������� �������� ����� �������������� � ������������� ������������
        cv::Mat gradient = dilated - eroded;

        // ���������� ��������� �����������, �������� ������� �������
        outputImage = cv::max(outputImage, gradient);
    }
}

void sobelOperator( cv::Mat& inputImage, cv::Mat& outputImage)
{
    outputImage = inputImage.clone();

    int rows = inputImage.rows;
    int cols = inputImage.cols;

    // �������� ������ ��� �������������� � ������������ ���������
    int sobelX[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int sobelY[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

    for (int y = 1; y < rows - 1; ++y) {
        for (int x = 1; x < cols - 1; ++x) {
            int gradientX = 0;
            int gradientY = 0;

            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    gradientX += inputImage.at<uchar>(y + ky, x + kx) * sobelX[ky + 1][kx + 1];
                    gradientY += inputImage.at<uchar>(y + ky, x + kx) * sobelY[ky + 1][kx + 1];
                }
            }

            // ������������ ����� ��������
            int gradientMagnitude = std::sqrt(gradientX * gradientX + gradientY * gradientY);

            // ������������ �������� ���������
            gradientMagnitude = std::min(255, std::max(0, gradientMagnitude));

            outputImage.at<uchar>(y, x) = gradientMagnitude;
        }
    }
}