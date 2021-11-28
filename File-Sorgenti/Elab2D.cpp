#include <iostream>
#include "ShaderMaker.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cstdlib>

#define PI   3.14159265358979323846

using namespace glm;
using namespace std;

static unsigned int Quadrat_MatModel, Bordo_Model,programId, programId_text, programId_Sfondo, MatProj,  MatModel, MatView, MatrixProjS, MatModelS, MatViewS, loc_time, loc_res, loc_mouse, Sfondo_Model;
unsigned int lsceltavs, lsceltafs;
int width = 1280;
int height = 720;
int BORDO_X_DESTRO_GIOCO = width - 50;
int BORDO_X_SINISTRO_GIOCO = 50;
mat4 Projection;
vec2 res, mouse;
int posx = (float)width / 4.0, posy = (float)height / 2.0;
bool GAME = true;
float angolo = 0;
float posx_quadrato;
int OUT_BOUND = width;
int TOTALE_QUADRATINI = 150;
float MAX_SCALE_CERCHIO = 35.0;
float MIN_SCALE_CERCHIO = 20.0;
float SCALE_TRIANGOLO = 15.0f;
float SCALE_CERCHIO = SCALE_TRIANGOLO + 10.0f;
float SCALE_BORDO_TRIANGOLO = SCALE_TRIANGOLO + 10.0;
float angolo_triangolo = 0.0f;
float angolo_bordo_triangolo = 0.0f;
float DANNO_END_GAME = 15.0f;
float danno_triangolo = 0.0f;
const float DANNO_STELLA = 0.1f;
int nTriangles = 32;  //Numero di triangoli
int nVertices = nTriangles + 2;  //Numero di vertici
float MARGIN_X_TRIANGLE = 15.0;
float MARGIN_Y_TOP = -11.0;
float MARGIN_Y_BOTTOM = 11.0;


typedef struct {
	GLuint VAO;
	GLuint VBO_G;
	GLuint VBO_C;
	GLuint EBO_indici;
	int nTriangles;
	// Vertici
	vector<vec3> vertici;
	vector<vec3> CP;
	vector<vec4> colors;
	vector<int> indici;
	// Numero vertici
	int nv;
	//Matrice di Modellazione: Traslazione*Rotazione*Scala
	mat4 Model;
	int sceltaVS;
	int sceltaFS;
	string name;
} Figura;


Figura Triangolo, BordoTriangolo,Quadratino,BordiQuadrato,Sfondo;
Figura Cerchio;
vector<Figura> Navicella;

void INIT_SHADER(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader_S.glsl";
	char* fragmentShader = (char*)"fragmentShader_S.glsl";
	char* fragmentShaderSfondo = (char*)"fragmentShader_Sfondo.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	programId_Sfondo = ShaderMaker::createProgram(vertexShader, fragmentShaderSfondo);
	glUseProgram(programId);
}

void crea_VAO_vector(Figura* fig) {

	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, fig->vertici.size() * sizeof(vec3), fig->vertici.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &fig->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(vec4), fig->colors.data(), GL_STATIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

}

void costruisci_cerchio(float cx, float cy, float raggiox, float raggioy, Figura* cerchio) {
		
	int i;
	float stepA = (2 * PI) / nTriangles;
	
	cerchio->vertici.push_back(vec3(cx, cy, 0.0));
	cerchio->colors.push_back(vec4(217.0/256.0, 217.0/256.0, 217.0/256.0, 1.0));

	for (i = 0; i <= nTriangles; i++)
	{
		cerchio->vertici.push_back(vec3(cx + cos((double)i * stepA) * raggiox, cy + sin((double)i * stepA) * raggioy,0.0));

		if (i > (nTriangles / 4) && i < 12) {
			cerchio->colors.push_back(vec4(242.0/256.0, 123.0/256, 53.0/256, 1.0));
			cerchio->colors.push_back(vec4(242.0 / 256.0, 123.0 / 256, 53.0 / 256, 1.0));
			cerchio->colors.push_back(vec4(242.0 / 256.0, 123.0 / 256, 53.0 / 256, 1.0));
			cerchio->colors.push_back(vec4(242.0 / 256.0, 123.0 / 256, 53.0 / 256, 1.0));
		}

		cerchio->colors.push_back(vec4(64.0 / 256.0, 62.0 / 256.0, 63.0 / 256.0, 1.0));
	
	}

}

//Costruzione del triangolo principale
void costruzione_Triangolo(vec4 color1, vec4 color2, vec4 color3, Figura* triangolo) {

	/*Carico i vertici e i colori del triangolo*/
	triangolo->vertici.push_back(vec3(1.0, 0.0, 0.0));
	triangolo->colors.push_back(color2);
	triangolo->vertici.push_back(vec3(-1.0, -0.6, 0.0));
	triangolo->colors.push_back(color3);
	triangolo->vertici.push_back(vec3(-1.0, 0.6, 0.0));
	triangolo->colors.push_back(color1);

	//setto il numero di vertici
	triangolo->nv = triangolo->vertici.size();
}


void costruzione_BordoTriangolo(vec4 color, Figura* triangolo) {

	/*Carico i vertici e i colori del triangolo*/
	triangolo->vertici.push_back(vec3(1.0, 0.0, 0.0));
	triangolo->colors.push_back(color);
	triangolo->vertici.push_back(vec3(-1.0, -0.6, 0.0));
	triangolo->colors.push_back(color);
	triangolo->vertici.push_back(vec3(-1.0, 0.6, 0.0));
	triangolo->colors.push_back(color);

	//setto il numero di vertici
	triangolo->nv = triangolo->vertici.size();
}


void costruzione_Quadrato(vec4 color, Figura* quadrato) {

	//carico i veritici
	quadrato->vertici.push_back(vec3(0.2, -0.2, 0.0));
	quadrato->colors.push_back(color);
	quadrato->vertici.push_back(vec3(0.2, 0.2, 0.0));
	quadrato->colors.push_back(color);
	quadrato->vertici.push_back(vec3(-0.2, 0.2, 0.0));
	quadrato->colors.push_back(color);
	quadrato->vertici.push_back(vec3(-0.2, -0.2, 0.0));
	quadrato->colors.push_back(color);

	quadrato->nv = quadrato->vertici.size();


}

void costruzione_Bordi_Quadrato(vec4 color, Figura* quadrato) {

	//carico i veritici
	quadrato->vertici.push_back(vec3(0.2, -0.2, 0.0));
	quadrato->colors.push_back(color);
	quadrato->vertici.push_back(vec3(0.2, 0.2, 0.0));
	quadrato->colors.push_back(color);
	quadrato->vertici.push_back(vec3(-0.2, 0.2, 0.0));
	quadrato->colors.push_back(color);
	quadrato->vertici.push_back(vec3(-0.2, -0.2, 0.0));
	quadrato->colors.push_back(color);

	quadrato->nv = quadrato->vertici.size();
}


vector<vec2> coordinate_quadratini;
//la funzione si occupa di rimepire un vettore di coordinate per ogni quadratino
void riempi_vettore_coordinate() {
	
	//pulisco il vetor
	coordinate_quadratini.clear();

	float posy_gen = 0;
	float posx_gen = 0;
	

	srand(std::time(NULL));
	for (int i = 0; i < TOTALE_QUADRATINI; i++) {
		//genero la posizione random del quadratino

		do {
			posy_gen = rand();
		} while (posy_gen > height || posy_gen < 0);

		do {
			posx_gen = rand();
		} while (posx_gen<width || posx_gen>width + OUT_BOUND );
		coordinate_quadratini.push_back(vec2(posx_gen, posy_gen));
	}
}



void INIT_VAO()
{


	/*Costruisco il cerchio*/
	costruisci_cerchio(0.0, 0.0, 0.8, 0.8,&Cerchio);
	//crea_VAO_vector_Cerchio(&Cerchio);
	crea_VAO_vector(&Cerchio);
	Cerchio.name = "Cerchio";
	Cerchio.Model = mat4(1.0);
	Cerchio.Model = translate(Cerchio.Model, vec3(posx, posy, 0.0));
	Cerchio.Model = scale(Cerchio.Model, vec3(SCALE_CERCHIO, SCALE_CERCHIO, 1.0f));
	Cerchio.Model = rotate(Cerchio.Model, radians(0.0f), vec3(1.0, 1.0, 1.0));
	
	Navicella.push_back(Cerchio);


	/*Costruzione Triangolo Superiore*/
	vec4 color_superiore1 = vec4(217.0 / 256.0, 217.0 / 256.0, 217.0 / 256.0, 1.0); //bianco
	vec4 color_superiore2 = vec4(170.0 / 256.0, 176.0 / 256.0, 181.0 / 256.0, 1.0);
	vec4 color_superiore3 = vec4(64.0 / 256.0, 62.0 / 256.0, 63.0 / 256.0, 1.0); //grigio scuro
	costruzione_Triangolo(color_superiore3, color_superiore2, color_superiore1, &Triangolo);
	crea_VAO_vector(&Triangolo);
	Triangolo.name = "Triangolo Superiore";
	Triangolo.Model = mat4(1.0);
	Triangolo.Model = translate(Triangolo.Model, vec3(posx+MARGIN_X_TRIANGLE, posy + MARGIN_Y_TOP, 0.0));
	Triangolo.Model = scale(Triangolo.Model, vec3(SCALE_TRIANGOLO, SCALE_TRIANGOLO, 1.0f));
	Triangolo.Model = rotate(Triangolo.Model, radians(0.0f), vec3(1.0, 1.0, 1.0));

	Navicella.push_back(Triangolo);

	/*Costruzione Triangolo Inferiore*/
	vec4 color_inferiore = { 0.1, 0.9, 0.8, 1.0 };
	//costruzione_Triangolo(color_superiore3, color_superiore1, vec4(1.0,0.0,0.0,1.0), &Triangolo);
	crea_VAO_vector(&Triangolo);
	Triangolo.name = "Triangolo Inferiore";
	Triangolo.Model = mat4(1.0);
	Triangolo.Model = translate(Triangolo.Model, vec3(posx + MARGIN_X_TRIANGLE, posy + MARGIN_Y_BOTTOM, 0.0));
	Triangolo.Model = scale(Triangolo.Model, vec3(SCALE_TRIANGOLO, SCALE_TRIANGOLO, 1.0f));
	Triangolo.Model = rotate(Triangolo.Model, radians(0.0f), vec3(1.0, 1.0, 1.0));

	Navicella.push_back(Triangolo);

	/*Costruisco il Quadrato*/
	
	vec4 color_quadrato = vec4(86.0 / 256.0, 161.0 / 256.0, 191.0 / 256.0, 1.0);
	costruzione_Quadrato(color_quadrato, &Quadratino);
	crea_VAO_vector(&Quadratino);
	Quadratino.name = "Quadratino";

	Quadratino.Model = mat4(1.0);
	Quadratino.Model = translate(Quadratino.Model, vec3(width / 2, height / 2, 0.0));
	Quadratino.Model = scale(Quadratino.Model, vec3(30.0f, 30.0f, 1.0f));
	Quadratino.Model = rotate(Quadratino.Model, radians(float(0)), vec3(1.0, 1.0, 1.0));


	/*Costruisco i bordi del quadrato*/
	vec4 color_bordi = vec4(37.0 / 256.0, 184.0 / 256.0, 217.0 / 256.0, 1.0);;
	costruzione_Bordi_Quadrato(color_bordi,&BordiQuadrato);
	crea_VAO_vector(&BordiQuadrato);

	BordiQuadrato.Model = mat4(1.0);
	BordiQuadrato.Model = translate(BordiQuadrato.Model, vec3(width / 2, height / 2, 0.0));
	BordiQuadrato.Model = scale(BordiQuadrato.Model, vec3(35.0f, 35.0f, 1.0f));
	BordiQuadrato.Model = rotate(BordiQuadrato.Model, radians(float(0)), vec3(1.0, 1.0, 1.0));


	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");
	lsceltavs = glGetUniformLocation(programId, "sceltaVS");
	lsceltafs = glGetUniformLocation(programId, "sceltaFS");
	loc_time = glGetUniformLocation(programId, "time");
	loc_res = glGetUniformLocation(programId, "res");
	loc_mouse = glGetUniformLocation(programId, "mouse");

}

void aumenta_difficolta(int quantita, int bound) {
	int AUMENTO_QUADRATINI = 20;
	
	if (quantita % 2) {
		TOTALE_QUADRATINI += AUMENTO_QUADRATINI;
	}
	if (bound % 2==0){
		//diminuisco la dimensione in cui staranno i quadratini
		OUT_BOUND -= 15;
	}
}


bool touch_star() {

	float delta_quad = 10.0;

	for (int i = 0; i < coordinate_quadratini.size(); i++) {

		if (posx > int(coordinate_quadratini[i].x - posx_quadrato - delta_quad) && posx<int(coordinate_quadratini[i].x - posx_quadrato + delta_quad)) {
			if (posy > int(coordinate_quadratini[i].y - delta_quad) && posy<int(coordinate_quadratini[i].y + delta_quad)) {
				danno_triangolo += DANNO_STELLA;
				if (danno_triangolo >= DANNO_END_GAME) {
					printf("\n\n########################  GAME OVER!!!!!!  ########################\n\n");
					std::exit(EXIT_SUCCESS);
				}
			}

		}
		
	}

	return false;
}


void updateMovimento(int value) {
	posx_quadrato++;
	float posy_gen = value;
	if (posx_quadrato >= width + OUT_BOUND) {
		posx_quadrato = 0;
		
		//supero un livello quindi aumento la difficolta
		aumenta_difficolta(rand(), rand());
		//printf("Spazio BOUND = %d\nnum Quadratini = %d\n", OUT_BOUND, TOTALE_QUADRATINI);
		riempi_vettore_coordinate();
	}

	touch_star();
	angolo_bordo_triangolo += 10.0;
	angolo += 0.5;
	glutTimerFunc(1, updateMovimento, 0);
	glutPostRedisplay();

}

void genera_quadrati(int value=0) {
	//devo generare i quadratini
	float posy_gen = 0;
	float posx_gen = 0;


	for (int i = 0; i < TOTALE_QUADRATINI; i++) {
		vec2 coord = coordinate_quadratini[i];

		mat4 ModelQuad = mat4(1.0);
		ModelQuad = translate(ModelQuad, vec3(coord.x - posx_quadrato, coord.y, 0.0));
		ModelQuad = scale(ModelQuad, vec3(30.0f, 30.0f, 1.0f));
		ModelQuad = rotate(ModelQuad, radians(float(0)), vec3(1.0, 1.0, 1.0));


		glUniformMatrix4fv(Quadrat_MatModel, 1, GL_FALSE, value_ptr(ModelQuad));
		glBindVertexArray(Quadratino.VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, Quadratino.nv);
		glBindVertexArray(0);

		mat4 ModelBordo = mat4(1.0);
		ModelBordo = translate(ModelBordo, vec3(coord.x - posx_quadrato, coord.y, 0.0));
		ModelBordo = scale(ModelBordo, vec3(35.0f, 35.0f, 1.0f));
		ModelBordo = rotate(ModelBordo, radians(float(angolo)), vec3(0.0, 0.0, 1.0));

		glUniformMatrix4fv(Bordo_Model, 1, GL_FALSE, value_ptr(ModelBordo));
		glBindVertexArray(BordiQuadrato.VAO);
		glDrawArrays(GL_LINE_LOOP, 0, BordiQuadrato.nv);
		glBindVertexArray(0);
	}

}

float angolo_cerchio = 0.0;

void update_Matrix_Navicella() {

	float c = cos(angolo_triangolo);
	float s = sin(angolo_triangolo);
	float c1 = 1.0f - c;
	//float x = posx;
	//float y = posy;
	float z = 0.0;

	for (int i = 0; i < Navicella.size(); i++) {
		float scala = Navicella[i].name == "Cerchio" ? SCALE_CERCHIO : SCALE_TRIANGOLO;
		float x = Navicella[i].name == "Cerchio" ? posx : MARGIN_X_TRIANGLE + posx;
		float y = Navicella[i].name == "Cerchio" ? posy : Navicella[i].name == "Triangolo Superiore" ? 
				posy + MARGIN_Y_TOP : posy + MARGIN_Y_BOTTOM ;
		
		//posx_elem = posx * cos(angolo_triangolo) - posy * sin(angolo_triangolo);
		//posy_elem = posx * sin(angolo_triangolo) + posy * cos(angolo_triangolo);

		
		//matrice per ruotare i triangoli della figura insieme al cerchio
		/*
		mat4 rotazione = mat4(x * x * c1 + c, x * y * c1 + z * s, x * z * c1 - y * s, 0.0,
								x * y * c1 - z * s, y * y * c1 + c, y * z * c1 + x * s, 0.0,
								x * z * c1 + y * s, y * z * c1 - x * s, z * z * c1 + c, 0.0,
								0.0, 0.0, 1.0, 1.0);
		*/

		Navicella[i].Model = mat4(1.0);
		Navicella[i].Model = translate(Navicella[i].Model, vec3(x, y, 0.0));
		Navicella[i].Model = scale(Navicella[i].Model, vec3(scala, scala, 1.0f));
		Navicella[i].Model = rotate(Navicella[i].Model, radians(angolo_triangolo), vec3(1.0, 0.0, 0.0));
	}

}

float scale_tot = 0.0f;

void keyboard_pressed(unsigned char key, int x, int y) {
	
	float scale_amount = 2.0;
	
	switch (key) 
	{

	case 'b'://ingrandisco il triangolo
		if (SCALE_CERCHIO < MAX_SCALE_CERCHIO) {
			SCALE_TRIANGOLO += scale_amount;
			SCALE_CERCHIO += scale_amount*1.5;
			MARGIN_X_TRIANGLE += scale_amount;
			MARGIN_Y_TOP -= scale_amount * 0.7;
			MARGIN_Y_BOTTOM += scale_amount * 0.7;
		}
		break;

	case 's'://rimpicciolisco il triangolo
		
		if (SCALE_CERCHIO > MIN_SCALE_CERCHIO) {
			SCALE_TRIANGOLO -= scale_amount;
			SCALE_CERCHIO -= scale_amount*1.5;
			MARGIN_X_TRIANGLE -= scale_amount;
			MARGIN_Y_TOP += scale_amount*0.7;
			MARGIN_Y_BOTTOM -= scale_amount*0.7;
		}
		break;
	case 'r':
		angolo_triangolo = 0.0;
		break;

	case '0':
		std::exit(E_APPLICATION_EXITING);
		break;

	default:
		break;
	}
	update_Matrix_Navicella();

	glutPostRedisplay();
}

void draw_Navicella() {

	for (Figura comp : Navicella){

		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(comp.Model));
		glBindVertexArray(comp.VAO);

		if (comp.name == "Cerchio") {	
			glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		glBindVertexArray(0);
	}

}


void drawScene(void)
{

	glViewport(0, 0, width, height);
	int i;
	res.x = (float)width;
	res.y = (float)height;


	float time = glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(loc_time, time);
	
	
	//glUniform1i(lsceltafs, 2);
	glClearColor(11.0 / 256.0, 18.0 / 256.0, 38.0 / 256.0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(programId);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	glUniform2f(loc_res, res.x, res.y);
	glUniform2f(loc_mouse, mouse.x, mouse.y);
	
	//Funzione per il disegno della Navicella
	draw_Navicella();

	genera_quadrati();
	
	glutSwapBuffers();
	
}


void genera_nuovi_qudratini(int value) {
	glutTimerFunc(2.0, updateMovimento, value);
}


//con la rotellina vario l'angolo del triangolo
void rotellaMouse(int wheel, int direction, int x, int y) {

	int fattore_angolazione = 5.0;
	
	angolo_triangolo += direction * fattore_angolazione;
	//angolo_cerchio += (direction * fattore_angolazione);

	update_Matrix_Navicella();

	glutPostRedisplay();

}

void mouseMotion(int x, int y) {

	posx = x;
	posy = height - y;
	//setto y in modo che non esca dai bordi dello schermo
	if (y > height || y < 0) {
		y = y > 0 ? height : 0;
	}

	//setto x in modo che stia nel range di gioco
	if (x > BORDO_X_DESTRO_GIOCO || x < BORDO_X_SINISTRO_GIOCO) {
		x = x > BORDO_X_DESTRO_GIOCO ? BORDO_X_DESTRO_GIOCO : BORDO_X_SINISTRO_GIOCO;
	}
	
	update_Matrix_Navicella();

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	
	riempi_vettore_coordinate();

	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Through the stars");
	glutDisplayFunc(drawScene);


	glutPassiveMotionFunc(mouseMotion);
	glutTimerFunc(0.1, updateMovimento,0);
	glutTimerFunc(20, genera_nuovi_qudratini, 0);
	glutMouseWheelFunc(rotellaMouse);
	glutKeyboardFunc(keyboard_pressed);

	glewExperimental = GL_TRUE;
	glewInit();
	INIT_SHADER();
	INIT_VAO();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glutMainLoop();


}


