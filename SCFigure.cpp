#include "SCFigure.h"

CSCFigure::CSCFigure()
{
	r = Gdiplus::Rect(0, 0, 160, 40); 
	round[0] = round[1] = round[2] = round[3] = 10;
	cr_fill = Gdiplus::Color::RoyalBlue;
	cr_stroke = Gdiplus::Color::Navy;
	stroke_alpha = 255;
	stroke_width = 2;
	//shadow_depth = 8;
	cr_shadow = Gdiplus::Color::Gray;
	shadow_sigma = 25;	//실제 적용할때에는 10으로 나눈 값을 사용한다.
	shadow_offset_x = 2;
	shadow_offset_y = 2;
}

CSCFigure::~CSCFigure()
{

}

void CSCFigure::draw(CSCGdiplusBitmap* img, bool draw_fore, bool draw_shadow)
{
	CSCGdiplusBitmap fore;
	fore.create_round_rect(r.Width, r.Height, round[0], cr_fill, cr_stroke, stroke_width);

	//shadow 이미지에는 stroke가 적용되지 않아야 하므로 create_back_shadow_image() 함수를 사용해서는 안된다.
	CSCGdiplusBitmap shadow;

	if (draw_shadow)
	{
		shadow.create_round_rect(r.Width, r.Height, round[0], cr_fill);
		shadow.gray();
		//shadow.resize_canvas(r.Width + shadow_depth, r.Height + shadow_depth);
		shadow.blur((float)shadow_sigma / 10.0f);
		img->draw(&shadow, r.X + shadow_offset_x, r.Y + shadow_offset_y);
	}

	if (draw_fore)
		img->draw(&fore, r.X, r.Y);
}