#include "MnistUI.h"
#include <stdio.h>
#include <stdlib.h>
#include "util/MnistDoc.h"

Fl_Menu_Item MnistUI::menuItems[] = {
	{"&File", 0, 0, 0, FL_SUBMENU},
		{0},
	{"&View", 0, 0, 0, FL_SUBMENU},
		{0},
	{0}
};
MnistUI::MnistUI(){
	// initialize data
	m_nPreviewIndex = -1;
	m_imageBuffer = NULL;
	m_mainWindow = new Fl_Window(600, 600, "Mnist");
	m_mainWindow->user_data((void*)(this));

	m_menuBar = new Fl_Menu_Bar(0, 0, 600, 20);
	m_menuBar->menu(menuItems);
	 //---To install an int input-----------------------
	m_previewIndexInput = new Fl_Input(100, 50, 40, 20, "Preview Index");
	m_previewIndexInput->user_data((void*)(this));   // record self to be used by static callback functions
	m_previewIndexInput->callback(cb_preview_index_input);
	m_previewIndexInput->static_value("0");
	m_previewIndexInput->when(FL_WHEN_CHANGED);

	m_previewImage = new Fl_RGB_Image(m_imageBuffer, 28, 28, 1);
	m_previewImage->draw(100, 100, 100, 100);
	m_previewBox = new Fl_Box(20, 100, 100, 100);
	m_previewBox->box(FL_UP_BOX);
	m_previewBox->image(m_previewImage);

	m_mainWindow->end();
}

MnistUI::~MnistUI()
{
	if (m_imageBuffer)
	{
		delete m_imageBuffer;
	}
}

void MnistUI::show()
{
	m_mainWindow->show();
}

void MnistUI::setDoc(MnistDoc* doc)
{
	m_mnistDoc = doc;
}

void MnistUI::cb_preview_index_input(Fl_Widget* o, void* v)
{
	MnistUI* m_UI = (MnistUI*)(o->user_data());
	m_UI->m_nPreviewIndex=atoi( ((Fl_Input *)o)->value() );
	if (m_UI->m_imageBuffer)
	{
		delete[] m_UI->m_imageBuffer;
	}

	// convert unsigned char in to a const char* to int first hhh
	int tmp =  m_UI->m_mnistDoc->getTrainLabel(m_UI->m_nPreviewIndex);
	sprintf(&m_UI->m_cPreviewLabel,"%d", tmp);
	
	double * temp = m_UI->m_mnistDoc->getTrainImage(m_UI->m_nPreviewIndex);
	m_UI->m_imageBuffer = new unsigned char[m_UI->m_mnistDoc->getSizeTrainImage()];
	for (int i = 0; i < m_UI->m_mnistDoc->getSizeTrainImage(); ++i)
	{
		m_UI->m_imageBuffer[i] = 255 * temp[i];
	}
	delete[] temp;
	delete m_UI->m_previewImage;
	m_UI->m_previewImage = new Fl_RGB_Image(m_UI->m_imageBuffer, 28, 28, 1);
	m_UI->m_previewBox->image(m_UI->m_previewImage);
	m_UI->m_previewBox->label(&m_UI->m_cPreviewLabel);
	m_UI->m_mainWindow->redraw();
}