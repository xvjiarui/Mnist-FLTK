#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include "ui/MnistUI.h"
#include "util/MnistDoc.h"
#include "model/Softmax.h"

MnistUI* mnistUI;
MnistDoc* mnistDoc;
Softmax* softmax;


int main(int argc, char const *argv[])
{
	mnistUI = new MnistUI();
	mnistDoc = new MnistDoc();
	softmax = new Softmax(mnistUI, mnistDoc);
	mnistUI->setDoc(mnistDoc);
	mnistDoc->setUI(mnistUI);
	mnistUI->show();
	softmax->train();
	softmax->evaluate();
	return Fl::run();
}