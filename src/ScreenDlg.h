 
// ScreenSpyDlg.h : 头文件
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
	int		m_yScreen;  //屏幕的分辨率
	
	HCURSOR m_hCursor;//光标
	CPoint  m_startPt;//截取矩形左上角
	 
	BOOL    m_bStartDraw;//是否可以开始绘制橡皮筋
	 
	CPngImage2	m_pngMask;/*左上角 文本区*/
	CPngImage2	m_pngDot;	/*9个点 拖拽区*/
	CPngImage2  m_pngBtn;
	CPngImage2	m_pngDrBoard;

	std::bitset<10> m_HoverState;//鼠标移动 hover状态
	vector<CToolBtn> m_toolBtn;
	UINT m_uBtnPushedIndex;	/*被按下的按钮 m_uBtnPushedIndex*/ 
	CSelectBoard m_Selectboard;
 
	CArray<CPoint,CPoint>  m_pointArray; 
	CBitmap	*m_backGround;
	CBitmap *m_SaveBmp;
	vector<HBITMAP> m_vecHBitmap;

	CDC m_editDc;		//编辑dc
	CDC BufferDC;		//显示dc
	CBitmap *m_pBufferOldBmp;

	CPoint		m_ptLButtonDown;
	CPoint		m_ptDrawStart;
	 
	CRect		m_rcSel[9];//矩形区域[0]选区，其他为 1-8为拖拽
	UINT		m_curArray[10];//鼠标图
	UINT		m_curindex; //鼠标索引
	UINT		m_iCapture;//鼠标左键 按下的任务编号， -1 无任务 0平移 1-8 拖拽 10 画图

	enum WORDSTATE
	{
		NONE,//初始
		BEGIN,//开始
		SELECTING,//选区
		SELECTED,//选择完成
		BTNDOWN,//菜单按钮按下		
		DRAW, //编辑
		TEXT, //文字
		CTRLZ
	};
	WORDSTATE	eum_state; //工作状态 
	
	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void	ShowInfo(CRect &inRect);
	HBITMAP CopyScreenToBitmap(LPRECT lpRect,CDC * dc = NULL );
	 

	DECLARE_MESSAGE_MAP()
public:
	//重绘窗口
	void PaintWindow();
	void SetTool(CDC *pmemDc,Point *pt);
	void SetBoard(CDC *pmemDc,Point *pt);
	
	void ShowRectTracker(CDC * pdc);
	void CheckBtnArea(CPoint pt);
	void CheckColorArea(CPoint pt);
	void CheckDropArea(CPoint pt);
	 int CheckDropRect(CPoint point);//拖拽区域
	void CheckDrawArea(CPoint pt);//可编辑区域
	void CheckHoverState(CPoint pt);//颜色板
	void ChangeSelArea(CPoint point);//拖拽点
	void CheckBian(CPoint *pt);//选择矩形内边沿检测
	void FixSelRect();//修正
	void DrawFrame(void);
	void DrawBoard(CPoint pt,bool IsSave=FALSE);/*在截图上面编辑*/
	void DrawArrow(CDC *pdc,CPoint p1,CPoint p2,double theta,int length,CPen &pen);
	void SaveToFile(CString strSaveFile="");
	void SaveToClip();
	void SaveToVecHbmp(bool isFirst = false);
	void DoCancel();//撤销
	
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
