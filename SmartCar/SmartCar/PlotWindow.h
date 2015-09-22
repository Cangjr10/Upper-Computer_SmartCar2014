#pragma once
#include <cassert>
//#include "stdafx.h"
class CPlotWindow : public CObject
{
	DECLARE_SERIAL (CPlotWindow)
public:
	CPlotWindow()
	{
		x_range=17;
		y1=0;
		y2=255;
		X1=0;
		X2=0;
		Y1=0;
		Y2=0;
		WindowShowFlag = FALSE;
	}
	void SetRect(CRect &rect)//����ʵ����ʾ��������
	{
		X1 = rect.left;
		X2 = rect.right;
		Y1 = rect.bottom;
		Y2 = rect.top;
	}
	void SetXrange(UINT x){x_range = x;}
	UINT GetXrange(){return x_range;}
	void SetYrange(long _y1, long _y2){y1=_y1;y2=_y2;}
	BOOL GetShowFlag(){return WindowShowFlag;}
	void SetShowFlag(BOOL flag){WindowShowFlag = flag;}
	void Serialization (CArchive & ar);
	void SetBackColor(COLORREF color){backcolor = color;}
	void DrawAxis(CDC &MemDC);
	void PlotLine(CDC &MemDC,vector<float>& _plotdata,COLORREF color);
	long GetY1(){return y1;}
	long GetY2(){return y2;}
private:
	int X1,X2;//��������
	int Y1,Y2;//��������
	int Axis_X1,Axis_X2;//���ݻ�ͼ������������
	int Axis_Y1,Axis_Y2;//���ݻ�ͼ������������
	UINT x_range;//������
	long y1,y2;//������
	BOOL WindowShowFlag;
	COLORREF backcolor;
};