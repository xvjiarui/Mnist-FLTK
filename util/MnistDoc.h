#ifndef MNIST_DOC_H
#define MNIST_DOC_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;
class MnistUI;
class MnistDoc
{
public:
	MnistDoc();
	void setUI(MnistUI* ui);
	~MnistDoc();

	unsigned char* getTrainImage(int index);
	unsigned char getTrainLabel(int index);
	unsigned char* getTestImage(int index);
	unsigned char getTestLabel(int index);
	int getSizeTrainImage();
	int getSizeTestImage();


	
	static const string trainImageFilePath;
	static const string trainLabelFilePath;
	static const string testImageFilePath;
	static const string testLabelFilePath;
private:
	MnistUI* m_mnistUI;
	int reverseInt(int i);
	unsigned char** readMnistImage(string full_path, int& number_of_images, int& image_size);
	unsigned char* readMnistLabel(string full_path, int& number_of_labels);

	unsigned char** m_trainX;
	unsigned char* m_trainY;
	unsigned char** m_testX;
	unsigned char* m_testY;

	int numTrainImage;
	int numTrainLabel;
	int sizeTrainImage;

	int numTestImage;
	int numTestLabel;
	int sizeTestImage;
};

#endif
