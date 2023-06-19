#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstring>
#include "./include/Matrix4f.h"
#include "./include/util.h"

GLuint VBO;
GLint gTranslationLocation;

static void RenderSceneCB(){
	glClear(GL_COLOR_BUFFER_BIT);
	
	static float Scale = 0.0f;   // in this cate it is translation speed
	static float Delta = 0.005f;
	
	Scale += Delta;
	
	if((Scale >= 1.0f) || (Scale <= -1.0f)){
		Delta *= -1.0f;
	}
	
	Matrix4f Translation(1.0f, 0.0f, 0.0f, Scale * 2,
						 0.0f, 1.0f, 0.0f, Scale,
						 0.0f, 0.0f, 1.0f, 0.0f,
						 0.0f, 0.0f, 0.0f, 1.0f);
						 
	glUniformMatrix4fv(gTranslationLocation, 1, GL_TRUE, &Translation.m[0][0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	
	glutPostRedisplay();
	
	glutSwapBuffers();
}

static void CreateVertesBuffer(){
	Vector3f vertices[3];
	vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
	vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
	vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	
}

static void AddShader(GLuint ShaderProgram, const char *pShaderText, GLenum ShaderType){
	GLuint ShaderObj = glCreateShader(ShaderType);
	
	if(ShaderObj == 0){
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(1);
	}
	
	const GLchar *p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	
	if(!success){
		GLchar infoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: 's'\n", ShaderType, infoLog);
		exit(1);		
	}
	
	glAttachShader(ShaderProgram, ShaderObj);
}

const char *pVSFileName = "shader_m.vs";
const char *pFSFileName = "shader_m.fs";

static void CompileShaders(){
	GLuint ShaderProgram = glCreateProgram();
	
	if(ShaderProgram == 0){
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}
	
	string vs, fs;
	
	if(!ReadFile(pVSFileName, vs)){
		exit(1);
	}
	
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	
	if(!ReadFile(pFSFileName, fs)){
		exit(1);
	}
	
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);
	
	GLint Success = 0;
	GLchar errorLog[1024] = { 0 };
	
	glLinkProgram(ShaderProgram);
	
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	
	if(Success == 0){
		glGetProgramInfoLog(ShaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}
	
	gTranslationLocation = glGetUniformLocation(ShaderProgram, "gTranslation");
	
	if(gTranslationLocation == -1){
		printf("Error getting uniform location of 'gTranslation'\n");
		exit(1);
	}
	
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	
	if(!Success){
		glGetProgramInfoLog(ShaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}
	
	glUseProgram(ShaderProgram);
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	
	int width = 700;
	int height = 500;
	glutInitWindowSize(width, height);
	
	int x = 200;
	int y = 100;
	glutInitWindowPosition(x, y);
	int win = glutCreateWindow("Shaders Example");
	printf("windows id: %d\n", win);
	
	GLenum res = glewInit();
	
	if(res != GLEW_OK){
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	
	GLclampf Red = 0.0f, Blue = 0.0f, Green = 0.0f, Alpha = 0.0f;
	glClearColor(Red, Blue, Green, Alpha);
	
	CreateVertesBuffer();
	CompileShaders();
	
	glutDisplayFunc(RenderSceneCB);
	glutMainLoop();
}

























