/* Primeira versão, focarei no tratamento de uma única imagem, depois irei adaptar o código para funcionar para uma pasta de imagens*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <dirent.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

int main ( int argc, char** argv ) {

    cv::Mat image;
    image = cv::imread ("../imteste1.JPG", CV_LOAD_IMAGE_COLOR);  	//Carregando a imagem a ser convertida
	int linhas = image.rows;
	int colunas = image.cols;
	
    // Se não for possível abrir a imagem
	if(! image.data ) {
	      std::cout <<  "Could not open or find the image" << std::endl ;
	      return -1;
	}

    return 0;
}