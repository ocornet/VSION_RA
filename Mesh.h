#pragma once


#include "vector3d.h"
#include <vector>
#include <stack>

#include <stdlib.h>
#include <math.h>

#include <Windows.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>

class Mesh
{
private:
	
	std::vector< Vector3D > vertices, normals;
	Vector3D color;
	bool isOnHUD;
	bool reactToLight;

public:
	
	//Couleur du mesh
	Vector3D getColor(); 
	void setColor(Vector3D col);

	//Dessine t-on l'objet sur le HUD ?
	void setIsOnHUD(bool hud);

	void setReactToLight(bool react);

	Mesh();
	Mesh(const char *path);
	bool LoadOBJ(const char * path);
	void Display() const;

};
