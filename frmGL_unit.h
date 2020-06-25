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
#include <math.h>
#include "RzSplit.hpp"
#include <Menus.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#define Pi M_PI

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
	TRzSplitter *RzSplitter1;
	TRzPanel *RzPanel2;
	TRzSpacer *RzSpacer1;
	TRzToolButton *RzToolButton2;
	TPopupMenu *PopupMenu1;
	TMenuItem *OpenGLGDI1;
	TMenuItem *OpenGL1;
	TMenuItem *OpenGLlines1;
	TMenuItem *OpenGLOxOy1;
	TMenuItem *BezierCurvewithFourControlPoints1;
	TMenuItem *OpenGLlinesstrip1;
	TMenuItem *OpenGLlinesloop1;
	TMenuItem *Randomlines1;
	TMenuItem *riangles1;
	TMenuItem *Rectangle1;
	TMenuItem *Disk1;
	TMenuItem *DrawVertexArray1;
	TMenuItem *DrawVertexArrayround1;
	TMenuItem *P1;
	TRzFieldStatus *RzGL_VENDOR;
	TRzFieldStatus *RzGL_RENDERER;
	TRzFieldStatus *RzGL_VERSION;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall RzToolButton1Click(TObject *Sender);
	void __fastcall RzPanel1Paint(TObject *Sender);
	void __fastcall OpenGLGDI2Click(TObject *Sender);
	void __fastcall OpenGL1Click(TObject *Sender);
	void __fastcall OpenGLlines1Click(TObject *Sender);
	void __fastcall OpenGLOxOy1Click(TObject *Sender);
	void __fastcall BezierCurvewithFourControlPoints1Click(TObject *Sender);
	void __fastcall OpenGLlinesstrip1Click(TObject *Sender);
	void __fastcall OpenGLlinesloop1Click(TObject *Sender);
	void __fastcall RzPanel2MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Randomlines1Click(TObject *Sender);
	void __fastcall RzPanel2Paint(TObject *Sender);
	void __fastcall riangles1Click(TObject *Sender);
	void __fastcall Rectangle1Click(TObject *Sender);
	void __fastcall Disk1Click(TObject *Sender);
	void __fastcall DrawVertexArray1Click(TObject *Sender);
	void __fastcall DrawVertexArrayround1Click(TObject *Sender);
	void __fastcall P1Click(TObject *Sender);

private: // User declarations
	HWND hwnd1,hwnd2;
	HGLRC hrc1,hrc2;
	HDC dc1,dc2;
	GLfloat R, G, B, xpos, ypos;
	bool mode1;
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
