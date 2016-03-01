#pragma once

#define  SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; }

#define WM_RECT    WM_USER+101
#define WM_CIRCLE  WM_USER+102
#define WM_PEN     WM_USER+103
#define WM_ARROW   WM_USER+104
#define WM_MASAI   WM_USER+105
#define WM_FONT	   WM_USER+106
#define WM_REDO	   WM_USER+107
#define WM_SAVE    WM_USER+108
#define WM_CANCEL  WM_USER+109
#define WM_RIGHT   WM_USER+110

class CPngImage2
{

	//变量定义
protected:
	Image *		m_pImage;	//图片对象

	//函数定义
public:
	//构造函数
	CPngImage2();
	//析构函数
	virtual ~CPngImage2();

	//状态函数
public:
	//是否加载
	bool IsNull();
	//获取宽度
	INT GetWidth();
	//获取高度
	INT GetHeight();
	//
	/*Point pos;
	void SetPos(Point ptr);
	Point GetPos();*/


	//管理函数
public:
	//销毁图片
	bool DestroyImage();
	//加载图片
	bool LoadImage(LPCTSTR pszFileName);
	//加载图片
	bool LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName);

	//绘画函数
public:
	//绘画图像
	bool DrawImage(CDC * pDC, INT nXPos, INT nYPos);
	//绘画图像
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc);
	//绘画图像
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc, INT nSrcWidth, INT nSrcHeight);

	//操作重载
public:
	//获取对象
	Image * GetImage() { return m_pImage; }
	//指针重载
	Image * operator->() { return m_pImage; }
};

class CToolBtn
{
public:
	CToolBtn ();
	~CToolBtn();
	CRect btn_rct;
	
	UINT _btnMsg;
	void SetRect(LPRECT rct);
};