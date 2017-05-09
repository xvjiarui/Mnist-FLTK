#ifndef SOFTMAX_H
#define SOFTMAX_H


typedef enum GradientType
{
	BGD,
	SGD,
	MBGD	
} GradientType;

class MnistDoc;
class MnistUI;
class Softmax
{
public:
	Softmax(MnistUI* ui, MnistDoc* doc);
	void softmax_evidence();
	void softmax_evidence_test();
	double get_evidence(int imageIndex, int classIndex);
	double get_evidence_test(int imageIndex, int classIndex);
	unsigned char get_prediction(int imageIndex);
	unsigned char get_prediction_test(int imageIndex);
	unsigned char label_distribution(int imageIndex, int classIndex);
	unsigned char label_distribution_test(int imageIndex, int classIndex);
	void cross_entropy_evidence();
	double get_cross_entropy();
	double get_softmax_theta_gradient(int classIndex, int featureIndex);
	double get_softmax_theta_gradient(int imageIndex, int classIndex, int featureIndex);
	double get_softmax_bias_gradient(int classIndex);
	double get_softmax_bias_gradient(int imageIndex, int classIndex);
	void train();
	void evaluate();
	~Softmax();

private:
	MnistUI* m_mnistUI;
	MnistDoc* m_mnistDoc;
	double **theta;         // model parameter, K*D matrix
	double **x;      // training features, N*D matrix
	unsigned char *y;       // training labels, N*1 matrix
	double **xt; 	// testing features, Nt*D matrix
	unsigned char *yt;		// testing labels, Nt*1 matrix
	double *bias;			// bias. K*1 matrix
	double **prediction_distribution; // evidence after softmax, prediction distribution, N*K matrix
	double **test_distribution; // Nt*K matrix
	double cross_entropy;	// cross_entropy err
	int K;                  // number of classes
	int D;                  // dimension of features
	int N;                  // number of training points
	int Nt;					// number of testing points
	int epoch;              // number of training iterations
	double alpha;           // learning rate
	double lambda;          // weight of regularization term
	int cur_epoch;

};

#endif