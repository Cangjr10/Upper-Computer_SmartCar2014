#include "stdafx.h"
#include "PlotWindow.h"
//#include <cassert>
#define Xoffset 20
#define Yoffset 25
#define arrowsize 5
void CPlotWindow:: DrawAxis(CDC &MemDC)
{
	CString str;
// 	CBrush br,*oldbr;
// 	br.CreateSolidBrush(backcolor);
// 	oldbr = MemDC.SelectObject(&br);
// // 	MemDC.Rectangle(X1,Y2,X2,Y1);
// 	MemDC.SelectObject(oldbr);
// 	br.DeleteObject();
	MemDC.FillSolidRect(X1,Y2,X2,Y1,backcolor);
//	MemDC.Rectangle(X1,Y2,X2,Y1);
//	MemDC.SetBkColor(backcolor);
//	MemDC.SelectPalette()
	COLORREF color_text=RGB(255,255,255);
	CPen pen(PS_DASH,1,color_text);
	CPen* pOldPen = MemDC.SelectObject(&pen);
	MemDC.SetTextColor(color_text);
	//Draw X Axis
	MemDC.MoveTo(X1,Y2-2);
	MemDC.LineTo(X2,Y2-2);
	CPen pen1(PS_SOLID,1,color_text);
	MemDC.SelectObject(&pen1);
	if(y1<0 && y2>0)
	{
		int b;
		b= static_cast<int>(((Y1-10)*y2-(Y2+20)*y1)/(y2-y1));
		//Draw Y Axis
		MemDC.MoveTo(X1+Xoffset,Y1-10);
		MemDC.LineTo(X1+Xoffset,Y2+2);
		//Draw Arrow;
		MemDC.MoveTo(X1+Xoffset-arrowsize,Y2+2+arrowsize);
		MemDC.LineTo(X1+Xoffset,Y2+2);
		MemDC.LineTo(X1+Xoffset+arrowsize,Y2+2+arrowsize);
		//Draw X Axis
		MemDC.MoveTo(X1+Xoffset,b);
		MemDC.LineTo(X2-2,b);
		//Draw Arrow;
		MemDC.MoveTo(X2-2-arrowsize,b+arrowsize);
		MemDC.LineTo(X2-2,b);
		MemDC.LineTo(X2-2-arrowsize,b-arrowsize);
		CRect rect_text(X1,b-15,X1+Xoffset-4,b+5);
		str.Format(_T("%ld"),0);
		MemDC.DrawText(str,rect_text,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		CRect rect_text1(X1+Xoffset+10,Y2+10,X1+Xoffset+100,Y2+30);
		str.Format(_T("%ld"),y2);
		MemDC.DrawText(str,rect_text1,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		CRect rect_text2(X1+Xoffset+10,Y1-10,X1+Xoffset+100,Y1-30);
		str.Format(_T("%ld"),y1);
		MemDC.DrawText(str,rect_text2,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		str.Format(_T("%d"),x_range-1);
		CRect rect_text3(X2-100,b+4,X2-10,b+24);
		MemDC.DrawText(str,rect_text3,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		MemDC.MoveTo(X1+Xoffset,Y2+20);
		MemDC.LineTo(X1+Xoffset+6,Y2+20);
		MemDC.MoveTo(X1+Xoffset,Y1-15);
		MemDC.LineTo(X1+Xoffset+6,Y1-15);
		MemDC.MoveTo(X2-20,b);
		MemDC.LineTo(X2-20,b-6);
		Axis_X1 = X1+Xoffset;
		Axis_X2 = X2-20;
		Axis_Y1 = Y1-15;
		Axis_Y2 = Y2+20;
	}
	else
	{
		//X轴在最下面
		CRect rect_text1(X1+Xoffset+10,Y2+10,X1+Xoffset+200,Y2+30);
		str.Format(_T("%ld"),y2);
		MemDC.DrawText(str,rect_text1,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		CRect rect_text2(X1+Xoffset+10,Y1-Yoffset,X1+Xoffset+200,Y1-Yoffset+20);
		str.Format(_T("%ld"),y1);
		MemDC.DrawText(str,rect_text2,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		//Draw Y Axis
		MemDC.MoveTo(X1+Xoffset,Y1-Yoffset);
		MemDC.LineTo(X1+Xoffset,Y2+2);
		//Draw Arrow;
		MemDC.MoveTo(X1+Xoffset-arrowsize,Y2+2+arrowsize);
		MemDC.LineTo(X1+Xoffset,Y2+2);
		MemDC.LineTo(X1+Xoffset+arrowsize,Y2+2+arrowsize);
		//Draw X Axis
		MemDC.LineTo(X1+Xoffset+arrowsize,Y2+2+arrowsize);
		MemDC.MoveTo(X1+Xoffset,Y1-Yoffset);
		MemDC.LineTo(X2-2,Y1-Yoffset);
		//Draw Arrow;
		MemDC.MoveTo(X2-2-arrowsize,Y1-Yoffset+arrowsize);
		MemDC.LineTo(X2-2,Y1-Yoffset);
		MemDC.LineTo(X2-2-arrowsize,Y1-Yoffset-arrowsize);
		str.Format(_T("%d"),x_range-1);
		CRect rect_text3(X2-100,Y1-Yoffset+4,X2-10,Y1-Yoffset+24);
		MemDC.DrawText(str,rect_text3,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		MemDC.MoveTo(X1+Xoffset,Y2+20);
		MemDC.LineTo(X1+Xoffset+6,Y2+20);
		MemDC.MoveTo(X2-20,Y1-Yoffset);
		MemDC.LineTo(X2-20,Y1-Yoffset-6);
		Axis_X1 = X1+Xoffset;
		Axis_X2 = X2-20;
		Axis_Y1 = Y1-Yoffset;
		Axis_Y2 = Y2+20;
	}
	MemDC.SelectObject(pOldPen);
}
void CPlotWindow::PlotLine(CDC &MemDC,vector<float>& _plotdata,COLORREF color)
{
	//DrawAxis(pDC);
	for(UINT i=0;i<_plotdata.size();i++)
	{
		if (_plotdata[i]>y2)
			_plotdata[i]=y2;
		if(_plotdata[i]<y1)
			_plotdata[i]=y1;
	}
	CPen pen(PS_SOLID,2,color);
	CPen* pOldPen = MemDC.SelectObject(&pen);
	float k = static_cast<float>((Axis_Y2-Axis_Y1))/(y2-y1);
	float b = static_cast<float>(Axis_Y1*y2-Axis_Y2*y1)/(y2-y1);
	//MemDC.FillSolidRect(X1,Y2,X2-X1,Y1-Y2,RGB(200,239,240));
 		if (_plotdata.size()>0)
			MemDC.MoveTo(Axis_X1,static_cast<int>(k*_plotdata[0]+b));
		for (UINT j=1;j<_plotdata.size();j++)
		{
			MemDC.LineTo(static_cast<int>(j*(Axis_X2-Axis_X1)/(x_range-1))+Axis_X1,static_cast<int>(k*_plotdata[j]+b));
		}
	MemDC.SelectObject(pOldPen);
}

IMPLEMENT_SERIAL(CPlotWindow, CObject, 1)
void CPlotWindow ::Serialization (CArchive & ar)
{
	CObject::Serialize (ar);
	if (ar.IsStoring ())
	{
		ar<< x_range;//横坐标
		ar<< y1;
		ar<<y2;//纵坐标
		ar<< WindowShowFlag;
	}
	else
	{
		ar>> x_range;//横坐标
		ar>> y1;
		ar>>y2;//纵坐标
		ar>> WindowShowFlag;
	}
	
}