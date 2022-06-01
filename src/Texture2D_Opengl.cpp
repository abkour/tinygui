#include "Texture2D_Opengl.hpp"

Texture2D_OGL::Texture2D_OGL() {
	glGenTextures(1, &TextureID);
}

Texture2D_OGL::~Texture2D_OGL() {
	glDeleteTextures(1, &TextureID);
}

void Texture2D_OGL::AllocateSpace(const Vec2& TextureDimensions, TextureFormat& TextureFormat) {
	glBindTexture(GL_TEXTURE_2D, TextureID);

	switch (TextureFormat) {
	case TextureFormat::RGB:
		Format = GL_RGB;
		break;
	case TextureFormat::RGBA:
		Format = GL_RGBA;
		break;
	default:
		Format = GL_RGB;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Format, TextureDimensions.x, TextureDimensions.y, 0, Format, GL_FLOAT, NULL);
}

void Texture2D_OGL::UpdateContents(const void* pSource, const Vec2& pTextureDimensions) {
	glTexImage2D(GL_TEXTURE_2D, 0, Format, pTextureDimensions.x, pTextureDimensions.y, 0, Format, GL_FLOAT, pSource);
}

void Texture2D_OGL::GenerateMipmaps() {
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D_OGL::Bind() {
	glBindTexture(GL_TEXTURE_2D, TextureID);
}

void Texture2D_OGL::BindToUnit(const std::size_t pTextureUnit) {
	glBindTextureUnit(pTextureUnit, TextureID);
}