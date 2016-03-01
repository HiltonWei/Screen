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

	//��������
protected:
	Image *		m_pImage;	//ͼƬ����

	//��������
public:
	//���캯��
	CPngImage2();
	//��������
	virtual ~CPngImage2();

	//״̬����
public:
	//�Ƿ����
	bool IsNull();
	//��ȡ���
	INT GetWidth();
	//��ȡ�߶�
	INT GetHeight();
	//
	/*Point pos;
	void SetPos(Point ptr);
	Point GetPos();*/


	//������
public:
	//����ͼƬ
	bool DestroyImage();
	//����ͼƬ
	bool LoadImage(LPCTSTR pszFileName);
	//����ͼƬ
	bool LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName);

	//�滭����
public:
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXPos, INT nYPos);
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc);
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc, INT nSrcWidth, INT nSrcHeight);

	//��������
public:
	//��ȡ����
	Image * GetImage() { return m_pImage; }
	//ָ������
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