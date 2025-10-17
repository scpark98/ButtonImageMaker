
// ButtonImageMakerDlg.h: 헤더 파일
//

#pragma once

#include "Common/ResizeCtrl.h"
#include "Common/CStatic/ImageStatic/ImageStatic.h"
#include "Common/CMFCCtrl/CMFCPropertyGridCtrl/CSCMFCPropertyGridCtrl/SCMFCPropertyGridCtrl.h"

// CButtonImageMakerDlg 대화 상자
class CButtonImageMakerDlg : public CDialogEx
{
// 생성입니다.
public:
	CButtonImageMakerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CResizeCtrl		m_resize;

	void			init_grid();
	LRESULT			OnPropertyChanged(WPARAM wparam, LPARAM lparam);

	void			apply_settings();
	void			adjust_position();

	CSCGdiplusBitmap m_img;

	enum PROPERTY_GRID_ITEM
	{
		id_image_width = 1,	//반드시 1 이상부터 시작해야함.(dwID를 별도로 지정하지 않은 모든 아이템이 0으로 세팅되므로 구분할 수 없음)
		id_image_height,
		id_image_alpha,
		id_image_back_color,

		id_button_width,
		id_button_height,
		id_fill_color,
		id_stroke_color,
		id_stroke_thickness,
		id_radius,

		id_shadow_depth,
		id_shadow_color,
		id_shadow_sigma,
		id_shadow_offset_x,
		id_shadow_offset_y,

		id_font,
		id_text_color,
		id_text_color_alpha,
		id_text_align,
	};

	//image properties
	int				width = 200;
	int				height = 60;
	int				alpha = 255;
	Gdiplus::Color	cr_back = Gdiplus::Color::Transparent;

	//button image properties
	int				button_width = 160;
	int				button_height = 40;
	Gdiplus::Color	cr_fill = Gdiplus::Color::RoyalBlue;
	Gdiplus::Color	cr_stroke = Gdiplus::Color::Navy;
	int				stroke_thickness = 2;
	int				radius = 10;

	//shadow properties
	int				shadow_depth = 8;
	Gdiplus::Color	cr_shadow = Gdiplus::Color::Gray;
	int				shadow_sigma = 25;	//실제 적용할때에는 10으로 나눈 값을 사용한다.
	int				shadow_offset_x = 0;
	int				shadow_offset_y = 0;


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUTTONIMAGEMAKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CSCMFCPropertyGridCtrl m_grid;
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CImageStatic m_static_img;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_check_show_image;
	CButton m_check_show_shadow;
	afx_msg void OnBnClickedCheckShowImage();
	afx_msg void OnBnClickedCheckShowShadow();
	afx_msg void OnBnClickedButtonSave();
};
