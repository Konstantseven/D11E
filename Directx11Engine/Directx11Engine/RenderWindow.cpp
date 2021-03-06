#include "WindowContainer.h"

bool RenderWindow::Initialize(WindowContainer* pWindowContainer ,HINSTANCE hInitialise, std::string windowTitle, std::string windowClass, int width, int height) {
	this->hInstance       = hInstance;
	this->width           = width;
	this->height          = height;
	this->windowTitle     = windowTitle;
	this->windowTitleWide = helpers::converter::StringToWString(this->windowTitle);
	this->windowClass     = windowClass;
	this->windowClassWide = helpers::converter::StringToWString(this->windowClass);

	this->RegisterWindowClass();

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - this->width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - this->height / 2;

	RECT windowRectangle;
	windowRectangle.left = centerScreenX;
	windowRectangle.top = centerScreenY;
	windowRectangle.right = windowRectangle.left + this->width;
	windowRectangle.bottom = windowRectangle.top + this->height;
	AdjustWindowRect(&windowRectangle, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	this->handle = CreateWindowEx(0, //Extended Windows style - we are using the default. For other options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ff700543(v=vs.85).aspx
								  this->windowClassWide.c_str(), //Window class name
								  this->windowTitleWide.c_str(), //Window Title
								  WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //Windows style - See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms632600(v=vs.85).aspx
								  windowRectangle.left, //Window X Position
								  windowRectangle.top, //Window Y Position
								  windowRectangle.right - windowRectangle.left, //Window Width
								  windowRectangle.bottom - windowRectangle.top, //Window Height
								  NULL, //Handle to parent of this window. Since this is the first window, it has no parent window.
								  NULL, //Handle to menu or child window identifier. Can be set to NULL and use menu in WindowClassEx if a menu is desired to be used.
								  this->hInstance, //Handle to the instance of module to be used with this window
								  pWindowContainer); //Param to create window

	if (this->handle == NULL){
		helpers::error_logger::Log(GetLastError(), "CreateWindowsEX Failed for window: " + this->windowTitle);
		return false;
	}

	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool RenderWindow::ProcessMessages() {
	// Handle the windows messages.
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.

	while (PeekMessage(	&msg, //Where to store message (if one exists) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
						this->handle, //Handle to window we are checking messages for
						0,    //Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used to filter only mouse messages for example.
						0,    //Maximum Filter Msg Value
						PM_REMOVE)){//Remove message after capturing it via PeekMessage. For more argument options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
							TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644955(v=vs.85).aspx
							DispatchMessage(&msg); //Dispatch message to our Window Proc for this window. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644934(v=vs.85).aspx
						}
	// Check if the window was closed
	if (msg.message == WM_NULL){
		if (!IsWindow(this->handle)){
			this->handle = NULL; //Message processing loop takes care of destroying this window
			UnregisterClass(this->windowClassWide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

RenderWindow::~RenderWindow() {
	if (this->handle != NULL){
		UnregisterClass(this->windowClassWide.c_str(), this->hInstance);
		DestroyWindow(handle);
	}	
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE: {
			DestroyWindow(hwnd);
			return 0;
		}
		default: {
			WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_NCCREATE: {
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);

			if (pWindow == nullptr) {
				helpers::error_logger::Log("Critical error: Pointer to window container is null during WM_NCREATE!");
				exit(-1);
			}

			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));

			return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		}
		default: {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

void RenderWindow::RegisterWindowClass() {
	WNDCLASSEX wc; //Our Window Class (This has to be filled before our window can be created) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx

	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
	wc.lpfnWndProc   = HandleMessageSetup; //Pointer to Window Proc function for handling messages from this window
	wc.cbClsExtra    = 0; //# of extra bytes to allocate following the window-class structure. We are not currently using this.
	wc.cbWndExtra    = 0; //# of extra bytes to allocate following the window instance. We are not currently using this.
	wc.hInstance     = this->hInstance; //Handle to the instance that contains the Window Procedure
	wc.hIcon         = NULL;   //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
	wc.hIconSm       = NULL; //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW); //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
	wc.hbrBackground = NULL; //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
	wc.lpszMenuName  = NULL; //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
	wc.lpszClassName = this->windowClassWide.c_str(); //Pointer to null terminated string of our class name for this window.
	wc.cbSize        = sizeof(WNDCLASSEX); //Need to fill in the size of our struct for cbSize

	RegisterClassEx(&wc); // Register the class so that it is usable.
}

HWND RenderWindow::GetHWND() const {
	return this->handle;
}