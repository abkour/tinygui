#include "include/tinygui.hpp"
#include "test/window.hpp"
#include "impl/shader.hpp"

#include <iostream>

int main() {
	try {
		using namespace tinygui;

		Window window(1920, 1080, "tinygui");
		glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		window.enableCursorCallback();

		float quad[12];
		int id[6];
		Rectangle rect(Point2(-0.5f, -0.5f), Point2(0.5f, 0.5f));
		rect.makeQuad(quad);
		
		for (int i = 0; i < 6; ++i) {
			id[i] = 2;
		}

		GLuint vao, vbo;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad) + sizeof(id), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad), quad);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(quad), sizeof(id), id);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribIPointer(1, 1, GL_INT, sizeof(int), reinterpret_cast<void*>(sizeof(quad)));

		Shader shaderProgram(	"C://Users//flora//source//repos//tinygui//impl//gui_shader.glsl.vs", 
								"C://Users//flora//source//repos//tinygui//impl//gui_shader.glsl.fs");
		shaderProgram.bind(); 

		while (!glfwWindowShouldClose(window.window)) {
			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClear(GL_COLOR_BUFFER_BIT);

			auto xoff = window.getXoffset();
			auto yoff = window.getYoffset();
			if (xoff != 0.f || yoff != 0.f) {
				
			}

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