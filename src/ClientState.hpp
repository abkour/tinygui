#pragma once
#include "Vec2.hpp"

struct ClientState {
	Vec2 MouseDelta;
	Vec2 MousePosition;

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