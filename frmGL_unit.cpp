﻿// ---------------------------------------------------------------------------

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
	wglMakeCurrent(dc1, hrc1);
	InitializeRC();
	InitViewProection();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MC);

	xpos = RzPanel2->Width / 2.;
	ypos = RzPanel2->Height / 2.;

	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_LINE);

	PosZoomX1 = -1;
	PosZoomX2 = -1;
	PosZoomY1 = -0.5;
	PosZoomY2 = -0.5;

	Zoom = -6;

	hwnd2 = RzPanel2->Handle;
	dc2 = GetDC(hwnd2);
	SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);

	RzFieldStatus1->Caption = FloatToStr(R) + "  " + FloatToStr(G) + "  " + FloatToStr(B);
	RzGL_VENDOR->Caption = (char *) glGetString(GL_VENDOR);
	RzGL_RENDERER->Caption = (char *) glGetString(GL_RENDERER);
	RzGL_VERSION->Caption = (char *) glGetString(GL_VERSION);
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

void __fastcall TfrmGL::InitializeRC() {
	glEnable(GL_DEPTH_TEST); // разрешаем тест глубины
	glEnable(GL_LIGHTING); // разрешаем работу с освещенностью
	glEnable(GL_LIGHT0); // включаем источник света 0
}

// ---------------------------------------------------------------------------
void __fastcall TfrmGL::OpenGL1Click(TObject *Sender) {

	mode1 = false;

	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	Randomize();
	R = Random();
	G = Random();
	B = Random();

	// PAINTSTRUCT ps;

	// BeginPaint(hwnd2, &ps);
	wglMakeCurrent(dc2, hrc2);
	// glViewport(0, 0, RzPanel2->Width, RzPanel2->Height); // область вывода
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
	a = -M_PI; // начало интервала
	b = M_PI; // конец интервала
	GLuint num = 2000; // количество точек на интервале

	for (GLuint i = 0; i <= num; i++) {
		x = a + i * (b - a) / num;
		glVertex2f(2 * (x - a) / (b - a) - 1.0, cos(x));

	}
	glEnd();

	glPointSize(20); // размер точек
	glColor3f(1.0, 0.0, 0.5); // текущий цвет примитивов
	glBegin(GL_POINT); // открываем командную скобку
	glVertex2f(-1, -1);
	glVertex2f(-1, 1);
	glVertex2f(0, 0);
	glVertex2f(1, -1);
	glVertex2f(1, 1);
	glEnd(); // закрываем командную скобку

	GLuint glErr = glGetError();
	if (glErr) {
		switch (glErr) {
		case GL_INVALID_ENUM:
			Caption = "Неверный аргумент!";
			break;
		case GL_INVALID_VALUE:
			Caption = "Неверное значение аргумента!!";
			break;
		case GL_INVALID_OPERATION:
			Caption = "Неверная операция!";
			break;
		case GL_STACK_OVERFLOW:
			Caption = "Переполнение стека!";
			break;
		case GL_STACK_UNDERFLOW:
			Caption = "Потеря значимости стека!";
			break;
		case GL_OUT_OF_MEMORY:
			Caption = "Не хватает памяти!";
			break;
		}

	}

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
	wglDeleteContext(hrc2);
	// EndPaint(hwnd2, &ps);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::OpenGLGDI2Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
	hrc2 = wglCreateContext(dc2);
	Randomize();
	R = Random();
	G = Random();
	B = Random();

	// PAINTSTRUCT ps;

	// BeginPaint(hwnd2, &ps);
	wglMakeCurrent(dc2, hrc2);
	// glViewport(0, 0, RzPanel2->Width, RzPanel2->Height); // область вывода
	glClearColor(R, G, B, 1.0); // цвет фона
	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
	wglDeleteContext(hrc2);
	// EndPaint(hwnd2, &ps);

	RzPanel2->Canvas->Brush->Color = clGreen;
	RzPanel2->Canvas->Ellipse(400, 400, 450, 450);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::OpenGLlines1Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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
	wglDeleteContext(hrc2);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::OpenGLOxOy1Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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
	wglDeleteContext(hrc2);

}

// ---------------------------------------------------------------------------

void __fastcall TfrmGL::BezierCurvewithFourControlPoints1Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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
	wglDeleteContext(hrc2);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::OpenGLlinesstrip1Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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
	wglDeleteContext(hrc2);

}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::OpenGLlinesloop1Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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
	wglDeleteContext(hrc2);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::RzPanel2MouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	xpos = 2. * X / RzPanel2->Width - 1;
	ypos = 2. * (RzPanel2->Height - Y) / RzPanel2->Height - 1;
	if (mode1)
		Randomlines1Click(this);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::Randomlines1Click(TObject *Sender) {
	mode1 = true;
	if (mode1) {
		// PAINTSTRUCT ps;
		// BeginPaint(hwnd2, &ps);
		//
		// hwnd2 = RzPanel2->Handle;
		// dc2 = GetDC(hwnd2);
		// SetDCPixelFormat(dc2);
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

		GLuint glErr = glGetError();
		if (glErr) {
			switch (glErr) {
			case GL_INVALID_ENUM:
				Caption = "Неверный аргумент!";
				break;
			case GL_INVALID_VALUE:
				Caption = "Неверное значение аргумента!!";
				break;
			case GL_INVALID_OPERATION:
				Caption = "Неверная операция!";
				break;
			case GL_STACK_OVERFLOW:
				Caption = "Переполнение стека!";
				break;
			case GL_STACK_UNDERFLOW:
				Caption = "Потеря значимости стека!";
				break;
			case GL_OUT_OF_MEMORY:
				Caption = "Не хватает памяти!";
				break;
			}

		}

		SwapBuffers(dc2);
		wglMakeCurrent(0, 0);
		// EndPaint(hwnd2, &ps);
		wglDeleteContext(hrc2);
	}

}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::RzPanel2Paint(TObject *Sender) {
	Rotation1Click(this);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::riangles1Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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
	wglDeleteContext(hrc2);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::Rectangle1Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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

}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::Disk1Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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
	wglDeleteContext(hrc2);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::DrawVertexArray1Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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
	glRotatef(5, 0.0, 0.0, 1.0);
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
	wglDeleteContext(hrc2);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::DrawVertexArrayround1Click(TObject *Sender) {
	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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
	wglDeleteContext(hrc2);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::P1Click(TObject *Sender) {

	mode1 = false;
	// hwnd2 = RzPanel2->Handle;
	// dc2 = GetDC(hwnd2);
	// SetDCPixelFormat(dc2);
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
	wglDeleteContext(hrc2);

}

// ---------------------------------------------------------------------------
void __fastcall TfrmGL::PaintCurrentCircle() {
	int i = 0, j = 0;
	float a = 0.0, x = 0.0, y = 0.0, k = 0.0, Cl = 0.0;
	int n = 0;
	n = 100;
	glLineWidth(1);
	MCGrid[0] = 1;
	MCGrid[1] = 0.5;
	MCGrid[2] = 0;
	MCGrid[3] = 0.5;
	k = 1; // sqrt(PosZoomX2 * PosZoomX2 + PosZoomY2 * PosZoomY2);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);
	if (true) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);
	}
	else {
		glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);
	}

	// if Focused then k:=1 else k:=0.5;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= n; i++) {
		a = i * 2 * Pi / n;
		x = k * sin(a);
		y = k * cos(a);
		glVertex3f(x, y, 0.2);
	}
	glEnd();
}

void __fastcall TfrmGL::PaintGrid() {
	const int nSec = 40;
	int i = 0, j = 0;
	float a = 0.0, x = 0.0, y = 0.0, k = 0.0, Cl = 0.0, ClMax = 0.0, ClMin = 0.0;
	int n = 0;
	n = 100;
	glLineWidth(1);
	MCGrid[0] = 1;
	MCGrid[1] = 1;
	MCGrid[2] = 1;
	MCGrid[3] = 1;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);
	// if (true) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	// }
	// else {
	// glDisable(GL_BLEND);
	// glDisable(GL_LINE_SMOOTH);
	// }
	// if (Focused)
	k = 1;
	// else
	// k = 0.5;
	Cl = 0.15 * k; // * FGridOpacity;
	ClMax = 0.25 * k; // * FGridOpacity;
	ClMin = 0.03 * k; // * FGridOpacity;
	for (int j = 0; j <= nSec; j++) {
		if (Frac(double(j) / 5) == 0)
			MCGrid[3] = Cl;
		else
			MCGrid[3] = ClMin;
		if (Frac(double(j) / 10) == 0)
			MCGrid[3] = ClMax;
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);

		glBegin(GL_LINE_STRIP);
		for (int stop = n - 1, i = 0; i <= stop; i++) {
			a = i * 2 * Pi / n;
			x = j * sin(a) / nSec;
			y = j * cos(a) / nSec;
			glVertex3f(x, y, 0.01);
		}
		a = 0 * 2 * Pi / n;
		x = j * sin(a) / nSec;
		y = j * cos(a) / nSec;
		glVertex3f(x, y, 0.01);
		glEnd();
	}
	j = 60;
	for (int i = 0; i <= j; i++) {
		if (Frac(double(i) / 5) == 0)
			MCGrid[3] = Cl;
		else
			MCGrid[3] = ClMin;
		if (Frac(double(i) / 15) == 0)
			MCGrid[3] = ClMax;
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);
		glBegin(GL_LINE_STRIP);
		a = 2 * Pi * i / j;
		x = sin(a);
		y = cos(a);
		glVertex3f(x * 6. / 400, y * 6. / 400, 0.02);
		glVertex3f(x, y, 0.02);
		glEnd();
	}
}

void __fastcall TfrmGL::PaintCurve() {
	int i = 0, j = 0;
	float a = 0.0, x = 0.0, y = 0.0, k = 0.0, Beta = 225.0;
	int n = 0;
	n = 100;
	glLineWidth(1);
	MCGrid[0] = 1.0;
	MCGrid[1] = 0.5;
	MCGrid[2] = 1.0;
	MCGrid[3] = 0.2;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);
	// if ( true )
	// {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	// }
	// else
	// {
	// glDisable( GL_BLEND );
	// glDisable( GL_LINE_SMOOTH );
	// }
	unsigned int Cl = ColorToRGB(clRed);
	glColor3f(double(LOBYTE(Cl)) / 255, double(HIBYTE(Cl)) / 255, double(LOBYTE(HIWORD(Cl))) / 255);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0.1);
	glVertex3f(sin(Beta * Pi / 180), cos(Beta * Pi / 180), 0.1);

	glEnd();
	Cl = RGB(55, 53, 74);
	glColor3f(double(LOBYTE(Cl)) / 255, double(HIBYTE(Cl)) / 255, double(LOBYTE(HIWORD(Cl))) / 255);
}

void __fastcall TfrmGL::PaintTarget() {
	float a = 0.0, x = 0.0, y = 0.0;
	int i = 0;
	glPointSize(17);
	MCTarget[3] = 100.0 / 100.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);
	// if ( true )
	// {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	// }

	glBegin(GL_POINTS);
	tX = tY = -0.5;
	glVertex3f(tX, tY, 0.70);
	glEnd();
}

void __fastcall TfrmGL::DrawWall() {
	// --------Down Plane XY--------------------
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, -1, 1);
	glVertex3f(-1, -1, 1);
	glEnd();

	// --------Left Plane YZ--------------------
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, -1, 1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, 1, -1);
	glEnd();

	// --------Left Plane XZ--------------------
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(-1, 1, -1);
	glEnd();
}

void __fastcall TfrmGL::DrawAxis() {

	// --------Axis X--------------------
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1, 0, 0);
	glVertex3f(1, 0, 0);
	glEnd();

	// --------Axis Y--------------------
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, -1, 0);
	glVertex3f(0, 1, 0);
	glEnd();

	// --------Axis Z--------------------
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, -1);
	glVertex3f(0, 0, 1);
	glEnd();
}

void __fastcall TfrmGL::PaintSector(TColorGL Cl, float dA, float dD, float Angl, float D, float Opacity) {
	int i = 0, j = 0;
	float a = 0.0, x = 0.0, y = 0.0, D1 = 0.0, D2 = 0.0;
	int n = 0;
	glLineWidth(1.0);
	MCGrid[3] = Opacity;

	// if ( ( Cl.a == - 1 ) || ( StateInput == siSector ) )
	// {
	MCGrid[0] = 0.6;
	MCGrid[1] = 0.7;
	MCGrid[2] = 0.25;
	// }
	// else
	// {
	// MCGrid[0] = Cl.R;
	// MCGrid[1] = Cl.G;
	// MCGrid[2] = Cl.B;
	// MCGrid[3] = Cl.a;
	// }
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);
	glEnable(GL_BLEND);
	glEnable(GL_POLYGON_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	D1 = D + dD;
	D2 = D - dD;
	if (D1 > 1)
		D1 = 1.0;
	if (D2 > 1)
		D2 = 1.0;
	if (D1 < 0)
		D1 = 0.0;
	if (D2 < 0)
		D2 = 0.0;
	n = 100;
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= n; i++) {
		a = Angl + i * 2 * dA / n - dA;
		x = sin(a) * D1;
		y = cos(a) * D1;
		glVertex3f(x, y, 0.3);
		a = Angl + i * 2 * dA / n - dA;
		x = sin(a) * D2;
		y = cos(a) * D2;
		glVertex3f(x, y, 0.3);
	}
	glEnd();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= n; i++) {
		a = Angl + i * 2 * dA / n - dA;
		x = sin(a) * D1;
		y = cos(a) * D1;
		glVertex3f(x, y, 0.56);
	}
	for (int stop = 0, i = n; i >= stop; i--) {
		a = Angl + i * 2 * dA / n - dA;
		x = sin(a) * D2;
		y = cos(a) * D2;
		glVertex3f(x, y, 0.56);
	}
	a = Angl + 0 * 2 * dA / n - dA;
	x = sin(a) * D1;
	y = cos(a) * D1;
	glVertex3f(x, y, 0.56);
	glEnd();
}

TRecSector __fastcall TfrmGL::ConvertCoordToStrob() {

	return ConvertCoordToStrob(PosZoomX1, PosZoomY1, PosZoomX2, PosZoomY2);
}

TRecSector __fastcall TfrmGL::ConvertCoordToStrob(float X1, float Y1, float X2, float Y2) {
	TRecSector result;
	float Angl2 = 0.0, D0 = 0.0, a = 0.0;
	int tmp = 0;
	result.Angl = ArcTan2(X1, Y1);
	Angl2 = ArcTan2(X2, Y2);
	if (result.Angl < 0)
		result.Angl = result.Angl + 2 * Pi;
	if (Angl2 < 0)
		Angl2 = Angl2 + 2 * Pi;
	if (Sign(Angl2 - result.Angl) != 0)
		tmp = Sign(Angl2 - result.Angl);
	else
		tmp = FBreakPosSector;
	if (((Angl2 >= (2 * Pi / 360 * 358)) && (Angl2 <= (2 * Pi / 360 * 360))) || ((Angl2 <= (2 * Pi / 360 * 1)) && (Angl2 >= 0)))
		tmp = FBreakPosSector;
	FOldSectorPos = Angl2;
	if ((result.Angl + Pi) >= 2 * Pi)
		if ((result.Angl - Angl2) > Pi)
			tmp = 1;
	if ((result.Angl - Pi) < 0)
		if (Angl2 > result.Angl + Pi)
			tmp = -1;
	if (tmp > 0)
		if ((result.Angl + Pi) >= 2 * Pi)
			if (Angl2 < result.Angl)
				Angl2 = Angl2 + 2 * Pi;
	if (tmp < 0)
		if ((result.Angl - Pi) < 0)
			if (Angl2 > result.Angl)
				Angl2 = Angl2 - 2 * Pi;
	a = abs(Angl2 - result.Angl);
	result.dA = abs(Angl2 - result.Angl);
	if (result.dA > Pi)
		result.dA = Pi;
	D0 = sqrt(Y2 * Y2 + X2 * X2);
	result.D = sqrt(Y1 * Y1 + X1 * X1);
	result.dD = abs(result.D - D0);
	FBreakPosSector = tmp;
	return result;
}

void __fastcall TfrmGL::PaintCurrentSector() {
	TRecSector tmp;
	tmp = ConvertCoordToStrob();

	// float dA, dD, Angl, D;
	// int UserID;
	tmp.Color.R = 1.0;
	tmp.Color.R = 0.0;
	tmp.Color.R = 0.0;
	tmp.Color.R = 1.0;
	tmp.Enable = true;

	// if (tmp.Angl != -1)
	// PaintSector(tmp.Color, tmp.dA, tmp.dD, tmp.Angl, tmp.D, 0.8);
	PaintSector(tmp.Color, tmp.dA, tmp.dD, tmp.Angl, tmp.D, 0.8);
}

void __fastcall TfrmGL::myPaint() {
	// TPaintStruct ps;
	int i = 0, j = 0;
	float x = 0.0, y = 0.0, a = 0.0;
	unsigned int Cl = 0;
	// InitViewProection();
	// BeginPaint( hwnd1, ps );
	// if ( dc1 == 0 )
	// CreateOGL;
	// wglMakeCurrent( DC, hrc );

	// Cl=ColorToRGB(clTeal);
	Cl = RGB(55, 53, 74);
	/////Cl = Color; // rgb(55,53,74);
	glClearColor(double(LOBYTE(Cl)) / 255, double(HIBYTE(Cl)) / 255, double(LOBYTE(HIWORD(Cl))) / 255, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// if ( FViewGrid )
	PaintGrid();
	// if ( FViewCurve )
	PaintCurve();
	// if ( FViewSectors )
	// if ( StateInput == siSector )

	PaintCurrentSector();

	// if ( FViewSectors )
	// PaintListSectors;
	// if ( FViewCircles )
	// if ( StateInput == siCircle )
	PaintCurrentCircle();
	// if (( FonPostPaint != NULL ) )
	// FonPostPaint( this, false );
	// SwapBuffers( DC );   // конец работы
	// EndPaint( hwnd1, ps );
}

void __fastcall TfrmGL::RadarCircle1Click(TObject *Sender) {
	hrc1 = wglCreateContext(dc1);
	wglMakeCurrent(dc1, hrc1);
	glClearColor(0.0, 0.0, 0.0, 1.0); // цвет фона
	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета

	// PaintCurrentCircle();
	// PaintGrid();
	// PaintCurve();

	// DrawWall();
	// DrawAxis();
	// PaintCurrentSector();
	myPaint();

	// PaintGL();

	glColor3f(0.0, 1., 1.);
	PaintTarget();

	SwapBuffers(dc1);
	wglMakeCurrent(0, 0);
	wglDeleteContext(hrc1);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::RzPanel1Paint(TObject *Sender)

{
	RadarCircle1Click(this);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::RzPanel1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)

{
	float k = 0.0;
	int NeSel = 0;
	float tmpX = 0.0, tmpY = 0.0;
	tmpX = (X - RzPanel1->Width / 2.0) / (RzPanel1->Width / 2.0) * Diagonal;
	tmpY = -(Y - RzPanel1->Height / 2.0) / (RzPanel1->Height / 2.0);
	tmpX = tmpX / Zoom;
	tmpY = tmpY / Zoom;
	// if (MousePosMove.State == siManualDrag) {
	MousePosMove.x = tmpX;
	MousePosMove.y = tmpY;
	// }
	// if ((FOnGLMouseMove != NULL)) {
	// FOnGLMouseMove(this, tmpX - ResultShiftScene.x, tmpY - ResultShiftScene.y);
	// }
	// if (((StateInput == siSector) || (StateInput == siCircle))&!LockUpMouse) {
	PosZoomX2 = tmpX - ResultShiftScene.x;
	PosZoomY2 = tmpY - ResultShiftScene.y;
	// }

	FLastMousePoint = Point(X, Y);

}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::InitViewProection() {

	float tmpX = 0.0, tmpY = 0.0;
	Diagonal = (float)(RzPanel1->Width / RzPanel1->Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0 * Diagonal / Zoom, 1.0 * Diagonal / Zoom, -1.0 / Zoom, 1.0 / Zoom, -1, 1);
	tmpX = 0;
	tmpY = 0;

	if (MousePosMove.State == siManualDrag) {
		tmpX = MousePosMove.x - MousePosDown.x;
		tmpY = MousePosMove.y - MousePosDown.y;
		ResultShiftScene.x = tmpX;
		ResultShiftScene.y = tmpY;
	}
	if (MousePosMove.State == siNone) {
		tmpX = ResultShiftScene.x;
		tmpY = ResultShiftScene.y;
	}
	glTranslatef(tmpX, tmpY, 0);
	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_MODELVIEW);
}

void __fastcall TfrmGL::InitViewProection(float x, float y, float dXX, float dYY) {
	float tmpX = 0.0, tmpY = 0.0;
	Diagonal = double(RzPanel1->Width) / RzPanel1->Height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0 * Diagonal / Zoom, 1.0 * Diagonal / Zoom, -1.0 / Zoom, 1.0 / Zoom, -1.0, 1.0);
	tmpX = x * Zoom;
	tmpY = y * Zoom;
	tmpX = -(tmpX - dXX);
	tmpY = -(tmpY - dYY);
	DX = tmpX / Zoom;
	DY = tmpY / Zoom;
	// glTranslatef(DX,DY,0);
	glViewport(0, 0, RzPanel1->Width, RzPanel1->Height);
	glMatrixMode(GL_MODELVIEW);
}

void __fastcall TfrmGL::PaintGL() {

	int i = 0, cnt = 0;
	float x = 0.0, y = 0.0, z = 0.0, k = 0.0, B = 0.0, D = 0.0;
	TPointFloat P1, P2, P3, P4;
	// TDetectedScan* Obj = NULL;
	// GLfloat ObjColor[ 4/*# range 0..3*/ ];
	glPointSize(4);
	glColor3f(1.0, 1.0, 1.0);
	x = 0.1;
	y = 0.1;
	z = 0.1;
	// glEnable(GL_BLEND);
	// glEnable(GL_POLYGON_SMOOTH);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// if ( ListTr == NULL )
	// return;
	// cnt = 0;
	// for ( int stop = ListTr.Count - 1, i = 0; i <= stop; i++)
	// {
	// Obj = ListTr.Items[i];
	// if ( PositionReg != 0 )
	// {
	// if ( Obj->RotID > PositionReg )
	// Continue;
	// if ( ( PositionReg - Obj->RotID ) > Shadown - 1 )
	// Continue;
	// }
	// if ( Obj->TypeTr == ttPrimaryMarabu )
	// if ( ( Obj->T & MainForm.chkTight.Checked ) | ( Obj->KMP & MainForm.chkKmp.Checked ) | ( Obj->W & MainForm.chkWidth.Checked ) )
	// ;
	// else
	// Continue;
	// if ( ( Obj->TypeTr == ttSecondary ) & ! MainForm.chkTrace.Checked )
	// Continue;
	// if ( ( Obj->TypeTr == ttUook ) & ! MainForm.chkUOOK.Checked )
	// Continue;
	// switch ( Obj->TypeTr )
	// {
	// case ttPrimaryMarabu: case ttAmplitude: case ttUook: case ttUOOKMod:
	// {

	B = 60 * 1.0 / 16384 * 2 * Pi;
	D = (50000 + 22) * 250.0 / 400000;

	// }
	// break;
	// case ttSecondary:
	// {
	// B = double( Obj->B ) * 1 / 4096 * 2 * Pi;
	// D = double( Obj->D ) * 50 / 400000;
	// }
	// break;
	// }
	x = D * sin(B);
	y = D * cos(B);
	P1.x = D * sin(B);
	P1.y = D * cos(B);
	P2.x = (D + 240.0 / 400000) * sin(B);
	P2.y = (D + 240.0 / 400000) * cos(B);
	P3.x = (D + 240.0 / 400000) * sin(B + 0.5 / 180 * Pi);
	P3.y = (D + 240.0 / 400000) * cos(B + 0.5 / 180 * Pi);
	P4.x = D * sin(B + 0.5 / 180 * Pi);
	P4.y = D * cos(B + 0.5 / 180 * Pi);
	// if ( Select )
	// glLoadName( i );
	// switch ( Obj->TypeTr )
	// {
	// case ttPrimaryMarabu:
	// {
	// if ( Obj->W )
	// Move( WColorGLObj, ObjColor, sizeof( GLfloat ) * 4 );
	// if ( Obj->T )
	// Move( TColorGLObj, ObjColor, sizeof( GLfloat ) * 4 );
	// if ( Obj->KMP )
	// Move( KMPColorGLObj, ObjColor, sizeof( GLfloat ) * 4 );
	// }
	// break;
	// case ttAmplitude:
	// {
	// Move( AColorGLObj, ObjColor, sizeof( GLfloat ) * 4 );
	// ObjColor[3] = Obj->Opacity;
	// }
	// break;
	// case ttSecondary:
	// Move( SecColorGLObj, ObjColor, sizeof( GLfloat ) * 4 );
	// break;
	// case ttUook:
	// {
	// Move( UookColorGLObj, ObjColor, sizeof( GLfloat ) * 4 );
	// }
	// break;
	// case ttUOOKMod:
	// {
	// Move( UookModColorGLObj, ObjColor, sizeof( GLfloat ) * 4 );
	// if ( Obj->AMax < 35 )
	// ObjColor[3] = 0.5;
	// }
	// break;
	// }
	// if ( Obj->Selected )
	// Move( SelColorGLObj, ObjColor, sizeof( GLfloat ) * 4 );
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ObjColor);
	// switch ( Obj->TypeTr )
	// {
	// case ttSecondary: case ttUook: case ttUOOKMod:
	// {
	// glBegin( GL_POINTS );
	// glVertex3d( x, y, 0.12 );
	// glEnd;
	// }
	// break;
	// default:
	// {
	z = 0.1 + i * 1e-7;
	// if ( Obj->Tag != 0 )
	// {
	glBegin(GL_POLYGON);
	glVertex3f(P1.x, P1.y, z);
	glVertex3f(P2.x, P2.y, z);
	glVertex3f(P3.x, P3.y, z);
	glVertex3f(P4.x, P4.y, z);
	glEnd();
	// }
	// else
	// {
	// glBegin( GL_POLYGON );
	// glVertex3f( P1.x, P1.y, z );
	// glVertex3f( P2.x, P2.y, z );
	// glVertex3f( P3.x, P3.y, z );
	// glVertex3f( P4.x, P4.y, z );
	// glEnd;
	// }
	// }
	// }
	// cnt++;
	// }
	// i = PositionReg;
	// if ( i == 0 )
	// i = CntRotInReg;
	// MainForm.Caption = "RotID:" + IntToStr( i ) + "," + IntToStr( ShiftSecondReg ) + "  [Objects:" + IntToStr( cnt ) + "] " + MainForm.OD.Filename;
}

void __fastcall TfrmGL::UpdateOGL() {
	InvalidateRect(hwnd1, 0, false);
}

// procedure TPanelOGL.SetOGLEnable(const Value: Boolean);

// begin
// FOGLEnable := Value;
// UpdateOGL;
// end;

// procedure TPanelOGL.SetAngleX(const Value: integer);

// begin
// FAngleX := Value;
// UpdateOGL;
// end;

// procedure TPanelOGL.SetAngleY(const Value: integer);
// begin
// FAngleY := Value;
// UpdateOGL;
// end;

// procedure TPanelOGL.SetOGLAxis(const Value: Boolean);
// begin
// FOGLAxis := Value;
// UpdateOGL;
// end;

// procedure TPanelOGL.SetOGLAxisText(const Value: Boolean);
// begin
// FOGLAxisText := Value;
// UpdateOGL;
// end;

// procedure TPanelOGL.SetOGLWall(const Value: Boolean);
// begin
// FOGLWall := Value;
// UpdateOGL;
// end;

void __fastcall TfrmGL::Rotation1Click(TObject *Sender) {
	wglMakeCurrent(dc2, hrc2);

	glClearColor(0.75, 0.75, 0.75, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.75);

	glRotatef(5, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.23678, 0.35118);
	glVertex2f(-0.23678, 0.7764);
	glVertex2f(-0.37966, 0.7764);
	glVertex2f(-0.55, 0.60606);
	glVertex2f(-0.55, -0.4);
	glVertex2f(-0.23576, -0.4);
	glVertex2f(-0.23678, 0.35118);
	glVertex2f(-0.23576, -0.4);
	glVertex2f(0.1375, -0.4);
	glVertex2f(0.13678, 0.35118);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0.1375, -0.4);
	glVertex2f(0.45, -0.4);
	glVertex2f(0.45, 0.60606);
	glVertex2f(0.27966, 0.7764);
	glVertex2f(0.13678, 0.7764);
	glVertex2f(0.13678, 0.35118);
	glEnd();

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);

}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::FormKeyPress(TObject *Sender, wchar_t &Key)

{
	// RzPanel2->Refresh();
	InvalidateRect(hwnd2, NULL, false);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::Cube1Click(TObject *Sender) {
	int wrk;
	wglMakeCurrent(dc2, hrc2);
	glClearColor(0.5, 0.5, 0.75, 1.0); // цвет фона
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glGetIntegerv(GL_MAX_LIGHTS, &wrk);
	Caption = IntToStr(wrk);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистка буфера цвета

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 4, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// этот фрагмент нужен для придания трёхмерности
	glTranslatef(0.0, 0.0, -8.0); // перенос объекта - ось Z
	glRotatef(30.0, 1.0, 0.0, 0.0); // поворот объекта - ось X
	glRotatef(70.0, 0.0, 1.0, 0.0); // поворот объекта - ось Y

	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::GLUTfigures1Click(TObject *Sender) {

	wglMakeCurrent(dc2, hrc2);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, ClientWidth / ClientHeight, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.5, 0.5, 0.75, 1.0); // цвет фона
	glColor3f(1.0, 0.0, 0.0);

	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета

	glPushMatrix();

	glTranslatef(0.0, 0.0, -3.0);

	gluSphere(quadObj, 1.0, 15, 10);
	glTranslatef(1, 1, 1);
	glutSolidCube(1);

	glPopMatrix();

	SwapBuffers(dc2);
	wglMakeCurrent(0, 0);
}

// ---------------------------------------------------------------------------

void __fastcall TfrmGL::Selectfigure1Click(TObject *Sender) {
	if (hrc2) {
		wglDeleteContext(hrc2);
	}
	hrc2 = wglCreateContext(dc2);
	wglMakeCurrent(dc2, hrc2);
	glClearColor(0.5, 0.5, 0.75, 1.0); // цвет фона
	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета

	// красный треугольник-- -
	glColor3f(1.0, 0.0, 0.0); // текущий цвет примитивов
	glBegin(GL_TRIANGLES);
	glVertex2f(-1, -1);
	glVertex2f(-1, 1);
	glVertex2f(-0.1, 0);
	glEnd();

	// синий треугольник-- -
	glColor3f(0.0, 0.0, 1.0); // текущий цвет примитивов
	glBegin(GL_TRIANGLES);
	glVertex2f(0.1, 0);
	glVertex2f(1, 1);
	glVertex2f(1, -1);
	glEnd();

	SwapBuffers(dc2);
	// wglMakeCurrent(0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::RzPanel2MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	unsigned char wrk[3];
	wglMakeCurrent(dc2, hrc2);
//	glReadPixels(X, Y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, wrk);
	 glReadPixels(X, RzPanel2->Height - Y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, wrk);

	if ((wrk[0] != 0) && (wrk[2] == 0))
		ShowMessage("Выбран красный треугольник");
	else if ((wrk[0] == 0) && (wrk[2] != 0))
		ShowMessage("Выбран синий треугольник");
	else
		ShowMessage("Ничего не выбрано");

	wrk[0] = 0;
	wrk[1] = 0;
	wrk[2] = 0;
	wglMakeCurrent(0, 0);

}
// ---------------------------------------------------------------------------

void __fastcall TfrmGL::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)

{
	// unsigned char wrk[3];
	// glReadPixels(X, Y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, wrk);
	// if ((wrk[0] != 0) && (wrk[2] == 0))
	// ShowMessage("Выбран красный треугольник");
	// else if ((wrk[0] == 0) && (wrk[2] != 0))
	// ShowMessage("Выбран синий треугольник");
	// else
	// ShowMessage("Ничего не выбрано");

}
// ---------------------------------------------------------------------------
