#include "Render.hpp"
#include "text_render.hpp"

#define GET_BUFF_POS(x,y,w) ((x)+(y)*w)
//中心坐标系到buff坐标的转换
#define ADDR_X(x) (width/2+x)
#define ADDR_Y(y) (height/2-y)
// #define ADDR(x,y) ((width/2+x)+(height/2-y)*width)
#define ADDR(x,y) (ADDR_X+ADDR_Y*width)


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

//working center coord
void Render::setPixel(int x, int y, Color color)
{
	pframe_buffer->set_alpha_pixel(ADDR_X(x),ADDR_Y(y),color);
}
void Render::draw_point(int x,int y, Color color)
{
	pframe_buffer->set_alpha_pixel(ADDR_X(x),ADDR_Y(y),color);
}

void Render::draw_line(int x0,int y0,int x1,int y1,Color color)
{
	//DDA draw line, y=mx+b
	int dx = x1-x0;
	int dy = y1-y0;
	int setps;
	float xinc,yinc;
	//m>1
	if(fabs(dx)>fabs(dy))
	{
		//inc in y.
		setps = fabs(dx);
	}
	else
	{
		setps = fabs(dy);
	}
	xinc = float(dx)/float(setps);
	yinc = float(dy)/float(setps);
	float x=x0;
	float y=y0;
	setPixel(round(x), round(y),color);
	for(int k=0;k<setps;++k)
	{
		x+=xinc;
		y+=yinc;
		setPixel(round(x),round(y),color);
	}
}

void Render::draw_line(Vec2i p0,Vec2i p1,Color color)
{
	draw_line(p0[0],p0[1],p1[0],p1[1],color);
}

void Render::draw_polygon(int points[],int cnt, Color color)
{
	for(int i=1;i<cnt;++i)
	{
		draw_line(points[i*2],points[i*2+1],points[(i-1)*2],points[(i-1)*2+1],color);
	}
	draw_line(points[(cnt-1)*2],points[(cnt-1)*2+1],points[0],points[1],color);

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