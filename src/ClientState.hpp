#pragma once
#include "float2.hpp"

struct ClientState {
	float2 MouseDelta;
	float2 MousePosition;

	bool RMB_Down = false;
	bool LMB_Down = false;
	bool MMB_Down = false;
	bool MMB_ScrollUp = false;
	bool MMB_ScrollDown = false;
	bool W_Down = false;
	bool A_Down = false;
	bool S_Down = false;
	bool D_Down = false;
	bool Esc_Down = false;
	bool Space_Down = false;
};

struct ClientStateManager {

	bool LMBPressedThisFrame() const;
	bool RMBPressedThisFrame() const;
	void UpdatePreviousState();

	ClientState prevState, currentState;
};