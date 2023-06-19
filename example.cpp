#include <GL/freeglut.h>
#include <stdio.h>
#include "include/Vector3f.h"

static void RenderSceneCB(){
	static GLclampf c = 0.0f;
	glClearColor(c, c, c, c);
	printf("%f\n", c);
	c += 1.0f/256.0f;
	
	if(c >= 1.0f){
		c = 0.0f;
	}
	
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
	glutSwapBuffers();
}
//g++ example.cpp `pkg-config --libs glew` -lglut -o example
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	
	int width = 500;
	int height = 300;
	glutInitWindowSize(width, height);
	
	int x = 200;
	int y = 100;
	glutInitWindowPosition(x, y);
	int win = glutCreateWindow("Example 1 Tutorial");
	printf("window id: %d\n", win);
	
	GLclampf Red = 1.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
	//glClearColor(Red, Green, Blue, Alpha);
	
	glutDisplayFunc(RenderSceneCB);
	glutMainLoop();
	return 0;
	
}
