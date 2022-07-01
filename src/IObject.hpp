#pragma once
#include "float2.hpp"
#include <cstddef>
#include <vector>

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
	
	virtual float2 GetTranslationVector() const = 0 {}
	virtual void Translate(const float2 TranslationDelta) = 0 {}

	virtual ObjectStatus Update(const float2 CursorPosition, const ClientStateManager pClientStateManager) = 0 {}
	virtual void Render(unsigned int shaderID) = 0 {}

	std::vector<IObject*> attachedObjects;
	unsigned int id;
};