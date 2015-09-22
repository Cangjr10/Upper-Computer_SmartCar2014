// ImageProcess.h : ImageProcess DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif
#include "resource.h"		// 主符号
#include  <vector>
using std::vector;
//保存图像信息类
class Image_class
{
public:
	Image_class(UINT w=0,UINT h=0)//默认vector数组初始化为0字节长
	{
		width=w;
		height=h;
	}
	Image_class(const Image_class & _image)//复制构造函数
	{
		width = _image.width;
		height = _image.height;
		image_data = _image.image_data;
		image_color = _image.image_color;
	}
	UINT getsize()//获取数据大小
	{
		ASSERT(image_data.size() == width*height);
		return image_data.size();
	}
public:
	vector<BYTE> image_data;
	UINT width;
	UINT height;
	vector<BYTE> image_color;//用于返回需要显示的颜色信息

};

// CImageProcessApp
// 有关此类实现的信息，请参阅 ImageProcess.cpp
//

class CImageProcessApp : public CWinApp
{
public:
	CImageProcessApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
//extern "C" __declspec(dllexport) void ImageProcess(const Image_class image_input,Image_class &image_output);
extern "C" void ImageProcess( const Image_class &_image_input,Image_class &_image_output);