// this programm calculates the daily budget left until next payday based on your balance and payday input.
// Version: 1.2
// author: andi@corradi.ch release Date: 19.March 2021

#include <windows.h>
#include <WinUser.h>
#include <cassert>
#include <time.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

// TODO:
// - save Window Position and remember on next start
// - create an animated Splash Screen
// DONE set Mainwindow size to non resizable
// - Write Payday Selection to preference file and read every start
// DONE - Error Handling: prevent calculate on empty Balance
// - Set Window Icon
// - Create MSI with Logos on Shortcuts

using namespace std;

#define IDC_CALCULATE_BUTTON 100
#define ID_PAYDAYBOX 101
#define IDC_BALANCE_BUTTON_0 102
#define IDC_BALANCE_BUTTON_1 103
#define IDC_BALANCE_BUTTON_2 104
#define IDC_BALANCE_BUTTON_3 105
#define IDC_BALANCE_BUTTON_4 106
#define IDC_BALANCE_BUTTON_5 107
#define IDC_BALANCE_BUTTON_6 108
#define IDC_BALANCE_BUTTON_7 109
#define IDC_BALANCE_BUTTON_8 110
#define IDC_BALANCE_BUTTON_9 111
#define IDC_BALANCE_BUTTON_C 112

// declare variables

HWND hResult;
HWND hResultLabel;
HWND hBalance;
HWND hBalanceLabel;
HWND hPayDayBox;
HWND hPayDayLabel;
LPCSTR Zero = "0";
LPCSTR One = "1";
LPCSTR Two = "2";
LPCSTR Three = "3";
LPCSTR Four = "4";
LPCSTR Fife = "5";
LPCSTR Six = "6";
LPCSTR Seven = "7";
LPCSTR Eight = "8";
LPCSTR Nine = "9";
HBITMAP hLogoImage, hTitleImage;
HWND hLogo;
HWND hTitle;

// declare function prototypes

LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
int CalcCurrentDaysOfMonth();
int AmmountOfDaysOfCurrentMonth();
int FillPayDayBox(int, int);
int GetPayDayCurSel();
int GetBalance();
void loadImages();
int CalcDaysToPayday(int);
int CalculateDailyBudget(int, int);

// declare Variable

int AmmountDaysOfCurrentMonth = AmmountOfDaysOfCurrentMonth();
int DayOfMonth = CalcCurrentDaysOfMonth();
int PayDay = GetPayDayCurSel();
int DaysToPayday = CalcDaysToPayday(PayDay);
int CurselPayDay = GetPayDayCurSel();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	loadImages();
	HWND hWnd; // Name f?rs Window Handle
	HWND hButton;
	WNDCLASS wc; // Struct/Klasse f?r die Fenster definieren
	MSG msg; // Variable f?r das Eventhandling
	HWND hButton0;
	HWND hButton1;
	HWND hButton2;
	HWND hButton3;
	HWND hButton4;
	HWND hButton5;
	HWND hButton6;
	HWND hButton7;
	HWND hButton8;
	HWND hButton9;
	HWND hButtonC;

	// Struct erstellen:
	wc.style = CS_HREDRAW | CS_VREDRAW; // ganzes Fenster neu zeichnen bei resize
	wc.lpfnWndProc = MessageHandler; // Pointer auf Windows Procedure
	wc.cbClsExtra = 0; // MSDN: The number of extra bytes to allocate following the window-class structure. The system initializes the bytes to zero.
	wc.cbWndExtra = 0; // MSDN: The number of extra bytes to allocate following the window instance. The system initializes the bytes to zero. If an application uses WNDCLASSEX to register a dialog box created by using the CLASS directive in the resource file, it must set this member to DLGWINDOWEXTRA.
	wc.hInstance = hInstance; // ?bergeben des Instanznamens (Muss gleich sein wie der Parameter Name bei der WinMain definition)
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Cursor im Fenster
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // Fensterhintergrund
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "WINAPITest"; // Name f?r die erstellte Struktur(Klasse)
	// wc = {}; initialisiert alle Parameter oben mit dem Standard NULL (Alternative zum Parameter ausf?llen).

	// Struktur (WINAPITest) bei Windows registrieren:
	assert(RegisterClass(&wc)); // Pointer auf die Window Klasse

	//Fenster erstellen:
	hWnd = CreateWindow("WINAPITest", "TangLeft 1.2", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 490, 400, 0, 0, hInstance, 0);
	//GetWindowPlacement(HWND hWnd, WINDOWPLACEMENT * lpwndpl);
	// Button erstellen:
	hButton = CreateWindow("button", "calculate", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 270, 187, 195, 25, hWnd, (HMENU) IDC_CALCULATE_BUTTON, hInstance, 0);
	// Balance Engabefeld erstellen:
	hBalance = CreateWindow("edit", "", WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 10, 90, 215, 20, hWnd, 0, 0, 0); //ES_NUMBER: allows only digits to enter.
	hBalanceLabel = CreateWindow("static", "Balance:", WS_CHILD | WS_VISIBLE, 10, 70, 200, 20, hWnd, 0, 0, 0);
	// Ausgabe Textfeld erstellen:
	hResult = CreateWindow("edit", 0, WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 250, 455, 100, hWnd, 0, 0, 0);
	hResultLabel = CreateWindow("static", "daily budget unti next payday:", WS_CHILD | WS_VISIBLE, 10, 225, 250, 20, hWnd, 0, 0, 0);
	// Combobox PayDay erstellen:
	hPayDayBox = CreateWindowEx(WS_EX_CLIENTEDGE, "combobox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | CBS_DROPDOWNLIST, 10, 190, 215, 200, hWnd, (HMENU)ID_PAYDAYBOX, 0, 0);
	hPayDayLabel = CreateWindow("static", "PayDay:", WS_CHILD | WS_VISIBLE, 10, 170, 200, 20, hWnd, 0, 0, 0);
	FillPayDayBox(AmmountDaysOfCurrentMonth, DayOfMonth);
	// Logo anzeigen:
	hLogo = CreateWindow("Static",0, WS_TABSTOP | WS_CHILD | WS_VISIBLE | SS_BITMAP, 250, 15, 100,100, hWnd, 0,0,0);
	SendMessage(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hLogoImage );
	// Titel Schrift anzeigen:
	hTitle = CreateWindow("Static", 0, WS_TABSTOP | WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 20, 215, 47, hWnd, 0, 0, 0);
	SendMessage(hTitle, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hTitleImage);
	// Button 0 erstellen:
	hButton0 = CreateWindow("button", "0", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 110, 140, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_0, hInstance, 0);
	// Button 1 erstellen:
	hButton1 = CreateWindow("button", "1", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 115, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_1, hInstance, 0);
	// Button 2 erstellen:
	hButton2 = CreateWindow("button", "2", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 35, 115, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_2, hInstance, 0);
	// Button 3 erstellen:
	hButton3 = CreateWindow("button", "3", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 60, 115, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_3, hInstance, 0);
	// Button 4 erstellen:
	hButton4 = CreateWindow("button", "4", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 85, 115, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_4, hInstance, 0);
	// Button 5 erstellen:
	hButton5 = CreateWindow("button", "5", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 110, 115, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_5, hInstance, 0);
	// Button 6 erstellen:
	hButton6 = CreateWindow("button", "6", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 140, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_6, hInstance, 0);
	// Button 7 erstellen:
	hButton7 = CreateWindow("button", "7", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 35, 140, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_7, hInstance, 0);
	// Button 8 erstellen:
	hButton8 = CreateWindow("button", "8", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 60, 140, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_8, hInstance, 0);
	// Button 9 erstellen:
	hButton9 = CreateWindow("button", "9", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 85, 140, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_9, hInstance, 0);
	// Button C erstellen:
	hButtonC = CreateWindow("button", "C", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 205, 115, 20, 20, hWnd, (HMENU)IDC_BALANCE_BUTTON_C, hInstance, 0);

	// Debug MessageBox to show a Value
	int CurSelPayday = GetPayDayCurSel();
	char buff[1024];
	sprintf_s(buff, "%d", CurSelPayday);
	MessageBox(0, buff, "DebugBox", MB_OK);

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



LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int CurSelPayday;
	int BalanceInt;
	int DaysToPayday = 0;
	int DailyBudget = 0;
	WINDOWPLACEMENT wndpl = { 0 };
	HKEY hkhandle;
	int xpos = 0;

	HDC hdcStatic = (HDC)wParam; //Set Variable for static control Background Color
	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		wndpl.length = sizeof(wndpl);

		GetWindowPlacement(hWnd, &wndpl);
		if (RegCreateKeyA(HKEY_CURRENT_USER, "SOFTWARE\\TANGLEFT", &hkhandle) == ERROR_SUCCESS)
		{
			RegSetValueExA(hkhandle, "xpos", 0, REG_DWORD,  (const BYTE *)&xpos, sizeof(xpos));
			RegCloseKey(hkhandle);
		}
		// else Fehlermeldung
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_CALCULATE_BUTTON)
		{
			CurSelPayday = GetPayDayCurSel();
			BalanceInt = GetBalance();

			if (BalanceInt == 0)
			{
				SetWindowText(hBalance, TEXT(""));
			}
			else
			{
				DaysToPayday = CalcDaysToPayday(CurSelPayday);
				DailyBudget = CalculateDailyBudget(BalanceInt, DaysToPayday);

				//convert int to string
				stringstream DailyBudgetStr;
				DailyBudgetStr << DailyBudget;
				string DailyBudgetString = DailyBudgetStr.str();

				//convert string to LPCSTR
				LPCSTR DailyBudgetLP = DailyBudgetString.c_str();

				//GetWindowText(hBalance, Balance, 1024);
				SetWindowText(hResult, TEXT(DailyBudgetLP));
			}

			//Debug Message Box PayDay
			//int CurSelPayday = GetPayDayCurSel();
			//char buff[1024];
			//sprintf_s(buff, "%d", CurSelPayday);
			//MessageBox(0, buff, "PayDay", MB_OK);

			// Debug Message Box Balance
			//int BalanceInt = GetBalance();
			//char buff2[1024];
			//sprintf_s(buff2, "%d", BalanceInt);
			//MessageBox(0, buff2, "Balance", MB_OK);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_0)
		{
			SendMessage(hBalance, EM_SETSEL, 100, 100);
			SendMessage(hBalance, EM_REPLACESEL, TRUE, (LPARAM)Zero);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_1)
		{
			SendMessage(hBalance, EM_SETSEL, 100, 100);
			SendMessage(hBalance, EM_REPLACESEL, TRUE, (LPARAM)One);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_2)
		{
			SendMessage(hBalance, EM_SETSEL, 100, 100);
			SendMessage(hBalance, EM_REPLACESEL, TRUE, (LPARAM)Two);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_3)
		{
			SendMessage(hBalance, EM_SETSEL, 100, 100);
			SendMessage(hBalance, EM_REPLACESEL, TRUE, (LPARAM)Three);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_4)
		{
			SendMessage(hBalance, EM_SETSEL, 100, 100);
			SendMessage(hBalance, EM_REPLACESEL, TRUE, (LPARAM)Four);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_5)
		{
			SendMessage(hBalance, EM_SETSEL, 100, 100);
			SendMessage(hBalance, EM_REPLACESEL, TRUE, (LPARAM)Fife);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_6)
		{
			SendMessage(hBalance, EM_SETSEL, 100, 100);
			SendMessage(hBalance, EM_REPLACESEL, TRUE, (LPARAM)Six);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_7)
		{
			SendMessage(hBalance, EM_SETSEL, 100, 100);
			SendMessage(hBalance, EM_REPLACESEL, TRUE, (LPARAM)Seven);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_8)
		{
			SendMessage(hBalance, EM_SETSEL, 100, 100);
			SendMessage(hBalance, EM_REPLACESEL, TRUE, (LPARAM)Eight);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_9)
		{
			SendMessage(hBalance, EM_SETSEL, 100, 100);
			SendMessage(hBalance, EM_REPLACESEL, TRUE, (LPARAM)Nine);
		}

		if (LOWORD(wParam) == IDC_BALANCE_BUTTON_C)
		{
			SetWindowText(hBalance, TEXT(""));
			SetWindowText(hResult, TEXT(""));
		}

	// Set static control Background Color
	case WM_CTLCOLORSTATIC:
		SetBkColor(hdcStatic, RGB(255, 255, 255));
		return (INT_PTR)CreateSolidBrush(RGB(255, 255, 255));

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

int CalcDaysToPayday(int CurSelPayDay) // integer function with argument (integer parameter)
{
	// Calculate Days to Payday

	int DayOfMonth = CalcCurrentDaysOfMonth(); // set and initialize local variable with result of function "CalcCurrentDayOfMonth"
	int AmmountDaysOfCurrentMonth = AmmountOfDaysOfCurrentMonth(); //set and initialize local variable with result of function "AmmountOfDaysOfCurrentMonth"

	if (DayOfMonth < CurSelPayDay) // check if DayOfMonth is below PayDay
	{
		DaysToPayday = CurSelPayDay - DayOfMonth; // subtract DayOfMonth from PayDay to get DaysToPayDay
	}
	else // if DayOfMonth is higher then PayDay:
	{
		DaysToPayday = AmmountDaysOfCurrentMonth - DayOfMonth + CurSelPayDay; // subtract DayOfMonth from AmmountDaysOfCurrentMonth and add 24 to ger DaysToPayday
	}

	return DaysToPayday; // returns the result to the main function
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

int CalculateDailyBudget(int BalanceInt, int DaysToPayday) // datatype integer function with argument (2 integer parameters)
{
	int DailyBudget;
	DailyBudget = BalanceInt / DaysToPayday; // divide Balance thrue DaysToPayday to get the daily budget until payday
	return DailyBudget; // returns the result to main function.
}

int GetPayDayCurSel() // Picks up the User PayDay Combobox Selection and returns INT.
{
	int nIndex = SendMessage(hPayDayBox, CB_GETCURSEL, 0, 0);
	char CharCurSel[1024];
	SendMessage(hPayDayBox, CB_GETLBTEXT, nIndex, (LPARAM)CharCurSel);
	string Cursel = CharCurSel;
	int PayDayCursel;
	stringstream(Cursel) >> PayDayCursel;
	
	return PayDayCursel;
}

int GetBalance()
{
	char Balance[1024];
	GetWindowText(hBalance, Balance, 1024);
	string SBalance = Balance;
	int BalanceInt(0);
	stringstream(SBalance) >> BalanceInt;

	return BalanceInt;
}

void loadImages()
{
	hLogoImage = (HBITMAP)LoadImageW(NULL, L"LogoTangLeft.bmp", IMAGE_BITMAP, 222, 150, LR_LOADFROMFILE);
	hTitleImage = (HBITMAP)LoadImageW(NULL, L"TangLeftTitle.bmp", IMAGE_BITMAP, 215, 47, LR_LOADFROMFILE);
}
