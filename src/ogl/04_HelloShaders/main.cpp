#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_3d.h"
#include "util.h"

GLuint VBO;

const char* pVSFileName = "../../../src/ogl/04_HelloShaders/vs.glsl";
const char* pFSFileName = "../../../src/ogl/04_HelloShaders/fs.glsl";

static void RenderSceneCallback()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCallback);
}

static void CreateVertexBuffer()
{
	Vector3f vertices[3];
	vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
	vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
	vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0)
	{
		fprintf(stderr, "Error glCreateShader(), type = %d\n", ShaderType);
		exit(0);
	}

	const GLchar* sources[1];
	sources[0] = pShaderText;

	GLint sourceLengths[1];
	sourceLengths[0] = strlen(pShaderText);

	glShaderSource(ShaderObj, 1, sources, sourceLengths);
	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar log[1024];
		glGetShaderInfoLog(ShaderObj, sizeof(log), NULL, log);
		fprintf(stderr, "Error glCompilerShader(), type %d: '%s'\n", ShaderType, log);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();
	if (ShaderProgram == 0)
	{
		fprintf(stderr, "glCreateProgram() error\n");
		exit(1);
	}

	std::string vs, fs;
	if (!Util_ReadFile(pVSFileName, vs))
	{
		fprintf(stderr, "ReadFile(VS) fail, file = %s\n", pVSFileName);
		exit(1);
	}

	if (!Util_ReadFile(pFSFileName, fs))
	{
		fprintf(stderr, "ReadFile(FS) fail, file = %s\n", pFSFileName);
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);


	GLint success = 0;
	GLchar log[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(log), NULL, log);
		fprintf(stderr, "glLinkProgram fail: '%s'\n", log);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &success);
	if (success == 0)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(log), NULL, log);
		fprintf(stderr, "glValidateProgram fail: '%s'\n", log);
		exit(1);
	}

	glUseProgram(ShaderProgram);
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("04 - Hello Shaders (OpenGL)");

	InitializeGlutCallbacks();

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));
	glClearColor(0.f, 0.f, 0.f, 0.f);

	CreateVertexBuffer();
	CompileShaders();

	glutMainLoop();
	return 0;
}

