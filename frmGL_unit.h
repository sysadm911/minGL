// ---------------------------------------------------------------------------

#ifndef frmGL_unitH
#define frmGL_unitH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "RzPanel.hpp"
#include "RzStatus.hpp"
#include "RzButton.hpp"
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

// ---------------------------------------------------------------------------
class TfrmGL : public TForm {
__published: // IDE-managed Components
	TRzStatusBar *RzStatusBar1;
	TRzToolbar *RzToolbar1;
	TRzClockStatus *RzClockStatus1;
	TRzResourceStatus *RzResourceStatus1;
	TImageList *ImageList1;
	TRzToolButton *RzToolButton1;
	TColorDialog *ColorDialog1;
	TRzPanel *RzPanel1;
	TRzFieldStatus *RzFieldStatus1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall RzToolButton1Click(TObject *Sender);
	void __fastcall RzPanel1Paint(TObject *Sender);
	void __fastcall RzPanel1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

private: // User declarations
	HWND hwnd;
	HGLRC hrc;
	HDC dc;
	GLfloat R, G, B, xpos, ypos;

	void __fastcall SetDCPixelFormat(HDC hdc);
	void __fastcall ColorToGL(TColor c, GLfloat &R, GLfloat &G, GLfloat &B);
public: // User declarations

	__fastcall TfrmGL(TComponent* Owner);

	// void __fastcall WMPaint(TWMPaint Msg );
	// BEGIN_MESSAGE_MAP
	// MESSAGE_HANDLER(WM_PAINT, TWMPaint, WMPaint)
	// END_MESSAGE_MAP(TForm)
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmGL *frmGL;
// ---------------------------------------------------------------------------
#endif