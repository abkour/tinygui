#pragma once
#include "IObject.hpp"

class RootObject : public IObject {

public:

	RootObject() = default;

	// This sits on top of all other objects
	virtual ObjectStatus Update(const float2 CursorPosition, const ClientStateManager pClientStateManager) override { 
		return ObjectStatus::DEFAULT; 
	}
	virtual void Render(unsigned int shaderID) {}
	virtual void Translate(const float2 TranslationDelta) {}

	float2 GetTranslationVector() const { return float2(); }

	virtual std::size_t GetVertexCount() const override { return 0; }
	virtual float* GetVertices() const override { return nullptr; }

};