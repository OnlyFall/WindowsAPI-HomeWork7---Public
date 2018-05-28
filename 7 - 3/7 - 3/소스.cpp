#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>
#include "resource.h"

#define IDC_CHECK1 101
#define IDC_CHECK2 102
#define IDC_CHECK3 103
#define IDC_CHECK4 104
#define IDC_CHECK5 105
#define IDC_CHECK6 106

#define IDC_MAP 201

LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK ChildProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

BOOL CollisionCheck(int x, int y, int mx, int my);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //메인
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hinst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, NULL, WS_OVERLAPPEDWINDOW, 0, 0,1000, 800, NULL, (HMENU)NULL, hInstance, NULL);

	WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpszClassName = "ChildClass"; // 차일드 윈도우 클래스 이름
	WndClass.lpfnWndProc = (WNDPROC)ChildProc; // 차일드 윈도우 프로시저 지정
	RegisterClassEx(&WndClass);
	//WndClass.lpfnWndProc = (WNDPROC)ChildProc2; // 차일드 윈도우 프로시저 지정

	//WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//WndClass.lpszClassName = "ChildClass2"; // 차일드 윈도우 클래스 이름
	//WndClass.lpfnWndProc = (WNDPROC)ChildProc2; // 차일드 윈도우 프로시저 지정
	//RegisterClassEx(&WndClass);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

struct Picture {
	int index = -1;
};

HDC hdc, memdc, memdc2;

static int Select = 3;

static Picture picture[7];
static Picture Release;
static int selection = -1;




inline LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	PAINTSTRUCT ps;
	HWND hButton;
	static int check = 0;
	static int count = 0;
	char text[100];
	static HWND hCheck[2], CLOSE, board, Comfirm, SelectList, Success;
	char Items[][15] = { "1번 그림", "2번 그림", "3번 그림", "4번 그림", "5번 그림" , "6번 그림", "7번 그림" };
	static int cList[2];
	static int selection;
	static BOOL Complete = FALSE;
	switch (iMessage) {
	case WM_CREATE:
		hdc = GetDC(hWnd);
		//		board1 = CreateWindow("CHILD", "좀 나와바" , WS_CHILD | WS_CHILD | WS_BORDER, 50, 50, 300, 300, hWnd, NULL, NULL, NULL);
		
		hCheck[0] = CreateWindow("button", "<<<<<", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 50, 400, 50, 50, hWnd, (HMENU)IDC_CHECK1, g_hinst, NULL);
		hCheck[1] = CreateWindow("button", ">>>>>", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,530, 400, 50, 50, hWnd, (HMENU)IDC_CHECK2, g_hinst, NULL);
		Comfirm = CreateWindow("button", "적용", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 700, 450, 140, 20, hWnd, (HMENU)IDC_CHECK3, g_hinst, NULL);
		Success = CreateWindow("button", "완성", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 700, 480, 140, 20, hWnd, (HMENU)IDC_CHECK6, g_hinst, NULL);

	
		CLOSE = CreateWindow("button", "종료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 820, 700, 140, 30, hWnd, (HMENU)IDC_CHECK4, g_hinst, NULL);
		SelectList = CreateWindow("listbox", NULL , WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD, 700, 300, 140, 130, hWnd, (HMENU)IDC_CHECK5, g_hinst, NULL);

		for (int i = 0; i < 7; ++i) {
			SendMessage(SelectList, LB_ADDSTRING,0,(LPARAM)Items[i]);
		}
		//	hList = CreateWindow("check", "테스트중", WS_CHILD | WS_VISIBLE | BS_BOX
		

		ReleaseDC(hWnd, hdc);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			
		case IDC_CHECK1:
			if(Select > 0)
			Select--;
			break;

		case IDC_CHECK2:
			if(Select < 6)
				Select++;
			break;

		case IDC_CHECK3:
			picture[Select].index = selection;
			Release.index = -1;
			break;

		case IDC_CHECK5:
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				selection = SendMessage(SelectList, LB_GETCURSEL, 0, 0);
				if(Complete == FALSE)
					Release.index = SendMessage(SelectList, LB_GETCURSEL, 0, 0);
			}
			break;

		case IDC_CHECK6:
			hButton = GetDlgItem(hWnd, IDC_CHECK3);
			EnableWindow(hButton, FALSE); // 시작버튼 비활성화
			Complete = TRUE;
			break;
	
		case IDC_CHECK4:
			PostQuitMessage(0);
			break;

		}
		InvalidateRect(hWnd, NULL,FALSE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		
		if (check == 0)
			board = CreateWindowEx(WS_EX_WINDOWEDGE, "ChildClass", NULL, WS_CHILD | WS_VISIBLE | WS_THICKFRAME, 110, 200, 400, 400, hWnd, (HMENU)IDC_MAP, g_hinst, NULL);
		check++;
	
		wsprintf(text, TEXT("%d번째 이미지"), Select + 1);
		TextOut(hdc, 260, 150, text, strlen(text));

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}


LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	HBITMAP Block[7];
	static HBITMAP hbit, oldBit1, oldBit2;;

	static int sizeX, sizeY;

	switch (uMsg) {

	case WM_CREATE:

		break;

	case WM_TIMER:


		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		memdc2 = CreateCompatibleDC(memdc);

		hbit = CreateCompatibleBitmap(hdc, 400, 400);

		Block[0] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		Block[1] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP2));
		Block[2] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP3));
		Block[3] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP4));
		Block[4] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP5));
		Block[5] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP6));
		Block[6] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP7));

		oldBit1 = (HBITMAP)SelectObject(memdc, hbit);
		oldBit2 = (HBITMAP)SelectObject(memdc2, NULL);

		(HBITMAP)SelectObject(memdc2, Block[picture[Select].index]);
		if (picture[Select].index == 0) {
			sizeX = 463;
			sizeY = 656;
		}
		else if (picture[Select].index == 1) {
			sizeX = 280;
			sizeY = 280;
		}
		else if (picture[Select].index == 2) {
			sizeX = 1200;
			sizeY = 800;
		}
		else if (picture[Select].index == 3) {
			sizeX = 480;
			sizeY = 480;
		}
		else if (picture[Select].index == 4) {
			sizeX = 480;
			sizeY = 360;
		}
		else if (picture[Select].index == 5) {
			sizeX = 258;
			sizeY = 195;
		}
		else if (picture[Select].index == 6) {
			sizeX = 320;
			sizeY = 316;
		}

		StretchBlt(memdc, 0, 0, 400, 400, memdc2, 0, 0, sizeX, sizeY, SRCCOPY);

		(HBITMAP)SelectObject(memdc2, Block[Release.index]);
		if (Release.index == 0) {
			sizeX = 463;
			sizeY = 656;
		}
		else if (Release.index == 1) {
			sizeX = 280;
			sizeY = 280;
		}
		else if (Release.index == 2) {
			sizeX = 1200;
			sizeY = 800;
		}
		else if (Release.index == 3) {
			sizeX = 480;
			sizeY = 480;
		}
		else if (Release.index == 4) {
			sizeX = 480;
			sizeY = 360;
		}
		else if (Release.index == 5) {
			sizeX = 258;
			sizeY = 195;
		}
		else if (Release.index == 6) {
			sizeX = 320;
			sizeY = 316;
		}

		StretchBlt(memdc, 0, 0, 400, 400, memdc2, 0, 0, sizeX, sizeY, SRCCOPY);

		BitBlt(hdc, 0, 0, 400, 400, memdc, 0, 0, SRCCOPY);

		for (int i = 0; i < 7; ++i)
			DeleteObject(Block[i]);

		DeleteObject(hbit);
		DeleteDC(memdc);
		DeleteDC(memdc2);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
