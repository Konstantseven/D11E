#include "WindowContainer.h"

WindowContainer::WindowContainer() {
	static bool rawInputInitialized = false;

	if (!rawInputInitialized) {
		RAWINPUTDEVICE rawInputDevice;

		rawInputDevice.usUsagePage = 0x01;
		rawInputDevice.usUsage = 0x02;
		rawInputDevice.dwFlags = 0;
		rawInputDevice.hwndTarget = 0;

		if (!RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice))) {
			helpers::error_logger::Log(GetLastError(), "Failed to register raw input devices!");
			exit(-1);
		}
		rawInputInitialized = true;
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

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
		case WM_INPUT: {
			UINT dataSize;

			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

			if (dataSize > 0) {
				std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]> (dataSize);

				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
					RAWINPUT* rawInput = reinterpret_cast<RAWINPUT*> (rawData.get());

					if (rawInput->header.dwType == RIM_TYPEMOUSE) {
						mouse.OnMouseMoveRaw(rawInput->data.mouse.lLastX, rawInput->data.mouse.lLastY);
					}
				}
			}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		default: {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

