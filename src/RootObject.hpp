#pragma once
#include "IObject.hpp"

class RootObject : public IObject {

public:

	RootObject() = default;

	// This sits on top of all other objects
	virtual bool Update(const Vec2 CursorPosition, const ClientState pClientState) override { return true; }
	virtual void Render(unsigned int shaderID) {}

	virtual std::size_t GetVertexCount() const override { return 0; }
	virtual float* GetVertices() const override { return nullptr; }

};