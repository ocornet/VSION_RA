#include "Mesh.h"



Mesh::Mesh(){
	vertices.clear();
	normals.clear();
	color = Vector3D(1,1,1);
	isOnHUD = false;
	reactToLight = true;
}

Mesh::Mesh(const char * path){
	LoadOBJ(path);
	color = Vector3D(1,1,1);
	isOnHUD = false;
	reactToLight=true;
}

Vector3D Mesh::getColor(){
	return color;
}
void Mesh::setColor(Vector3D col){
	color = col;
}


void Mesh::setIsOnHUD(bool hud){
	isOnHUD = hud;
}

void Mesh::setReactToLight(bool react){
	reactToLight = react;
}

//Charge un mesh à partir d'un fichier .obj
bool Mesh::LoadOBJ(const char * path){
	//vector qui contiendront les indices des vertices et normales dans l'ordre d'affichage
	std::vector< unsigned int > vertexIndices, normalIndices;

	//vector temporaires
	std::vector< Vector3D > temp_vertices;
	std::vector< Vector3D > temp_normals;

	//Variables intermédiaires
	double x,y,z;
	int vertexIndex[3], normalIndex[3];

	fstream file;
	file.open(path);

	if( file == NULL ){
		std::cout << "Impossible to open the file !" << endl;
		return false;
	}

	char lineHeader[128];
	while(!file.eof()){

		file >> lineHeader;
		//On lit les positions des vertices
		//Format de la ligne : "v <x> <y> <z>" sans les chevrons
		if ( strcmp( lineHeader, "v" ) == 0 ){
			Vector3D vertex;
			file >> x >> y >> z;
			vertex.setX(x);
			vertex.setY(y);
			vertex.setZ(z);
			temp_vertices.push_back(vertex);
		}
		//Les normales
		//Format de la ligne : "vn <x> <y> <z>" sans les chevrons
		else if ( strcmp( lineHeader, "vn" ) == 0 ){
			Vector3D normal;
			file >> x >> y >> z;
			normal.setX(x);
			normal.setY(y);
			normal.setZ(z);
			temp_normals.push_back(normal);
		}
		//Et enfin, l'ordre d'affichage des vertices pour afficher les triangles
		//Le format de la ligne est "f <v1>//<vn1> <v2>//<vn2> <v3>//<vn3>" sans les chevrons
		else if ( strcmp( lineHeader, "f" ) == 0 ){
			int ent;
			char c;
			for(int i=0; i<3; i++){
				file >> ent; // Indice du vertice
				vertexIndex[i] = ent; 
				file >> c; // Premier slash
				file >> c; // Deuxième slash
				file >> ent; // Indice de la normale
				normalIndex[i] = ent;
			}
			//On forme les triangles
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		
	}
	

	// Pour chaque vertice de chaque triangle
	for( int i=0; i < vertexIndices.size(); i++ ){
		int vertexIndex = vertexIndices[i];
		Vector3D vertex = temp_vertices[ vertexIndex-1 ];
		vertices.push_back(vertex);
	}
	for( int i=0; i < normalIndices.size(); i++ ){
		int normalIndex = normalIndices[i];
		Vector3D normal = temp_normals[ normalIndex-1 ];
		normals.push_back(normal);
	}

	std::cout << "Modele " << path << " charge avec succes !" << endl;
	return true;
}

//Afficher le mesh transformé par le transform t
void Mesh::Display() const{
		glPushMatrix();

		if(isOnHUD) glLoadIdentity();

		if(!reactToLight) glDisable(GL_LIGHTING);

		glBegin(GL_TRIANGLES);
		glColor3f(color.getX(),color.getY(),color.getZ());
		for(int i=0; i<vertices.size(); i++){
			glNormal3f(normals[i].getX(),normals[i].getY(),normals[i].getZ());
			glVertex3f(vertices[i].getX(),vertices[i].getY(),vertices[i].getZ());
			//glTexCoord2d(textures[i].getX(),textures[i].getY());
		}
		glEnd();

		if(!reactToLight) glEnable(GL_LIGHTING);

		glPopMatrix();
}

