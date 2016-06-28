/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	backbuffer.cpp
Description	:	backbuffer implementation file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

//Include header
#include "BackBuffer.h"


/***********************
* Constructor: constructs backbuffer, sets pointers to 0
* @author: Joshua Tanner
* @parameter: none
* @return:
********************/
CBackBuffer::CBackBuffer()
	: m_hWnd(0)
	, m_hDC(0)
	, m_hSurface(0)
	, m_hOldObject(0)
	, m_iWidth(0)
	, m_iHeight(0)
{

}

/***********************
* Destructor: destroy object and delete objects
* @author: Joshua Tanner
* @parameter: none
* @return:
********************/
CBackBuffer::~CBackBuffer()
{
	//Select the bitmap handle into the device context
	SelectObject(m_hDC, m_hOldObject);
	//Delete objects
	DeleteObject(m_hSurface);
	DeleteObject(m_hDC);
}

/***********************
* Initialise: set member variables
* @author: Joshua Tanner
* @parameter: handle to window, with & height of window
* @return: bool
********************/
bool CBackBuffer::Initialise(HWND _hWnd, int _iWidth, int _iHeight)
{
	m_hWnd = _hWnd;

	m_iWidth = _iWidth;
	m_iHeight = _iHeight;

	//Get device context of window
	HDC hWindowDC = ::GetDC(m_hWnd);
	m_hDC = CreateCompatibleDC(hWindowDC);
	m_hSurface = CreateCompatibleBitmap(hWindowDC, m_iWidth, m_iHeight);
	ReleaseDC(m_hWnd, hWindowDC);

	//Loading into device context
	m_hOldObject = static_cast<HBITMAP>(SelectObject(m_hDC, m_hSurface));
	HBRUSH brushWhite = static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH));
	HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(m_hDC, brushWhite));
	Rectangle(m_hDC, 0, 0, m_iWidth, m_iHeight);
	SelectObject(m_hDC, oldBrush);
	return (true);
}

/***********************
* Clear: clear the device context
* @author: Joshua Tanner
* @parameter: none
* @return: void
********************/
void CBackBuffer::Clear()
{
	//Back buffer is cleared with black brush every frame
	HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(GetBFDC(), GetStockObject(LTGRAY_BRUSH)));
	Rectangle(GetBFDC(), 0, 0, GetWidth(), GetHeight());
	SelectObject(GetBFDC(), hOldBrush);
}

/***********************
* GetBFDC: returns backbuffer's device context
* @author: Joshua Tanner
* @parameter: none
* @return: HDC
********************/
HDC CBackBuffer::GetBFDC() const
{
	return (m_hDC);
}

/***********************
* GetWidth: return width of backbuffer
* @author: Joshua Tanner
* @parameter: none
* @return: int
********************/
int CBackBuffer::GetWidth() const
{
	return (m_iWidth);
}

/***********************
* GetHeight: return height of backbuffer
* @author: Joshua Tanner
* @parameter: none
* @return: int
********************/
int CBackBuffer::GetHeight() const
{
	return (m_iHeight);
}

/***********************
* Present: bitblt contents of DC
* @author: Joshua Tanner
* @parameter: none
* @return: void
********************/
void CBackBuffer::Present()
{
	//Bit blting from back buffer device context
	HDC hWndDC = ::GetDC(m_hWnd);
	BitBlt(hWndDC, 0, 0, m_iWidth, m_iHeight, m_hDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hWndDC);
}

