#include "Render.hpp"
#include "text_render.hpp"

#define GET_BUFF_POS(x,y,w) ((x)+(y)*(w))

Render::Render(int w,int h)
{
	width 	= w;
	height 	= h;
	point_size = 1;

	pframe_buffer = new FrameBuffer(w,h);
	pdev = new Device(w,h);
	pdev->set_buf(pframe_buffer);
}

void Render::init_render()
{
	pdev->init_render();
}
void Render::set_buf(FrameBuffer* p_buff)
{
	pframe_buffer = p_buff;
	pdev->set_buf(pframe_buffer);
}

void Render::set_point_size(int size)
{
	point_size = size;
}

void Render::init_pen_font(const char* s,int size)
{
	init_font(s,size);
}
void Render::set_pen_size(int size)
{
	set_font_size(size);
}

void Render::draw_text(const char* s,int x,int y, Color color)
{
	render_text(pframe_buffer,s,x,y,color);
}

void Render::load_r8b8g8a8_bitmap(unsigned int* pbuff,int w,int h,int x0,int y0)
{
	int max_w = (x0+w>=width)?width:x0+w;
	int max_h = (y0+h>=height)?height:y0+h;
	for(int y=y0;y<max_h;++y)
	{
		for(int x=x0;x<max_w;++x)
		{
			pframe_buffer->set_alpha_pixel(x,y,pbuff[GET_BUFF_POS(x-x0,y-y0,w)]);
		}
	}
}


void Render::draw_point(int x,int y, Color color)
{

}
void Render::draw_line(int x0,int y0,int x1,int y1,Color color)
{

}

FrameBuffer* Render::get_buff()
{
	return pframe_buffer;
}

void Render::clear_buff_color(Color col)
{
	pframe_buffer->clear_color(col);
}
void Render::render(double interver)
{
	pdev->render(interver);

}


Render::~Render()
{
	delete pframe_buffer;
	delete pdev;
}