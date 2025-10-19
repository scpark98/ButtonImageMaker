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

