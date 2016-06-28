/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	scene.h
Description	:	scene header file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#pragma once
#include <Windows.h>
#include <memory>
#include "Pathfind.h"
#include "BackBuffer.h"
#include "resource.h"
#include "Tile.h"

//line struct
struct Line {
	Line();
	Line(COLORREF color, int StartX, int StartY, int EndX, int EndY)
		:m_color(color), m_iStartX(StartX), m_iStartY(StartY),
		m_iEndX(EndX), m_iEndY(EndY){}
	COLORREF m_color;
	HPEN m_Pen;
	int m_iStartX;
	int m_iEndX;
	int m_iStartY;
	int m_iEndY;
};


class CScene
{
private:
	CScene();
	~CScene();
	CScene& operator= (const CScene& _kr);

	
	
	std::list<Node*> path;
	

protected:
	HINSTANCE m_hAppInstance;
	HWND m_hMainWindow;
	HDC spriteHDC;

	CBackBuffer* m_pBackBuffer;
	
	static CScene* s_pScene;
	


public:
	int Search[10][10];
	CPathfind* m_pPathfinder;
	std::vector<CTile*> vecTiles;
	std::vector<Line*> vecLines;
	void RunSearch(Node* start);
	void SetTiles();
	static CScene& GetInstance();
	bool Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight);
	void Draw();
	void Reset();
	void DestroyInstance();


	//std::vector<Line> vecLines;
};

