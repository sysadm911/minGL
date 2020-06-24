// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmGL_unit.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzStatus"
#pragma link "RzButton"
#pragma resource "*.dfm"
TfrmGL *frmGL;

// ---------------------------------------------------------------------------
__fastcall TfrmGL::TfrmGL(TComponent* Owner) : TForm(Owner) {
}
// ---------------------------------------------------------------------------

// TForm1::WMPaint(TWMPaint Msg) {
// TPaintStruct ps;
//
// BeginPaint(Handle, &ps);
// Rectangle(Canvas->Handle, 10, 10, 100, 100);
// EndPaint(Handle, &ps);
//
// }

/* =======================================================================
 ������ ������� */
void __fastcall TfrmGL::SetDCPixelFormat(HDC hdc) {
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat = 0;

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);
}

/* =======================================================================
 �������� ����� */
void __fastcall TfrmGL::FormCreate(TObject* Sender) {
	hwnd = RzPanel1->Handle;
	dc = GetDC(hwnd);
	SetDCPixelFormat(dc);
	hrc = wglCreateContext(dc);
	Randomize();
	R = Random();
	G = Random();
	B = Random();
	RzFieldStatus1->Caption = FloatToStr(R) + "  " + FloatToStr(G) + "  " + FloatToStr(B);
}

/* =======================================================================
 ��������� �������� *//* ======================================================================
 ������� ����� �� TColor � OpenGL */
void __fastcall TfrmGL::ColorToGL(TColor c, GLfloat &R, GLfloat &G, GLfloat &B) {
	R = (GLfloat)(c & 0xFF) / 255;
	G = (GLfloat)((c & 0xFF00) >> 8) / 255;
	B = (GLfloat)((c & 0xFF0000) >> 16) / 255;
	RzFieldStatus1->Caption = FloatToStr(R) + "  " + FloatToStr(G) + "  " + FloatToStr(B);
}

/* =======================================================================
 ����� ������ ���������� */
void __fastcall TfrmGL::FormDestroy(TObject *Sender) {
	wglDeleteContext(hrc);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmGL::RzToolButton1Click(TObject *Sender) {
	if (ColorDialog1->Execute()) {
		ColorToGL(ColorDialog1->Color, R, G, B);
	}
	RzPanel1->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmGL::RzPanel1Paint(TObject *Sender) {
	PAINTSTRUCT ps;

	BeginPaint(hwnd, &ps);
	wglMakeCurrent(dc, hrc);
	glViewport(0, 0, RzPanel1->Width, RzPanel1->Height); // ������� ������
	glClearColor(R, G, B, 1.0); // ���� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������� ������ �����
	glPointSize(20); // ������ �����
	glColor3f(1.0, 0.0, 0.5); // ������� ���� ����������
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS); // ��������� ��������� ������
	glVertex2f(-1, -1);
	glVertex2f(-1, 1);
	glVertex2f(0, 0);
	glVertex2f(1, -1);
	glVertex2f(1, 1);


	for (int i = 0; i <= num; i++) {


   x = a + i * (b - a) / num;
   glVertex2f (2 * (x - a) / (b - a) - 1.0, sin(x) * 0.75);
 }
 glEnd;
	SwapBuffers(dc);
	wglMakeCurrent(0, 0);
	EndPaint(hwnd, &ps);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::RzPanel1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	xpos = 2. * X / RzPanel1->Width - 1;
	ypos = 2. * (RzPanel1->Height - Y) / RzPanel1->Height - 1;

}
// ---------------------------------------------------------------------------