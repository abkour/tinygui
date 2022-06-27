#include "ClientState.hpp"
#include <memory>

bool ClientStateManager::LMBPressedThisFrame() const {
	return !prevState.LMB_Down && currentState.LMB_Down;
}

bool ClientStateManager::RMBPressedThisFrame() const {
	return !prevState.RMB_Down && currentState.RMB_Down;
}

void ClientStateManager::UpdatePreviousState() {
	std::memcpy(&prevState, &currentState, sizeof(ClientState));
}
