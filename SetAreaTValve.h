#pragma once


#include "commonFile.h"


// CSetAreaTValve dialog

class CSetAreaTValve : public CDialog
{
	DECLARE_DYNAMIC(CSetAreaTValve)

public:
	CSetAreaTValve(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetAreaTValve();

// Dialog Data
	enum { IDD = IDD_SETTVALVE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_SelAreaCob;
	UINT m_uSaveSelArea;
	int m_iHighLimitEdit;
	int m_iLowLimitEdit;
	virtual BOOL OnInitDialog();
private:
	AreaTParaSet* m_AreaTParaSet;
public:
	AreaTParaSet* GetAreaTParaSetPointer(void);
	void SetIAreaTLimit(AreaTParaSet* setAreaTLimit,UINT nAreaNum);
	afx_msg void OnSelchangeAreaCob();
	afx_msg void OnChangeLowlimitEdit();
	afx_msg void OnChangeHighlimitEdit();
};
