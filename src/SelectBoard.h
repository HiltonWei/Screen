#include "StdAfx.h"

#include <vector>
#include <bitset>
#include "PngImage.h"

#define ColorBoxwidth 13
#define ColorBoxHeight 13
#define ColorDrawBoxWidth 26
#define ColorDrawBoxHeight 26
#define SelLineBtnWidth  24
#define SelLineBtnHeight 24 //���θ߶ȺͿ��

using namespace std;
class CSelectBoard :
	public CPngImage2
{
public:
	CSelectBoard(void);
	~CSelectBoard(void);

vector<CRect> vecbtnbox;//��ťλ��
vector<CRect> vecclrbox;//��ɫ��λ�� 0Ϊչʾ���Σ�1-16Ϊ��ɫ����Ρ�

	std::bitset<19> Hoverstate;//����ƶ� hover״̬
	void	SetPoint (Point * pt);
	void 	SetLineWidth(int lineWeight);
	int  	GetLineWidth();
	void 	SetSelColor(Color cr);
	Color 	GetSelColor();

	int m_LineWidth; //�߿�
	Color m_SelectClr;//��ɫ
	COLORREF m_SelectClrRef;
	Point m_ptpos;//��ɫ�嶥��λ��
private :

};

