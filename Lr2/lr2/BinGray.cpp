//////////////////////���������� ����������� �������////////////////////////////
//
//cv::Mat grayscaleImage(input_image.rows, input_image.cols, CV_8UC1);
//for (int i = 0; i < input_image.rows; i++) {
//    for (int j = 0; j < input_image.cols; j++) {
//        cv::Vec3b intensity = input_image.at<cv::Vec3b>(i, j);
//        // ������� �������� ������������� (������, ��� (R+G+B)/3)
//        uchar grayValue = (intensity[0] + intensity[1] + intensity[2]) / 3;
//        grayscaleImage.at<uchar>(i, j) = grayValue;
//    }
//}
//
//// ����� ����������� ����������� �� �����
//show_image(input_image, grayscaleImage, "����������� �����������");
//
//////////////////////////�������� ����������� �������////////////////////////////////////////////////////
//
//    // �������������� ����������� � ��������
//cv::Mat binaryImage(input_image.rows, input_image.cols, CV_8UC1);
//int threshold = 128;  // ��������� �������� ��� �����������
//for (int i = 0; i < input_image.rows; i++) {
//    for (int j = 0; j < input_image.cols; j++) {
//        if (input_image.at<uchar>(i, j) >= threshold) {
//            binaryImage.at<uchar>(i, j) = 255;  // ������������� ������� ������� � 255 (�����)
//        }
//        else {
//            binaryImage.at<uchar>(i, j) = 0;    // ������������� ������� ������� � 0 (������)
//        }
//    }
//}
//
//// ����� ��������� ����������� �� �����
//show_image(input_image, binaryImage, "��������� �����������");
//
//Mat binResult = binaryImage.clone();
//Mat monoResult = grayscaleImage.clone();