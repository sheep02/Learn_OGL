﻿/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| "Hello Triangle". Just the basics.                                           |
| If you're on Mac un-comment the version number code at the beginning. It     |
| will give you the latest, even if you say 3.2!                               |
| This uses the libraries GLEW and GLFW3 to start GL. Download and compile     |
| these first. Linking them might be a pain, but you'll need to master this.   |
|                                                                              |
| I wrote this so that it compiles in pedantic ISO C90, to show that it's      |
| easily done. I usually use minimalist C++ though, for tidier-looking maths   |
| functions.                                                                   |
\******************************************************************************/
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GL/glfw3.h> /* GLFW helper library */
#include <stdio.h>
#pragma comment(lib, "GLFW3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

int main() {
	GLFWwindow* window = NULL;
	const GLubyte* renderer;
	const GLubyte* version;
	GLuint vao;
	GLuint vbo;
	/* geometry to use. these are 3 xyz points (9 floats total) to make a
	triangle */
	GLfloat points[] = {
		0.0f,	0.5f,	0.0f,
		0.5f, -0.5f,	0.0f,
		-0.5f, -0.5f,	0.0f
	};
	/* these are the strings of code for the shaders
	the vertex shader positions each vertex point */
	const char* vertex_shader =
		"#version 430\n"
		"layout (location = 3)"
		"in vec3 vp;"
		"void main () {"
		"	gl_Position = vec4 (vp, 1.0);"
		"}";
	/* the fragment shader colours each fragment (pixel-sized area of the
	triangle) */
	const char* fragment_shader =
		"#version 430\n"
		"out vec4 frag_colour;"
		"void main () {"
		"	frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
		"}";
	/* GL shader objects for vertex and fragment shader [components] */
	GLuint vs, fs;
	/* GL shader programme object [combined, to link] */
	GLuint shader_programme;

	/* start GL context and O/S window using the GLFW helper library */
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	/* We must specify 3.2 core if on Apple OS X -- other O/S can specify
	anything here. I defined 'APPLE' in the makefile for OS X */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	/* start GLEW extension handler */
	glewExperimental = GL_TRUE;
	glewInit();

	/* get version info */
	renderer = glGetString(GL_RENDERER); /* get renderer string */
	version = glGetString(GL_VERSION); /* version as a string */
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/* tell GL to only draw onto a pixel if the shape is closer to the viewer
	*/
	glEnable(GL_DEPTH_TEST); /* enable depth-testing */
							 /* with LESS depth-testing interprets a smaller value as "closer" */
	glDepthFunc(GL_LESS);/* http://kaworu.jpn.org/doc/FreeBSD/xjman/man3/glDepthFunc.3x.php */
						 /* a vertex buffer object (VBO) is created here. this stores an array of
						 data on the graphics adapter's memory. in our case - the vertex points */

	glGenBuffers(1, &vbo
		/*バッファ作成順に生成されるインデックスに対するエイリアスとして変数名自身([i])が使用される
		vbo[3]の場合第一引数を3としなければインデックスは全ての要素に与えられない
		この時点では実体のない vbo([i]) という名前を持つバッファ(仮)*/);
	glBindBuffer(GL_ARRAY_BUFFER, vbo
		/*新たにバインドされない限りこの行以下の処理はvboに対するもの
		　GL_ARRAY_BUFFER=頂点バッファのターゲット
		 GL_ELEMENT_ARRAY_BUFFER=インデックスバッファ*/);/*未確認*/
	glBufferData(GL_ARRAY_BUFFER/*デ－タタイプ*/, 9 * sizeof(GLfloat)/*サイズ*/, points/*アドレス*/, GL_STATIC_DRAW/*描画タイプ*/
	/*ここで実体をもつ*/);

	/* the vertex array object (VAO) is a little descriptor that defines which
	data from vertex buffer objects should be used as input variables to vertex
	shaders. in our case - use our only VBO, and say 'every three floats is a
	variable' */
	glGenVertexArrays(1, &vao/*vboに同じく*/);
	glBindVertexArray(vao);
	//attribを名に含むものはGLSL関係
	glVertexAttribPointer(3/*inシェーダナンバ_43行*/, 3/*vec*/, GL_FLOAT/*データタイプ*/, GL_FALSE/*normalize真偽(0.0～1.0)*/, 0/*ストライド値*/, NULL/*オフセット値*/);
	glEnableVertexAttribArray(3/*inシェーダナンバ*/);/*GLSL*/

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	/* here we copy the shader strings into GL shaders, and compile them. we
	then create an executable shader 'program' and attach both of the compiled
	shaders. we link this, which matches the outputs of the vertex shader to
	the inputs of the fragment shader, etc. and it is then ready to use */
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	/* this loop clears the drawing surface, then draws the geometry described
	by	the VAO onto the drawing surface. we 'poll events' to see if the window
	was closed, etc. finally, we 'swap the buffers' which displays our drawing
	surface onto the view area. we use a double-buffering system which means
	that we have a 'currently displayed' surface, and 'currently being drawn'
	surface. hence the 'swap' idea. in a single-buffering system we would see
	stuff being drawn one-after-the-other */
	while (!glfwWindowShouldClose(window)) {
		/* wipe the drawing surface clear */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		/* draw points 0-3 from the currently bound VAO with current in-use
		shader */
		glDrawArrays(GL_TRIANGLES, 0, 3);
		/* update other events like input handling */
		glfwPollEvents();
		/* put the stuff we've been drawing onto the display */
		glfwSwapBuffers(window);
	}

	/* close GL context and any other GLFW resources */
	glfwTerminate();
	return 0;
}
