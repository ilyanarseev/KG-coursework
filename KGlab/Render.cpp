#include "Render.h"
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "GUItextRectangle.h"

#include <array>
#include <math.h>

#define PI 3.14159265

using namespace std;

struct point {
	double x, y, z;
};

point A{ 0, -0.5, 0 };
point B{ 2, -1.5, 0 };
point C{ 4, 0, 0 };
point D{ 0.5, 0.5, 0 };
point E{ -1, 4.5, 0 };
point F{ -3.5, 1, 0 };
point G{ -1.5, -0.5, 0 };
point H{ -2, -3, 0 };

array<point, 8> dots{ A, B, C, D, E, F, G, H };

const double colors_set[17][3]{ { 0, 0, 1 }, {0.36, 0.57, 0.69}, { 0, 1, 0 }, {0.37, 0.15, 0.62}, { 0, 1, 1 },
	{1, 0, 0}, {0.35, 0.35, 0.67}, { 1, 0, 1 }, {0.4, 0, 1}, { 1, 1, 0 }, {0.59, 0.78, 0.64}, {0.01, 0.47, 0.58},
	{0.75, 1, 0.24}, {0.7, 0.2, 0}, {0.5, 0.6, 0.4}, {0.3, 0.73, 0.09}, {0.93, 0.6, 0} };

double coord_x_rotate(double x, double y, double c, double s) {
	return c * x - s * y;
}

double coord_y_rotate(double x, double y, double c, double s) {
	return s * x + c * y;
}

void set_dot(point A, double h) {
	glVertex3d(A.x, A.y, A.z + h);
}

// rc means ratio_convert
void rc(point A, point& c) {
	c.x = A.x / 7.5;
	c.y = A.y / 7.5;
}

double sqr(double x) { return x * x; }

void calcNormal(point A, point B, point C, point& N) {
	double nv;
	point v1, v2;

	v1.x = A.x - B.x;
	v1.y = A.y - B.y;
	v1.z = A.z - B.z;

	v2.x = B.x - C.x;
	v2.y = B.y - C.y;
	v2.z = B.z - C.z;

	nv = sqrt(sqr(v1.y * v2.z - v1.z * v2.y) + sqr(v1.z * v2.x - v1.x * v2.z) + sqr(v1.x * v2.y - v1.y * v2.x));
	N.x = (v1.y * v2.z - v1.z * v2.y) / nv;
	N.y = (v1.z * v2.x - v1.x * v2.z) / nv;
	N.z = (v1.x * v2.y - v1.y * v2.x) / nv;
}

int base_face(double h, int color_queue, GLdouble alpha = 0, bool isTexture = false) {
	point c;

	if (alpha)
		glColor4d(colors_set[color_queue][0], colors_set[color_queue][1], colors_set[color_queue][2], alpha);
	else glColor3dv(colors_set[color_queue]);
	color_queue++;
	if (isTexture) { rc(A, c); glTexCoord2d(c.x, c.y); }
	set_dot(A, h);
	if (isTexture) { rc(D, c); glTexCoord2d(c.x, c.y); }
	set_dot(D, h);
	if (isTexture) { rc(G, c); glTexCoord2d(c.x, c.y); }
	set_dot(G, h);
	if (isTexture) { rc(H, c); glTexCoord2d(c.x, c.y); }
	set_dot(H, h);

	if (alpha)
		glColor4d(colors_set[color_queue][0], colors_set[color_queue][1], colors_set[color_queue][2], alpha);
	else glColor3dv(colors_set[color_queue]);
	color_queue++;
	if (isTexture) { rc(G, c); glTexCoord2d(c.x, c.y); }
	set_dot(G, h);
	if (isTexture) { rc(F, c); glTexCoord2d(c.x, c.y); }
	set_dot(F, h);
	if (isTexture) { rc(E, c); glTexCoord2d(c.x, c.y); }
	set_dot(E, h);
	if (isTexture) { rc(D, c); glTexCoord2d(c.x, c.y); }
	set_dot(D, h);

	if (alpha)
		glColor4d(colors_set[color_queue][0], colors_set[color_queue][1], colors_set[color_queue][2], alpha);
	else glColor3dv(colors_set[color_queue]);
	color_queue++;
	if (isTexture) { rc(A, c); glTexCoord2d(c.x, c.y); }
	set_dot(A, h);
	if (isTexture) { rc(B, c); glTexCoord2d(c.x, c.y); }
	set_dot(B, h);
	if (isTexture) { rc(C, c); glTexCoord2d(c.x, c.y); }
	set_dot(C, h);
	if (isTexture) { rc(D, c); glTexCoord2d(c.x, c.y); }
	set_dot(D, h);

	return color_queue;
}

int side_face(double h, int color_queue, bool isRound = false) {
	point N;
	point upDot;
	for (int i = 0; i < 7; i++) {
		if (isRound) if (i == 1) continue;
		upDot = { dots.at(i + 1).x, dots.at(i + 1).y, dots.at(i + 1).z + h };
		calcNormal(dots.at(i), dots.at(i + 1), upDot, N);
		glNormal3d(N.x, N.y, N.z);

		glColor3dv(colors_set[color_queue]);
		color_queue++;
		set_dot(dots.at(i), 0);
		set_dot(dots.at(i + 1), 0);
		set_dot(dots.at(i + 1), h);
		set_dot(dots.at(i), h);
	}
	upDot = { dots.at(0).x, dots.at(0).y, dots.at(0).z + h };
	calcNormal(dots.at(7), dots.at(0), upDot, N);
	glNormal3d(N.x, N.y, N.z);

	glColor3dv(colors_set[color_queue]);
	color_queue++;
	set_dot(dots.at(7), 0);
	set_dot(dots.at(0), 0);
	set_dot(dots.at(0), h);
	set_dot(dots.at(7), h);

	return color_queue;
}

int round_base_face(double h, int color_queue, int d, point A1, point A2, GLdouble alpha = 0, bool isTexture = false) {
	point tex;

	double c = cos(d * PI / 180);
	double s = sin(d * PI / 180);
	double mx = (A1.x + A2.x) / 2;
	double my = (A1.y + A2.y) / 2;
	double x = A1.x - mx;
	double y = A1.y - my;
	double nx;
	double ny;

	glBegin(GL_POLYGON);
	if (alpha)
		glColor4d(colors_set[color_queue][0], colors_set[color_queue][1], colors_set[color_queue][2], alpha);
	else glColor3dv(colors_set[color_queue]);
	color_queue++;
	for (int i = 0; i <= 180; i = i + d) {
		if (isTexture) { rc(point{ x + mx, y + my, h }, tex); glTexCoord2d(tex.x, tex.y); }
		glVertex3d(x + mx, y + my, h);
		nx = coord_x_rotate(x, y, c, s);
		ny = coord_y_rotate(x, y, c, s);
		x = nx;
		y = ny;
	}
	glEnd();

	return color_queue;
}

int round_side_face(double h, int color_queue, int d, point A1, point A2) {
	double c = cos(d * PI / 180);
	double s = sin(d * PI / 180);
	double mx = (A1.x + A2.x) / 2;
	double my = (A1.y + A2.y) / 2;
	double x = A1.x - mx;
	double y = A1.y - my;
	double nx;
	double ny;

	glBegin(GL_QUAD_STRIP);
	glColor3dv(colors_set[color_queue]);
	color_queue++;
	for (int i = 0; i <= 180; i = i + d) {
		glVertex3d(x + mx, y + my, 0);
		glVertex3d(x + mx, y + my, h);
		nx = coord_x_rotate(x, y, c, s);
		ny = coord_y_rotate(x, y, c, s);
		x = nx;
		y = ny;
	}
	glEnd();

	return color_queue;
}

void sample1() {
	int color_queue = 0;
	int h = 2;

	glBegin(GL_QUADS);
	glNormal3d(0, 0, -1);
	color_queue = base_face(0, color_queue);
	glNormal3d(0, 0, 1);
	color_queue = base_face(h, color_queue);
	color_queue = side_face(h, color_queue);
	glEnd();
}

void sample2() {
	int color_queue = 0;
	int h = 2;
	point c;

	glBegin(GL_QUADS);
	glNormal3d(0, 0, -1);
	color_queue = base_face(0, color_queue);

	glNormal3d(0, 0, 1);
	color_queue = base_face(h, color_queue, 0, true);

	color_queue = side_face(h, color_queue);
	glEnd();
}

void sample3() {
	int color_queue = 0;
	int h = 2;
	int d = 10;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	color_queue = base_face(0, color_queue);
	color_queue = base_face(h, color_queue, 0.5);
	color_queue = side_face(h, color_queue, true);
	glEnd();

	color_queue = round_base_face(0, color_queue, d, B, C);
	color_queue = round_base_face(h, color_queue, d, B, C, 0.5);
	color_queue = round_side_face(h, color_queue, d, B, C);
}

void sample4() {
	int color_queue = 0;
	int h = 2;
	int d = 10;

	glBegin(GL_QUADS);
	color_queue = base_face(0, color_queue);
	color_queue = base_face(h, color_queue);
	color_queue = side_face(h, color_queue, true);
	glEnd();

	color_queue = round_base_face(0, color_queue, d, B, C);
	color_queue = round_base_face(h, color_queue, d, B, C);
	color_queue = round_side_face(h, color_queue, d, B, C);
}

void sample5() {
	int color_queue = 0;
	int h = 2;
	int d = 10;

	glBegin(GL_QUADS);
	color_queue = base_face(0, color_queue);
	color_queue = base_face(h, color_queue, 0, true);
	color_queue = side_face(h, color_queue, true);
	glEnd();

	color_queue = round_base_face(0, color_queue, d, B, C);
	color_queue = round_base_face(h, color_queue, d, B, C, 0, true);
	color_queue = round_side_face(h, color_queue, d, B, C);
}

#ifdef _DEBUG
#include <Debugapi.h> 
struct debug_print
{
	template<class C>
	debug_print& operator<<(const C& a)
	{
		OutputDebugStringA((std::stringstream() << a).str().c_str());
		return *this;
	}
} debout;
#else
struct debug_print
{
	template<class C>
	debug_print& operator<<(const C& a)
	{
		return *this;
	}
} debout;
#endif

//библиотека для разгрузки изображений
//https://github.com/nothings/stb
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//внутренняя логика "движка"
#include "MyOGL.h"
extern OpenGL gl;
#include "Light.h"
Light light;
#include "Camera.h"
Camera camera;


bool texturing = true;
bool lightning = true;
bool alpha = false;


//переключение режимов освещения, текстурирования, альфаналожения
void switchModes(OpenGL *sender, KeyEventArg arg)
{
	//конвертируем код клавиши в букву
	auto key = LOWORD(MapVirtualKeyA(arg.key, MAPVK_VK_TO_CHAR));

	switch (key)
	{
	case 'L':
		lightning = !lightning;
		break;
	case 'T':
		texturing = !texturing;
		break;
	case 'A':
		alpha = !alpha;
		break;
	}
}

//Текстовый прямоугольничек в верхнем правом углу.
//OGL не предоставляет возможности для хранения текста
//внутри этого класса создается картинка с текстом (через виндовый GDI),
//в виде текстуры накладывается на прямоугольник и рисуется на экране.
//Это самый простой способ что то написать на экране
//но ооооочень не оптимальный
GuiTextRectangle text;

//айдишник для текстуры
GLuint texId;
//выполняется один раз перед первым рендером
void initRender()
{
	//==============НАСТРОЙКА ТЕКСТУР================
	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//просим сгенерировать нам Id для текстуры
	//и положить его в texId
	glGenTextures(1, &texId);

	//делаем текущую текстуру активной
	//все, что ниже будет применено texId текстуре.
	glBindTexture(GL_TEXTURE_2D, texId);


	int x, y, n;

	//загружаем картинку
	//см. #include "stb_image.h" 
	unsigned char* data = stbi_load("texture.png", &x, &y, &n, 4);
	//x - ширина изображения
	//y - высота изображения
	//n - количество каналов
	//4 - нужное нам количество каналов
	//пиксели будут хранится в памяти [R-G-B-A]-[R-G-B-A]-[..... 
	// по 4 байта на пиксель - по байту на канал
	//пустые каналы будут равны 255

	//Картинка хранится в памяти перевернутой 
	//так как ее начало в левом верхнем углу
	//по этому мы ее переворачиваем -
	//меняем первую строку с последней,
	//вторую с предпоследней, и.т.д.
	unsigned char* _tmp = new unsigned char[x * 4]; //времянка
	for (int i = 0; i < y / 2; ++i)
	{
		std::memcpy(_tmp, data + i * x * 4, x * 4);//переносим строку i в времянку
		std::memcpy(data + i * x * 4, data + (y - 1 - i) * x * 4, x * 4); //(y-1-i)я строка -> iя строка
		std::memcpy(data + (y - 1 - i) * x * 4, _tmp, x * 4); //времянка -> (y-1-i)я строка
	}
	delete[] _tmp;


	//загрузка изображения в видеопамять
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//выгрузка изображения из опперативной памяти
	stbi_image_free(data);


	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
												  //GL_REPLACE -- полная замена политога текстурой
	//настройка тайлинга
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//настройка фильтрации
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//======================================================

	//================НАСТРОЙКА КАМЕРЫ======================
	camera.caclulateCameraPos();

	//привязываем камеру к событиям "движка"
	gl.WheelEvent.reaction(&camera, &Camera::Zoom);
	gl.MouseMovieEvent.reaction(&camera, &Camera::MouseMovie);
	gl.MouseLeaveEvent.reaction(&camera, &Camera::MouseLeave);
	gl.MouseLdownEvent.reaction(&camera, &Camera::MouseStartDrag);
	gl.MouseLupEvent.reaction(&camera, &Camera::MouseStopDrag);
	//==============НАСТРОЙКА СВЕТА===========================
	//привязываем свет к событиям "движка"
	gl.MouseMovieEvent.reaction(&light, &Light::MoveLight);
	gl.KeyDownEvent.reaction(&light, &Light::StartDrug);
	gl.KeyUpEvent.reaction(&light, &Light::StopDrug);
	//========================================================
	//====================Прочее==============================
	gl.KeyDownEvent.reaction(switchModes);
	text.setSize(512, 180);
	//========================================================
	   

	camera.setPosition(2, 1.5, 1.5);
}

void Render(double delta_time)
{    
	glEnable(GL_DEPTH_TEST);
	
	//натройка камеры и света
	//в этих функциях находятся OGLные функции
	//которые устанавливают параметры источника света
	//и моделвью матрицу, связанные с камерой.

	if (gl.isKeyPressed('F')) //если нажата F - свет из камеры
	{
		light.SetPosition(camera.x(), camera.y(), camera.z());
	}
	camera.SetUpCamera();
	light.SetUpLight();


	//рисуем оси
	gl.DrawAxes();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	

	//включаем режимы, в зависимости от нажания клавиш. см void switchModes(OpenGL *sender, KeyEventArg arg)
	if (lightning)
		glEnable(GL_LIGHTING);
	if (texturing)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0); //сбрасываем текущую текстуру
	}
		
	if (alpha)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
		
	//=============НАСТРОЙКА МАТЕРИАЛА==============


	//настройка материала, все что рисуется ниже будет иметь этот метериал.
	//массивы с настройками материала
	float  amb[] = { 0.2, 0.2, 0.1, 1. };
	float dif[] = { 0.4, 0.65, 0.5, 1. };
	float spec[] = { 0.9, 0.8, 0.3, 1. };
	float sh = 0.2f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); 
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH); //закраска по Гуро      
			   //(GL_SMOOTH - плоская закраска)

	//============ РИСОВАНИЕ ПРИЗМЫ ==============

	glBindTexture(GL_TEXTURE_2D, texId);
	sample5();

	//===============================================

	//рисуем источник света
	light.DrawLightGizmo();

	//================Сообщение в верхнем левом углу=======================

	//переключаемся на матрицу проекции
	glMatrixMode(GL_PROJECTION);
	//сохраняем текущую матрицу проекции с перспективным преобразованием
	glPushMatrix();
	//загружаем единичную матрицу в матрицу проекции
	glLoadIdentity();
	
	//устанавливаем матрицу паралельной проекции
	glOrtho(0, gl.getWidth() - 1, 0, gl.getHeight() - 1, 0, 1);

	//переключаемся на моделвью матрицу
	glMatrixMode(GL_MODELVIEW);
	//сохраняем матрицу
	glPushMatrix();
    //сбразываем все трансформации и настройки камеры загрузкой единичной матрицы
	glLoadIdentity();

	//отрисованное тут будет визуалзироватся в 2д системе координат
	//нижний левый угол окна - точка (0,0)
	//верхний правый угол (ширина_окна - 1, высота_окна - 1)

	
	std::wstringstream ss;
	ss << std::fixed << std::setprecision(3);
	ss << "T - " << (texturing ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"текстур" << std::endl;
	ss << "L - " << (lightning ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"освещение" << std::endl;
	ss << "A - " << (alpha ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"альфа-наложение" << std::endl;
	ss << L"F - Свет из камеры" << std::endl;
	ss << L"G - двигать свет по горизонтали" << std::endl;
	ss << L"G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << L"Коорд. света: (" << std::setw(7) <<  light.x() << "," << std::setw(7) << light.y() << "," << std::setw(7) << light.z() << ")" << std::endl;
	ss << L"Коорд. камеры: (" << std::setw(7) << camera.x() << "," << std::setw(7) << camera.y() << "," << std::setw(7) << camera.z() << ")" << std::endl;
	ss << L"Параметры камеры: R=" << std::setw(7) << camera.distance() << ",fi1=" << std::setw(7) << camera.fi1() << ",fi2=" << std::setw(7) << camera.fi2() << std::endl;
	ss << L"delta_time: " << std::setprecision(5)<< delta_time << std::endl;

	
	text.setPosition(10, gl.getHeight() - 10 - 180);
	text.setText(ss.str().c_str());
	text.Draw();

	//восстанавливаем матрицу проекции на перспективу, которую сохраняли ранее.
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}   
