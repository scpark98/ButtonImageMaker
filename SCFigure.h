/*
-라인부터 다각형까지 템플릿을 제공하고 벡터형태로 그려준다(ppt 도형과 같이)

- CSCFigure를 상속받아 CSCTriangle, CSCRect...등을 만드느냐
CSCFigure 하나로 하느냐...
만약 get_area()라는 함수를 구현한다면 if 또는 switch로 구분.
그럼 제어점은 ?
*/

#pragma once

#include <afxwin.h>
#include <gdiplus.h>

enum SCFIGURE_TYPES
{
	figure_type_line,
	figure_type_rect,
	figure_type_ellipse,
	figure_type_polygon,
};

class CSCFigure
{
public:
	CSCFigure();
	~CSCFigure();

	int				type;
	Gdiplus::Rect 	r;
	int				round[4];
	Gdiplus::Color 	cr_fill;
	int				fill_alpha;
	Gdiplus::Color 	cr_stroke;
	int				stroke_alpha;
	int				stroke_width;
};

