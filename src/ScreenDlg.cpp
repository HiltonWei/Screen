
// ScreenSpyDlg.cpp : 实现文件
//



#include "stdafx.h"
#include "Screen.h"
#include "ScreenDlg.h"
#include "afxdialogex.h"

#define  WIDHTMSK 5 //马赛克宽度
const int sys_width=GetSystemMetrics(SM_CXSCREEN);
CScreenDlg::CScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenDlg::IDD, pParent)
{
	m_pngMask.LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_SC_MASK));
	m_pngDot.LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_SC_DOT));
	 
	m_pngBtn.LoadImageA(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_SC_ACTION));
	m_pngDrBoard.LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_SC_DRAWBOARD));

	m_HoverState=0;
	m_uBtnPushedIndex=-1;
	m_ptDrawStart=0;

	for (int x=0;x<10;x++)
	{	
		CToolBtn m_forBt;
		m_toolBtn.push_back(m_forBt);
	}
	eum_state = NONE;

	m_hCursor=AfxGetApp()->LoadCursor(ARROW_COLOR); 
	m_curindex=-1;

	m_bStartDraw=FALSE;
	m_startPt=0;

	m_xScreen = GetSystemMetrics(SM_CXSCREEN);
	m_yScreen = GetSystemMetrics(SM_CYSCREEN);

	CRect rect(0, 0,m_xScreen,m_yScreen);
	m_backGround=CBitmap::FromHandle(CopyScreenToBitmap(&rect));
	m_SaveBmp = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pBufferOldBmp = NULL;

}

CScreenDlg::~CScreenDlg()
{
	m_backGround->DeleteObject();
	if (m_SaveBmp)
		m_SaveBmp->DeleteObject();
	
		size_t len = m_vecHBitmap.size();
	  for (size_t i = 0; i != len; ++i)
		  {
			  DeleteObject(m_vecHBitmap[i]);
		  }

	BufferDC.SelectObject(m_pBufferOldBmp);
	BufferDC.DeleteDC();
	m_editDc.DeleteDC();
}

void CScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg message handlers

BOOL CScreenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//把对化框设置成全屏顶层窗口
	SetWindowPos(&wndTopMost,0,0,m_xScreen,m_yScreen,SWP_SHOWWINDOW);
	//SetWindowPos(&wndTopMost,-700,800,m_xScreen/4,m_yScreen/4,SWP_SHOWWINDOW);

	//捕获按键消息窗口,将对话框的句柄传递到CCatchScreenApp中
	((CScreenApp*)AfxGetApp())->m_hwndDlg=m_hWnd;
	
	m_curArray[0]=32646;//IDC_SIZEALL;
	m_curArray[1]=32642;//IDC_SIZENWSE;
	m_curArray[2]=32645;//IDC_SIZENS;
	m_curArray[3]=32643;//IDC_SIZENWSE;
	m_curArray[4]=32644;//IDC_SIZEWE;
	m_curArray[5]=32644;//IDC_SIZEWE;
	m_curArray[6]=32643;//IDC_SIZENWSE;
	m_curArray[7]=32645;//IDC_SIZENS;
	m_curArray[8]=32642;//IDC_SIZENWSE;
	m_curArray[9]=32515;	//IDC_CROSS
	m_iCapture=-1;
	//8个 拖拽点+ 1个 截屏矩形
	for(int i=0;i<9;i++)
	{
		m_rcSel[i].left=0;
		m_rcSel[i].top=0;
		m_rcSel[i].right=0;
		m_rcSel[i].bottom=0;
	}
	return TRUE; 
}

void CScreenDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{		
		DrawFrame();
	}
	CDialog::OnPaint();
}


/************************************************************************/
/* 
显示相关的信息  inRect为传进来的橡皮筋矩形
*/
/************************************************************************/
void CScreenDlg::ShowInfo(CRect &inRect)
{
	//截取矩形大小信息离鼠标间隔
	const int space=3;

	CClientDC dc(this);
	//设置字体颜色大小
	CPoint pt;
	//CPen pen(PS_SOLID,1,RGB(147,147,147));
	dc.SetTextColor(RGB(255,255,255));
	CFont cfont;
	cfont.CreatePointFont(90,"宋体");
	dc.SelectObject(&cfont);

	//得到字体宽度和高度
	GetCursorPos(&pt);
	dc.SetBkMode(TRANSPARENT);
	TEXTMETRIC tm;
	int charHeight;
	CSize size;
	int	lineLength;
	dc.GetTextMetrics(&tm);
	charHeight = tm.tmHeight+tm.tmExternalLeading + 10;
	size=dc.GetTextExtent("%d x %d",strlen(" %d x %d   "));
	lineLength=size.cx;

	//初始化矩形, 以保证写下一行文字  20*50
	CRect rect(inRect.left,inRect.top-20,inRect.left+70,inRect.top);

	int x = GetDeviceCaps(dc, HORZRES);
	int y = GetDeviceCaps(dc, VERTRES);

	//创建临时矩形
	CRect rectTemp;
	//当矩形到达桌面边缘时调整方向和大小
	if((pt.x+rect.Width())>=x)
	{
		//桌面上方显示不下矩形
		rectTemp=rect;
		rectTemp.left=rect.left-rect.Width()-space*2;
		rectTemp.right=rect.right-rect.Width()-space*2;;
		rect=rectTemp;
	}

	if((pt.y-rect.Height())<=0)
	{
		//桌面右方显示不下矩形
		rectTemp=rect;
		rectTemp.top=rect.top+rect.Height()+space*2;;
		rectTemp.bottom=rect.bottom+rect.Height()+space*2;;
		rect=rectTemp;

	}

	//创建灰色画刷画矩形
	dc.SelectObject((HBRUSH)GetStockObject(GRAY_BRUSH));
	dc.Rectangle(rect);
	rect.top+=2;
	//在矩形中显示文字	
	CString string;
	string.Format(" %d x %d",inRect.Width(),inRect.Height());
	dc.TextOut(rect.left,rect.top,string);//
}

HCURSOR CScreenDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CScreenDlg::OnOK() 
{
}

void CScreenDlg::OnCancel() 
{
	if(m_bStartDraw)
	{
		m_bStartDraw=FALSE;
		PaintWindow();
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CScreenDlg::CheckBian(CPoint *pt)
{

	pt->x = pt->x < m_rcSel[0].left ? m_rcSel[0].left : pt->x;
	pt->x = pt->x > m_rcSel[0].right ? m_rcSel[0].right : pt->x;
	pt->y = pt->y >m_rcSel[0].bottom ? m_rcSel[0].bottom : pt->y;
	pt->y = pt->y < m_rcSel[0].top ? m_rcSel[0].top:pt->y;
}

void CScreenDlg::ChangeSelArea(CPoint point)
{
	int x=point.x-m_ptLButtonDown.x;
	int y=point.y-m_ptLButtonDown.y;
	switch(m_iCapture)
	{
	case 1:
		m_rcSel[0].left+=x;
		m_rcSel[0].top+=y;
		break;
	case 2:
		m_rcSel[0].top+=y;
		break;
	case 3:
		m_rcSel[0].top+=y;
		m_rcSel[0].right+=x;
		break;
	case 4:
		m_rcSel[0].left+=x;
		break;
	case 5:
		m_rcSel[0].right+=x;
		break;
	case 6:
		m_rcSel[0].left+=x;
		m_rcSel[0].bottom+=y;
		break;
	case 7:
		m_rcSel[0].bottom+=y;
		break;
	case 8:
		m_rcSel[0].right+=x;
		m_rcSel[0].bottom+=y;
		break;
	case 0:
		m_rcSel[0].left+=x;
		m_rcSel[0].right+=x;
		m_rcSel[0].top+=y;
		m_rcSel[0].bottom+=y;
		break;
	}
	DrawFrame();
}

void CScreenDlg::OnMouseMove(UINT nFlags, CPoint point) 
{	
	switch (eum_state)
	{
	case BEGIN:
		m_rcSel[0].right=point.x;
		m_rcSel[0].bottom=point.y;
		break;
	case SELECTING:
		ChangeSelArea(point);
		m_ptLButtonDown=point;

		break;
	case SELECTED:
		m_curindex = CheckDropRect(point);
		CheckHoverState(point);
		break;
	case BTNDOWN:

		m_curindex = PtInRect(m_rcSel[0],point)?9:-1;//鼠标状态
		CheckHoverState(point);
		break;
	case DRAW:
		m_curindex = PtInRect(m_rcSel[0],point)?9:-1;//鼠标状态
		CheckHoverState(point);

		if (m_iCapture ==10)
		{
			CheckBian(&point);
			CPoint cp;
			cp.x=point.x-m_rcSel[0].left;
			cp.y=point.y-m_rcSel[0].top;
			m_pointArray.Add(cp);

			DrawBoard(point);		 
		}
		m_ptLButtonDown=point;

		break;
	default:
		break;
	}
	DrawFrame();

	CDialog::OnMouseMove(nFlags, point);
}

void CScreenDlg::DrawArrow(CDC *pdc,CPoint p1,CPoint p2,double theta,int length,CPen& pen)
{
	theta=3.1415926*theta/180;//转换为弧度
	double Px,Py,P1x,P1y,P2x,P2y;
	//以P2为原点得到向量P2P1（P）
	Px=p1.x-p2.x;
	Py=p1.y-p2.y;
	//向量P旋转theta角得到向量P1
	P1x=Px*cos(theta)-Py*sin(theta);
	P1y=Px*sin(theta)+Py*cos(theta);
	//向量P旋转-theta角得到向量P2
	P2x=Px*cos(-theta)-Py*sin(-theta);
	P2y=Px*sin(-theta)+Py*cos(-theta);
	//伸缩向量至制定长度
	double x1,x2;
	x1=sqrt(P1x*P1x+P1y*P1y);
	P1x=P1x*length/x1;
	P1y=P1y*length/x1;
	x2=sqrt(P2x*P2x+P2y*P2y);
	P2x=P2x*length/x2;
	P2y=P2y*length/x2;
	//平移变量到直线的末端
	P1x=P1x+p2.x;
	P1y=P1y+p2.y;
	P2x=P2x+p2.x;
	P2y=P2y+p2.y;

	HPEN oldpen= (HPEN)(m_editDc.SelectObject(pen));
	m_editDc.MoveTo(p1.x,p1.y);
	m_editDc.LineTo(p2.x,p2.y);
	m_editDc.SelectObject(pen);
	m_editDc.MoveTo(p2.x,p2.y);
	m_editDc.LineTo((int)P1x,(int)P1y);
	m_editDc.MoveTo(p2.x,p2.y);
	m_editDc.LineTo((int)P2x,(int)P2y);
	m_editDc.SelectObject(oldpen);
}
void CScreenDlg::SaveToFile(CString strSaveFile)
{
	CString strExt=".png";
	if(strSaveFile=="")
	{
		CFileDialog fd(FALSE,"PNG","未命名",4|2,"PNG 图片格式 (*.png)|*.png|BMP 图片格式 (*.bmp)|*.bmp|JPG 图片格式 (*.jpg)|*.jpg||");
		if(fd.DoModal()==IDCANCEL)
			return;
		strExt=fd.GetFileExt();
		strExt.Trim();
		strExt.MakeLower();
		strSaveFile=fd.GetPathName();
	}
	if(m_SaveBmp ==NULL || m_SaveBmp->m_hObject ==NULL)
		return;
	CImage image;
	image.Attach((HBITMAP)m_SaveBmp->GetSafeHandle());
	if(strExt=="bmp")
		image.Save(strSaveFile, Gdiplus::ImageFormatBMP);
	else if(strExt=="jpg")
		image.Save(strSaveFile, Gdiplus::ImageFormatJPEG);
	else
		image.Save(strSaveFile, Gdiplus::ImageFormatPNG);

	m_rcSel[0].right=m_rcSel[0].left;
	m_rcSel[0].bottom=m_rcSel[0].top;
	 
	CDialog::OnCancel();
}

void CScreenDlg::SaveToClip()
{
	if (OpenClipboard()) 
	{
		EmptyClipboard();//清空剪贴板	//把屏幕内容粘贴到剪贴板上,
		SetClipboardData(CF_BITMAP, (HBITMAP)m_SaveBmp->GetSafeHandle());//hBitmap 为刚才的屏幕位图句柄
		CloseClipboard();//关闭剪贴板
	}
}

void CScreenDlg::CheckBtnArea(CPoint pt)
{
	m_HoverState=0;
	for (int x=0;x<10;x++)
	{
		if (PtInRect(m_toolBtn[x].btn_rct,pt) )
		{
			switch (x)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				m_uBtnPushedIndex=(m_uBtnPushedIndex==x)?-1:x;
				if (eum_state ==  SELECTED) 
					eum_state=  BTNDOWN;
				else if(eum_state ==  BTNDOWN ) 
					eum_state = SELECTED;
				break;

			case 6:
				eum_state = CTRLZ;
				DoCancel();
				break;
			case 7:
				SaveToFile();
				break;
			case 8:
				CDialog::OnCancel();
				break;
			case 9:
				SaveToClip();						
				OnRButtonDown(NULL,NULL);
				break;
			}
		}
	}
}

void CScreenDlg::CheckDropArea(CPoint pt)
{
	m_iCapture = CheckDropRect(pt);	
}
int CScreenDlg::CheckDropRect(CPoint point)
{
	int res= -1;
	for( int i=8;i>=0;i--)
	{
		if(PtInRect(m_rcSel[i],point))
		{
			res = i;
			break;
		}
	}

	return res;
}

void CScreenDlg::CheckHoverState(CPoint pt)
{
	std::bitset<10> btnstate_old=m_HoverState;
	m_HoverState=0;

	for (int i=0;i<10;i++)  
		if (PtInRect(m_toolBtn[i].btn_rct,pt))  
			m_HoverState[i]=1; //偷懒 不break了 下同

	if(m_HoverState!=btnstate_old)  
		DrawFrame();
	std::bitset<19> Hoverstate_old;
	if((m_uBtnPushedIndex!=-1) && (m_Selectboard.vecbtnbox.size()!=0))
	{
		Hoverstate_old=m_Selectboard.Hoverstate;
		m_Selectboard.Hoverstate=0;
		for(int i=0;i<16;i++)  
			if (PtInRect(m_Selectboard.vecclrbox[i],pt)) 
				m_Selectboard.Hoverstate[i]=1 ;//绘边
		for(int i=0;i<3;i++) 
			if (PtInRect(m_Selectboard.vecbtnbox[i],pt)) 
				m_Selectboard.Hoverstate[i+16]=1 ;//绘边
		if(Hoverstate_old!=m_Selectboard.Hoverstate)  
			DrawFrame();
	}
}
void CScreenDlg::CheckDrawArea(CPoint pt)
{
	m_iCapture = -1;
	if (PtInRect(m_rcSel[0],pt))
	{
		m_iCapture=10;
		m_ptDrawStart=pt;
		eum_state =  DRAW;
	}
}

void CScreenDlg::CheckColorArea(CPoint pt)
{
	if(m_uBtnPushedIndex!=-1&&m_uBtnPushedIndex<6)
	{
		for (int x=0;x<3;x++)
			if (PtInRect(m_Selectboard.vecbtnbox[x],pt) ) m_Selectboard.m_LineWidth=2*x;
		for (int x=0;x<16;x++)
		{	
			if (PtInRect(m_Selectboard.vecclrbox[x],pt) )
			{
				CPoint p_tmp;
				GetCursorPos(&p_tmp);

				COLORREF color;
				CClientDC dc(this);
				color=dc.GetPixel(p_tmp);
				m_Selectboard.m_SelectClr=color;
				m_Selectboard.m_SelectClrRef=color;
			}		
		}
		DrawFrame();
	}
}

void CScreenDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	switch (eum_state)
	{
	case  NONE:
		m_rcSel[0].left=point.x;
		m_rcSel[0].top=point.y;
		m_rcSel[0].right=point.x;
		m_rcSel[0].bottom=point.y;
		eum_state =  BEGIN;
		break;
	case  SELECTED:
		CheckDropArea(point);
		if (m_iCapture != -1)
		{
			eum_state =  SELECTING;
			m_ptLButtonDown = point;
		}
		CheckBtnArea(point);
		break;

	case  BTNDOWN:
	case  DRAW:
		CheckDrawArea(point);
		CheckBtnArea(point);
		CheckColorArea(point);
		break;
	default:
		break;
	}
	DrawFrame();
	CDialog::OnLButtonDown(nFlags, point);
}

void CScreenDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rect;
	switch (eum_state)
	{
	case  BEGIN:
	case  SELECTING:
		FixSelRect();
		eum_state =  SELECTED;
		SaveToVecHbmp(true);
		break;
	case  DRAW:
		m_pointArray.RemoveAll();
		if(m_iCapture != -1)
			SaveToVecHbmp();
		break;
	case CTRLZ:
		m_iCapture = 10;
		eum_state =DRAW;	
		break;
	default:
		break;
	}
	
	DrawFrame();
	m_iCapture = -1;
	CDialog::OnLButtonUp(nFlags, point);
}
void CScreenDlg::DrawTextTrackRect(CPoint pt)
{
}

void CScreenDlg::FixSelRect()
{
	int x=m_rcSel[0].left;
	int y=m_rcSel[0].top;
	int w=m_rcSel[0].Width();
	int h=m_rcSel[0].Height();

	if(x>m_rcSel[0].right)   x=m_rcSel[0].right;
	if(y>m_rcSel[0].bottom)   y=m_rcSel[0].bottom;
	if(w<0)w=-w;
	if(h<0)h=-h;
	m_rcSel[0].left=x;
	m_rcSel[0].top=y;
	m_rcSel[0].right=x+w;
	m_rcSel[0].bottom=y+h;
}

void CScreenDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(PtInRect(m_rcSel[0],point))
	{	
		SaveToClip();
		OnRButtonDown(NULL,NULL);
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}


//处理鼠标右键单击 .ESC. 
void CScreenDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bStartDraw)//如果已经截取矩则清除截取矩形
	{

		m_bStartDraw=FALSE;
		PaintWindow();
	}
	else{
		PostQuitMessage(0);
	}
	CDialog::OnRButtonDown(nFlags, point);
}

BOOL CScreenDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
BOOL CScreenDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{

	if (pWnd == this) 
	{	
		SetCursor(m_curindex== -1?m_hCursor:LoadCursor(NULL,MAKEINTRESOURCE(m_curArray[m_curindex])));

	}
	else 
	{
		SetCursor(m_curindex==-1?m_hCursor:LoadCursor(NULL,MAKEINTRESOURCE(m_curArray[m_curindex])));
	}
	return true;
}

void  CScreenDlg::SetTool(CDC *pMemDc,Point *pt)
{	
	int startx=pt->X;
	int starty=pt->Y;
	CRect ctemprect(startx,starty,startx+40,starty+37);
	for (int x=0;x<10;x++)
	{
		m_toolBtn[x].SetRect(&ctemprect);

		m_pngBtn.DrawImage(pMemDc,ctemprect.left,ctemprect.top,40,37,3+x*40,(1&m_HoverState[x]/*(m_toolBtn[x]._HasPushed)*/)*37,40,37);
		ctemprect.left+=40;
		ctemprect.right=ctemprect.left+37;
	}
	if (m_uBtnPushedIndex!=-1)
	{
		m_pngBtn.DrawImage(pMemDc,startx+40*m_uBtnPushedIndex,starty,40,37,        3+m_uBtnPushedIndex*40, 37,40,37);
		SetBoard(pMemDc,&Point(startx,starty+39));
	}
}
void CScreenDlg::SaveToVecHbmp(bool isFirst)
{
	 HBITMAP hbmp = (HBITMAP) CopyScreenToBitmap(&m_rcSel[0],&m_editDc);
	 if (hbmp)
	 {
		 isFirst?(m_vecHBitmap.size()>0? m_vecHBitmap[0] = hbmp:m_vecHBitmap.push_back(hbmp) ): m_vecHBitmap.push_back(hbmp);
		 if (m_SaveBmp)
			 SafeDeleteBMP(m_SaveBmp);	  
		 m_SaveBmp = CBitmap::FromHandle((HBITMAP)(m_vecHBitmap.back()));
	 }
}
void CScreenDlg::DoCancel()
{	if (m_SaveBmp)
		SafeDeleteBMP(m_SaveBmp);
	if (m_vecHBitmap.size()> 1 )
	{
		DeleteObject(m_vecHBitmap[m_vecHBitmap.size() -1]);
		m_vecHBitmap.pop_back();
	}
	 m_SaveBmp = CBitmap::FromHandle((HBITMAP)(m_vecHBitmap.back()));
}

void CScreenDlg::SetBoard(CDC *pMemDc,Point *pt)
{
	int startx=pt->X;
	int starty=pt->Y;
	m_Selectboard.SetPoint(pt);
	int imageWi=m_pngDrBoard.GetWidth();
	int imageHi=m_pngDrBoard.GetHeight()/2;
	if(m_uBtnPushedIndex==-1||m_uBtnPushedIndex>5) return;
	//Drawbtn 被选中的 线宽按钮

	m_pngDrBoard.DrawImage(pMemDc,startx,starty,imageWi,imageHi,0,0,imageWi,imageHi);

	//Draw选择的线宽按钮
	m_pngDrBoard.DrawImage(pMemDc,
		startx+2+(m_Selectboard.m_LineWidth/2)*(SelLineBtnWidth+2),
		starty+2,
		SelLineBtnWidth,
		SelLineBtnWidth,
		(m_Selectboard.m_LineWidth/2)*(SelLineBtnWidth+2),
		2+imageHi ,
		SelLineBtnWidth,
		SelLineBtnWidth);

	//m_pngDrBoard.DrawImage(pMemDc,)
	//创建屏幕
	ASSERT(pMemDc!=NULL);
	Graphics graphics(pMemDc->GetSafeHdc());
	int r=m_Selectboard.m_SelectClr.GetR();
	int g=m_Selectboard.m_SelectClr.GetG();
	int b=m_Selectboard.m_SelectClr.GetB();


	Color Selcolor(b,g,r);//颜色倒置
	graphics.FillRectangle( new SolidBrush(Selcolor) ,
		(m_Selectboard.vecclrbox[16]).left,(m_Selectboard.vecclrbox[16]).top,
		(m_Selectboard.vecclrbox[16]).right-(m_Selectboard.vecclrbox[16]).left,
		(m_Selectboard.vecclrbox[16]).bottom-(m_Selectboard.vecclrbox[16]).top);  

	//	Graphics graphics(pMemDc->GetSafeHdc());
	Color Selcolor2(128,128,128);
	Pen  skyBluePen ( Selcolor2,2 );
 
	if (m_Selectboard.Hoverstate!=0)
	{

		for (int i=0;i<19;i++)
		{	
			if (m_Selectboard.Hoverstate[i]&&i<16) 
			{
				graphics.DrawRectangle(&skyBluePen,m_Selectboard.vecclrbox[i].left-1,
					m_Selectboard.vecclrbox[i].top,
					m_Selectboard.vecclrbox[i].Width(),
					m_Selectboard.vecclrbox[i].Height()+1);
			}
			else if(m_Selectboard.Hoverstate[i]&&i>15)
			{
				m_pngDrBoard.DrawImage(pMemDc,m_Selectboard.vecbtnbox[i-16].left,m_Selectboard.vecbtnbox[i-16].top,
					m_Selectboard.vecbtnbox[i-16].Width(),
					m_Selectboard.vecbtnbox[i-16].Height(),
					(i-16)*(SelLineBtnWidth+2),
					2+imageHi ,
					SelLineBtnWidth,
					SelLineBtnWidth
					);		
			}
		}		
	}	
}

//拷贝屏幕到位图中
HBITMAP CScreenDlg::CopyScreenToBitmap(LPRECT lpRect,CDC * dc)
	//lpRect 代表选定区域
{
	HDC       hScrDC, hMemDC;  // 屏幕和内存设备描述表    
	HBITMAP    hBitmap, hOldBitmap;// 位图句柄   
	int       nX, nY, nX2, nY2;   // 选定区域坐标   
	int       nWidth, nHeight;
	if (IsRectEmpty(lpRect))	// 确保选定区域不为空矩形
		return NULL;

	nX = lpRect->left;// 获得选定区域坐标
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	if (nX < 0) nX = 0;
	if (nY < 0) nY = 0;
	if (nX2 > m_xScreen) nX2 = m_xScreen;
	if (nY2 > m_yScreen) nY2 = m_yScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;

	int startx = 0 ;
	int starty = 0;
	 
	if (dc == NULL || m_editDc.m_hDC ==NULL)
	{ 
		hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
		startx = nX;
		starty = nY;

	}
	else hScrDC = dc->GetSafeHdc();

	hMemDC = CreateCompatibleDC(hScrDC);
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	BitBlt(hMemDC,0, 0,nWidth,nHeight,hScrDC, startx, starty, SRCCOPY);

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	DeleteObject(hOldBitmap);

	return hBitmap;
}

//重画窗口
void CScreenDlg::PaintWindow()
{
	//获取当全屏对话框窗口大小
	/**/CRect rect1;
	GetWindowRect(rect1);

	CRgn rgn1;
	rgn1.CreateRectRgnIndirect(rect1);

	InvalidateRect(rect1);
}

void CScreenDlg::DrawFrame(void)
{
	CDC *dc=GetDC();	

	if (!BufferDC.m_hDC)
	{
		CBitmap BufferBmp;
		BufferDC.CreateCompatibleDC(dc);
		BufferBmp.CreateCompatibleBitmap(dc,m_xScreen,m_yScreen);
		m_pBufferOldBmp = BufferDC.SelectObject(&BufferBmp);
		BufferBmp.DeleteObject();

		CDC pdc;
		pdc.CreateCompatibleDC(&BufferDC);
		CBitmap *oldbmp =pdc.SelectObject(m_backGround);
		BufferDC.BitBlt(0,0,m_xScreen,m_yScreen,&pdc,0,0,SRCCOPY);
		pdc.SelectObject(oldbmp);
		
		pdc.DeleteDC();
	}

	CDC pdc;
	pdc.CreateCompatibleDC(&BufferDC);
	CBitmap *oldbmp =pdc.SelectObject(m_backGround);

	if (m_iCapture == 10)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(&pdc);
		memDC.SelectObject(m_SaveBmp);
		pdc.BitBlt(m_rcSel[0].left,m_rcSel[0].top,m_rcSel[0].Width(),m_rcSel[0].Height(),&memDC,0,0,SRCCOPY);
		memDC.DeleteDC();
	}
	BufferDC.BitBlt(0,0,m_xScreen,m_yScreen,&pdc,0,0,SRCCOPY);
	pdc.SelectObject(oldbmp);
	pdc.DeleteDC();

	BufferDC.BitBlt(m_rcSel[0].left,m_rcSel[0].top,m_rcSel[0].Width(),m_rcSel[0].Height(),&m_editDc,0,0,SRCCOPY);
	ShowRectTracker(&BufferDC);

	dc->BitBlt(0,0,m_xScreen,m_yScreen,&BufferDC,0,0,SRCCOPY);
	//清理资源
	ReleaseDC(dc);


}

//显示Tracker信息
void CScreenDlg::ShowRectTracker(CDC * pdc)
{
	//////////////////////////////////////////////////////////////////////////
	if(m_rcSel[0].Width()==0 || m_rcSel[0].Height()==0)
		m_pngMask.DrawImage(pdc,0,0,m_xScreen,m_yScreen,0,0,8,8);
	else
	{
		int x=m_rcSel[0].left;
		int y=m_rcSel[0].top;
		int w=m_rcSel[0].Width();
		int h=m_rcSel[0].Height();
		if(x>m_rcSel[0].right)x=m_rcSel[0].right;
		if(y>m_rcSel[0].bottom)y=m_rcSel[0].bottom;
		if(w<0)w=-w;
		if(h<0)h=-h;


		m_pngMask.DrawImage(pdc,0,0,m_xScreen,y,0,0,8,8);
		m_pngMask.DrawImage(pdc,0,y+h,m_xScreen,m_yScreen-y-h,0,0,8,8);
		m_pngMask.DrawImage(pdc,0,y,x,h,0,0,8,8);
		m_pngMask.DrawImage(pdc,x+w,y,m_xScreen-x-w,h,0,0,8,8);

		pdc->Draw3dRect(x,y,w,h,RGB(0,0,0),RGB(0,0,0));
		//////////////////////////////////////////////////////////////////////////
		CRect rc; rc.left=x+2;rc.top=y-24;rc.right=x+268;rc.bottom=y-2;
		if(rc.top<0){rc.top=y+2;rc.bottom=y+24;}
		m_pngMask.DrawImage(pdc,rc.left,rc.top,rc.Width(),rc.Height(),0,0,8,8);

		CFont font;
		font.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("宋体"));
		pdc->SelectObject((HFONT)font);
		pdc->SetBkMode(TRANSPARENT);
		pdc->SetTextColor(RGB(255,255,255));
		CString stxt;stxt.Format(" 起始位置: %4d.%4d  区域大小: %4d * %4d",x,y,w,h);
		pdc->DrawText(stxt,rc,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
		//////////////////////////////////////////////////////////////////////////
		Point pt;
		pt.X=x+w-400;
		pt.Y=y+h+2+2+26;
		SetTool(pdc,&pt);

		//////////////////////////////////////////////////////////////////////////
		x=m_rcSel[0].left;
		y=m_rcSel[0].top;
		w=m_rcSel[0].Width();
		h=m_rcSel[0].Height();


		m_pngDot.DrawImage(pdc,x-2,y-2);
		m_rcSel[1].left=x-2;m_rcSel[1].top=y-2;

		m_pngDot.DrawImage(pdc,x+w/2-2,y-2);
		m_rcSel[2].left=x+w/2-2;m_rcSel[2].top=y-2;

		m_pngDot.DrawImage(pdc,x+w-3,y-2);
		m_rcSel[3].left=x+w-3;m_rcSel[3].top=y-2;

		m_pngDot.DrawImage(pdc,x-2,y+h/2-2);
		m_rcSel[4].left=x-2;m_rcSel[4].top=y+h/2-2;

		m_pngDot.DrawImage(pdc,x+w-3,y+h/2-2);
		m_rcSel[5].left=x+w-3;m_rcSel[5].top=y+h/2-2;

		m_pngDot.DrawImage(pdc,x-2,y+h-3);
		m_rcSel[6].left=x-2;m_rcSel[6].top=y+h-3;

		m_pngDot.DrawImage(pdc,x+w/2-2,y+h-3);
		m_rcSel[7].left=x+w/2-2;m_rcSel[7].top=y+h-3;

		m_pngDot.DrawImage(pdc,x+w-3,y+h-3);
		m_rcSel[8].left=x+w-3;m_rcSel[8].top=y+h-3;

		for(int i=1;i<9;i++)
		{
			m_rcSel[i].right=m_rcSel[i].left+5;
			m_rcSel[i].bottom=m_rcSel[i].top+5;
		}
	}

}

void CScreenDlg::DrawBoard(CPoint draw_pt,bool IsSave)
{
	 
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	if(m_editDc.m_hDC )
	{
		m_editDc.DeleteDC();	
	}

	m_editDc.CreateCompatibleDC(&BufferDC);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&BufferDC,m_rcSel[0].Width(),m_rcSel[0].Height());
	m_editDc.SelectObject(&bitmap);
	m_editDc.SelectObject(pBrush);
	bitmap.DeleteObject();

	CDC pdc;
	pdc.CreateCompatibleDC(&m_editDc);
	pdc.SelectObject(m_SaveBmp);	
	m_editDc.BitBlt(0,0,m_rcSel[0].Width(),m_rcSel[0].Height(),&pdc,0,0,SRCCOPY);
	pdc.DeleteDC();

	CPoint tmeppoint;
	tmeppoint.SetPoint(m_ptDrawStart.x-m_rcSel[0].left,m_ptDrawStart.y-m_rcSel[0].top);
	draw_pt.SetPoint(draw_pt.x-m_rcSel[0].left,draw_pt.y-m_rcSel[0].top);

	CPen m_penCur ,* m_oldpen ;
	m_penCur.CreatePen(PS_SOLID, m_Selectboard.m_LineWidth, m_Selectboard.m_SelectClrRef);
	m_oldpen= m_editDc.SelectObject(&m_penCur);
	m_oldpen->DeleteObject();

	CPoint p_tmp;
	
	COLORREF color;
	CClientDC dc(this);
	CRect rectx;

	switch(m_uBtnPushedIndex)
	{
	case 0:
		m_editDc.Rectangle(CRect(tmeppoint ,draw_pt ));
		break;
	case 1:
		m_editDc.Ellipse(CRect(tmeppoint ,draw_pt ));
		break;
	case 2:
		{
			DrawArrow(&m_editDc,tmeppoint,draw_pt,10,20,m_penCur);
		}
		break;
	case 3:
		//m_editDc.SelectObject(&m_penCur);
		m_editDc.MoveTo(m_pointArray[0]);
		for (int i=0; i < m_pointArray.GetSize(); i++)
		{
			m_editDc.LineTo(m_pointArray[i]);
		}
		break;
	case 4:

		for (int i=0; i < m_pointArray.GetSize(); i++)
		{
			CPoint pt=m_pointArray[i];
			GetCursorPos(&p_tmp);
			color=dc.GetPixel(pt);
			int size = WIDHTMSK * m_Selectboard.m_LineWidth;
			int x = size*(pt.x/size);
			int y = size*(pt.y/size);
			rectx.SetRect(x-size/2,y-size/2,x+size/2,y+size/2);
			m_editDc.FillRect(&rectx,&CBrush(color));
		}

		break;
	case 5:
		break;
	default:
		break;
	}
	pBrush->DeleteObject();
}

BOOL CScreenDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE )
		{
			CDialog::OnCancel();
			
		}	
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}