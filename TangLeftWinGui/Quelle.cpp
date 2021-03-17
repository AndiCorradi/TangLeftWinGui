#include <windows.h>
#include <cassert>
#include <time.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
//#include <conio.h>
//#include <iomanip>
//#include <limits>

using namespace std;


#define IDC_CALCULATE_BUTTON 100
#define ID_PAYDAYBOX 101

// declare variables

HWND hResult;
HWND hBalance;
HWND hPayDayBox;
char Balance[1024];

// declare function prototypes

LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
int CalcCurrentDaysOfMonth();
int AmmountOfDaysOfCurrentMonth();
int FillPayDayBox(int, int);
int GetPayDayCurSel();

// declare Variable

int AmmountDaysOfCurrentMonth = AmmountOfDaysOfCurrentMonth();
int DayOfMonth = CalcCurrentDaysOfMonth();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//FillPayDayBox(AmmountDaysOfCurrentMonth, DayOfMonth);

	HWND hWnd; // Name fürs Window Handle
	HWND hButton;
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
	// Button erstellen:
	hButton = CreateWindow("button", "calculate", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 70, 200, 50, hWnd, (HMENU) IDC_CALCULATE_BUTTON, hInstance, 0);
	// Balance Engabefeld erstellen:
	hBalance = CreateWindow("edit", "Balance:", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 200, 50, hWnd, 0, 0, 0);
	// Ausgabe Textfeld erstellen:
	hResult = CreateWindow("edit", "Result:", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 150, 200, 50, hWnd, 0, 0, 0);
	// Combobox PayDay erstellen:
	hPayDayBox = CreateWindowEx(WS_EX_CLIENTEDGE, "combobox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | CBS_DROPDOWNLIST, 10, 250, 200, 200, hWnd, (HMENU)ID_PAYDAYBOX, 0, 0);
	FillPayDayBox(AmmountDaysOfCurrentMonth, DayOfMonth);

	// Debug MessageBox to show a Value
	int CurSelPayday = GetPayDayCurSel();
	char buff[1024];
	sprintf_s(buff, "%d", CurSelPayday);
	MessageBox(0, buff, "Error", MB_OK);

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
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_CALCULATE_BUTTON)
		{
			GetWindowText(hBalance, Balance, 1024);
			SetWindowText(hResult, TEXT(Balance));
			
			int CurSelPayday = GetPayDayCurSel();
			char buff[1024];
			sprintf_s(buff, "%d", CurSelPayday);
			MessageBox(0, buff, "Error", MB_OK);
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // default Windows Message handler
}

int CalcCurrentDaysOfMonth() // datatype integer function without argument
{
	// current Day of the month

	struct tm nowlocal; // set variable/instance "nowlocal" using the predefined structure "tm" as defined in time.h
	time_t now; // set variable now: datatype "time_t" (as defined in "time.h")
	now = time(0); // initialize variable now with "time(0)" The function "time(0) delivers passed seconds since 1.1.1970
	nowlocal = *localtime(&now); // functionpointer that converts calendartime to localtime Parameter (&now) = pointer to memory address of the variable now (& = Addressoperator)
	int DayOfMonth = nowlocal.tm_mday; // set and initialize variable with structure.datafield "tm_mday" (current day of the month)

	return DayOfMonth; // returns the result to the main method
}

int AmmountOfDaysOfCurrentMonth() // datatype integer function without argument
{
	// ammount of days current month (with LeapYear Detection)

	struct tm nowlocal; // set variable/instance "nowlocal" using the predefined structure "tm" as defined in time.h
	time_t now; // set variable now: datatype "time_t" (as defined in "time.h")
	now = time(0); // initialize variable now with "time(0)" The function "time(0) delivers passed seconds since 1.1.1970
	nowlocal = *localtime(&now); // functionpointer that converts calendartime to localtime Parameter (&now) = pointer to memory address of the variable now (& = Addressoperator)
	int currentMonth = nowlocal.tm_mon + 1; //set and initialize variable with structure.datafield "month" and add 1 as the month start with 0
	int currentYear = nowlocal.tm_year + 1900; // set and initialize variable with structure.datafield "tm_year" and add +1900 to get the right year
	int DaysOfMonthsLeap[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // set and initialize array with ammount of days for every month of a leap year
	int DaysOfMonths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // set and initialize array with ammount of days for every month of a regular year
	int AmmountDaysOfCurrentMonth(0); // set and initialize variable

	if (currentYear % 4 == 0) // detect LeapYear
	{
		AmmountDaysOfCurrentMonth = DaysOfMonthsLeap[currentMonth - 1]; // set and initialize variable in leap year

	}
	else //if it is a regular year:
	{
		AmmountDaysOfCurrentMonth = DaysOfMonths[currentMonth - 1]; // set and initialize variable for a regular year

	}

	return AmmountDaysOfCurrentMonth; // returns the result to the main method
}


int FillPayDayBox(int AmmountDaysOfCurrentMonth, int DayOfMonth)  // Populates the Combobox "Payday" with the possible values of the current month (with leapyear detection)
{
	for (int i = 1; i <= AmmountDaysOfCurrentMonth; i++)
	{
		char buff[1024];
		sprintf_s(buff, "%d", i);
		SendMessage(hPayDayBox, CB_ADDSTRING, 0, (LPARAM)buff);
			
	}

	SendMessage(hPayDayBox, CB_SETCURSEL, (WPARAM) 23, 0);
	SendMessage(hPayDayBox, CB_DELETESTRING, (WPARAM)DayOfMonth-1, 0);

	return 0;
}

int GetPayDayCurSel()
{
	int nIndex = SendMessage(hPayDayBox, CB_GETCURSEL, 0, 0);
	char CharCurSel[1024];
	SendMessage(hPayDayBox, CB_GETLBTEXT, nIndex, (LPARAM)CharCurSel);
	string Cursel = CharCurSel;
	int PayDayCursel;
	stringstream(Cursel) >> PayDayCursel;
	

	int b = 0;


	// NEXT STEP: Convert string Cursel

	//return Cursel;
	return PayDayCursel;
}