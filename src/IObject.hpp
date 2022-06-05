#pragma once
#include "Vec2.hpp"
#include <cstddef>
#include <list>

#include "ClientState.hpp"

enum class ObjectStatus {
	DEFAULT, // This is a placeholder for now
	DRAG,
	TERMINATED
};

class IObject {

public:

	IObject() = default;

	virtual ~IObject() = 0 {}

	// Getters
	unsigned int GetId() const { return id; }
	virtual std::size_t GetVertexCount() const = 0 {}
	virtual float* GetVertices() const = 0 {}
	Vec2 GetTranslationVector() const { return TranslationVector; }
	virtual void Translate(const Vec2 TranslationDelta) = 0 {}

	virtual ObjectStatus Update(const Vec2 CursorPosition, const ClientState pClientState) = 0 {}
	virtual void Render(unsigned int shaderID) = 0 {}

	std::list<IObject*> attachedObjects;
	unsigned int id;

protected:

	Vec2 TranslationVector;
};