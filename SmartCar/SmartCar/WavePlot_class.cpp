#include "stdafx.h"
#include "WavePlot_class.h"
#include <cassert>
//函数作用：解析起始标志
//思路：打开一扇门需要两把钥匙，且这两把钥匙是连续的，逐次更换，知道拿到正确的钥匙，即正确的其实标志。
BOOL WavePlot::IsDoorOpen(BYTE NewReceivedData)
{
	assert(!DoorOpen && Key.size()<3);//门没开再去开门，若门开了就别再开了
	if (Key.size()==KeyNumber)//已经有足够的钥匙，但打不开，需要更换一把钥匙
	{
		Key[0]=Key[1];
		Key[1]=NewReceivedData;
		if (Key[0]==RightKey[0] && Key[1]==RightKey[1])//钥匙匹配
		{
			DoorOpen = TRUE;
			return TRUE;
		}
		else
			return FALSE;
	}
	else if (Key.size()<KeyNumber)//钥匙数目不够，继续索取钥匙
	{
		Key.push_back(NewReceivedData);
		if (Key.size()==KeyNumber)//加了一把钥匙之后，钥匙数目足够，试下是否匹配
		{
			if (Key[0]==RightKey[0] && Key[1]==RightKey[1])//钥匙匹配
			{
				DoorOpen = TRUE;
				return TRUE;//钥匙正确，返回
			}
			else
				return FALSE;//钥匙不正确
		}
		else
			return FALSE;//加了一把钥匙钥匙，数目依旧不够，返回
	}
	else
		return FALSE;
}

BOOL WavePlot::decode(BYTE NewReceivedData)
{
	assert(DoorOpen);//确保门市开着的，进行数据接受
	ByteStream.push_back(NewReceivedData);
	if (ByteStream.size()==DataNumber*BytesPerData)//数据接受完全
	{
		DoorOpen = FALSE;//数据已经全部接受完毕，关门开始整理数据
		switch(data_style)
		{
		case s_BYTE:
			for (UINT i=0;i<DataNumber;i++)
				plotdata_byte.push_back(ByteStream[i]);
			break;
		case s_INT16:
			for (UINT i=0;i<DataNumber;i++)
			{
				INT16 a = 0;
				char* hehe = (char*)&a;
				*hehe = ByteStream[2*i+0];
				*(hehe+1)=ByteStream[2*i+1];
				plotdata_int16.push_back(a);
			}
			break;
		case s_UINT16:
			for (UINT i=0;i<DataNumber;i++)
			{
				UINT16 a = 0;
				char* hehe = (char*)&a;
				*hehe = ByteStream[2*i+0];
				*(hehe+1)=ByteStream[2*i+1];
				plotdata_uint16.push_back(a);
			}
			break;
		case s_FLOAT:
			for (UINT i=0;i<DataNumber;i++)
			{
				float a = 0;
				char* hehe = (char*)&a;
				*hehe = ByteStream[4*i+0];
				*(hehe+1)=ByteStream[4*i+1];
				*(hehe+2) = ByteStream[4*i+2];
				*(hehe+3)=ByteStream[4*i+3];
				plotdata_float.push_back(a);
			}
			break;
		default:
			break;
		}
		ByteStream.resize(0);//处理完数据将接收缓冲区清零
		return TRUE;
	}
	return FALSE;
}
//不用将数据全部拿出，只需拿出需要显示的x_range个数即可
// void WavePlot::getdata(vector<BYTE> &_plotdata_byte)
// {
// 	assert(data_style == s_BYTE);
// 	if (x_range < plotdata_byte.size())//数据比显示的要多，取有限数目显示
// 	{
// 		_plotdata_byte.resize(x_range);
// 		for (UINT i=0;i<_plotdata_byte.size();i++)
// 		{
// 			_plotdata_byte[i]=plotdata_byte[plotdata_byte.size()-x_range+i];
// 		}
// 	}
// 	else//数据数目较少
// 	{
// 		_plotdata_byte.resize(plotdata_byte.size());
// 		for (UINT i=0;i<_plotdata_byte.size();i++)
// 		{
// 			_plotdata_byte[i]=plotdata_byte[i];
// 		}
// 	}
// }
// void WavePlot::getdata(vector<INT16> &_plotdata_int16)
// {
// 	assert(data_style == s_INT16);
// 	
// 	if (x_range<plotdata_int16.size())//数据比显示的要多，取有限数目显示
// 	{
// 		_plotdata_int16.resize(x_range);//不足用0补足
// 		for (UINT i=0;i<_plotdata_int16.size();i++)
// 		{
// 			_plotdata_int16[i]=plotdata_int16[plotdata_int16.size()-x_range+i];
// 		}
// 	}
// 	else//数据数目较少
// 	{
// 		_plotdata_int16.resize(plotdata_int16.size());//不足用0补足
// 		for (UINT i=0;i<_plotdata_int16.size();i++)
// 		{
// 			_plotdata_int16[i]=plotdata_int16[i];
// 		}
// 	}
// }
// void WavePlot::getdata(vector<UINT16> &_plotdata_uint16)
// {
// 	assert(data_style == s_UINT16);
// 	if (x_range<plotdata_uint16.size())//数据比显示的要多，取有限数目显示
// 	{
// 		_plotdata_uint16.resize(x_range);//只取x_range个
// 		for (UINT i=0;i<_plotdata_uint16.size();i++)
// 		{
// 			_plotdata_uint16[i]=plotdata_uint16[plotdata_uint16.size()-x_range+i];
// 		}
// 	}
// 	else//数据数目较少
// 	{
// 		_plotdata_uint16.resize(plotdata_uint16.size());
// 		for (UINT i=0;i<_plotdata_uint16.size();i++)
// 		{
// 			_plotdata_uint16[i]=plotdata_uint16[i];
// 		}
// 	}
// 
// }
// void WavePlot::getdata(vector<float> &_plotdata_float)
// {
// 	assert(data_style == s_FLOAT);
// 	if (x_range<plotdata_float.size())//数据比显示的要多，取有限数目显示
// 	{
// 		_plotdata_float.resize(x_range);
// 		for (UINT i=0;i<_plotdata_float.size();i++)
// 		{
// 			_plotdata_float[i]=plotdata_float[plotdata_float.size()-x_range+i];
// 		}
// 	}
// 	else//数据数目较少
// 	{
// 		_plotdata_float.resize(plotdata_float.size());
// 		for (UINT i=0;i<_plotdata_float.size();i++)
// 		{
// 			_plotdata_float[i]=plotdata_float[i];
// 		}
// 	}
// }
	/************************************************************************/
	/* 获取可用数组的大小                                                                     */
	/************************************************************************/
UINT WavePlot::GetDataSize()
{
	switch(data_style)
	{
	case s_BYTE:
		return plotdata_byte.size();
	case s_INT16:
		return plotdata_int16.size();
	case s_UINT16:
		return plotdata_uint16.size();
	case s_FLOAT:
		return plotdata_float.size();
	default:
		return 0;
	}
}

	/************************************************************************/
	/* 读取数据，存储到统一变量PlotData中，方便绘图的统一进行                                                                     */
	/************************************************************************/
void WavePlot::GetPlotData(vector<float> &_PlotData,UINT x_range)
{
	UINT data_size = GetDataSize();
	//给_PlotData分配内存空间
	if (data_size < x_range)//数据个数较少
		_PlotData.resize(data_size);
	else
		_PlotData.resize(x_range);
	//传递数据
	switch(data_style)
	{
	case s_BYTE:
		for (UINT index = 0;index<_PlotData.size();index++)//get the last few elements
			_PlotData[index] = static_cast<float>(plotdata_byte[data_size-_PlotData.size()+index]);
		break;
	case s_INT16:
		for (UINT index = 0;index<_PlotData.size();index++)//get the last few elements
			_PlotData[index] = static_cast<float>(plotdata_int16[data_size-_PlotData.size()+index]);
		break;
	case s_UINT16:
		for (UINT index = 0;index<_PlotData.size();index++)//get the last few elements
			_PlotData[index] = static_cast<float>(plotdata_uint16[data_size-_PlotData.size()+index]);
		break;
	case s_FLOAT:
		for (UINT index = 0;index<_PlotData.size();index++)//get the last few elements
			_PlotData[index] = plotdata_float[data_size-_PlotData.size()+index];
		break;
	default:
		break;
	}

	
}



IMPLEMENT_SERIAL (WavePlot, CObject, 1)

void WavePlot::Serialize (CArchive & ar)
{
	CObject::Serialize (ar);
	if (ar.IsStoring ())
	{
		int a=0;
		switch(data_style)
		{
		case s_NULL:
			a = 0 ;
			ar << a ;break;
		case s_BYTE:
			a = 1 ;
			ar << a ;break;
		case s_INT16:
			a = 2 ;
			ar << a ;break;
		case s_UINT16:
			a = 3 ;
			ar << a ;break;
		case s_FLOAT:
			a = 4 ;
			ar << a ;break;
		default:
			break;
		}
		ar<<IsShow;
		ar<<BytesPerData;
		ar<<DataNumber;
		ar<<title;
		assert(RightKey.size()==2);
		ar<<RightKey[0];
		ar<<RightKey[1];

		
	}	
	else
	{
		int a=0;
		ar>>a;
		switch(a)
		{
		case 0:
			data_style=s_NULL;break;
		case 1:
			data_style=s_BYTE;break;
		case 2:
			data_style=s_INT16;break;
		case 3:
			data_style=s_UINT16;break;
		case 4:
			data_style=s_FLOAT;break;
		default:
			break;
		}
		ar>>IsShow;
		ar>>BytesPerData;
		ar>>DataNumber;
		ar>>title;
		RightKey.resize(2);
		ar>>RightKey[0];
		ar>>RightKey[1];
	}
}
