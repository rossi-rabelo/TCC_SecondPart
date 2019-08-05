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
	image = leimagem("../imteste1.JPG");
	 // Se não for possível abrir a imagem
	if(! image.data ) {
	      std::cout <<  "Could not open or find the image" << std::endl ;
	      return -1;
	}

	int linhas = image.rows;
	int colunas = image.cols;

	cv:: Mat hsv_image;
	hsv_image = convertehsv(image); //Convertendo a imagem para hsv
	

	/***************************************************************************************************************************/
	/*Escrevendo a imagem resultante*/

	cv:: imwrite("test_hsv.jpg", hsv_image);


	cv::waitKey(0);
    return 0;
}