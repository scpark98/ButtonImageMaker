/*
-라인부터 다각형까지 템플릿을 제공하고 벡터형태로 그려준다(ppt 도형과 같이)

- CSCFigure를 상속받아 CSCTriangle, CSCRect...등을 만드느냐
CSCFigure 하나로 하느냐...
만약 get_area()라는 함수를 구현한다면 if 또는 switch로 구분.
그럼 제어점은 ?
*/

#pragma once

#include <afxwin.h>
#include "Common/SCGdiplusBitmap.h"


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

	//타깃에 현재 도형의 속성대로 그려준다.
	void			draw(Gdiplus::Graphics& g);
	void			draw(CSCGdiplusBitmap* img, bool draw_fore = true, bool draw_shadow = true);

	int				type;
	Gdiplus::Rect	r;					//도형은 자기 스스로의 위치를 기억할 수 있어야 하므로 w, h가 아닌 Rect로 정의함.
	int				round[4];	
	Gdiplus::Color 	cr_fill;
	int				fill_alpha;
	Gdiplus::Color 	cr_stroke;
	int				stroke_alpha;
	int				stroke_width;
	//int				shadow_depth;
	Gdiplus::Color	cr_shadow;
	int				shadow_sigma;
	int				shadow_offset_x;
	int				shadow_offset_y;
};

