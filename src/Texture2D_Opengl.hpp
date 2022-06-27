 #pragma once
#include "Texture2D.hpp"
#include <glad/glad.h>

class Texture2D_OGL : public ITexture2D {

public:

	Texture2D_OGL();
	~Texture2D_OGL() override;

	virtual void AllocateSpace(const float2& TextureDimensions, TextureFormat& TextureFormat) override;
	virtual void UpdateContents(const void* pSource, const float2& pTextureDimensions) override;
	virtual void GenerateMipmaps() override;
	virtual void Bind() override;
	virtual void BindToUnit(const std::size_t pTextureUnit) override;

protected:

	GLuint Format;
};