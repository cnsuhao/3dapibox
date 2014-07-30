#include <sb6.h>
#include <math.h>

class simpleclear_app : public sb6::application
{
	void init()
	{
		static const char title[] = "OpenGL SuperBible - Color Between Stage";
		sb6::application::init();
		memcpy(info.title, title, sizeof(title));
	}

	virtual void startup()
	{
		static const char* vs_source[] = {
			"#version 410 core                                               \n"
			"                                                                \n"
			"layout (location = 0) in vec4 offset;                           \n"
			"layout (location = 1) in vec4 color;                            \n"
			"                                                                \n"
			"out vec4 vs_color;                                              \n"
			"                                                                \n"
			"void main(void)                                                 \n"
			"{                                                               \n"
			"  const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
			"                                 vec4(-0.25, -0.25, 0.5, 1.0),  \n"
			"                                 vec4( 0.25,  0.25, 0.5, 1.0)); \n"
			"                                                                \n"
			"  gl_Position = vertices[gl_VertexID] + offset;                 \n"
			"  vs_color = color;                                             \n"
			"}                                                               \n"
		};

		static const char* fs_source[] = {
			"#version 410 core                                               \n"
			"                                                                \n"
			"in vec4 vs_color;                                               \n"
			"                                                                \n"
			"out vec4 color;                                                 \n"
			"                                                                \n"
			"void main(void)                                                 \n"
			"{                                                               \n"
			"  color = vs_color;                                             \n"
			"}                                                               \n"
		};

		program = glCreateProgram();
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, fs_source, NULL);
		glCompileShader(fs);

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, vs_source, NULL);
		glCompileShader(vs);

		glAttachShader(program, vs);
		glAttachShader(program, fs);

		glLinkProgram(program);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	virtual void render(double currentTime)
	{
		static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);

		glUseProgram(program);

		GLfloat attrib[] = { (float)sin(currentTime) * 0.5f, (float)cos(currentTime) * 0.5f, 0.0f, 0.0f };
		glVertexAttrib4fv(0, attrib);

		GLfloat color[] = { (float)sin(currentTime)/2.0f + 0.5f, 0.0f, 0.0f, 0.0f };
		glVertexAttrib4fv(1, color);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}

private:
	GLuint program;
	GLuint vao;
};

DECLARE_MAIN(simpleclear_app)
