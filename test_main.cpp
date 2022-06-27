#include "include/tinygui.hpp"
#include "test/window.hpp"
#include "impl/font_engine.hpp"
#include <shaderdirect.hpp>

#include <chrono>
#include <iostream>
#include <thread>

#include "applications/frame_time_ui.hpp"
#include "impl/histogram.hpp"

struct StateOfMouseAtClick {
	bool mouseClicked;
	int rect_id;
};

double round(double d)
{
	return floor(d + 0.5);
}

float foo(float input, float x0, float x1, float y0, float y1) {
	float slope = 1.0 * (y1 - y0) / (x1 - x0);
	return y1 + round(slope * (input - y0));
}

int main() {
	try {
		std::srand(std::time(nullptr));

		using namespace tinygui;

		Point2 screen_resolution(1920, 1080);

		Window window(screen_resolution.x, screen_resolution.y, "tinygui");

		glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		window.enableCursorCallback();

		glfwSwapInterval(0);

		float quad[12];
		int id[6];
		Rectangle rect(Point2(-0.5f, -0.5f), Point2(0.5f, 0.5f));
		rect.makeQuad(quad);
		std::fill_n(id, 6, 1);

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

		const int defaultRectId = 0;

		StateOfMouseAtClick mouseState;
		mouseState.mouseClicked = false;
		mouseState.rect_id = 0;

		GLuint ssbo;
		glGenBuffers(1, &ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int), &defaultRectId, GL_STATIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

		const unsigned fontsize = 192;
		const float fontcolor[] = { 1.f, 1.f, 0.f };
		std::shared_ptr<TGUIFontEngine> fontengine = std::make_shared<TGUIFontEngine>(fontsize);
		fontengine->setColor(fontcolor);

		TGUIFrameDisplay ftd(fontengine);
		ftd.setScreenPosition(100, 500);

		ShaderWrapper shaderProgram(
			true, 
			shader_p(GL_VERTEX_SHADER, "C://Users//flora//source//repos//tinygui//impl//gui_shader.glsl.vs"),
			shader_p(GL_FRAGMENT_SHADER, "C://Users//flora//source//repos//tinygui//impl//gui_shader.glsl.fs")
		);
		shaderProgram.bind();
	
		// Upload initial cursor position
		glUniform1f(glGetUniformLocation(shaderProgram.id(), "cursorX"), window.getXpos());
		glUniform1f(glGetUniformLocation(shaderProgram.id(), "cursorY"), window.getYpos());

		std::vector<std::string> text;
		std::string longString;
		for (int i = 0; i < 30'000; ++i) {
			longString.push_back('a' + (std::rand() % 25));
		}
		text.push_back(longString);

		double totalFrameTime = 0.f;
		double minFrameTime = 10000000.f;
		double maxFrameTime = 0.f;
		int nFramesRendered = 0;

		bool cursorMoved = false;

		std::string helloworld = "hello world";

		TGUIHistogram histogram(2, 50, 0, 30);

		std::cout.sync_with_stdio(false);
		while (!glfwWindowShouldClose(window.window)) {
			//ftd.timeStart();
			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClear(GL_COLOR_BUFFER_BIT);
			/*bool cursorMoved = false;

			auto xoff = window.getXoffset();
			auto yoff = window.getYoffset();

			if (xoff != 0.f || yoff != 0.f) {
				// Cursor position changed, update uniform
				double xpos, ypos;
				glfwGetCursorPos(window.window, &xpos, &ypos);
				glUniform1f(glGetUniformLocation(shaderProgram.id(), "cursorX"), xpos);
				glUniform1f(glGetUniformLocation(shaderProgram.id(), "cursorY"), ypos);
				window.resetCursorOffset();
				cursorMoved = true;
			}

			glBindVertexArray(vao);
			//glDrawArrays(GL_TRIANGLES, 0, 6);

			if (glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !mouseState.mouseClicked) {
				mouseState.mouseClicked = true;
				glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int), &mouseState.rect_id);
				std::cout << "mousestate: " << mouseState.rect_id << '\n';
			}
			if (glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
				mouseState.mouseClicked = false;
			}

			if (cursorMoved && mouseState.mouseClicked) {
				if (mouseState.rect_id != 0) {
					auto spXoff = xoff * 2 / screen_resolution.x;
					auto spYoff = yoff * 2 / screen_resolution.y;
					for (int i = 0; i < 6; ++i) {
						quad[i * 2] += spXoff;
						quad[i * 2 + 1] += spYoff;
					}
					// Update buffer contents
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad), quad);
				}
			}*/
			shaderProgram.bind();
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int), &defaultRectId);
			if (nFramesRendered % 1000 == 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
				//ftd.timeEnd();
			}
			//ftd.drawFPS();

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			histogram.plot(15.f + (rand() % 100) * 0.05);

			glfwSwapBuffers(window.window);
			glfwPollEvents();
			if (glfwGetKey(window.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window.window, GLFW_TRUE);
			}
			nFramesRendered++;
		}
		std::cout << "Number of frames rendered: " << nFramesRendered << '\n';
		std::cout << "Average frame time: " << totalFrameTime / nFramesRendered << '\n';

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