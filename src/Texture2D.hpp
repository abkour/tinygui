#pragma once
#include "Vec2.hpp"
#include <cstddef>

enum class TextureFormat {
	RGB = 0,
	RGBA = 1
};

class ITexture2D {

public:

	ITexture2D() = default;
	virtual ~ITexture2D() = 0 {}

	virtual void AllocateSpace(const Vec2& TextureDimensions, TextureFormat& TextureFormat) = 0;
	virtual void UpdateContents(const void* pSource, const Vec2& pTextureDimensions) = 0;
	virtual void GenerateMipmaps() = 0;
	virtual void Bind() = 0;
	virtual void BindToUnit(const std::size_t pTextureUnit) = 0;

	virtual unsigned int GetTextureID() { return TextureID; }

protected:

	unsigned int TextureID;
};

