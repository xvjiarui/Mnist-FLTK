#include "Softmax.h"
#include "util/MnistDoc.h"
#include "ui/MnistUI.h"
#include <math.h> 
#include <time.h>

double randomFloatRange(double a, double b) {
  double ret = ((double)rand())/((double)RAND_MAX); // in range (0, 1)
  return ret*(b-a)+a;
}

Softmax::Softmax(MnistUI* ui, MnistDoc* doc)
{
	srand(time(NULL));
	m_mnistUI = ui;
	m_mnistDoc = doc;
	K = 10;
	D = m_mnistDoc->getSizeTrainImage();
	N = m_mnistDoc->getNumTrainImage();
	epoch = 30;
	alpha = 0.1;
	lambda = 0;
	theta = new double*[K];
	bias = new double [K];
	for (int i = 0; i < K; ++i)
	{
		theta[i] = new double[D];
		for (int j = 0; j < D; ++j)
		{
			theta[i][j] = randomFloatRange(-0.01, 0.01);
		}
		bias[i] = randomFloatRange(-0.01, 0.01);

	}
	x = new double* [N];
	y = new unsigned char [N];
	prediction_distribution = new double* [N];
	for (int i = 0; i < N; ++i)
	{
		x[i] = m_mnistDoc->getTrainImage(i);
		y[i] = m_mnistDoc->getTrainLabel(i);
		prediction_distribution[i] = new double[K];
		for (int j = 0; j < K; ++j)
		{
			prediction_distribution[i][j] = 1.0/K;
		}
	}
}

void Softmax::softmax_evidence()
{
	// ToDo:in case of overflow
	double total[N];
	double raw_evidence[N][K];
	double raw_max[N];
	for (int i = 0; i < N; ++i)
	{
		// iteration of i th image
		raw_max[i] = get_evidence(i, 0);
		for (int j = 0; j < K; ++j)
		{
			// iteration of k th class
			raw_evidence[i][j] = get_evidence(i, j);
			if (raw_evidence[i][j] > raw_max[i])
			{
				raw_max[i] = raw_evidence[i][j];
			}
		}
		total[i] = 0;
		for (int j = 0; j < K; ++j)
		{
			// iteration of k th class
			total[i] += exp(raw_evidence[i][j]-raw_max[i]);
			// printf("%f\n", raw_evidence[i][j] - raw_max[i]);
		}
		for (int j = 0; j < K; ++j)
		{
			// iteration of k th class

			prediction_distribution[i][j] = exp(raw_evidence[i][j] - raw_max[i])/total[i];
			// printf("pd:%f\n", prediction_distribution[i][j]);
			// printf("%f %f\n",(raw_evidence[i][j]),  total[i]);
		}
		for (int j = 0; j < K; ++j)
		{
			for (int k = 0; k < D; ++k)
			{
				theta[j][k] -= alpha / (1 + cur_epoch) * get_softmax_theta_gradient(i, j, k);
			}
			bias[j] -= alpha / (1 + cur_epoch) * get_softmax_bias_gradient(i, j);
		}
	}
}
void Softmax::softmax_evidence_test()
{
	// ToDo:in case of overflow
	double total[Nt];
	double raw_evidence[Nt][K];
	double raw_max[Nt];
	for (int i = 0; i < Nt; ++i)
	{
		raw_max[i] = get_evidence_test(i, 0);
		// iteration of i th image
		for (int j = 0; j < K; ++j)
		{
			// iteration of k th class
			raw_evidence[i][j] = get_evidence_test(i, j);
			if (raw_evidence[i][j] > raw_max[i])
			{
				raw_max[i] = raw_evidence[i][j];
			}
		}
		total[i] = 0;
		for (int j = 0; j < K; ++j)
		{
			// iteration of k th class
			total[i] += exp(raw_evidence[i][j]-raw_max[i]);
		}
		for (int j = 0; j < K; ++j)
		{
			// iteration of k th class
			test_distribution[i][j] = exp(raw_evidence[i][j] - raw_max[i])/total[i];
		}
	}
}

double Softmax::get_evidence(int imageIndex, int classIndex)
{
	double ret = 0;
	for (int i = 0; i < D; ++i)
	{
		ret += theta[classIndex][i] * x[imageIndex][i];
	}
	ret += bias[classIndex];
	// printf("ret%f\n", ret);
	return ret;
}

double Softmax::get_evidence_test(int imageIndex, int classIndex)
{

	double ret = 0;
	for (int i = 0; i < D; ++i)
	{
		ret += theta[classIndex][i] * xt[imageIndex][i];
	}
	ret += bias[classIndex];
	return ret;
}

unsigned char Softmax::get_prediction(int imageIndex)
{
	unsigned char ret = 0;
	double max = 0;
	for (int i = 0; i < K; ++i)
	{
		if (prediction_distribution[imageIndex][i] > max)
		{
			max = prediction_distribution[imageIndex][i];
			ret = i;
		}
	}
	return ret;
}

unsigned char Softmax::get_prediction_test(int imageIndex)
{
	unsigned char ret = 0;
	double max = 0;
	for (int i = 0; i < K; ++i)
	{
		if (test_distribution[imageIndex][i] > max)
		{
			max = test_distribution[imageIndex][i];
			ret = i;
		}
	}
	return ret;
}

unsigned char Softmax::label_distribution(int imageIndex, int classIndex)
{
	return (y[imageIndex] == classIndex) ? 1:0;
}
		
unsigned char Softmax::label_distribution_test(int imageIndex, int classIndex)
{
	return (yt[imageIndex] == classIndex) ? 1:0;
}

void Softmax::cross_entropy_evidence()
{
	cross_entropy = 0;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < K; ++j)
		{
			// printf("log:%f\n", log(prediction_distribution[i][j]));
			cross_entropy -= label_distribution(i, j) * log(prediction_distribution[i][j]);
		}
	}
}

double Softmax::get_softmax_theta_gradient(int classIndex, int featureIndex)
{
	double ret = 0;
	for (int i = 0; i < N; ++i)
	{
		ret += (prediction_distribution[i][classIndex] - label_distribution(i, classIndex)) * x[i][featureIndex];
	}
	return ret/N;
}

double Softmax::get_softmax_theta_gradient(int imageIndex, int classIndex, int featureIndex)
{
	return (prediction_distribution[imageIndex][classIndex] - label_distribution(imageIndex, classIndex)) * x[imageIndex][featureIndex];
}

double Softmax::get_softmax_bias_gradient(int classIndex)
{
	double ret = 0;
	for (int i = 0; i < N; ++i)
	{
		ret += (prediction_distribution[i][classIndex] - label_distribution(i, classIndex));		
	}	
	return ret/N;
}

double Softmax::get_softmax_bias_gradient(int imageIndex, int classIndex)
{
	return (prediction_distribution[imageIndex][classIndex] - label_distribution(imageIndex, classIndex));
}

void Softmax::train()
{
	for (cur_epoch = 0; cur_epoch < epoch; ++cur_epoch)
	{
		printf("training%d\n", cur_epoch);

		softmax_evidence();
		cross_entropy_evidence();
		printf("err:%f\n", cross_entropy);
	}
}

void Softmax::evaluate()
{
	// load testImage 
	Nt = m_mnistDoc->getNumTestImage();
	printf("Nt%d\n", Nt);
	xt = new double*[Nt];
	yt = new unsigned char [Nt];
	test_distribution = new double* [Nt];
	for (int i = 0; i < Nt; ++i)
	{
		xt[i] = m_mnistDoc->getTestImage(i);
		yt[i] = m_mnistDoc->getTestLabel(i);
		test_distribution[i] = new double [K];
	}
	softmax_evidence_test();
	int err = 0;
	for (int i = 0; i < Nt; ++i)
	{
		if (yt[i] != get_prediction_test(i))
		{
			err++;
		}
	}
	printf("err_rate:%f\n",err/(double)Nt );



	for (int i = 0; i < Nt; ++i)
	{
		delete[] xt[i];
	}
	delete[] xt;
	delete[] yt;
}

Softmax::~Softmax()
{
	for (int i = 0; i < K; ++i)
	{
		delete[] theta[i];
	}
	delete theta;
	for (int i = 0; i < N; ++i)
	{
		delete[] x[i];
		delete[] prediction_distribution[i];
	}
	delete[] x;
	delete[] bias;
	delete[] prediction_distribution;
	delete[] y;
}