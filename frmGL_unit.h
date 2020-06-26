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
#include <Math.hpp>
#include <math.h>
#include "RzSplit.hpp"
#include <Menus.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#define Pi M_PI

enum TStateInput {
	siNone, siManualDrag, siSector, siCircle, siClick
};

struct TColorGL {
	float R, G, B, a;
};

struct TRecSector {
	float dA, dD, Angl, D;
	int UserID;
	TColorGL Color;
	bool Enable;
};

struct TRecGPSFloat {
	float x, y, z, m;
	TStateInput State;
};

const GLfloat MC[4] = {0.5, 0.0, 1.0, 1.0}; // ������ ������� ���������

GLfloat ColorGLObj[4];
GLfloat ObjColor[4] = {0, 1, 0, 0.5};
GLfloat KMPColorGLObj[4] = {0.6, 0.6, 0, 0.5};
GLfloat TColorGLObj[4] = {0, 1, 0, 0.5};
GLfloat WColorGLObj[4] = {0, 1, 0.3, 0.5};
GLfloat AColorGLObj[4] = {1, 0, 0, 1};
GLfloat SelColorGLObj[4] = {1, 1, 1, 1};
GLfloat SecColorGLObj[4] = {1, 1, 0.1, 0.9};
GLfloat UookColorGLObj[4] = {153 / 255, 153 / 255, 1, 1};
GLfloat UookModColorGLObj[4] = {1, 0, 0, 1};

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
	TMenuItem *RadarCircle1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall RzToolButton1Click(TObject *Sender);
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
	void __fastcall RadarCircle1Click(TObject *Sender);
	void __fastcall RzPanel1Paint(TObject *Sender);
	void __fastcall RzPanel1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

private: // User declarations

	void __fastcall InitializeRC();

	HWND hwnd1, hwnd2;
	HGLRC hrc1, hrc2;
	HDC dc1, dc2;
	GLfloat R, G, B, xpos, ypos;
	bool mode1;
	GLfloat MCGrid[4];
	GLfloat MCPoint[4];
	GLfloat MCTarget[4];
	GLfloat tX, tY, DX, DY;
	TColorGL Cl;
	int FBreakPosSector;
	float FOldSectorPos;
	TPoint FLastMousePoint;

	TRecSector __fastcall ConvertCoordToStrob(float X1, float Y1, float X2, float Y2);
	TRecSector __fastcall ConvertCoordToStrob();
	void __fastcall PaintSector(TColorGL Cl, float dA, float dD, float Angl, float D, float Opacity);
	void __fastcall PaintCurrentSector();
	void __fastcall SetDCPixelFormat(HDC hdc);
	void __fastcall ColorToGL(TColor c, GLfloat &R, GLfloat &G, GLfloat &B);
	void __fastcall PaintCurrentCircle();
	void __fastcall PaintGrid();
	void __fastcall PaintCurve();
	void __fastcall PaintTarget();
	void __fastcall DrawWall();
	void __fastcall DrawAxis();
	void __fastcall myPaint();
	void __fastcall PaintGL();
	void __fastcall InitViewProection();
	void __fastcall InitViewProection(float x, float y, float dXX, float dYY);

public: // User declarations

	__fastcall TfrmGL(TComponent* Owner);

	TRecGPSFloat MousePosMove, MousePosDown, GPSCursor0, GPSCursor1, ResultShiftScene;
	float PosZoomX1, PosZoomY1, PosZoomX2, PosZoomY2, Diagonal, Zoom;

	void __fastcall UpdateOGL();
	// void __fastcall WMPaint(TWMPaint Msg );
	// BEGIN_MESSAGE_MAP
	// MESSAGE_HANDLER(WM_PAINT, TWMPaint, WMPaint)
	// END_MESSAGE_MAP(TForm)
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmGL *frmGL;
// ---------------------------------------------------------------------------
#endif
