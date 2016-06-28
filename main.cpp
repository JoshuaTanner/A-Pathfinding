//Library Includes
#include <windows.h>
#include <windowsx.h>
#include <crtdbg.h>


//Local Includes
#include "Pathfind.h"
#include "Scene.h"

#define WINDOW_CLASS_NAME L"Pathfinding"
#define START_SELECTED 1
#define END_SELECTED 2
#define WALL_SELECTED 3
#define NONE_SELECTED 4

CScene* rScene;
int SelectedMode;
int StartX, StartY;



LRESULT CALLBACK
WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	int XCoord, YCoord;

	switch (_uiMsg)
	{
	case WM_CREATE:
	{
		SelectedMode = START_SELECTED;
		break;
		return(0);
	}
	case WM_COMMAND:
	{
		switch (LOWORD(_wParam))
		{
		case ID_FILE_RESESTALL:
		{
			rScene->GetInstance().Reset();
			SelectedMode = START_SELECTED;

			break;
			return(0);
		}
		case ID_FILE_EXIT:
		{
			rScene->DestroyInstance();
			PostQuitMessage(0);
			return TRUE;
			return(0);
			break;
		}

		}
		break;
		return(0);
	}



	case WM_LBUTTONDOWN:
	{
		XCoord = (static_cast<int>(LOWORD(_lParam)) / 128);
		YCoord = (static_cast<int>(HIWORD(_lParam)) / 128);
		Node* node = rScene->GetInstance().m_pPathfinder->SearchSpace[YCoord][XCoord];

		if (SelectedMode == START_SELECTED && node->_Type != UNWALKABLE)
		{
			node->_Type = START;
			rScene->GetInstance().vecTiles[(YCoord * 10) + XCoord]->_iTileType = IDB_START;
			StartX = XCoord;
			StartY = YCoord;
			SelectedMode = END_SELECTED;
		}
		else if (SelectedMode == END_SELECTED && node->_Type != UNWALKABLE)
		{
			if (node->_Type != START)
			{
				node->_Type = GOAL;
				rScene->GetInstance().vecTiles[(YCoord * 10) + XCoord]->_iTileType = IDB_GOAL;
				SelectedMode = WALL_SELECTED;
			}
		}
		else if (SelectedMode == WALL_SELECTED)
		{
			if (node->_Type != START && node->_Type != GOAL)
			{
				node->_Type = UNWALKABLE;
				rScene->GetInstance().vecTiles[(YCoord * 10) + XCoord]->_iTileType = IDB_OBSTACLE;
			}
		}
		rScene->GetInstance().SetTiles();
		break;
		return(0);
	}
	case WM_RBUTTONDOWN:
	{
		if (rScene->GetInstance().vecLines.empty() && SelectedMode == WALL_SELECTED)
		{
			rScene->GetInstance().RunSearch(rScene->GetInstance().m_pPathfinder->SearchSpace[StartY][StartX]);
			SelectedMode = NONE_SELECTED;

		}

		break;
		return(0);
	}

	case WM_DESTROY:
	{
		rScene->DestroyInstance();
		PostQuitMessage(0);
		return TRUE;
		return(0);
	}
	break;

	default:break;
	}

	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}



HWND
CreateAndRegisterWindow(HINSTANCE _hInstance, int _iWidth, int _iHeight, const wchar_t* _pcTitle)
{
	HBRUSH WhiteScreen = CreateSolidBrush(RGB(0, 0, 0));
	WNDCLASSEX winclass;
	winclass.hInstance = _hInstance;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_HAND);
	winclass.hbrBackground = static_cast<HBRUSH> (GetStockObject(NULL_BRUSH));
	winclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);



	if (!RegisterClassEx(&winclass))
	{
		// Failed to register.
		return (0);
	}

	HWND hwnd;
	hwnd = CreateWindowEx(NULL,
		WINDOW_CLASS_NAME,
		_pcTitle,
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		_iWidth, _iHeight,
		NULL,
		NULL,
		_hInstance,
		NULL);

	if (!hwnd)
	{
		// Failed to create.
		return (0);
	}

	return (hwnd);
}

int WINAPI
WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdline, int _iCmdshow)
{
	MSG msg;
	RECT _rect;
	ZeroMemory(&msg, sizeof(MSG));
	const int kiWidth = 1295;
	const int kiHeight = 950;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	HWND hwnd = CreateAndRegisterWindow(_hInstance, kiWidth, kiHeight, L"Pathfinding");
	CScene* rScene;

	rScene = &CScene::GetInstance();
	GetClientRect(hwnd, &_rect);

	if (!rScene->Initialise(_hInstance, hwnd, kiWidth, kiHeight))
		return 0;


	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			rScene->Draw();
		}
	}


	return (static_cast<int>(msg.wParam));
}
