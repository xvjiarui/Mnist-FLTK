#ifndef MNIST_UI_H
#define MNIST_UI_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Box.H>

class MnistDoc;
class MnistUI
{
public:
	MnistUI();
	~MnistUI();
	void show();
	void setDoc(MnistDoc* doc);
	Fl_Window* m_mainWindow;
	Fl_Menu_Bar* m_menuBar;
	Fl_Input* m_previewIndexInput;
	Fl_RGB_Image* m_previewImage;
	Fl_Box* m_previewBox;

	// static callback functions
	static void cb_preview_index_input(Fl_Widget* o, void* v);
private:
	MnistDoc* m_mnistDoc;
	static Fl_Menu_Item menuItems[];
	int m_nPreviewIndex;
	unsigned char* m_imageBuffer;	
};

#endif