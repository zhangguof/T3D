#ifndef _H_RENDER_
#define _H_RENDER_
#include "T3D.hpp"


class Render
{
	Device* pdev;
	FrameBuffer* pframe_buffer;
	int point_size;
public:
	int width;
	int height;

	Render(int w,int h);
	void init_render();
	void set_buf(FrameBuffer* p_buff);
	void set_point_size(int size);
	FrameBuffer* get_buff();
	
	void load_r8b8g8a8_bitmap(unsigned int* pbuff,int w,int h);

	void init_pen_font(const char* s,int size);
	void set_pen_size(int size);
	void draw_text(const char* s,int x,int y, Color color);
	
	void draw_point(int x,int y, Color color);
	void draw_line(int x0,int y0,int x1,int y1,Color color);
	
	void load_r8b8g8a8_bitmap(unsigned int* pbuff,int w,int h,int x0,int y0);
	void clear_buff_color(Color col=0);



	void render(double interver);

	~Render();


};



#endif