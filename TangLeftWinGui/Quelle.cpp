#include <windows.h>
#include <cassert> // ?

LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM); //Funktion


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd; // Name fürs Window Handle   
	WNDCLASS wc; // Struct/Klasse für die Fenster definieren
	MSG msg; // Variable für das Eventhandling

	// Struct erstellen:
	wc.style = CS_HREDRAW | CS_VREDRAW; // ganzes Fenster neu zeichnen bei resize
	wc.lpfnWndProc = MessageHandler; // Pointer auf Windows Procedure
	wc.cbClsExtra = 0; // MSDN: The number of extra bytes to allocate following the window-class structure. The system initializes the bytes to zero.
	wc.cbWndExtra = 0; // MSDN: The number of extra bytes to allocate following the window instance. The system initializes the bytes to zero. If an application uses WNDCLASSEX to register a dialog box created by using the CLASS directive in the resource file, it must set this member to DLGWINDOWEXTRA.
	wc.hInstance = hInstance; // übergeben des Instanznamens (Muss gleich sein wie der Parameter Name bei der WinMain definition)
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Cursor im Fenster
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // Fensterhintergrund
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "WINAPITest"; // Name für die erstellte Struktur(Klasse)
	// wc = {}; initialisiert alle Parameter oben mit dem Standard NULL (Alternative zum Parameter ausfüllen).

	// Struktur (WINAPITest) bei Windows registrieren:
	assert(RegisterClass(&wc)); // Pointer auf die Window Klasse

	//Fenster erstellen:
	hWnd = CreateWindow("WINAPITest", "Fenster Titel", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, hInstance, 0);

	//Fenster aufrufen:
	ShowWindow(hWnd, nCmdShow);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	UpdateWindow(hWnd);

	// Event handling
	while (true)
	{
		BOOL result = GetMessage(&msg, 0, 0, 0);
		if (result > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			return result;
		}
	}
}

LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //Funktion
{
	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // default Windows Message handler
}