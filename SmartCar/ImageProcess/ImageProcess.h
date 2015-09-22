// ImageProcess.h : ImageProcess DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif
#include "resource.h"		// ������
#include  <vector>
using std::vector;
//����ͼ����Ϣ��
class Image_class
{
public:
	Image_class(UINT w=0,UINT h=0)//Ĭ��vector�����ʼ��Ϊ0�ֽڳ�
	{
		width=w;
		height=h;
	}
	Image_class(const Image_class & _image)//���ƹ��캯��
	{
		width = _image.width;
		height = _image.height;
		image_data = _image.image_data;
		image_color = _image.image_color;
	}
	UINT getsize()//��ȡ���ݴ�С
	{
		ASSERT(image_data.size() == width*height);
		return image_data.size();
	}
public:
	vector<BYTE> image_data;
	UINT width;
	UINT height;
	vector<BYTE> image_color;//���ڷ�����Ҫ��ʾ����ɫ��Ϣ

};

// CImageProcessApp
// �йش���ʵ�ֵ���Ϣ������� ImageProcess.cpp
//

class CImageProcessApp : public CWinApp
{
public:
	CImageProcessApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
//extern "C" __declspec(dllexport) void ImageProcess(const Image_class image_input,Image_class &image_output);
extern "C" void ImageProcess( const Image_class &_image_input,Image_class &_image_output);