#include "StdAfx.h"
#include "CGLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"

/*
#include "GL\glaux.h"
#include "GL\glut.h"
*/
#define PI 3.14
#define toRad (PI/180)

/*
Podesavanje okruzenja:
Project->Properties -> Link ->Input
Additional Dependencies(dodati ovde sledece) OpenGL32.lib glu32.lib
13. slajd prezentacija
*/

CGLRenderer::CGLRenderer()
{
	valjak2_z = 0;
	x = y = z = 0;
	alfaX = alfaY = alfaZ = 0;
	kvadar_x = 0;
	kupa_z = kupa_y = 0;
	lx = ly = lz = 0;
}


CGLRenderer::~CGLRenderer()
{
}

//kod sa prezentacija

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	PrepareLighting();
	PrepareMaterials();
	//glEnable(GL_NORMALIZE);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);

}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glViewport(0, 0, w, h);
	double aspect = (double)w / (double)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	//PrepareMaterials();

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::PrepareLighting()
{
	float light1_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	float light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { 4, 3, 4 };
	GLfloat light1_direction[] = { 0,0,0 };

	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);

	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	//Slabljenje
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);

	//Usmerenje izvora
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 7.0);

	//Aktiviranje
	glEnable(GL_LIGHT1);
}

void CGLRenderer::PrepareMaterials()
{
	matBase.SetAmbient(0.3, 0.3, 0.3, 0);
	matBase.SetDiffuse(0.3, 0.3, 0.3, 0);
	matBase.SetEmission(100, 100, 100, 0);
	matBase.SetShininess(0.2);
	matBase.SetSpecular(0, 0, 0, 0);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	//SPRECAVA TRANSPARENCIJU
	//END

	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//da bih podesio pogled kako meni odgovara jer
	//je pocetno stanje jako cudno i tesko se prati
	//desavanje na prozoru.
	glTranslatef(x, y, z);
	glRotatef(alfaX, 1, 0, 0);
	glRotatef(alfaY, 0, 1, 0);
	glRotatef(alfaZ, 0, 0, 1);
	//glRotatef(0, 0.0, 1.0, 0.0);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//generateAxes();

	this->DrawBox(50);
	glTranslatef(0, 25, 0);
	this->DrawVase(10);



	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::generateAxes() {
	glLineWidth(3);
	glPointSize(10);
	glBegin(GL_LINES);

	matBlue.Select();
	glVertex3f(0, 0, 0);
	glVertex3f(100, 0, 0);

	matRed.Select();
	glVertex3f(0, 0, 0);
	glVertex3f(0, 100, 0);

	matGreen.Select();
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 100);

	glEnd();
}

void CGLRenderer::DrawKvadar(float w, float h)
{
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glVertex3f(-w / 2, h, -w / 2);
	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2, 0, -w / 2);
	glVertex3f(-w / 2, 0, -w / 2);

	glNormal3f(0, 0, 1);
	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(w / 2, h, w / 2);
	glVertex3f(-w / 2, h, w / 2);

	glNormal3f(-1, 0, 0);
	glVertex3f(-w / 2, 0, -w / 2);
	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(-w / 2, h, w / 2);
	glVertex3f(-w / 2, h, -w / 2);

	glNormal3f(1, 0, 0);
	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2, h, w / 2);
	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(w / 2, 0, -w / 2);

	glNormal3f(0, -1, 0);
	glVertex3f(w / 2, 0, -w / 2);
	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(-w / 2, 0, -w / 2);

	glNormal3f(0, 1, 0);
	glVertex3f(-w / 2, h, -w / 2);
	glVertex3f(-w / 2, h, w / 2);
	glVertex3f(w / 2, h, w / 2);
	glVertex3f(w / 2, h, -w / 2);

	glEnd();

	matGreen.Select();
	glBegin(GL_LINES);

	glVertex3f(-w / 2, h, -w / 2);
	glVertex3f(-w / 2, h, -w / 2 - 1);

	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2, h, -w / 2 - 1);

	glVertex3f(w / 2, 0, -w / 2);
	glVertex3f(w / 2, 0, -w / 2 - 1);

	glVertex3f(-w / 2, 0, -w / 2);
	glVertex3f(-w / 2, 0, -w / 2 - 1);


	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(-w / 2, 0, w / 2 + 1);

	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(w / 2, 0, w / 2 + 1);

	glVertex3f(w / 2, h, w / 2);
	glVertex3f(w / 2, h, w / 2 + 1);

	glVertex3f(-w / 2, h, w / 2);
	glVertex3f(-w / 2, h, w / 2 + 1);


	glVertex3f(-w / 2, 0, -w / 2);
	glVertex3f(-w / 2 - 1, 0, -w / 2);

	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(-w / 2 - 1, 0, w / 2);

	glVertex3f(-w / 2, h, w / 2);
	glVertex3f(-w / 2 - 1, h, w / 2);

	glVertex3f(-w / 2, h, -w / 2);
	glVertex3f(-w / 2 - 1, h, -w / 2);


	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2 + 1, h, -w / 2);

	glVertex3f(w / 2, h, w / 2);
	glVertex3f(w / 2 + 1, h, w / 2);

	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(w / 2 + 1, 0, w / 2);

	glVertex3f(w / 2, 0, -w / 2);
	glVertex3f(w / 2 + 1, 0, -w / 2);


	glVertex3f(w / 2, 0, -w / 2);
	glVertex3f(w / 2, 0 - 1, -w / 2);

	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(w / 2, 0 - 1, w / 2);

	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(-w / 2, 0 - 1, w / 2);

	glVertex3f(-w / 2, 0, -w / 2);
	glVertex3f(-w / 2, 0 - 1, -w / 2);


	glVertex3f(-w / 2, h, -w / 2);
	glVertex3f(-w / 2, h + 1, -w / 2);

	glVertex3f(-w / 2, h, w / 2);
	glVertex3f(-w / 2, h + 1, w / 2);

	glVertex3f(w / 2, h, w / 2);
	glVertex3f(w / 2, h + 1, w / 2);

	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2, h + 1, -w / 2);

	glEnd();
	matObject.Select();
}

void CGLRenderer::DrawBase(float r, float h) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, h / 2, 0.0f);
	for (int i = 0; i <= 360; i = i + 45)
	{
		glVertex3f(r * cos(i * toRad), h, r * sin(i * toRad));
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, -h / 2, 0.0f);
	for (int i = 0; i <= 360; i = i + 45)
	{
		glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i = i + 45) {
		glVertex3f(r * cos(i * toRad), h, r * sin(i * toRad));
		glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
	}
	glEnd();
}

void CGLRenderer::DrawValjak(float r, float h) {
	glBegin(GL_TRIANGLE_FAN);

	glNormal3f(0, 1, 0);
	glVertex3f(0.0f, h, 0.0f);
	for (int i = 0; i <= 360; i += 45)
	{
		glVertex3f(r * cos(-i * toRad), h, r * sin(-i * toRad));
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, -1, 0);
	glVertex3f(0.0f, 0, 0.0f);
	for (int i = 0; i <= 360; i += 45)
	{
		glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i += 45) {
		glNormal3f(cos(i * toRad), 0, sin(i * toRad));
		glVertex3f(r * cos(-i * toRad), h, r * sin(-i * toRad));
		glVertex3f(r * cos(-i * toRad), 0, r * sin(-i * toRad));
	}
	glEnd();

	matGreen.Select();
	glBegin(GL_LINES);
	for (int i = 360; i >= 0; i -= 45) {
		glVertex3f(r * cos(i * toRad), h, r * sin(i * toRad));
		glVertex3f(r * cos(i * toRad) + cos(i * toRad), h, r * sin(i * toRad) + sin(i * toRad));
		glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
		glVertex3f(r * cos(i * toRad) + cos(i * toRad), 0, r * sin(i * toRad) + sin(i * toRad));
	}
	glEnd();
	matObject.Select();
}

void CGLRenderer::DrawKupa(float r, float h) {
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, -1, 0);
	glVertex3f(0, 0, 0);
	for (int i = 0; i <= 360; i += 60) { //360/60 => 6 strana
		glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
	}
	glEnd();

	int alfa = atan(r / (2 * h));

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, h, 0);
	for (int i = 0; i <= 360; i += 60) { //360/60 => 6 strana
		glNormal3f(sin(alfa * toRad), cos(alfa * toRad), 0);
		glVertex3f(r * cos(-i * toRad), 0, r * sin(-i * toRad));
	}
	glEnd();

	float sinF = sin(atan((float)r / h));
	float cosF = cos(atan((float)r / h));
	matGreen.Select();

	glBegin(GL_LINES);
	for (int i = 360; i >= 0; i -= 60) {
		glVertex3f(0, h, 0);
		glVertex3f(0 + cos(i * toRad) * cosF, h + sinF, 0 + sin(i * toRad) * cosF);
		glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
		glVertex3f(r * cos(i * toRad) + cos(i * toRad) * cosF, 0 + sinF, r * sin(i * toRad) + sin(i * toRad) * cosF);
	}
	glEnd();
	matObject.Select();
}

void CGLRenderer::DrawBox(float size) {

	glBegin(GL_QUADS);

	glNormal3f(0, 0, -1);
	glVertex3f(size / 2, size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-size / 2, size / 2, size / 2);

	glNormal3f(-1, 0, 0);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(-size / 2, size / 2, -size / 2);

	glNormal3f(1, 0, 0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);

	glNormal3f(0, -1, 0);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, -size / 2);

	glNormal3f(0, 1, 0);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glVertex3f(-size / 2, size / 2, size / 2);
	glVertex3f(size / 2, size / 2, size / 2);
	glVertex3f(size / 2, size / 2, -size / 2);

	glEnd();
}
void CGLRenderer::DrawVase(double size)
{
	float dots[8][2] = { {0, 15}, {10, 10}, {20, 17.5}, {30, 18.5}, {40, 15}, {45, 8}, {55, 8}, {63, 10} };

	glBegin(GL_QUAD_STRIP);

	for (int j = 0; j < 7; j++) {

		for (int i = 0; i <= 360; i += 1) {
			glNormal3f(sin(i * toRad - (90 + 45.0) / 2), 0, cos(i * toRad - (90 + 45.0) / 2));

			glVertex3f(dots[j + 1][1] * cos(-i * toRad), dots[j + 1][0], dots[j + 1][1] * sin(-i * toRad));
			glVertex3f(dots[j][1] * cos(-i * toRad), dots[j][0], dots[j][1] * sin(-i * toRad));
		}
	}

	glEnd();

	glBegin(GL_QUAD_STRIP);

	for (int j = 0; j < 7; j++) {

		for (int i = 0; i <= 360; i += 1) {
			glNormal3f(sin(i * toRad - 90), 0, cos(i * toRad - 90));

			glTexCoord3f(0, 0, 0);
			glVertex3f(dots[j][1] * cos(-i * toRad), dots[j][0], dots[j][1] * sin(-i * toRad));
			glTexCoord3f(0, 1, 0);
			glVertex3f(dots[j + 1][1] * cos(-i * toRad), dots[j + 1][0], dots[j + 1][1] * sin(-i * toRad));
		}
	}

	glEnd();
}

//OVO NE KORISTITITI 
void CGLRenderer::drawUNOPTIMIZEDSphere(float r) {

	for (int j = 0; j <= 180; j += 1) {
		glPushMatrix();
		glRotatef(j, 1, 0, 0);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= 360; i++) { //360
			glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
		}
		glEnd();
		glPopMatrix();
	}

	for (int j = 0; j <= 180; j += 1) {
		glPushMatrix();
		glRotatef(j, 0, 1, 0);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= 360; i++) { //360
			glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
		}
		glEnd();
		glPopMatrix();
	}

	for (int j = 0; j <= 180; j += 1) {
		glPushMatrix();
		glRotatef(j, 0, 0, 1);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= 360; i++) { //360
			glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
		}
		glEnd();
		glPopMatrix();
	}
}

void CGLRenderer::drawSphere(float r) {
	glBegin(GL_QUAD_STRIP);
	for (double j = 0; j <= 180; j++) // Azimuth [0, 2PI]
	{
		for (double i = 0; i <= 360; i++)
		{
			glVertex3f(r * cos(j * toRad) * sin(i * toRad),
				r * sin(j * toRad) * sin(i * toRad),
				r * cos(i * toRad));
			glVertex3f(r * cos(j * toRad + 1) * sin(i * toRad + 1),
				r * sin(j * toRad + 1) * sin(i * toRad + 1),
				r * cos(i * toRad + 1));
		}
	}
	glEnd();

	//reference http://www.cplusplus.com/forum/general/65476/
}

void CGLRenderer::drawSphere2(float r) {
	glBegin(GL_QUAD_STRIP);
	for (int j = -90; j < 90; j++) {
		for (int i = 0; i <= 360; i++) {

			glNormal3f(cos(i * toRad) * cos(j * toRad), sin(j * toRad), sin(i * toRad) * cos(j * toRad));
			glVertex3f(r * cos(i * toRad) * cos(j * toRad), r * sin(j * toRad), r * sin(i * toRad) * cos(j * toRad));
			glNormal3f(cos(i * toRad) * cos((j + 1) * toRad), sin((j + 1) * toRad), sin(i * toRad) * cos((j + 1) * toRad));
			glVertex3f(r * cos(i * toRad) * cos((j + 1) * toRad), r * sin((j + 1) * toRad), r * sin(i * toRad) * cos((j + 1) * toRad));
		}
	}
	glEnd();
	glTranslatef(0, -r, 0);
}

void CGLRenderer::DrawHalftSphere(float r)
{
	glTranslatef(0, r, 0);
	//glutSolidSphere(r, 100, 100);
	int step = 3;
	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j < 90; j += step) {
		for (int i = 0; i <= 360; i += 11.25) {

			glNormal3f(cos(i * toRad) * cos(j * toRad), sin(j * toRad), sin(i * toRad) * cos(j * toRad));
			glVertex3f(r * cos(i * toRad) * cos(j * toRad), r * sin(j * toRad), r * sin(i * toRad) * cos(j * toRad));
			glNormal3f(cos(i * toRad) * cos((j + step) * toRad), sin((j + step) * toRad), sin(i * toRad) * cos((j + step) * toRad));
			glVertex3f(r * cos(i * toRad) * cos((j + step) * toRad), r * sin((j + step) * toRad), r * sin(i * toRad) * cos((j + step) * toRad));
		}
	}
	glEnd();
	matGreen.Select();
	glBegin(GL_LINES);
	for (int j = 0; j < 90; j += 3)
	{
		for (int i = 0; i <= 360; i += 11.25)
		{

			glVertex3f(r * cos(i * toRad) * cos(j * toRad), r * sin(j * toRad), r * sin(i * toRad) * cos(j * toRad));
			glVertex3f(r * cos(i * toRad) * cos(j * toRad) + cos(i * toRad) * cos(j * toRad), r * sin(j * toRad) + sin(j * toRad), r * sin(i * toRad) * cos(j * toRad) + sin(i * toRad) * cos(j * toRad));
			//glVertex3f(r * cos(i*toRad)*cos((j + 1)*toRad), r*sin((j + 1)*toRad), r * sin(i*toRad)*cos((j + 1)*toRad));
			//glVertex3f(r * cos(i*toRad)*cos((j + 1)*toRad) + cos(i*toRad)*cos((j + 1)*toRad), r*sin((j + 1)*toRad) + sin((j + 1)*toRad), r * sin(i*toRad)*cos((j + 1)*toRad + sin(i*toRad)*cos((j + 1)*toRad)));
		}
	}
	glEnd();
	matObject.Select();
	glTranslatef(0, -r, 0);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}