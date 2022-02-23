#include "include/tinygui.hpp"
#include "test/window.hpp"
#include "impl/shader.hpp"

#include <iostream>

int main() {

	try {
		using namespace tinygui;

		Window window(1920, 1080, "tinygui");
		glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		float quad[] =
		{
			0.f, 0.f,
			1.f, 0.f,
			1.f, 1.f,

			0.f, 0.f,
			1.f, 1.f,
			0.f, 1.f
		};

		GLuint vao, vbo;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		Shader shaderProgram(	"C://Users//flora//source//repos//tinygui//impl//gui_shader.glsl.vs", 
								"C://Users//flora//source//repos//tinygui//impl//gui_shader.glsl.fs");
		shaderProgram.bind(); 

		while (!glfwWindowShouldClose(window.window)) {
			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			glfwSwapBuffers(window.window);
			glfwPollEvents();

			if (glfwGetKey(window.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window.window, GLFW_TRUE);
			}
		}

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << '\n';
	}
	catch (...) {
		std::cout << "Unexpected error somewhere!\n";
	}

}