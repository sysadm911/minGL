// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmGL_unit.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzStatus"
#pragma link "RzButton"
#pragma link "RzSplit"
#pragma resource "*.dfm"
#include <tchar.h>
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
 Формат пикселя */
void __fastcall TfrmGL::SetDCPixelFormat(HDC hdc) {
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat = 0;

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);
}

/* =======================================================================
 Создание формы */
void __fastcall TfrmGL::FormCreate(TObject* Sender) {
	hwnd1 = RzPanel1->Handle;
	dc1 = GetDC(hwnd1);
	SetDCPixelFormat(dc1);
	hrc1 = wglCreateContext(dc1);
	Randomize();
	R = Random();
	G = Random();
	B = Random();

	xpos = RzPanel1->Width / 2.;
	ypos = RzPanel1->Height / 2.;

}

/* =======================================================================
 Рисование картинки *//* ======================================================================
 Перевод цвета из TColor в OpenGL */
void __fastcall TfrmGL::ColorToGL(TColor c, GLfloat &R, GLfloat &G, GLfloat &B) {
	R = (GLfloat)(c & 0xFF) / 255;
	G = (GLfloat)((c & 0xFF00) >> 8) / 255;
	B = (GLfloat)((c & 0xFF0000) >> 16) / 255;
	RzFieldStatus1->Caption = FloatToStr(R) + "  " + FloatToStr(G) + "  " + FloatToStr(B);
}

/* =======================================================================
 Конец работы приложения */
void __fastcall TfrmGL::FormDestroy(TObject *Sender) {
	wglDeleteContext(hrc1);
	wglDeleteContext(hrc2);
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

	hwnd1 = RzPanel1->Handle;
	dc1 = GetDC(hwnd1);
	SetDCPixelFormat(dc1);
	hrc1 = wglCreateContext(dc1);
	Randomize();
	R = Random();
	G = Random();
	B = Random();

	PAINTSTRUCT ps;

	// BeginPaint(hwnd2, &ps);
	wglMakeCurrent(dc1, hrc1);
	glViewport(0, 0, RzPanel1->Width, RzPanel1->Height); // область вывода
	glClearColor(R, G, B, 1.0); // цвет фона
	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета
	glPointSize(5); // размер точек
	glColor3f(1.0, 0.0, 0.5); // текущий цвет примитивов
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS); // открываем командную скобку
	glVertex2f(-1, -1);
	glVertex2f(-1, 1);
	glVertex2f(0, 0);
	glVertex2f(1, -1);
	glVertex2f(1, 1);

	GLfloat a, b, x;
	GLint i, num;
	a = -M_PI; // начало интервала
	b = M_PI; // конец интервала
	num = 2000; // количество точек на интервале

	for (i = 0; i <= num; i++) {
		x = a + i * (b - a) / num;
		glVertex2f(2 * (x - a) / (b - a) - 1.0, sin(x) * 0.75);

	}
	glEnd();

	RzFieldStatus1->Caption = FloatToStr(R) + "  " + FloatToStr(G) + "  " + FloatToStr(B);
	RzGL_VENDOR->Caption = (char *) glGetString(GL_VENDOR);
	RzGL_RENDERER->Caption = (char *) glGetString(GL_RENDERER);
	RzGL_VERSION->Caption = (char *) glGetString(GL_VERSION);
	SwapBuffers(dc1);
	wglMakeCurrent(0, 0);
	// EndPaint(hwnd2, &ps);
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------6

void __fastcall TfrmGL::OpenGL1Click(TObject *Sender)

{
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	Randomize();
	R = Random();
	G = Random();
	B = Random();

	PAINTSTRUCT ps;

	// BeginPaint(hwnd2, &ps);
	wglMakeCurrent(dc2, hrc2);
	glViewport(0, 0, RzPanel2->Width, RzPanel2->Height); // область вывода
	glClearColor(R, G, B, 1.0); // цвет фона
	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета
	glPointSize(5); // размер точек
	glColor3f(1.0, 0.0, 0.5); // текущий цвет примитивов
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS); // открываем командную скобку
	glVertex2f(-1, -1);
	glVertex2f(-1, 1);
	glVertex2f(0, 0);
	glVertex2f(1, -1);
	glVertex2f(1, 1);

	GLfloat a, b, x;
	GLint i, num;
	a = -M_PI; // начало интервала
	b = M_PI; // конец интервала
	num = 2000; // количество точек на интервале

	for (i = 0; i <= num; i++) {
		x = a + i * (b - a) / num;
		glVertex2f(2 * (x - a) / (b - a) - 1.0, cos(x));

	}
	glEnd();

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
	// EndPaint(hwnd2, &ps);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::OpenGLGDI2Click(TObject *Sender) {
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	Randomize();
	R = Random();
	G = Random();
	B = Random();

	PAINTSTRUCT ps;

	// BeginPaint(hwnd2, &ps);
	wglMakeCurrent(dc2, hrc2);
	glViewport(0, 0, RzPanel2->Width, RzPanel2->Height); // область вывода
	glClearColor(R, G, B, 1.0); // цвет фона
	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
	// EndPaint(hwnd2, &ps);

	RzPanel2->Canvas->Brush->Color = clGreen;
	RzPanel2->Canvas->Ellipse(400, 400, 450, 450);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::OpenGLlines1Click(TObject *Sender) {
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);
	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета
	// glLineWidth (1);
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINES);
	glVertex2f(-1, 1);
	glVertex2f(1, -1);
	glVertex2f(-1, -1);
	glVertex2f(1, 1);
	glEnd();
	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::OpenGLOxOy1Click(TObject *Sender)

{
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);
	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета

	glPointSize(5); // размер точек
	glColor3f(1.0, 0.0, 0.5); // текущий цвет примитивов
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS); // открываем командную скобку
	glVertex2f(0, 0);
	glEnd();

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);

}

// ---------------------------------------------------------------------------

void __fastcall TfrmGL::BezierCurvewithFourControlPoints1Click(TObject *Sender) {
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);

	GLfloat ctrlpoints[4][3] = { {-4.0, -4.0, 0.0}, {-2.0, 4.0, 0.0}, {2.0, -4.0, 0.0}, {4.0, 4.0, 0.0}};

	GLsizei w = RzPanel2->Width;
	GLsizei h = RzPanel2->Height;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-5.0, 5.0, -5.0*(GLfloat)h / (GLfloat)w, 5.0*(GLfloat)h / (GLfloat)w, -5.0, 5.0);
	else
		glOrtho(-5.0*(GLfloat)w / (GLfloat)h, 5.0*(GLfloat)w / (GLfloat)h, -5.0, 5.0, -5.0, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1);
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINE_STRIP);
	for (GLint i = 0; i <= 30; i++)
		glEvalCoord1f((GLfloat) i / 30.0);
	glEnd();
	/* The following code displays the control points as dots. */
	glPointSize(5.0);
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POINTS);
	for (GLint i = 0; i < 4; i++)
		glVertex3fv(&ctrlpoints[i][0]);
	glEnd();
	glFlush();

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::OpenGLlinesstrip1Click(TObject *Sender) {
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);

	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета

	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2);
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-0.8, -0.8);
	glVertex2f(-0.8, 0.8);
	glVertex2f(0.8, 0.8);
	glVertex2f(0.8, -0.8);
	glEnd();

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);

}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::OpenGLlinesloop1Click(TObject *Sender)

{
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);

	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета

	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2);
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.8, -0.8);
	glVertex2f(-0.8, 0.8);
	glVertex2f(0.8, 0.8);
	glVertex2f(0.8, -0.8);
	glEnd();

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::RzPanel2MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)

{
	xpos = 2. * X / RzPanel1->Width - 1;
	ypos = 2. * (RzPanel1->Height - Y) / RzPanel1->Height - 1;
	if (mode1)
		Randomlines1Click(this);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::Randomlines1Click(TObject *Sender) {
	mode1 = true;
	if (mode1) {
		// PAINTSTRUCT ps;
		// BeginPaint(hwnd2, &ps);

		hwnd2 = RzPanel2->Handle;
		dc2 = GetDC(hwnd2);
		SetDCPixelFormat(dc2);
		hrc2 = wglCreateContext(dc2);

		wglMakeCurrent(dc2, hrc2);
		// glViewport(0, 0, RzPanel2->Width, RzPanel2->Height); // область вывода
		glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета
		// glEnable(GL_LINE_STIPPLE);
		GLint i;
		for (i = 1; i < 50; i++) {
			glColor3f(Random(), Random(), Random());
			glLineStipple(Random(5), Random(0xFFFF));
			glBegin(GL_LINES);
			glVertex2f(xpos, ypos);
			glVertex2f(xpos + 0.5 * Random() * Sin(Random(360)), ypos + 0.5 * Random() * Cos(Random(360)));
			glEnd();
		}

		SwapBuffers(dc2);
		wglMakeCurrent(0, 0);
		// EndPaint(hwnd2, &ps);
	}

}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::RzPanel2Paint(TObject *Sender)

{
	BezierCurvewithFourControlPoints1Click(this);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::riangles1Click(TObject *Sender)

{
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);

	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета

	glColor3f(1.0, 0.0, 0.5); // текущий цвет примитивов
	// glBegin(GL_TRIANGLES);
	// glVertex2f(-1, -1);
	// glVertex2f(-1, 1);
	// glVertex2f(-0.5, 0.5);
	// glEnd();

	// glBegin(GL_TRIANGLE_FAN);
	// glVertex2f(0.8, 0.8); // вершина, общая для всех треугольников
	// for (GLint i = 0; i <= 6; i++) {
	// glColor3f(Random(), Random(), Random());
	// glVertex2f(0.9 * cos(2 * Pi * i / 6), 0.9 * sin(2 * Pi * i / 6));
	// }
	// glEnd();

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // треугольники будут рисоваться контурно - только линии границ
	glBegin(GL_TRIANGLE_STRIP);
	for (GLint i = 0; i <= 20; i++) {
		glColor3f(Random(), Random(), Random());
		glVertex2f(0, 0);
		glVertex2f(0.5 * cos(2 * Pi * i / 20), 0.5 * sin(2 * Pi * i / 20));
	};
	glEnd();

	// glBegin(GL_TRIANGLE_STRIP);
	// glColor3f(0.0, 0.0, 1.0);
	// glVertex2f(1, 1);
	// glVertex2f(-1, 1);
	// glColor3f(1.0, 0.0, 0.0);
	// glVertex2f(-1, -1);
	// glVertex2f(1, -1);
	// glEnd();
	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::Rectangle1Click(TObject *Sender)

{
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);

	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета
	glPointSize(1); // размер точек
	glColor3f(1.0, 0.5, 0.5); // текущий цвет примитивов
	glRectf(-0.8, -0.8, -0.5, -0.5);

	glBegin(GL_QUADS);
	glColor3f(Random(), Random(), Random());
	glVertex2f(-0.6, 0.2);
	glVertex2f(-0.7, 0.7);
	glVertex2f(0.1, 0.65);
	glVertex2f(0.25, -0.78);
	glColor3f(Random(), Random(), Random());
	glVertex2f(0.3, -0.6);
	glVertex2f(0.45, 0.7);
	glVertex2f(0.8, 0.65);
	glVertex2f(0.9, -0.8);
	glEnd();

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::Disk1Click(TObject *Sender)

{
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);

	GLint Level = 200; // уровень детализации
	GLfloat radius1 = 0.4; // радиус внутреннего отверстия
	GLfloat radius2 = 0.7; // радиус внешней кромки
	GLuint i;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < Level; i++) {
		glVertex2f(radius1 * sin(2 * Pi * i / Level), radius1 * cos(2 * Pi * i / Level));
		glVertex2f(radius2 * sin(2 * Pi * i / Level), radius2 * cos(2 * Pi * i / Level));
		glVertex2f(radius1 * sin(2 * Pi * (i + 1) / Level), radius1 * cos(2 * Pi * (i + 1) / Level));
		glVertex2f(radius2 * sin(2 * Pi * (i + 1) / Level), radius2 * cos(2 * Pi * (i + 1) / Level));
	}
	glEnd();

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::DrawVertexArray1Click(TObject *Sender) {
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);

	glClearColor(0.5, 0.5, 0.75, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat Vertex[4][2];
	GLfloat Colors[4][3];

	Vertex[0][0] = -0.9;
	Vertex[0][1] = -0.9;
	Colors[0][0] = 0.1;
	Colors[0][1] = 0.5;
	Colors[0][2] = 0.85;

	Vertex[1][0] = -0.9;
	Vertex[1][1] = 0.9;
	Colors[1][0] = 0.85;
	Colors[1][1] = 0.1;
	Colors[1][2] = 0.5;

	Vertex[2][0] = 0.9;
	Vertex[2][1] = 0.9;
	Colors[2][0] = 0.85;
	Colors[2][1] = 0.85;
	Colors[2][2] = 0.85;

	Vertex[3][0] = 0.9;
	Vertex[3][1] = -0.9;
	Colors[3][0] = 0.5;
	Colors[3][1] = 0.85;
	Colors[3][2] = 0.1;

	glVertexPointer(2, GL_FLOAT, 0, Vertex); // указатель на массив вершин
	glColorPointer(3, GL_FLOAT, 0, Colors); // указатель на массив цветов

	glEnableClientState(GL_VERTEX_ARRAY); // массив вершин - включаем режим
	glEnableClientState(GL_COLOR_ARRAY); // массив цветов - включаем режим

	glBegin(GL_POLYGON); //
	glArrayElement(0); //
	glArrayElement(1); // эквивалени glDrawArrays(GL_POLYGON, 0, 4); // рисование множества полигонов
	glArrayElement(2); //
	glArrayElement(3); //
	glEnd();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::DrawVertexArrayround1Click(TObject *Sender)

{
	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);

	// glClearColor(0.5, 0.5, 0.75, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat aVertex[64][2];
	GLfloat aColors[64][3];

	for (int i = 0; i < 64; i++) {
		aVertex[i][0] = RzPanel2->Width / 550 * sin(0.1 * i);
		aVertex[i][1] = RzPanel2->Height / 550 * cos(0.1 * i);
		aColors[i][0] = 0.75 - 0.01 * i;
		aColors[i][1] = 0.85 - 0.02 * i;
		aColors[i][1] = 0.85 - 0.02 * i;
	}

	glVertexPointer(2, GL_FLOAT, 0, aVertex);
	glColorPointer(3, GL_FLOAT, 0, aColors);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_POLYGON, 0, 64);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::P1Click(TObject *Sender) {

	mode1 = false;
	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);

	const GLuint ImageWidth = 64;
	const GLuint ImageHeight = 64;
	GLubyte ImageArr[ImageHeight][ImageWidth][3];

	for (int i = 0; i < ImageHeight; i++) {
		for (int j = 0; j < ImageWidth; j++) {
			if (!(i & 8) ^ !(j & 8)) {

				ImageArr[i][j][0] = 0;
				ImageArr[i][j][1] = 0;
				ImageArr[i][j][2] = 255;
			}
			else {

				ImageArr[i][j][0] = 255;

				ImageArr[i][j][1] = 0;

				ImageArr[i][j][2] = 0;
			}
		}
	}

	glClearColor(0.5, 0.5, 0.75, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2f(-1, -1);
	glDrawPixels(ImageWidth, ImageHeight, GL_RGB, GL_UNSIGNED_BYTE, ImageArr);

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);

}
// ---------------------------------------------------------------------------
