#include "WindowContainer.h"

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		//Keyboard messages:
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
		//Mouse messages
		case WM_MOUSEMOVE: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			mouse.OnMouseMove(x, y);
			return 0;
		}
		case WM_LBUTTONDOWN: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			mouse.OnLeftPressed(x, y);
			return 0;
		}
		case WM_RBUTTONDOWN: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			mouse.OnRightPressed(x, y);
			return 0;
		}
		case WM_MBUTTONDOWN: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			mouse.OnMiddlePressed(x, y);
			return 0;
		}
		case WM_LBUTTONUP: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			mouse.OnLeftReleased(x, y);
			return 0;
		}
		case WM_RBUTTONUP: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			mouse.OnRightReleased(x, y);
			return 0;
		}
		case WM_MBUTTONUP: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			mouse.OnMiddleReleased(x, y);
			return 0;
		}
		case WM_MOUSEWHEEL: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
				mouse.OnWheelUp(x, y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
				mouse.OnWheelDown(x, y);
			}

			return 0;
		}
		default: {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

