#include "Scene.h"

#define VALIDATE(a) if (!a) return (false)
CScene* CScene::s_pScene = 0;

CScene::CScene()
	:m_hAppInstance(0)
	, m_pBackBuffer(0)
	, m_pPathfinder(0)
{

}

CScene::~CScene()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_pPathfinder;
	m_pPathfinder = 0;

	for (unsigned int i = 0; i < vecTiles.size(); i++)
	{
		delete vecTiles[i];
		vecTiles[i] = 0;
	}

	for (unsigned int i = 0; i < vecLines.size(); i++)
	{
		delete vecLines[i];
		vecLines[i] = 0;
	}

}

void CScene::DestroyInstance()
{
delete s_pScene;
s_pScene = 0;
}

CScene& CScene::GetInstance()
{
	if (s_pScene == 0)
	{
		s_pScene = new CScene();
	}

	return (*s_pScene);
}


bool CScene::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight)
{
	m_hAppInstance = _hInstance;
	m_hMainWindow = _hWnd;

	m_pBackBuffer = new CBackBuffer();
	VALIDATE(m_pBackBuffer->Initialise(_hWnd, _iWidth, _iHeight));
	m_pPathfinder = new CPathfind();

	int _X = 0, _Y = 0;
	int Type = IDB_OBSTACLE;
	for (unsigned int i = 0; i < MAX_HEIGHT; i++)
	{
		for (unsigned int j = 0; j < MAX_WIDTH; j++)
		{
			if (m_pPathfinder->SearchSpace[i][j]->_Type == GOAL)
				Type = IDB_GOAL;
			else if (m_pPathfinder->SearchSpace[i][j]->_Type == UNWALKABLE)
				Type = IDB_OBSTACLE;
			else if (m_pPathfinder->SearchSpace[i][j]->_Type == WALKABLE)
				Type = IDB_STANDARD;
			else if (m_pPathfinder->SearchSpace[i][j]->_Type == START)
				Type = IDB_START;
			CTile* tile = new CTile(Type, _X, _Y, 128, 128);
			vecTiles.push_back(tile);
			_X += 128;
		}
		_Y += 128;
		_X = 0;
	}


	HINSTANCE hInstance = m_hAppInstance;
	if (!spriteHDC)
		spriteHDC = CreateCompatibleDC(NULL);
	for (unsigned int i = 0; i < vecTiles.size(); i++)
	{
		vecTiles[i]->m_hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(vecTiles[i]->_iTileType));

		GetObject(vecTiles[i]->m_hBitmap, sizeof(BITMAP), &vecTiles[i]->m_Bitmap);
	}
	return true;
}

void CScene::SetTiles()
{
	HINSTANCE hInstance = m_hAppInstance;
	if (!spriteHDC)
		spriteHDC = CreateCompatibleDC(NULL);
	for (unsigned int i = 0; i < vecTiles.size(); i++)
	{
		vecTiles[i]->m_hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(vecTiles[i]->_iTileType));
		GetObject(vecTiles[i]->m_hBitmap, sizeof(BITMAP), &vecTiles[i]->m_Bitmap);
	}
}

void CScene::RunSearch(Node* start)
{
	m_pPathfinder->startSearch(start);
	Node* end = m_pPathfinder->closedList.back();
	while (end->_Type != START)
	{
		Line* line = new Line(RGB(200, 30,50), end->_iX, end->_iY, end->parentNode->_iX, end->parentNode->_iY);
		vecLines.push_back(line);
		end = end->parentNode;
	}
}

void CScene::Reset()
{
	for (unsigned int i = 1; i < MAX_HEIGHT - 1; i++)
	{
		for (unsigned int j = 1; j < MAX_WIDTH - 1; j++)
		{
			m_pPathfinder->SearchSpace[i][j]->_Type = WALKABLE;
		}
	}

	for (unsigned int i = 0; i < MAX_HEIGHT; i++)
	{
		for (unsigned int j = 0; j < MAX_WIDTH; j++)
		{
			if(m_pPathfinder->SearchSpace[i][j]->_Type == WALKABLE)
			{
				vecTiles[(i*MAX_WIDTH)+j]->_iTileType = IDB_STANDARD;
			}
		}
	}

	for (unsigned int i = 0; i < vecLines.size(); i++)
	{
		delete vecLines[i];
		vecLines[i] -= 0;
	}

	vecLines.clear();
	SetTiles();
	m_pPathfinder->ResetSearchSpace();
}

void CScene::Draw()
{
	m_pBackBuffer->Clear();
	int _iWidth, _iHeight, _iX, _iY;
	for (unsigned int i = 0; i < vecTiles.size(); i++)
	{
		_iWidth = vecTiles[i]->_iWidth;
		_iHeight = vecTiles[i]->_iHeight;
		_iX = vecTiles[i]->_iX;
		_iY = vecTiles[i]->_iY;
		HGDIOBJ hOldObj = SelectObject(spriteHDC, vecTiles[i]->m_hBitmap);
		BitBlt(m_pBackBuffer->GetBFDC(), _iX, _iY, _iWidth, _iHeight, spriteHDC, 0, 0, SRCCOPY);
		SelectObject(spriteHDC, hOldObj);
	}
	for (unsigned int i = 0; i < vecLines.size(); i++)
	{
		HPEN linePen = CreatePen(PS_DASH, 2, vecLines[i]->m_color);
		HPEN old_Pen = static_cast<HPEN>(SelectObject(m_pBackBuffer->GetBFDC(), linePen));
		MoveToEx(m_pBackBuffer->GetBFDC(), ((vecLines[i]->m_iStartX*128)+64), ((vecLines[i]->m_iStartY*128)+64), NULL);
		LineTo(m_pBackBuffer->GetBFDC(), ((vecLines[i]->m_iEndX*128)+64), ((vecLines[i]->m_iEndY)*128)+64);
		SelectObject(m_pBackBuffer->GetBFDC(), old_Pen);
		DeleteObject(linePen);
	}

	TextOut(m_pBackBuffer->GetBFDC(), 10, 5, L"Left click to place Start, Goal and Obstacles", 46);
	TextOut(m_pBackBuffer->GetBFDC(), 10, 25, L"Right click after placing Obstacles to begin Pathfind", 54);

	m_pBackBuffer->Present();
}