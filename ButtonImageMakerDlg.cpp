
// ButtonImageMakerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ButtonImageMaker.h"
#include "ButtonImageMakerDlg.h"
#include "afxdialogex.h"

#include "Common/Functions.h"
#include "Common/MemoryDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CButtonImageMakerDlg 대화 상자



CButtonImageMakerDlg::CButtonImageMakerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BUTTONIMAGEMAKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CButtonImageMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_grid);
	DDX_Control(pDX, IDC_STATIC_IMG, m_static_img);
	DDX_Control(pDX, IDC_CHECK_SHOW_IMAGE, m_check_show_image);
	DDX_Control(pDX, IDC_CHECK_SHOW_SHADOW, m_check_show_shadow);
}

BEGIN_MESSAGE_MAP(CButtonImageMakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CButtonImageMakerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CButtonImageMakerDlg::OnBnClickedCancel)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_BN_CLICKED(IDC_CHECK_SHOW_IMAGE, &CButtonImageMakerDlg::OnBnClickedCheckShowImage)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SHADOW, &CButtonImageMakerDlg::OnBnClickedCheckShowShadow)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CButtonImageMakerDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CButtonImageMakerDlg 메시지 처리기

BOOL CButtonImageMakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_resize.Create(this);
	m_resize.Add(IDC_GRID, 0, 0, 0, 100);
	m_resize.Add(IDC_CHECK_SHOW_IMAGE, 0, 100, 0, 0);
	m_resize.Add(IDC_CHECK_SHOW_SHADOW, 0, 100, 0, 0);
	m_resize.Add(IDC_BUTTON_SAVE, 100, 100, 0, 0);

	init_grid();

	m_img.create(width, height, cr_back);

	m_check_show_image.SetCheck(BST_CHECKED);
	m_check_show_shadow.SetCheck(BST_CHECKED);

	apply_settings();


	RestoreWindowPosition(&theApp, this);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CButtonImageMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CButtonImageMakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialogEx::OnPaint();
		CPaintDC dc1(this);
		CRect rc;

		GetClientRect(rc);

		CMemoryDC dc(&dc1, &rc);
		Gdiplus::Graphics g(dc.m_hDC);

		g.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
		g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

		dc.FillSolidRect(rc, GetSysColor(COLOR_3DFACE));
		/*
		if (m_img.is_valid())
		{
			CRect r = get_ratio_rect(m_r_img_area, m_img.width, m_img.height);
			m_img.draw(g, r, CSCGdiplusBitmap::draw_mode_original_center);
		}
		*/
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CButtonImageMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CButtonImageMakerDlg::OnBnClickedOk()
{
}

void CButtonImageMakerDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CButtonImageMakerDlg::init_grid()
{
	m_grid.EnableDescriptionArea();
	m_grid.set_color_theme(CSCMFCPropertyGridCtrl::theme_gray);

	CMFCPropertyGridProperty* pGroup;
	CMFCPropertyGridProperty* pProperty = NULL;


	pGroup = new CMFCPropertyGridProperty(_T("Image Properties"));
	m_grid.AddChildItem(pGroup, _T("width"), width, _T("image width"), true, id_image_width, true, 10, 800);
	m_grid.AddChildItem(pGroup, _T("height"), height, _T("image height"), true, id_image_height, true, 10, 600);
	m_grid.AddSliderItem(pGroup, _T("alpha"), alpha, id_image_alpha, 0, 255);
	m_grid.AddColorSelect(pGroup, _T("back color"), cr_back.ToCOLORREF(), id_image_back_color);
	pGroup->Expand();
	m_grid.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Button Properties"));
	m_grid.AddChildItem(pGroup, _T("button width"), button_width, _T(""), true, id_button_width, true, 10, width);
	m_grid.AddChildItem(pGroup, _T("button height"), button_height, _T(""), true, id_button_height, true, 10, height);
	m_grid.AddColorSelect(pGroup, _T("fill color"), cr_fill.ToCOLORREF(), id_fill_color);
	m_grid.AddColorSelect(pGroup, _T("stroke color"), cr_stroke.ToCOLORREF(), id_stroke_color);
	m_grid.AddChildItem(pGroup, _T("stroke thickness"), stroke_thickness, _T(""), true, id_stroke_thickness, true, 0, 8);
	m_grid.AddSliderItem(pGroup, _T("round radius"), radius, id_radius, 0, button_height / 2.0);
	pGroup->Expand();
	m_grid.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Shadow Properties"));
	m_grid.AddChildItem(pGroup, _T("shadow_depth"), shadow_depth, _T(""), true, id_shadow_depth, true, 0, 20);
	m_grid.AddChildItem(pGroup, _T("shadow sigma"), shadow_sigma, _T("실제 적용시에는 1/10 값으로 적용됨"), true, id_shadow_sigma, true, 0, 200);
	m_grid.AddChildItem(pGroup, _T("shadow offset x"), shadow_offset_x, _T(""), true, id_shadow_offset_x, true, -20, 20);
	m_grid.AddChildItem(pGroup, _T("shadow offset y"), shadow_offset_y, _T(""), true, id_shadow_offset_y, true, -20, 20);
	pGroup->Expand();
	m_grid.AddProperty(pGroup);


	m_grid.SetLeftColumnWidth(theApp.GetProfileInt(_T("setting"), _T("property width"), 150));
}

LRESULT CButtonImageMakerDlg::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	CMFCPropertyGridProperty* pProperty = (CMFCPropertyGridProperty*)lparam;

	if (pProperty == NULL)
		return 0;

	DWORD	dwData = pProperty->GetData();
	TRACE(_T("dwData = %d\n"), dwData);

	switch (dwData)
	{
		case id_image_width:
		{
			int nw = getPropValue(pProperty);
			int h = m_img.height;
			width = nw;
			//m_img.resize_canvas(nw, h);
			break;
		}
		case id_image_height:
		{
			int w = m_img.width;
			int nh = getPropValue(pProperty);
			height = nh;
			; m_img.resize_canvas(w, nh);
			break;
		}
		case id_image_alpha:
		{
			int a = getPropValue(pProperty);
			m_img.set_alpha(a);
			break;
		}
		case id_image_back_color:
		{
			COLORREF cr = getPropValue(pProperty);
			cr_back = Gdiplus::Color(alpha, GetRValue(cr), GetGValue(cr), GetBValue(cr));
			break;
		}
		case id_button_width:
		{
			button_width = getPropValue(pProperty);
			break;
		}
		case id_button_height:
		{
			button_height = getPropValue(pProperty);
			m_grid.set_slider_range(id_radius, 0, button_height / 2.0f);
			break;
		}
		case id_fill_color:
		{
			COLORREF cr = getPropValue(pProperty);
			cr_fill = Gdiplus::Color(GetRValue(cr), GetGValue(cr), GetBValue(cr));
			break;
		}
		case id_stroke_color:
		{
			COLORREF cr = getPropValue(pProperty);
			cr_stroke = Gdiplus::Color(GetRValue(cr), GetGValue(cr), GetBValue(cr));
			break;
		}
		case id_stroke_thickness:
		{
			stroke_thickness = getPropValue(pProperty);
			break;
		}
		case id_radius:
		{
			float r = getPropValue(pProperty);
			radius = (int)r;
			break;
		}
		case id_shadow_depth:
		{
			shadow_depth = getPropValue(pProperty);
			break;
		}
		case id_shadow_sigma:
		{
			shadow_sigma = getPropValue(pProperty);
			break;
		}
		case id_shadow_offset_x:
		{
			shadow_offset_x = getPropValue(pProperty);
			break;
		}
		case id_shadow_offset_y:
		{
			shadow_offset_y = getPropValue(pProperty);
			break;
		}
	}

	apply_settings();

	return 0;
}

void CButtonImageMakerDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}

BOOL CButtonImageMakerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CButtonImageMakerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	adjust_position();
}

void CButtonImageMakerDlg::adjust_position()
{
	if (m_static_img.m_hWnd == NULL)
		return;

	CRect rc;
	GetClientRect(rc);

	CRect rgrid;
	m_grid.GetWindowRect(&rgrid);
	ScreenToClient(&rgrid);

	CRect rImageArea = rc;
	rImageArea.DeflateRect(12, 12);
	rImageArea.left = rgrid.right + 12;

	CRect r = get_ratio_rect(rImageArea, m_img.width, m_img.height, 0, false);

	m_static_img.MoveWindow(r);
	m_static_img.Invalidate();
	//Invalidate();
}

BOOL CButtonImageMakerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case 'S':
				if (!IsCtrlPressed())
					break;
				m_img.save(_T("d:\\img.png"));
				break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CButtonImageMakerDlg::apply_settings()
{
	CSCGdiplusBitmap btn_img;
	btn_img.create_round_rect(button_width, button_height, radius, cr_fill, cr_stroke, stroke_thickness);

	//shadow 이미지에는 stroke가 적용되지 않아야하므로 create_back_shadow_image() 함수를 그냥 사용해서는 안된다.
	CSCGdiplusBitmap shadow;
	//btn_img.create_back_shadow_image(&shadow, shadow_sigma, 0, shadow_depth);
	if (shadow_depth > 0)
	{
		shadow.create_round_rect(button_width, button_height, radius, cr_fill);
		shadow.gray();
		shadow.resize_canvas(button_width + shadow_depth, button_height + shadow_depth);
		shadow.blur((float)shadow_sigma / 10.0f);
	}

	m_img.create(width, height, cr_back);
	
	int x = (m_img.width - btn_img.width) / 2;
	int y = (m_img.height - btn_img.height) / 2;

	if (shadow_depth > 0 && m_check_show_shadow.GetCheck() == BST_CHECKED)
		m_img.draw(&shadow, x - shadow_depth / 2 + shadow_offset_x, y - shadow_depth / 2 + shadow_offset_y);
	if (m_check_show_image.GetCheck() == BST_CHECKED)
		m_img.draw(&btn_img, x, y);

	m_static_img.set_image(m_img);
	
	adjust_position();
}
void CButtonImageMakerDlg::OnBnClickedCheckShowImage()
{
	apply_settings();
}

void CButtonImageMakerDlg::OnBnClickedCheckShowShadow()
{
	apply_settings();
}

void CButtonImageMakerDlg::OnBnClickedButtonSave()
{
	CString recent = theApp.GetProfileString(_T("setting"), _T("recent saved file"), get_exe_directory() + _T("\\image_button.png"));
	CFileDialog dlg(FALSE, _T("png"), recent, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("PNG Files (*.png)|*.png|All Files (*.*)|*.*||"), this);
	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		m_img.save(path);
		theApp.WriteProfileString(_T("setting"), _T("recent saved file"), path);
	}
}
