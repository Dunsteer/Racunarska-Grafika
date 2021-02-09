#pragma once
#include "CGLMaterial.h"

class CGLRenderer
{
public:

	//camera
	float valjak2_z;
	float global_x, global_y;
	bool mouseStart=false;
	float cameraAngleX, cameraAngleY, cameraAngleZ;
	float startX, startY, startZ;
	int lx, ly, lz;
	//end_camera


	float x, y, z;
	float alfaX, alfaY, alfaZ;
	float kvadar_x;
	float kupa_z, kupa_y;
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	//funckije sa prezentacije, neophodne za rad sa OpenGL-om
	bool CreateGLContext(CDC* pDC); //Create a OpenGL Rendering Context
	void PrepareScene(CDC* pDC); //Init scene
	void DestroyScene(CDC* pDC); 
	void Reshape(CDC* pDC, int w, int h); 
	void DrawScene(CDC* pDC);

	//my func
	void drawCube(float a);
	void DrawKvadar(float w, float h);
	void DrawBase(float r, float h);
	void DrawValjak(float r, float h);
	void DrawKupa(float r, float h);
	void drawUNOPTIMIZEDSphere(float r);
	void drawSphere(float r);
	void DrawBox(float a); //a je duzina visine(sirine) kocke
	void DrawVase(double size);
	void DrawHalftSphere(float r);
	void drawSphere2(float r);
	//end_my_func

	//light&material
	void PrepareLighting();
	void PrepareMaterials();
	//end_light&material

	//helper
	void generateAxes();
	//endhelper

	//material
	CGLMaterial matBase;
	CGLMaterial matPyramid;
	CGLMaterial matObject;
	CGLMaterial matSvetlo;
	CGLMaterial matRed;
	CGLMaterial matGreen;
	CGLMaterial matBlue;
	//end_material

protected:
	HGLRC m_hrc; //OpenGL Rendering context
};

