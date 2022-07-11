#include <iostream>
#include <GL/freeglut.h>
#include <vector>


/*
COMANDOS
ESC -> SAIR
Setas movimenta o cubo
Page UP aumenta o cubo
Page Down encolhe o cubo
1 e 3 -> Rotacionar eixo Z
4 -> Rotacionar para esquerda
6 -> Rotacionar para direita
7 -> Rotacionar para baixo
9 -> Rotacionar para cima
*/

struct vertice {
	double x, y, z;
};

vertice criar_vertice(double x, double y, double z) {
	vertice v;
	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}

using lista_vertices = std::vector<vertice>;
using aresta = std::pair<int, int>;
using lista_arestas = std::vector<aresta>;

struct Quadrado {
	vertice posicao;
	vertice escala;
	vertice rotacao;
	double matriz_rotacao_x[3][3];
	double matriz_rotacao_y[3][3];
	double matriz_rotacao_z[3][3];
	lista_vertices vertices;
	lista_arestas arestas;
};


Quadrado criar_quadrado(double posicao_x, double posicao_y, double posicao_z);
void desenhar(Quadrado poligono);
void movimentar(Quadrado& poligono, double distancia, double angulo);
void escalar(Quadrado& poligono, double escala_x, double escala_y, double escala_z);
void rotacionar(Quadrado& poligono, double angulo_x, double angulo_y, double angulo_z);
void movimentar_z(Quadrado& poligono, double distancia, double angulo);

void display();
void idle();
void redraw(int value);
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);
void initGL();
void reshape(GLsizei width, GLsizei height);

Quadrado quadrado;
int delay = 10;
double taxaMovimentacao = 0.05;
double taxaEscala = 0.05;
double taxaRotacao = 0.05;
double taxaRotacao_x = taxaRotacao;
double taxaRotacao_y = taxaRotacao;
double taxaRotacao_z = taxaRotacao;

int main(int argc, char** argv) {

	quadrado = criar_quadrado(0, 0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE); // Double buffer

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Trabalho 3D");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	glutTimerFunc(10, redraw, 0);

	//movimentar_z(quadrado, -100, (0 / 180.0) * 3.1416);

	initGL();

	glutMainLoop();

	return 0;
}

// Função de inicialização do OpenGL
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Coloca a cor de background para preto e opaco
	glClearDepth(1.0f);                   // Define o buffer de profundidade para o mais distante possível
	glEnable(GL_DEPTH_TEST);			  // Habilita o culling de profundidade
	glDepthFunc(GL_LEQUAL);               // Define o tipo de teste de profundidade
}

//Função de redesenhou prioriza o aspecto da projeção
void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

// Função de display, onde ira rodar na thread do OpenGL
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor e o de profundidade
	glMatrixMode(GL_MODELVIEW);						   //Operar na matriz de ModelView

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -5.5f);  // Move para a direta da view o que será desenhado

	desenhar(quadrado);

	glutSwapBuffers();
}

// Função de teclas comuns do keyboard
void keyboard(unsigned char key, int x, int y) {
	std::cout << "Key:" << key << std::endl;
	switch (key) {
	case 27:
		exit(0);
		break;
	case 55:
		rotacionar(quadrado, taxaRotacao, 0, 0); // Rotacionar para baixo
		break;
	case 57:
		rotacionar(quadrado, -taxaRotacao, 0, 0); // Rotacionar para cima
		break;
	case 52:
		rotacionar(quadrado, 0, -taxaRotacao, 0); // Rotacionar para esquerda
		break;
	case 54:
		rotacionar(quadrado, 0, taxaRotacao, 0); // Rotacionar para direita
		break;
	case 49:
		rotacionar(quadrado, 0, 0, taxaRotacao); // Rotacionar eixo z
		break;
	case 51:
		rotacionar(quadrado, 0, 0, -taxaRotacao); // Rotacionar eixo z
		break;
	}
}

// Função de teclas especiais do keyboard
void keyboard_special(int key, int x, int y) {
	std::cout << "Special Key:" << key << std::endl;
	switch (key) {
	case GLUT_KEY_DOWN:
		movimentar(quadrado, taxaMovimentacao, (270 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_UP:
		movimentar(quadrado, taxaMovimentacao, (90 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_RIGHT:
		movimentar(quadrado, taxaMovimentacao, (0 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_LEFT:
		movimentar(quadrado, taxaMovimentacao, (180 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_PAGE_UP:
		escalar(quadrado, 1 + taxaEscala, 1 + taxaEscala, 1 + taxaEscala);
		break;

	case GLUT_KEY_PAGE_DOWN:
		escalar(quadrado, 1 - taxaEscala, 1 - taxaEscala, 1 - taxaEscala);
		break;
	}
}

// Função de redraw para atualizar o display
void redraw(int value) {
	glutPostRedisplay();
	glutTimerFunc(delay, redraw, 0);
}

void idle() {

}

Quadrado criar_quadrado(double posicao_x, double posicao_y, double posicao_z) {
	Quadrado novo_quadrado;
	novo_quadrado.posicao = criar_vertice(posicao_x, posicao_y, posicao_z);
	novo_quadrado.escala = criar_vertice(1, 1, 1);

	novo_quadrado.rotacao.x = 0;
	novo_quadrado.rotacao.y = 0;
	novo_quadrado.rotacao.z = 0;

	// Face 01
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, 1.0f, -1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, 1.0f, -1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, 1.0f, 1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, 1.0f, 1.0f));

	// Face 02
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, -1.0f, 1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, -1.0f, 1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, -1.0f, -1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, -1.0f, -1.0f));

	// Face 03
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, 1.0f, 1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, 1.0f, 1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, -1.0f, 1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, -1.0f, 1.0f));

	// Face 04
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, -1.0f, -1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, -1.0f, -1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, 1.0f, -1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, 1.0f, -1.0f));

	// Face 05
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, 1.0f, 1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, 1.0f, -1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, -1.0f, -1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(-1.0f, -1.0f, 1.0f));

	// Face 06
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, 1.0f, -1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, 1.0f, 1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, -1.0f, 1.0f));
	novo_quadrado.vertices.push_back(criar_vertice(1.0f, -1.0f, -1.0f));

	return novo_quadrado;
}

// Função para movimentar eixo x e y
void movimentar(Quadrado& poligono, double distancia, double angulo) {
	poligono.posicao.x += distancia * cos(angulo);
	poligono.posicao.y += distancia * sin(angulo);

	for (int i = 0; i < 24; i++) {
		poligono.vertices[i].x += distancia * cos(angulo);
		poligono.vertices[i].y += distancia * sin(angulo);
	}
}

// Função para movimentar eixo z
void movimentar_z(Quadrado& poligono, double distancia, double angulo) {
	poligono.posicao.z += distancia * tan(angulo);

	for (int i = 0; i < 24; i++) {
		poligono.vertices[i].z += distancia * tan(angulo);
	}
}

// Função escalar para diminuir e aumentar o quadrado
void escalar(Quadrado& poligono, double escala_x, double escala_y, double escala_z) {
	poligono.escala.x = escala_x;
	poligono.escala.y = escala_y;

	for (int i = 0; i < poligono.vertices.size(); i++) {
		poligono.vertices[i].x *= escala_x;
		poligono.vertices[i].y *= escala_y;
	}
}

// Função de rotação
void rotacionar(Quadrado& poligono, double angulo_x, double angulo_y, double angulo_z) {
	poligono.rotacao.x += angulo_x;
	poligono.rotacao.y += angulo_y;
	poligono.rotacao.z += angulo_z;

	// Matriz de Rotação X
	poligono.matriz_rotacao_x[0][0] = 1;
	poligono.matriz_rotacao_x[0][1] = 0;
	poligono.matriz_rotacao_x[0][2] = 0;

	poligono.matriz_rotacao_x[1][0] = 0;
	poligono.matriz_rotacao_x[1][1] = cos(angulo_x);
	poligono.matriz_rotacao_x[1][2] = -sin(angulo_x);

	poligono.matriz_rotacao_x[2][0] = 0;
	poligono.matriz_rotacao_x[2][1] = sin(angulo_x);
	poligono.matriz_rotacao_x[2][2] = cos(angulo_x);

	// Matriz de Rotação Y
	poligono.matriz_rotacao_y[0][0] = cos(angulo_y);
	poligono.matriz_rotacao_y[0][1] = 0;
	poligono.matriz_rotacao_y[0][2] = sin(angulo_y);

	poligono.matriz_rotacao_y[1][0] = 0;
	poligono.matriz_rotacao_y[1][1] = 1;
	poligono.matriz_rotacao_y[1][2] = 0;

	poligono.matriz_rotacao_y[2][0] = -sin(angulo_y);
	poligono.matriz_rotacao_y[2][1] = 0;
	poligono.matriz_rotacao_y[2][2] = cos(angulo_y);

	// Matriz de Rotação Z
	poligono.matriz_rotacao_z[0][0] = cos(angulo_z);
	poligono.matriz_rotacao_z[0][1] = -sin(angulo_z);
	poligono.matriz_rotacao_z[0][2] = 0;

	poligono.matriz_rotacao_z[1][0] = sin(angulo_z);
	poligono.matriz_rotacao_z[1][1] = cos(angulo_z);
	poligono.matriz_rotacao_z[1][2] = 0;

	poligono.matriz_rotacao_z[2][0] = 0;
	poligono.matriz_rotacao_z[2][1] = 0;
	poligono.matriz_rotacao_z[2][2] = 1;

	// Criando o Pivo tridimensional
	vertice pivo;

	pivo.x = poligono.posicao.x;
	pivo.y = poligono.posicao.y;
	pivo.z = poligono.posicao.z;

	for (int i = 0; i < poligono.vertices.size(); i++) {
		poligono.vertices[i].x -= pivo.x;
		poligono.vertices[i].y -= pivo.y;
		poligono.vertices[i].z -= pivo.z;

		/*
		double matriz_x[3][3];
		double matriz_y[3][3];
		double matriz_z[3][3];
		// Matriz de Rotação X
		matriz_x[0][0] = 1;
		matriz_x[0][1] = 0;
		matriz_x[0][2] = 0;
		matriz_x[1][0] = 0;
		matriz_x[1][1] = cos(angulo);
		matriz_x[1][2] = -sin(angulo);
		matriz_x[2][0] = 0;
		matriz_x[2][1] = sin(angulo);
		matriz_x[2][2] = cos(angulo);
		// Matriz de Rotação Y
		matriz_y[0][0] = cos(angulo);
		matriz_y[0][1] = 0;
		matriz_y[0][2] = sin(angulo);
		matriz_y[1][0] = 0;
		matriz_y[1][1] = 1;
		matriz_y[1][2] = 0;
		matriz_y[2][0] = -sin(angulo);
		matriz_y[2][1] = 0;
		matriz_y[2][2] = cos(angulo);
		// Matriz de Rotação Z
		matriz_z[0][0] = cos(angulo);
		matriz_z[0][1] = -sin(angulo);
		matriz_z[0][2] = 0;
		matriz_z[1][0] = sin(angulo);
		matriz_z[1][1] = cos(angulo);
		matriz_z[1][2] = 0;
		matriz_z[2][0] = 0;
		matriz_z[2][1] = 0;
		matriz_z[2][2] = 1;
		*/

		// Derivação da multiplicação das 3 matrizes, multiplicando a matriz resultante pelo vetor de eixos x, y e z
		double old_x = poligono.vertices[i].x;
		double old_y = poligono.vertices[i].y;

		poligono.vertices[i].x = (cos(angulo_z) * cos(angulo_y)) * poligono.vertices[i].x + ((cos(angulo_z) * sin(angulo_y) * sin(angulo_x)) - (sin(angulo_z) * cos(angulo_x))) * poligono.vertices[i].y + ((cos(angulo_z) * sin(angulo_y) * cos(angulo_x)) + (sin(angulo_z) * sin(angulo_x))) * poligono.vertices[i].z;
		poligono.vertices[i].y = (sin(angulo_z) * cos(angulo_y)) * old_x + ((sin(angulo_z) * sin(angulo_y) * sin(angulo_x)) + (cos(angulo_z) * cos(angulo_x))) * old_y + ((sin(angulo_z) * sin(angulo_y) * cos(angulo_x)) - (cos(angulo_z) * sin(angulo_x))) * poligono.vertices[i].z;
		poligono.vertices[i].z = (-sin(angulo_y)) * old_x + (cos(angulo_y) * sin(angulo_x)) * old_y + (cos(angulo_y) * cos(angulo_x)) * poligono.vertices[i].z;

		poligono.vertices[i].x += pivo.x;
		poligono.vertices[i].y += pivo.y;
		poligono.vertices[i].z += pivo.z;
	}

}


// Função principal para desenhar o poligono
void desenhar(Quadrado poligono) {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);

	for (int i = 0; i < poligono.vertices.size(); i++) {
		// Escolhendo cor para cada face
		if (i % 4 == 0)
			glColor3f(0.0f, 1.0f, 0.0f); // Verde
		if (i % 8 == 0)
			glColor3f(1.0f, 0.5f, 0.0f); // Laranja
		if (i % 12 == 0)
			glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
		if (i % 16 == 0)
			glColor3f(1.0f, 1.0f, 0.0f); // Amarelo
		if (i % 20 == 0)
			glColor3f(0.0f, 0.0f, 1.0f); // Azul
		if (i % 24 == 0)
			glColor3f(1.0f, 0.0f, 1.0f); // Magenta

		glVertex3f(poligono.vertices[i].x, poligono.vertices[i].y, poligono.vertices[i].z);
	}
	glEnd();
}