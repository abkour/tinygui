#pragma once
#include "IObject.hpp"

class RootObject : public IObject {

public:

	RootObject() = default;

	// This sits on top of all other objects
	virtual ObjectStatus Update(const Vec2 CursorPosition, const ClientState pClientState) override { 
		return ObjectStatus::DEFAULT; 
	}
	virtual void Render(unsigned int shaderID) {}
	virtual void Translate(const Vec2 TranslationDelta) {}

	virtual std::size_t GetVertexCount() const override { return 0; }
	virtual float* GetVertices() const override { return nullptr; }

};