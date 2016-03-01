#include "StdAfx.h"
#include "SelectBoard.h"


CSelectBoard::CSelectBoard(void)
{
	m_LineWidth=2;
	m_SelectClr=RGB(0,0,0);
	m_SelectClrRef=RGB(0,0,0);
	Hoverstate=0;
}


CSelectBoard::~CSelectBoard(void)
{
}

//vector<CToolBtn> vecbtn;//按钮位置
//vector <CRect> vecclr;//颜色块位置，0-15为颜色块矩形。 16为展示矩形

//显示色框 相对位置 (2,60),色块间 2像素的间距 第i个(88,2) ,（88+i*ColorBoxwidth+i*2,2）
void	CSelectBoard::SetPoint (Point * pt)
{
	m_ptpos.X=pt->X;
	m_ptpos.Y=pt->Y;
	vecclrbox.clear();
		CRect rect;
	for(int i=0;i<16;++i)
	{
		rect.left=m_ptpos.X+120+(i/2)*(ColorBoxwidth+2);
		rect.top=m_ptpos.Y+3+(i%2)*(ColorBoxHeight+2);
		rect.right=rect.left+ColorBoxwidth;
		rect.bottom=rect.top+ColorBoxHeight;
		vecclrbox.push_back(rect);
	}

	rect.left=m_ptpos.X+ColorBoxwidth*2+62;
	rect.top=m_ptpos.Y+ColorBoxHeight*2-21;
	rect.right=rect.left+ColorBoxwidth*2;
	rect.bottom=rect.top+ColorBoxHeight*2;
	vecclrbox.push_back(rect);//第17个

	vecbtnbox.clear();
	for(int i=0;i<3;i++)
	{
		rect.left=m_ptpos.X+2+i*(SelLineBtnWidth+2);
		rect.top=m_ptpos.Y+2;
		rect.right=rect.left+SelLineBtnWidth;
		rect.bottom=rect.top+SelLineBtnHeight;
		vecbtnbox.push_back(rect);
	}

}

//设置颜色
void 	CSelectBoard::SetLineWidth(int lineWeight){m_LineWidth=lineWeight;}
int  	CSelectBoard::GetLineWidth(){return m_LineWidth;}
void 	CSelectBoard::SetSelColor(Color cr){}
Color 	CSelectBoard::GetSelColor(){ return m_SelectClr;}