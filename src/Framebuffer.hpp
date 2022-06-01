#pragma once
#include <glad/glad.h>

class IFrameBuffer {

public:

	IFrameBuffer() = default;

	virtual ~IFrameBuffer() = 0;

	virtual void Bind() = 0;
};

class OpenGLFrameBuffer : public IFrameBuffer {

public:

	OpenGLFrameBuffer() {
		//
	}
	~OpenGLFrameBuffer() {
		//
	}

protected:

	GLuint Fbo;

};

class DX11FrameBuffer : public IFrameBuffer {

public:

	DX11FrameBuffer() {
		//
	}

	~DX11FrameBuffer() {
		//
	}
};