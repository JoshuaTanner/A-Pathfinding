#pragma once
#include <Windows.h>




class CTile
{
	
	
public:

	CTile() {}
	CTile(int iTileType, int X, int Y, int W, int H)
		:_iTileType(iTileType), _iX(X), _iY(Y), _iWidth(W), _iHeight(H)
	{}
	~CTile() {}
	HBITMAP m_hBitmap;
	BITMAP m_Bitmap;
	int _iTileType;
	int _iX;
	int _iY;
	int _iWidth;
	int _iHeight;
	
};

