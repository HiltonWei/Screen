#include "StdAfx.h"

#include <vector>
#include <bitset>
#include "PngImage.h"

#define ColorBoxwidth 13
#define ColorBoxHeight 13
#define ColorDrawBoxWidth 26
#define ColorDrawBoxHeight 26
#define SelLineBtnWidth  24
#define SelLineBtnHeight 24 //矩形高度和宽度

using namespace std;
class CSelectBoard :
	public CPngImage2
{
public:
	CSelectBoard(void);
	~CSelectBoard(void);

vector<CRect> vecbtnbox;//按钮位置
vector<CRect> vecclrbox;//颜色块位置 0为展示矩形，1-16为颜色块矩形。

	std::bitset<19> Hoverstate;//鼠标移动 hover状态
	void	SetPoint (Point * pt);
	void 	SetLineWidth(int lineWeight);
	int  	GetLineWidth();
	void 	SetSelColor(Color cr);
	Color 	GetSelColor();

	int m_LineWidth; //线宽
	Color m_SelectClr;//颜色
	COLORREF m_SelectClrRef;
	Point m_ptpos;//着色板顶点位置
private :

};

