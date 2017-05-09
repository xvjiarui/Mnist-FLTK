#include "util/MnistDoc.h"
#include "ui/MnistUI.h"

const string MnistDoc::trainImageFilePath = "data/train-images-idx3-ubyte";
const string MnistDoc::trainLabelFilePath = "data/train-labels-idx1-ubyte";
const string MnistDoc::testImageFilePath = "data/t10k-images-idx3-ubyte";
const string MnistDoc::testLabelFilePath = "data/t10k-labels-idx1-ubyte";


MnistDoc::MnistDoc()
{
	m_trainX = readMnistImage(trainImageFilePath, numTrainImage, sizeTrainImage);
	m_trainY = readMnistLabel(trainLabelFilePath, numTrainLabel);

	m_testX = readMnistImage(testImageFilePath, numTestImage, sizeTestImage);
	m_testY = readMnistLabel(testLabelFilePath, numTestLabel);

	printf("numTrainImage:%d\n", numTrainImage);
	printf("numTrainLabel:%d\n", numTrainLabel);
	printf("sizeTrainImage:%d\n", sizeTrainImage);

	printf("numTestImage:%d\n", numTestImage);
	printf("numTestLabel:%d\n", numTestLabel);
	printf("sizeTestImage:%d\n", sizeTestImage);
}

void MnistDoc::setUI(MnistUI* ui)
{
	m_mnistUI = ui;
}

MnistDoc::~MnistDoc()
{
	for (int i = 0; i < numTrainImage; ++i)
	{
		delete []m_trainX[i];
	}
	delete []m_trainX;
	delete []m_trainY;

	for (int i = 0; i < numTestImage; ++i)
	{
		delete []m_testX[i];
	}
	delete []m_testX;
	delete []m_testY;
}

double* MnistDoc::getTrainImage(int index)
{
	double* ret = new double[sizeTrainImage];
    for (int i = 0; i < sizeTrainImage; ++i)
    {
        ret[i] = m_trainX[index][i]/255.0;
    }
	
	return ret;
}

unsigned char MnistDoc::getTrainLabel(int index)
{
	unsigned char ret = m_trainY[index]; 
	return ret;
}

double* MnistDoc::getTestImage(int index)
{
	double* ret = new double[sizeTestImage];
    for (int i = 0; i < sizeTestImage; ++i)
    {
        ret[i] = m_testX[index][i]/255.0;
    }

	return ret;
}
unsigned char MnistDoc::getTestLabel(int index)
{
	unsigned char ret = m_testY[index];
	return ret;
}

int MnistDoc::getNumTrainImage()
{
    return numTrainImage;
}

int MnistDoc::getSizeTrainImage()
{
	return sizeTrainImage;
}

int MnistDoc::getNumTestImage()
{
    return numTestImage;
}

int MnistDoc::getSizeTestImage()
{
	return sizeTestImage;
}

int MnistDoc::reverseInt (int i) 
{
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

unsigned char** MnistDoc::readMnistImage(string full_path, int& number_of_images, int& image_size) {

    ifstream file(full_path, ios::binary);

    if(file.is_open()) {
        int magic_number = 0, n_rows = 0, n_cols = 0;

        file.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        if(magic_number != 2051) throw runtime_error("Invalid MNIST image file!");

        file.read((char *)&number_of_images, sizeof(number_of_images)), number_of_images = reverseInt(number_of_images);
        file.read((char *)&n_rows, sizeof(n_rows)), n_rows = reverseInt(n_rows);
        file.read((char *)&n_cols, sizeof(n_cols)), n_cols = reverseInt(n_cols);

        image_size = n_rows * n_cols;

        unsigned char** _dataset = new unsigned char*[number_of_images];
        for(int i = 0; i < number_of_images; i++) {
            _dataset[i] = new unsigned char[image_size];
            file.read((char *)_dataset[i], image_size);
        }
        return _dataset;
    } else {
        throw runtime_error("Cannot open file `" + full_path + "`!");
    }
}

unsigned char* MnistDoc::readMnistLabel(string full_path, int& number_of_labels) {

    ifstream file(full_path, ios::binary);

    if(file.is_open()) {
        int magic_number = 0;
        file.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        if(magic_number != 2049) throw runtime_error("Invalid MNIST label file!");

        file.read((char *)&number_of_labels, sizeof(number_of_labels)), number_of_labels = reverseInt(number_of_labels);

        unsigned char* _dataset = new unsigned char[number_of_labels];
        for(int i = 0; i < number_of_labels; i++) {
            file.read((char*)&_dataset[i], 1);
        }
        return _dataset;
    } else {
        throw runtime_error("Unable to open file `" + full_path + "`!");
    }
}