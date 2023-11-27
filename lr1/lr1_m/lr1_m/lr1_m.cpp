 #include <opencv2/opencv.hpp> 
#include <cmath>
#include <iostream>

using namespace cv;
using namespace std;

void gauss_kernel(Mat&, int, float);
void gauss_bluray(const Mat&, Mat&, int, float);
void gauss_contour(const Mat&, Mat&, int, float);
void gauss_sharpness(const Mat&, Mat&, int, float, float);
void difference_of_gaussians(const Mat&, Mat&);
void roberts(const Mat&, Mat& );

void insertion_sort(int window[]);
void median_blure(Mat& , Mat& );
void mosaic(const Mat&, Mat&);

void show_image(const Mat, Mat&, string);

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
                gauss_bluray(input_image, output_image, 5, 1.0);
                show_image(input_image, output_image, "Гаусс размытие");
                break;
            case 2:
                gauss_contour(input_image, output_image, 5, 1.0);
                show_image(input_image, output_image, "Гаусс Контур");
                break;
            case 3:
                gauss_sharpness(input_image, output_image, 5, 1.0, 0.35);
                show_image(input_image, output_image, "Увеличение резкости");
                break;
            case 4:
                difference_of_gaussians(input_image, DoG);            
                output_image = DoG;
                show_image(input_image, output_image, "Разность гауссианов"); 
                break;
            case 5:
                Canny(input_image, output_image, 50, 200);
                show_image(input_image, output_image, "Фильтр Кэнни"); 
                break;
            case 6:
                roberts(input_image, output_image);
                show_image(input_image, output_image, "Оператор Робертса"); 
                break;
            case 7:
                median_blure(input_image, output_image);
                show_image(input_image, output_image, "Медианная фильтрация"); 
                break;
            case 8:
                mosaic(input_image, output_image);
                show_image(input_image, output_image, "Мозаика");
                break;
            case 0:
                cout << "Выход";
                break;
            default:
                cout << "Неверная команда\n";         
                break;
        }

        if (input_key == 0) break;
    }
    return 0;
}

void show_image(const Mat input_img, Mat& output_img, string filter_name) {
    imshow("Исходное изображение", input_img);
    waitKey(0);
    imshow(filter_name, output_img);
    waitKey(0);
    destroyAllWindows();
    imwrite("D:/фото/"+ filter_name +".jpg", output_img);
}

void gauss_kernel(Mat& kernel, int aperture_size, float sigma)
{
    float sum = 0.0;
    for (int i = 0; i < aperture_size; i++) {
        for (int j = 0; j < aperture_size; j++) {
            float x = i - aperture_size / 2;
            float y = j - aperture_size / 2;
            kernel.at<float>(i, j) = exp(-(x * x + y * y) / (2 * sigma * sigma));
            sum += kernel.at<float>(i, j);
        }
    }
    kernel = kernel / sum;
}

void gauss_bluray(const Mat& input_image, Mat& output_image, int aperture_size, float sigma)
{   
    Mat kernel(aperture_size, aperture_size, CV_32F);
    gauss_kernel(kernel, aperture_size, sigma);

    //// Применение фильтра Гаусса к изображению 
    for (int i = aperture_size / 2; i < input_image.rows - aperture_size / 2; i++) {
        for (int j = aperture_size / 2; j < input_image.cols - aperture_size / 2; j++) {

            float sum = 0.0;

            for (int x = -aperture_size / 2; x <= aperture_size / 2; x++) {
                for (int y = -aperture_size / 2; y <= aperture_size / 2; y++) {
                    sum += kernel.at<float>(x + aperture_size / 2, y + aperture_size / 2) * input_image.at<uchar>(i + x, j + y);
                }
            }

            output_image.at<uchar>(i, j) = sum;
        }
    }
}

void gauss_contour(const Mat& input_image, Mat& output_image, int aperture_size, float sigma)
{
    output_image = input_image.clone();

    Mat kernel(aperture_size, aperture_size, CV_32F);
    gauss_kernel(kernel, aperture_size, sigma);
    

    // Применение фильтра контуров к изображению  
    for (int i = aperture_size / 2; i < input_image.rows - aperture_size / 2; i++) {
        for (int j = aperture_size / 2; j < input_image.cols - aperture_size / 2; j++) {

            float sum_x = 0.0;
            float sum_y = 0.0;

            for (int x = -aperture_size / 2; x <= aperture_size / 2; x++) {
                for (int y = -aperture_size / 2; y <= aperture_size / 2; y++) {
                    sum_x += kernel.at<float>(x + aperture_size / 2, y + aperture_size / 2) * input_image.at<uchar>(i + x, j + y) * x;
                    sum_y += kernel.at<float>(x + aperture_size / 2, y + aperture_size / 2) * input_image.at<uchar>(i + x, j + y) * y;
                }
            }

            output_image.at<uchar>(i, j) = sqrt(sum_x * sum_x + sum_y * sum_y);
        }
    }
}

void gauss_sharpness(const Mat& input_image, Mat& output_image, int aperture_size, float sigma, float amount)
{
    output_image = input_image.clone();
    Mat kernel(aperture_size, aperture_size, CV_32F);
    gauss_kernel(kernel, aperture_size, sigma);

    // Применение фильтра увеличения резкости к изображению  
    for (int i = aperture_size / 2; i < input_image.rows - aperture_size / 2; i++) {
        for (int j = aperture_size / 2; j < input_image.cols - aperture_size / 2; j++) {

            float sum_x = 0.0;
            float sum_y = 0.0;

            for (int x = -aperture_size / 2; x <= aperture_size / 2; x++) {
                for (int y = -aperture_size / 2; y <= aperture_size / 2; y++) {
                    sum_x += kernel.at<float>(x + aperture_size / 2, y + aperture_size / 2) * input_image.at<uchar>(i + x, j + y) * x;
                    sum_y += kernel.at<float>(x + aperture_size / 2, y + aperture_size / 2) * input_image.at<uchar>(i + x, j + y) * y;
                }
            }

            float val = sqrt(sum_x * sum_x + sum_y * sum_y);
            output_image.at<uchar>(i, j) = input_image.at<uchar>(i, j) + amount * (input_image.at<uchar>(i, j) - val);
        }
    }
}

void difference_of_gaussians(const Mat& input_image, Mat& output_image)
{

    Mat blurred_image3x3(input_image.rows, input_image.cols, CV_8UC1);
    Mat blurred_image5x5(input_image.rows, input_image.cols, CV_8UC1);

    gauss_bluray(input_image, blurred_image3x3, 3, 1.0);
    gauss_bluray(input_image, blurred_image5x5, 5, 3.0);

    for (int i = 0; i < input_image.rows; i++)
    {
        for (int j = 0; j < input_image.cols; j++)
        {
            int diff = blurred_image5x5.at<uchar>(i, j) - blurred_image3x3.at<uchar>(i, j);
            output_image.at<uchar>(i, j) = abs(diff);
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
void insertion_sort(int window[])
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
void median_blure(Mat& input_image, Mat& output_image) 
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
            insertion_sort(window);

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