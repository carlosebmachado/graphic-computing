#include <iostream>
#include <GL/freeglut.h>

void display();
void idle();
void keyboard(unsigned char key, int x, int y);

//#define TOP 0b1001
//#define BOTTOM 0b0011
//#define LEFT 0b0110
//#define RIGHT 0b1100

#define TITLE "OpenGL"
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 250
//#define YX_POSITION TOP | LEFT

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	glutCreateWindow(TITLE);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//if (YX_POSITION == BOTTOM | LEFT) {
		//glOrtho(0, SCREEN_HEIGHT, 0, SCREEN_WIDTH, -1, 1);
	//} else {
		glOrtho(0, SCREEN_HEIGHT, SCREEN_WIDTH, 0, -1, 1);
	//}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}

void drawLine(int x1, int y1, int x2, int y2) {
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}

void drawRect(int x, int y, int width, int height) {
	// top
	drawLine(x, y, x + width, y);
	// bottom
	drawLine(x, y + height, x + width, y + height);
	// left
	drawLine(x, y, x, y + height);
	// height
	drawLine(x + width, y, x + width, y + height);
}

void drawCircle(int x, int y, int r, int s = 3) {
	for (int i = 0; i < 5; ++i) {

	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	drawLine(0, 0, 250, 500);
	drawRect(25, 25, 200, 450);

	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}

void idle() {

}