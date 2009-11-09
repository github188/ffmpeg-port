#pragma once

#include "mcudef.h"
#include "uibutton.h"
// CPtzButton


class CPtzButton : public CUIButton
{
	DECLARE_DYNAMIC(CPtzButton)

public:
	CPtzButton();
	virtual ~CPtzButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


