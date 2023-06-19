#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "./include/Vector3f.h"

GLuint VBO;

static void RenderSceneCB(){
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
}

static void CreateVertesBuffer(){
	Vector3f vertices[3];
	vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);	// bottom left
	vertices[1] = Vector3f(0.0f, 1.0f, 0.0f); 	// top 
	vertices[2] = Vector3f(1.0f, -1.0f, 0.0f);  // right bottom
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	int width = 700;
	int height = 400;
	glutInitWindowSize(width,height);
	
	int x = 200;
	int y = 100;
	glutInitWindowPosition(x,y);
	int win = glutCreateWindow("Tutorial 3 simple Triangle");
	
	GLenum res = glewInit();
	
	if(res != GLEW_OK){
		fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
		return 1;
	}
	
	GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
	glClearColor(Red, Green, Blue, Alpha);
	
	CreateVertesBuffer();
	glutDisplayFunc(RenderSceneCB);
	glutMainLoop();
	return 0;
}
