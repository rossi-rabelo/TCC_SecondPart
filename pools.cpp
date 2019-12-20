/* Primeira versão, focarei no tratamento de uma única imagem, depois irei adaptar o código para funcionar para uma pasta de imagens*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <dirent.h>
#include <limits.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

/*****************************************************************************************************************************************************************/
//DEFINIÇÃO DE STRUCTURES

typedef struct
{
	int codElement;
	int sizeElement;
} ComponentSize;

/*****************************************************************************************************************************************************************/

cv::Mat leimagem(String str)
{ //Função para ler uma imagem de nome str
	cv::Mat image;
	image = cv::imread(str, CV_LOAD_IMAGE_COLOR); //Função de leitura
	return image;
}

/*****************************************************************************************************************************************************************/

cv::Mat convertehsv(Mat image)
{ //Função para converter uma imagem de rgb para hsv

	cv::Mat hsv_image;
	cv::cvtColor(image, hsv_image, CV_BGR2HSV); //Função de conversão

	return hsv_image;
}

/*****************************************************************************************************************************************************************/

cv::Mat erode_imagem(Mat image)
{ //Função para erodir a imagem e eliminar objetos irrelevantes

	cv::Mat erodedImage;

	int raio = 15;																									 // Raio da erosão
	Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size((2 * raio) + 1, (2 * raio) + 1), Point(-1, -1)); //Definindo a matriz de dilatação.

	cv::erode(image, erodedImage, element, Point(-1, -1), 1, 1, 1);

	return erodedImage;
}

/*****************************************************************************************************************************************************************/

cv::Mat dilata_imagem(Mat someImage)
{ //Função que irá dilatar a imagem no raio escolhido;

	cv::Mat dilated_image;

	int raio = 15; //Raio da dilatação.

	Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size((2 * raio) + 1, (2 * raio) + 1), Point(-1, -1)); //Definindo a matriz de dilatação.

	cv::dilate(someImage, dilated_image, element, Point(-1, -1), 1, 1, 1);

	return dilated_image;
}

/*****************************************************************************************************************************************************************/

cv::Mat componentes_conexos(Mat image)
{ //Função que irá realizar a extração dos componentes conexos da imagem;

	cv::Mat connected_components;

	cv::connectedComponents(image, connected_components, 8, CV_32S, CCL_DEFAULT); //Obter os componentes conexos.

	return connected_components;
}

/*****************************************************************************************************************************************************************/

int num_componentes(Mat connected_components)
{ //Função que retorna o número de componentes conexos da imagem;

	int i, j;
	int num_componentes = 0;
	for (i = 0; i < connected_components.rows; i++)
	{ //Obtendo o número de componentes conexos da imagem.
		for (j = 0; j < connected_components.cols; j++)
		{
			if (num_componentes < connected_components.at<int>(i, j))
			{
				num_componentes = connected_components.at<int>(i, j);
			}
		}
	}

	return num_componentes;
}

/*****************************************************************************************************************************************************************/

void ordenaComponentes(int *componentesCop, int numComponentes, vector<ComponentSize> &imgComponents) {
	int menor = INT_MAX;
	int index = 0;
	ComponentSize auxImgComponents;
	for (int i = 0; i < numComponentes; i++)
	{
		for (int j = 0; j < numComponentes; j++)
		{
			if (componentesCop[j] < menor)
			{
				menor = componentesCop[j];
				index = j;
			}
		}
		auxImgComponents.codElement = index;
		auxImgComponents.sizeElement = menor;
		imgComponents.push_back(auxImgComponents);
		componentesCop[index] = INT_MAX;
		menor = INT_MAX;
	}
}

/*****************************************************************************************************************************************************************/

int main(int argc, char **argv)
{

	cv::Mat image;
	image = leimagem("../imteste3.JPG");
	// Se não for possível abrir a imagem
	if (!image.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	int linhas = image.rows;
	int colunas = image.cols;

	cv::Mat rgb[3];
	split(image, rgb); //Separando os canais rgb da imagem

	//BGR
	cv::Mat blueFilter = rgb[0] - (rgb[1] + rgb[2]) / 2;
	/***************************************************************************************************************************/
	// Binarizando a imagem resultante pelo método de otsu
	cv::Mat otsu_image;
	cv::threshold(blueFilter, otsu_image, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	// Binarizando a imagem utilizando um limiar de valor 30
	cv::Mat segmented_image;
	cv::threshold(blueFilter, segmented_image, 30, 255, CV_THRESH_BINARY);

	// Obtendo azuis com tons esverdeados na imagem
	cv::Mat blueFilter2 = ((rgb[1] - rgb[2]) > 50) & (rgb[0] > 140);

	// Fazendo a operação de "e lógico" entre as duas imagens
	cv::Mat threshold_image;
	threshold_image = otsu_image & segmented_image;

	cv::Mat binary_image;
	binary_image = blueFilter2 | threshold_image;

	// Erodir e dilatar a imagem resultante a fim de eliminar objetos irrelevantes

	cv::Mat eroded_image;
	eroded_image = erode_imagem(binary_image);

	// Etapa de dilatação
	cv::Mat dilated_image;
	dilated_image = dilata_imagem(eroded_image);

	// Extração de componentes conexos
	cv::Mat connected_components;
	connected_components = componentes_conexos(dilated_image);

	int numComponentes = num_componentes(connected_components);

	/**************************** Fazer o tratamento da imagem dos componentes conexos ********************************/

	// Alocar um  vetor de n posições, sendo n, o número de componentes conexos na imagem.

	int *componentes;
	componentes = (int *)malloc((numComponentes + 1) * sizeof(int)); //Atribuindo o tamanho do vetor como o valor máximo obtido da imagem

	for (int i = 0; i < (numComponentes + 1); i++)
	{ //Zerando o vetor
		componentes[i] = 0;
	}

	// Popular o vetor com os valores da imagem, o vetor no indice i deve conter o tamanho do objeto de valor de i em pixels

	int pos;

	for (int i = 0; i < linhas; i++)
	{
		for (int j = 0; j < colunas; j++)
		{
			pos = connected_components.at<int>(i, j);
			componentes[pos] = componentes[pos] + 1;
		}
	}

	// Depois de obter o array com o tamanho de cada componente, iremos atribui-lo na structure e ordena-lo,
	vector <ComponentSize> imgComponents;
	// Depois de declarar o novo array, iremos inserir de forma ordenada no vetor
	int componentesCop[numComponentes];
	for (int i = 0; i < numComponentes; i++)
	{
		componentesCop[i] = componentes[i];
	}
	
	ordenaComponentes(componentesCop, numComponentes, imgComponents);

	for (int i = 0; i < numComponentes; i++) {
		cout << imgComponents[i].codElement << ' ' << imgComponents[i].sizeElement << endl;
	}

	// Aqui o vetor imgComponents já está ordenado
	/******************************************************************************************************************/

	/*Escrevendo a imagem resultante*/

	/***************************************************************************************************************************/

	imwrite("../connectedComponents.JPG", connected_components);

	cv::waitKey(0);
	return 0;
}