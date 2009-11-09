// PtzButton.cpp : 实现文件
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "PtzButton.h"
#include "mcudef.h"

#include "ImageFactory.h"
#include "ImageShow.h"
// CPtzButton

IMPLEMENT_DYNAMIC(CPtzButton, CUIButton)

CPtzButton::CPtzButton()
{
}

CPtzButton::~CPtzButton()
{

}


BEGIN_MESSAGE_MAP(CPtzButton, CUIButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CPtzButton 消息处理程序



void CPtzButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->GetParent()->PostMessage( WM_PTZ_BUTTON_DOWN, GetDlgCtrlID() );
	
	__super::OnLButtonDown(nFlags, point);
}

void CPtzButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->GetParent()->PostMessage( WM_PTZ_BUTTON_UP, GetDlgCtrlID( ) );

	__super::OnLButtonUp(nFlags, point);
}

