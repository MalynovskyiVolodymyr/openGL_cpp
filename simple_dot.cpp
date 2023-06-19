
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "./include/Vector3f.h"

GLuint VBO;

static void RenderSceneCB(){
	glClear(GL_COLOR_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_POINTS, 0, 1);
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
	
}

static void CreateVertexBuffer(){
	Vector3f Vertices[1];
	Vertices[0] = Vector3f(0,0,0);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	int width = 700;
	int height= 400;
	glutInitWindowSize(width, height);
	
	int x = 200;
	int y = 100;
	glutInitWindowPosition(x, y);
	int win = glutCreateWindow("Tutorial 2 simple Dot");
	printf("window id: %d\n", win);
	
	GLenum res = glewInit();
	
	if(res != GLEW_OK){
		fprintf(stderr, "Error: %s\n:", glewGetErrorString(res));
		return 1;
	}
	
	GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
	glClearColor(Red, Green, Blue, Alpha);
	
	CreateVertexBuffer();
	
	glutDisplayFunc(RenderSceneCB);
	
	glutMainLoop();
	return 0;
}
