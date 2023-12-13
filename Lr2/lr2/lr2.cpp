
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
    // Загрузка изображения 
    Mat input_image = imread("myImg1.jpg");

    if (input_image.empty())
    {
        cout << "Ошибка загрузки файла" << endl;
        return -1;
    }

    Mat output_image(input_image.rows, input_image.cols, CV_8UC1, WINDOW_GUI_NORMAL);

    Mat grayscaleImage;
    cvtColor(input_image, grayscaleImage, COLOR_BGR2GRAY);

    Mat binaryImage;
    threshold(grayscaleImage, binaryImage, 128, 255, THRESH_BINARY);

    Mat binResult = binaryImage.clone();
    Mat monoResult = grayscaleImage.clone();
    show_image(input_image, binResult, "Исходное бинарное изображение");
    show_image(input_image, monoResult, "Исходное полутоновое изображение");

    int input_key;
    int apertureSize;

    while (true)
    {
        cout << "Выберите фильтр:\n"
            "1) Фильтр бинарной эрозии\n"
            "2) Фильтр бинарной дилатации\n"
            "3) Оператор закрытия (бинарное)\n"
            "4) Оператор открытия (бинарное)\n"
            "5) Фильтр полутоновой эрозии\n"
            "6) Фильтр полутоновой дилатации\n"
            "7) Оператор закрытия (полутоновое)\n"
            "8) Оператор открытия (полутоновое)\n"
            "9) Оператор выделения контуров\n"
            "10) Многомасштабный морфологический градиент\n"
            "0) Выход\n";
        cin >> input_key;

        switch (input_key)
        {
        case 1:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            binaryErosion(binaryImage, binResult, apertureSize);
            show_image(binaryImage, binResult, "Фильтр бинарной эрозии");
            break;
        }
        case 2:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            binaryDilation(binaryImage, binResult, apertureSize);
            show_image(binaryImage, binResult, "Фильтр бинарной дилатации");
            break;
        }
        case 3:
        {
            Mat binaryClosedImage;
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            binaryClosing(binaryImage, binaryClosedImage, apertureSize);
            show_image(binaryImage, binaryClosedImage, "Оператор закрытия (бинарное)");
            break;
        }
        case 4:
        {
            Mat binaryOpenedImage;
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            binaryOpening(binaryImage, binaryOpenedImage, apertureSize);
            show_image(binaryImage, binaryOpenedImage, "Оператор открытия (бинарное)");
            break;
        }
        case 5:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            grayscaleErosion(grayscaleImage, monoResult, apertureSize);
            show_image(grayscaleImage, monoResult, "Фильтр полутоновой эрозии");
            break;
        }
        case 6:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            grayscaleDilation(grayscaleImage, monoResult, apertureSize);
            show_image(grayscaleImage, monoResult, "Фильтр полутоновой дилатации");
            break;
        }
        case 7:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            grayscaleClosing(grayscaleImage, monoResult, apertureSize);
            show_image(grayscaleImage, monoResult, "Оператор закрытия (полутоновой)");
            break;
        }
        case 8:
        {
            cout << "Enter aperture size: ";
            cin >> apertureSize;
            grayscaleOpening(grayscaleImage, monoResult, apertureSize);
            show_image(grayscaleImage, monoResult, "Оператор открытия (полутоновой)");
            break;
        }
        case 9:
        {
            cv::Mat contoursImage;
            sobelOperator(grayscaleImage, contoursImage);
            show_image(grayscaleImage, contoursImage, "Оператор выделения контуров");
            break;
        }
        case 10:
        {
            Mat multiscaleGradientImage;
            int minKernelSize = 3;  // Минимальный размер структурного элемента
            int maxKernelSize = 15; // Максимальный размер структурного элемента
            multiscaleMorphologicalGradient(grayscaleImage, multiscaleGradientImage, minKernelSize, maxKernelSize);
            show_image(grayscaleImage, multiscaleGradientImage, "Многомасштабный морфологический градиент");
            break;
        }
        case 0:
            return 0;
        default:
            cout << "Неверна команда!\n Попробуйте снова\n";
            break;
        }
    }
    return 0;
}

void show_image(const Mat input_img, Mat& output_img, string filter_name)
{
    imshow("Исходное изображение", input_img);
    waitKey(0);
    imshow(filter_name, output_img);
    waitKey(0);
    destroyAllWindows();
    imwrite("D:/фото/" + filter_name + ".jpg", output_img);
}

// Функция бинарной дилатации
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

// Функция бинарной эрозии
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
    outputImage = inputImage.clone(); // Создаем копию входного изображения

    int border = kernelSize / 2;

    for (int y = border; y < inputImage.rows - border; ++y) {
        for (int x = border; x < inputImage.cols - border; ++x) {
            // Определяем область, покрываемую ядром
            cv::Rect regionOfInterest(x - border, y - border, kernelSize, kernelSize);

            // Вычисляем минимальное значение в области
            double minVal;
            minMaxLoc(inputImage(regionOfInterest), &minVal);

            // Устанавливаем текущий пиксель равным минимальному значению
            outputImage.at<uchar>(y, x) = static_cast<uchar>(minVal);
        }
    }
}

// 6
void grayscaleDilation(Mat& inputImage, Mat& outputImage, int kernelSize)
{
    outputImage = inputImage.clone(); // Создаем копию входного изображения

    int border = kernelSize / 2;

    for (int y = border; y < inputImage.rows - border; ++y) {
        for (int x = border; x < inputImage.cols - border; ++x) {
            // Определяем область, покрываемую ядром
            cv::Rect regionOfInterest(x - border, y - border, kernelSize, kernelSize);

            // Вычисляем максимальное значение в области
            double maxVal;
            cv::minMaxLoc(inputImage(regionOfInterest), nullptr, &maxVal);

            // Устанавливаем текущий пиксель равным максимальному значению
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
Многомасштабный морфологический градиент предполагает использование
различных размеров структурных элементов для выделения контуров
изображения на разных уровнях масштаба.
В данном случае, мы реализуем многомасштабный морфологический градиент
с использованием операций эрозии и дилатации.
*/

void multiscaleMorphologicalGradient(Mat& inputImage, Mat& outputImage, int minKernelSize, int maxKernelSize)
{
    // Создание выходного изображения того же размера, что и входное
    outputImage = cv::Mat::zeros(inputImage.size(), CV_8UC1);

    // Применение многомасштабного морфологического градиента
    for (int kernelSize = minKernelSize; kernelSize <= maxKernelSize; ++kernelSize) {
        cv::Mat eroded, dilated;

        // Эрозия
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
        cv::erode(inputImage, eroded, element);

        // Дилатация
        cv::dilate(inputImage, dilated, element);

        // Вычисление разности между дилатированным и эрозированным изображением
        cv::Mat gradient = dilated - eroded;

        // Обновление выходного изображения, учитывая текущий масштаб
        outputImage = cv::max(outputImage, gradient);
    }
}

void sobelOperator( cv::Mat& inputImage, cv::Mat& outputImage)
{
    outputImage = inputImage.clone();

    int rows = inputImage.rows;
    int cols = inputImage.cols;

    // Оператор Собеля для горизонтальной и вертикальной компонент
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

            // Подсчитываем общий градиент
            int gradientMagnitude = std::sqrt(gradientX * gradientX + gradientY * gradientY);

            // Ограничиваем значения градиента
            gradientMagnitude = std::min(255, std::max(0, gradientMagnitude));

            outputImage.at<uchar>(y, x) = gradientMagnitude;
        }
    }
}