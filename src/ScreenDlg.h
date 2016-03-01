 
// ScreenSpyDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "resource.h"
#include "CRectTrackerInheritance.h"

#include "PngImage.h"
#include "SelectBoard.h"
#include <vector>
#include <bitset>

#define  SafeDeleteBMP(pData) { try { pData->Detach();pData->DeleteObject();pData = NULL;} catch (...) { ASSERT(FALSE); } pData=NULL; }

using namespace std; 
class CScreenDlg : public CDialogEx
{

public:
	CScreenDlg(CWnd* pParent = NULL);	
	virtual ~CScreenDlg();

	enum { IDD = IDD_SCREEN_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	int		m_xScreen;
	int		m_yScreen;  //��Ļ�ķֱ���
	
	HCURSOR m_hCursor;//���
	CPoint  m_startPt;//��ȡ�������Ͻ�
	 
	BOOL    m_bStartDraw;//�Ƿ���Կ�ʼ������Ƥ��
	 
	CPngImage2	m_pngMask;/*���Ͻ� �ı���*/
	CPngImage2	m_pngDot;	/*9���� ��ק��*/
	CPngImage2  m_pngBtn;
	CPngImage2	m_pngDrBoard;

	std::bitset<10> m_HoverState;//����ƶ� hover״̬
	vector<CToolBtn> m_toolBtn;
	UINT m_uBtnPushedIndex;	/*�����µİ�ť m_uBtnPushedIndex*/ 
	CSelectBoard m_Selectboard;
 
	CArray<CPoint,CPoint>  m_pointArray; 
	CBitmap	*m_backGround;
	CBitmap *m_SaveBmp;
	vector<HBITMAP> m_vecHBitmap;

	CDC m_editDc;		//�༭dc
	CDC BufferDC;		//��ʾdc
	CBitmap *m_pBufferOldBmp;

	CPoint		m_ptLButtonDown;
	CPoint		m_ptDrawStart;
	 
	CRect		m_rcSel[9];//��������[0]ѡ��������Ϊ 1-8Ϊ��ק
	UINT		m_curArray[10];//���ͼ
	UINT		m_curindex; //�������
	UINT		m_iCapture;//������ ���µ������ţ� -1 ������ 0ƽ�� 1-8 ��ק 10 ��ͼ

	enum WORDSTATE
	{
		NONE,//��ʼ
		BEGIN,//��ʼ
		SELECTING,//ѡ��
		SELECTED,//ѡ�����
		BTNDOWN,//�˵���ť����		
		DRAW, //�༭
		TEXT, //����
		CTRLZ
	};
	WORDSTATE	eum_state; //����״̬ 
	
	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void	ShowInfo(CRect &inRect);
	HBITMAP CopyScreenToBitmap(LPRECT lpRect,CDC * dc = NULL );
	 

	DECLARE_MESSAGE_MAP()
public:
	//�ػ洰��
	void PaintWindow();
	void SetTool(CDC *pmemDc,Point *pt);
	void SetBoard(CDC *pmemDc,Point *pt);
	
	void ShowRectTracker(CDC * pdc);
	void CheckBtnArea(CPoint pt);
	void CheckColorArea(CPoint pt);
	void CheckDropArea(CPoint pt);
	 int CheckDropRect(CPoint point);//��ק����
	void CheckDrawArea(CPoint pt);//�ɱ༭����
	void CheckHoverState(CPoint pt);//��ɫ��
	void ChangeSelArea(CPoint point);//��ק��
	void CheckBian(CPoint *pt);//ѡ������ڱ��ؼ��
	void FixSelRect();//����
	void DrawFrame(void);
	void DrawBoard(CPoint pt,bool IsSave=FALSE);/*�ڽ�ͼ����༭*/
	void DrawArrow(CDC *pdc,CPoint p1,CPoint p2,double theta,int length,CPen &pen);
	void SaveToFile(CString strSaveFile="");
	void SaveToClip();
	void SaveToVecHbmp(bool isFirst = false);
	void DoCancel();//����
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnOK();
	afx_msg void OnCancel();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
		void DrawTextTrackRect(CPoint pt);
};
