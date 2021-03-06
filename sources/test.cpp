#include <iostream>
#include <GL/glut.h>

// prototipos das funcoes
void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

// funcao principal
int main() {                             // inicializa o glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);           // especifica o uso de cores e buffers
	glutInitWindowSize(256, 256);                          // especifica as dimensoes da janela
	glutInitWindowPosition(100, 100);                      // especifica aonde a janela aparece na tela
	glutCreateWindow("Desenhando uma linha");              // cria a janela
	init();
	glutDisplayFunc(display);                               // funcao que sera redesenhada pelo GLUT
	glutKeyboardFunc(keyboard);                             // funcoes de teclado
	glutMainLoop();                                         // mostra todas as janelas criadas
	return 0;
}

// definicao de cada funcao

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);    // cor de fundo
	glOrtho(0, 256, 0, 256, -1, 1);     // modo de projecao ortogonal
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);               // limpa a janela
	glColor3f(0.0, 0.0, 0.0);                  // cor da linha
	glBegin(GL_LINES);
	glVertex2i(10, 10);  
	glVertex2i(245, 245);    // coordenadas inicial e final da linha
	glEnd();
	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:                                         // tecla Esc (encerra o programa)
		exit(0);
		break;
	}
}