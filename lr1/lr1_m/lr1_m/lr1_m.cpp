 #include <opencv2/opencv.hpp> 
#include <cmath>
#include <iostream>

using namespace cv;
using namespace std;

void differenceOfGaussians(const Mat&, Mat&);
void roberts(const Mat&, Mat& );

void insertionSort(int window[]);
void medianBlure(Mat& , Mat& );
void mosaic(const Mat&, Mat&);

void showImage(const Mat, Mat&, string);


void applyGaussianBlur(const cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize, double sigma);
void calculateGradient(const cv::Mat& inputImage, cv::Mat& gradientX, cv::Mat& gradientY);
void applyGaussianContour(const cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize, double sigma);
void applySharpening(const cv::Mat& inputImage, cv::Mat& outputImage);
void applyDoG(const cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize1, double sigma1, int kernelSize2, double sigma2);

int main() {
    setlocale(LC_ALL, "Russian");
    // Загрузка изображения 
    Mat input_image = imread("myImg1.jpg", WINDOW_GUI_NORMAL);

    if (input_image.empty())
    {
        cout << "Ошибка загрузки файла" << endl;
        return -1;
    }

    Mat DoG(input_image.rows, input_image.cols, CV_8SC1);
    
    //Результат 
    Mat output_image(input_image.rows, input_image.cols, CV_8UC1, WINDOW_GUI_NORMAL);

    int input_key;

    while (true)
    {
        cout <<
            "Выбор команды:\n 1 - Гаусс размытие\n 2 - Гаусс Контур\n 3 - Увеличение резкости\n 4 - Разность гауссианов (DoG)\n "
            "5 - Фильтр Кэнни \n 6 - Оператор Робертса для горизонтальных контуров \n 7 - Медианная фильтрация "
            "\n 8 - Фильтр «Мозаика» \n 0 - Выход\n";
        cin >> input_key;

        switch (input_key)
        {
            case 1:
            {
                applyGaussianBlur(input_image, output_image, 5, 1.0);
                showImage(input_image, output_image, "Гаусс размытие");
                break;
            }
            case 2:
            {
                applyGaussianContour(input_image, output_image, 5, 1.0);
                showImage(input_image, output_image, "Гаусс Контур");
                break; 
            }
            case 3:
            {
                applySharpening(  input_image, output_image);
                showImage(input_image, output_image, "Увеличение резкости");
                break;
            }
            case 4:
            {
                cv::Mat dogImage;
                int kernelSize1 = 5, kernelSize2 = 9;
                double sigma1 = 1.0, sigma2 = 2.0;

                applyDoG(input_image, dogImage, kernelSize1, sigma1, kernelSize2, sigma2);

                //differenceOfGaussians(input_image, DoG);
                //output_image = DoG;
                showImage(input_image, dogImage, "Разность гауссианов");
                break;
            }
            case 5:
            {
                Canny(input_image, output_image, 50, 200);
                showImage(input_image, output_image, "Фильтр Кэнни");
                break;
            }
            case 6:
            {
                roberts(input_image, output_image);
                showImage(input_image, output_image, "Оператор Робертса");
                break;
            }
            case 7:
            {
                medianBlure(input_image, output_image);
                showImage(input_image, output_image, "Медианная фильтрация");
                break;
            }
            case 8:
            {
                mosaic(input_image, output_image);
                showImage(input_image, output_image, "Мозаика");
                break;
            }
            case 0:
            {
                cout << "Выход";
                break;
            }
            default:
            {
                cout << "Неверная команда\n";
                break;
            }
        }

        if (input_key == 0) break;
    }
    return 0;
}

void showImage(const Mat input_img, Mat& output_img, string filter_name) {
    imshow("Исходное изображение", input_img);
    waitKey(0);
    imshow(filter_name, output_img);
    waitKey(0);
    destroyAllWindows();
    imwrite("D:/фото/"+ filter_name +".jpg", output_img);
}

void applySharpening(const cv::Mat& inputImage, cv::Mat& outputImage)
{
    outputImage = cv::Mat::zeros(inputImage.size(), inputImage.type());

    // Ядро Лапласиана для увеличения резкости
    int kernelSize = 3;
    float kernelData[] = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };
    cv::Mat kernel = cv::Mat(kernelSize, kernelSize, CV_32F, kernelData);

    // Применяем ядро Лапласиана вручную
    for (int y = 1; y < inputImage.rows - 1; ++y) {
        for (int x = 1; x < inputImage.cols - 1; ++x) {
            float sum = 0.0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    sum += kernel.at<float>(ky + 1, kx + 1) * inputImage.at<uchar>(y + ky, x + kx);
                }
            }
            outputImage.at<uchar>(y, x) = cv::saturate_cast<uchar>(sum);
        }
    }
}

//void differenceOfGaussians(const Mat& input_image, Mat& output_image)
//{
//
//    Mat blurred_image3x3(input_image.rows, input_image.cols, CV_8UC1);
//    Mat blurred_image5x5(input_image.rows, input_image.cols, CV_8UC1);
//
//    applyGaussianBlur(input_image, blurred_image5x5, 5, 3.0);
//
//    for (int i = 0; i < input_image.rows; i++)
//    {
//        for (int j = 0; j < input_image.cols; j++)
//        {
//            int diff = blurred_image5x5.at<uchar>(i, j) - blurred_image3x3.at<uchar>(i, j);
//            output_image.at<uchar>(i, j) = abs(diff);
//        }
//    }
//}

void applyDoG(const cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize1, double sigma1, int kernelSize2, double sigma2)
{
    cv::Mat blurredImage1, blurredImage2;

    // Применяем первое размытие Гауссиана
    applyGaussianBlur(inputImage, blurredImage1, kernelSize1, sigma1);

    // Применяем второе размытие Гауссиана
    applyGaussianBlur(inputImage, blurredImage2, kernelSize2, sigma2);

    // Вычисляем разность Гауссиан
    outputImage = cv::Mat::zeros(inputImage.size(), inputImage.type());

    for (int y = 0; y < inputImage.rows; ++y) {
        for (int x = 0; x < inputImage.cols; ++x) {
            outputImage.at<uchar>(y, x) = cv::saturate_cast<uchar>(
                blurredImage1.at<uchar>(y, x) - blurredImage2.at<uchar>(y, x)
            );
        }
    }
}

void roberts(const Mat& input_image, Mat& output_image)
{
    output_image = input_image.clone();
    int nRows = output_image.rows;
    int nCols = output_image.cols;
    for (int i = 0; i < nRows - 1; i++) 
    {
        for (int j = 0; j < nCols - 1; j++) 
        {
            int t1 = (input_image.at<uchar>(i, j) -
                input_image.at<uchar>(i + 1, j + 1)) *
                (input_image.at<uchar>(i, j) -
                    input_image.at<uchar>(i + 1, j + 1));
            int t2 = (input_image.at<uchar>(i + 1, j) -
                input_image.at<uchar>(i, j + 1)) *
                (input_image.at<uchar>(i + 1, j) -
                    input_image.at<uchar>(i, j + 1));
            output_image.at<uchar>(i, j) = (uchar)sqrt(t1 + t2);
        }
    }
}


//сортировка window
void insertionSort(int window[])
{
    int temp, i, j;
    for (i = 0; i < 9; i++) 
    {
        temp = window[i];
        for (j = i - 1; j >= 0 && temp < window[j]; j--) {
            window[j + 1] = window[j];
        }
        window[j + 1] = temp;
    }
}

// Медианный фильтр 
void medianBlure(Mat& input_image, Mat& output_image) 
{
    int window[9];

    output_image = input_image.clone();
    for (int y = 0; y < input_image.rows; y++)
        for (int x = 0; x < input_image.cols; x++)
            output_image.at<uchar>(y, x) = 0.0;

    for (int y = 1; y < input_image.rows - 1; y++) 
    {
        for (int x = 1; x < input_image.cols - 1; x++) 
        {
            window[0] = input_image.at<uchar>(y - 1, x - 1);
            window[1] = input_image.at<uchar>(y, x - 1);
            window[2] = input_image.at<uchar>(y + 1, x - 1);
            window[3] = input_image.at<uchar>(y - 1, x);
            window[4] = input_image.at<uchar>(y, x);
            window[5] = input_image.at<uchar>(y + 1, x);
            window[6] = input_image.at<uchar>(y - 1, x + 1);
            window[7] = input_image.at<uchar>(y, x + 1);
            window[8] = input_image.at<uchar>(y + 1, x + 1);

            // сортируем window, чтобы найти медиану
            insertionSort(window);

            // медиану центральному элементу матрицы
            output_image.at<uchar>(y, x) = window[4];
        }
    }
}

//Фильтр «Мозаика»
void mosaic(const Mat& input_image, Mat& output_image) 
{
    output_image = input_image.clone();
    Mat ROI(output_image, cv::Rect(0, 0, input_image.cols, input_image.rows)); // область мозаики
    int W = 20, H = 20; // Ожидаемые ширина и высота большого пикселя
    int W1, H1; // Фактическая ширина и высота больших пикселей, область мозаики может не иметь возможности точно записать целое число больших пикселей, и ее необходимо обрезать на границе
    for (int i = 0; i < ROI.rows; i += H)
    {
        H1 = (ROI.rows - i >= H) ? H : (ROI.rows - i); // Определить граничные условия
        for (int j = 0; j < ROI.cols; j += W)
        {
            W1 = (ROI.cols - j >= W) ? W : (ROI.cols - j);
            cv::Mat tmp(ROI, cv::Rect(j, i, W1, H1));
            tmp = cv::mean(tmp);
        }
    }
}

void applyGaussianBlur(const cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize, double sigma)
{
    outputImage = inputImage.clone();
    int rows = inputImage.rows;
    int cols = inputImage.cols;

    // Вычисляем половину размера ядра
    int halfSize = kernelSize / 2;

    // Создаем ядро Гауссиана
    std::vector<std::vector<double>> gaussianKernel(kernelSize, std::vector<double>(kernelSize, 0.0));
    double sum = 0.0;

    for (int y = -halfSize; y <= halfSize; ++y) {
        for (int x = -halfSize; x <= halfSize; ++x) {
            gaussianKernel[y + halfSize][x + halfSize] = std::exp(-(x * x + y * y) / (2.0 * sigma * sigma));
            sum += gaussianKernel[y + halfSize][x + halfSize];
        }
    }

    // Нормализуем ядро Гауссиана
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            gaussianKernel[i][j] /= sum;
        }
    }

    // Применяем фильтр Гаусса
    for (int y = halfSize; y < rows - halfSize; ++y) {
        for (int x = halfSize; x < cols - halfSize; ++x) {
            double sum = 0.0;

            for (int ky = -halfSize; ky <= halfSize; ++ky) {
                for (int kx = -halfSize; kx <= halfSize; ++kx) {
                    sum += inputImage.at<uchar>(y + ky, x + kx) * gaussianKernel[ky + halfSize][kx + halfSize];
                }
            }

            outputImage.at<uchar>(y, x) = cv::saturate_cast<uchar>(sum);
        }
    }
}

void calculateGradient(const cv::Mat& inputImage, cv::Mat& gradientX, cv::Mat& gradientY)
{
    int rows = inputImage.rows;
    int cols = inputImage.cols;

    // Оператор Собеля для горизонтальной и вертикальной компонент
    int sobelX[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int sobelY[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

    gradientX = cv::Mat::zeros(rows, cols, CV_8U);
    gradientY = cv::Mat::zeros(rows, cols, CV_8U);

    // Применяем оператор Собеля для подсчета градиента
    for (int y = 1; y < rows - 1; ++y) {
        for (int x = 1; x < cols - 1; ++x) {
            int gx = 0, gy = 0;

            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    gx += inputImage.at<uchar>(y + ky, x + kx) * sobelX[ky + 1][kx + 1];
                    gy += inputImage.at<uchar>(y + ky, x + kx) * sobelY[ky + 1][kx + 1];
                }
            }

            gradientX.at<uchar>(y, x) = cv::saturate_cast<uchar>(gx);
            gradientY.at<uchar>(y, x) = cv::saturate_cast<uchar>(gy);
        }
    }
}
void applyGaussianContour(const cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize, double sigma)
{
    // Применяем Гауссово размытие
    cv::Mat blurredImage;
    applyGaussianBlur(inputImage, blurredImage, kernelSize, sigma);

    // Вычисляем градиент
    cv::Mat gradientX, gradientY;
    calculateGradient(blurredImage, gradientX, gradientY);

    // Уточняем размер выходного изображения
    outputImage.create(blurredImage.rows, blurredImage.cols, CV_8U);

    // Преобразуем градиентные изображения в тип данных с плавающей точкой
    cv::Mat gradientXFloat, gradientYFloat;
    gradientX.convertTo(gradientXFloat, CV_32F);
    gradientY.convertTo(gradientYFloat, CV_32F);

    // Вычисляем общий градиент
    cv::magnitude(gradientXFloat, gradientYFloat, outputImage);

    // Нормализуем значения градиента
    cv::normalize(outputImage, outputImage, 0, 255, cv::NORM_MINMAX, CV_8U);
}