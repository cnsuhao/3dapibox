#include <GL/freeglut.h>

static void renderSceneCallback()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("01_Window");

	glutDisplayFunc(renderSceneCallback);

	glClearColor(1.f, 0.f, 0.f, 0.f);
	glutMainLoop();	

	return 0;
}

