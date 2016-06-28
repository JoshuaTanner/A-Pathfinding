/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	Backbuffer.h
Description	:	Backbuffer header file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#pragma once

// Library Includes
#include <Windows.h>


class CBackBuffer
{
	// Member Functions
public:
	CBackBuffer();
	~CBackBuffer();
	bool Initialise(HWND _hWnd, int _iWidth, int _iHeight);
	HDC GetBFDC() const;  //const so member variables aren't changed
	int GetHeight() const;
	int GetWidth() const;
	void Clear();
	void Present();

protected:

private:
	CBackBuffer(const CBackBuffer& _kr);
	CBackBuffer& operator= (const CBackBuffer& _kr);

	// Member Variables
public:

protected:
	HWND m_hWnd;
	HDC m_hDC;
	HBITMAP m_hSurface;
	HBITMAP m_hOldObject;

	int m_iWidth;
	int m_iHeight;

private:

};


