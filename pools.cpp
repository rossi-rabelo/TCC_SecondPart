/* Primeira versão, focarei no tratamento de uma única imagem, depois irei adaptar o código para funcionar para uma pasta de imagens*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <dirent.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

/*****************************************************************************************************************************************************************/


cv::Mat leimagem(String str){           //Função para ler uma imagem de nome str
    cv:: Mat image;
    image = cv::imread (str, CV_LOAD_IMAGE_COLOR); //Função de leitura
    return image;
}

/*****************************************************************************************************************************************************************/

cv::Mat convertehsv(Mat image){     //Função para converter uma imagem de rgb para hsv 

    cv:: Mat hsv_image;
    cv::cvtColor(image, hsv_image, CV_BGR2HSV); //Função de conversão

    return hsv_image;
}

/*****************************************************************************************************************************************************************/


int main ( int argc, char** argv ) {

    cv::Mat image;
	image = leimagem("../imteste2.JPG");
	 // Se não for possível abrir a imagem
	if(! image.data ) {
	      std::cout <<  "Could not open or find the image" << std::endl ;
	      return -1;
	}

	int linhas = image.rows;
	int colunas = image.cols;

	cv::Mat rgb[3];
	split(image,rgb); //Separando os canais rgb da imagem

	cv::Mat blueFilter = rgb[0] - (rgb[1] + rgb[2])/2;
	/***************************************************************************************************************************/
	// Binarizando a imagem resultante pelo método de otsu
	cv::Mat otsu_image;
	cv::threshold(blueFilter, otsu_image, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	// Binarizando a imagem utilizando um limiar de valor 30
	cv::Mat segmented_image;
	cv::threshold(blueFilter, segmented_image, 30, 255, CV_THRESH_BINARY);

	// Fazendo a operação de "e lógico" entre as duas imagens
	cv::Mat threshold_image;
	threshold_image = otsu_image & segmented_image;

	/*Escrevendo a imagem resultante*/
	
	imwrite("../result.JPG", threshold_image);
	imwrite("../resultotsu.JPG", otsu_image);
	imwrite("../resultseg.JPG", segmented_image);
	cv::waitKey(0);
    return 0;
}