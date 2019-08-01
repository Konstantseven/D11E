#include "WindowContainer.h"

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_KEYDOWN: {
			unsigned char _char = static_cast<unsigned char>(wParam);
			if (keyboard.IsKeysAutoRepeat()) {
				keyboard.OnKeyPressed(_char);
			}
			else {
				const bool wasPressed = lParam & 1 << 30; // Bit #30 mean the previous
				if (!wasPressed) {                        // key state. 1 - if the key is down
					keyboard.OnKeyPressed(_char);         // before the message is sent, 
				}                                         // or 0 - if the key is up.
			}
			return 0;
		}
		case WM_KEYUP:{
			unsigned char _char = static_cast<unsigned char>(wParam);
			keyboard.OnKeyReleased(_char);
			return 0;
		}
		case WM_CHAR: {
			unsigned char _char = static_cast<unsigned char>(wParam);
			if (keyboard.IsCharAutoRepeat()) {
				keyboard.OnChar(_char);
			}
			else {
				const bool wasPressed = lParam & 1 << 30;
				if (!wasPressed) {
					keyboard.OnChar(_char);
				}
			}
			return 0;
		}
	
		default: {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

