#include <sb6.h>
#include <stdlib.h>

class simpleclear_app : public sb6::application
{
	void check_shader_compilation(GLuint shader, const char* tag)
	{
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar log[1024];
			glGetShaderInfoLog(shader, sizeof(log), NULL, log);
			fprintf(stderr, "[Error] glCompilerShader(), type %s: '%s'\n", tag, log);
			exit(1);
		}
	}

	void init()
	{
		static const char title[] = "OpenGL SuperBible - Tessellated Triangle";
		sb6::application::init();
		memcpy(info.title, title, sizeof(title));
	}

	virtual void startup()
	{
		static const char* vs_source[] = {
			"#version 410 core                                               \n"
			"                                                                \n"
			"void main(void)                                                 \n"
			"{                                                               \n"
			"  const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
			"                                 vec4(-0.25, -0.25, 0.5, 1.0),  \n"
			"                                 vec4( 0.25,  0.25, 0.5, 1.0)); \n"
			"                                                                \n"
			"  gl_Position = vertices[gl_VertexID];                          \n"
			"}                                                               \n"
		};

		static const char* tcs_source[] = {
			"#version 410 core                                                            \n"
			"                                                                             \n"
			"layout (vertices = 3) out;                                                   \n"
			"                                                                             \n"
			"void main(void)                                                              \n"
			"{                                                                            \n"
			"  if (gl_InvocationID == 0)                                                  \n"
			"  {                                                                          \n"
			"    gl_TessLevelInner[0] = 5.0;                                              \n"
			"    gl_TessLevelOuter[0] = 5.0;                                              \n"
			"    gl_TessLevelOuter[1] = 5.0;                                              \n"
			"    gl_TessLevelOuter[2] = 5.0;                                              \n"
			"  }                                                                          \n"
			"  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;  \n"
			"}                                                                            \n"
		};

		static const char* tes_source[] = {
			"#version 410 core                                                            \n"
			"                                                                             \n"
			"layout (triangles, equal_spacing, cw) in;                                    \n"
			"                                                                             \n"
			"void main(void)                                                              \n"
			"{                                                                            \n"
			"  gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                    \n"
			"                (gl_TessCoord.y * gl_in[1].gl_Position) +                    \n"
			"                (gl_TessCoord.z * gl_in[2].gl_Position);                     \n"
			"}                                                                            \n"
		};

		static const char* fs_source[] = {
			"#version 410 core                                               \n"
			"                                                                \n"
			"out vec4 color;                                                 \n"
			"                                                                \n"
			"void main(void)                                                 \n"
			"{                                                               \n"
			"  color = vec4(0.0, 0.8, 1.0, 1.0);                             \n"
			"}                                                               \n"
		};

		program = glCreateProgram();

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, vs_source, NULL);
		glCompileShader(vs);
		check_shader_compilation(vs, "vs");

		GLuint tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tcs, 1, tcs_source, NULL);
		glCompileShader(tcs);
		check_shader_compilation(tcs, "tcs");

		GLuint tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tes, 1, tes_source, NULL);
		glCompileShader(tes);
		check_shader_compilation(tes, "tes");

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, fs_source, NULL);
		glCompileShader(fs);
		check_shader_compilation(fs, "fs");

		glAttachShader(program, vs);
		glAttachShader(program, tcs);
		glAttachShader(program, tes);
		glAttachShader(program, fs);

		glLinkProgram(program);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void render(double currentTime)
	{
		static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);

		glUseProgram(program);
		glDrawArrays(GL_PATCHES, 0, 3);
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

