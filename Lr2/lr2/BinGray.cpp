//////////////////////ОДНОТОННОЕ ИЗОБРАЖЕНИЕ ВРУЧНУЮ////////////////////////////
//
//cv::Mat grayscaleImage(input_image.rows, input_image.cols, CV_8UC1);
//for (int i = 0; i < input_image.rows; i++) {
//    for (int j = 0; j < input_image.cols; j++) {
//        cv::Vec3b intensity = input_image.at<cv::Vec3b>(i, j);
//        // Среднее значение интенсивности (обычно, это (R+G+B)/3)
//        uchar grayValue = (intensity[0] + intensity[1] + intensity[2]) / 3;
//        grayscaleImage.at<uchar>(i, j) = grayValue;
//    }
//}
//
//// Вывод однотонного изображения на экран
//show_image(input_image, grayscaleImage, "однотонного изображения");
//
//////////////////////////БИНАРНОЕ ИЗОБРАЖЕНИЕ ВРУЧНУЮ////////////////////////////////////////////////////
//
//    // Преобразование изображения в бинарное
//cv::Mat binaryImage(input_image.rows, input_image.cols, CV_8UC1);
//int threshold = 128;  // Пороговое значение для бинаризации
//for (int i = 0; i < input_image.rows; i++) {
//    for (int j = 0; j < input_image.cols; j++) {
//        if (input_image.at<uchar>(i, j) >= threshold) {
//            binaryImage.at<uchar>(i, j) = 255;  // Устанавливаем яркость пикселя в 255 (белый)
//        }
//        else {
//            binaryImage.at<uchar>(i, j) = 0;    // Устанавливаем яркость пикселя в 0 (черный)
//        }
//    }
//}
//
//// Вывод бинарного изображения на экран
//show_image(input_image, binaryImage, "бинарного изображения");
//
//Mat binResult = binaryImage.clone();
//Mat monoResult = grayscaleImage.clone();