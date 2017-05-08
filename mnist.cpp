#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include "ui/MnistUI.h"
#include "util/MnistDoc.h"

MnistUI* mnistUI;
MnistDoc* mnistDoc;

int main(int argc, char const *argv[])
{
	mnistUI = new MnistUI();
	mnistDoc = new MnistDoc();
	mnistUI->setDoc(mnistDoc);
	mnistDoc->setUI(mnistUI);
	mnistUI->show();
	return Fl::run();
}