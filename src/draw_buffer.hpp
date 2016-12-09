
#ifndef _H_DRAW_BUFFER_
#define _H_DRAW_BUFFER_
#include "math.hpp"

#define GET_POS(x,y) (x+y*width)
const int COLORBITS = 8;
template<typename T>
class DrawBuffer
{
    T *data;
    static const int bitSize = sizeof(T);
public:
    int width;
    int heigth;
    DrawBuffer()
    {
        data = nullptr;
    }
    void alloc(int w, int h)
    {
       width = w;
       heigth = h;
       data = new T[w*h]; 
    }
    DrawBuffer(int w,int h)
    {
        alloc(w,h);
    }
    void set_alpth_pixel(int x, int y, T val)
    {
        //T &p = mix_pixel[GET_POS(x,y)];
        if(val&0x000000FF)
            data[GET_POS(x,y)] = val;

    }
    void set_range_pixel(int x0,int y0,int w,int h,T val)
    {
        int max_h = y0+h>=heigth?heigth:y0+h;
        for(int y=y0;y<max_h;++y)
        {
            set_row_pixel(x0,y,w,val);
        }
    }
    void set_row_pixel(int x0, int y0,int w,T val)
    {
        int max_w = x0+w>=width?width:x0+w;
        for(int x=x0;x<max_w;++x)
        {
            set_alpth_pixel(x,y0,val);
        }
    }

    void set_point(int x, int y, T val,bool is_revert_y=0)
    {
        if(is_revert_y)
            y = heigth - y;
        assert(x<width && y<heigth && y>=0);
        data[x+y*width] = val;
    }
    void clear_color(RGB color)
    {
        T val = color.get_rgb_val();
        mem_set(val);
    }
    void clear_color(RGBA color)
    {
        T val = color.get_rgba_val();
        mem_set(val);
    }
    void clear_color(unsigned int color=0)
    {
        mem_set(color);
    }
    void mem_set(T val)
    {
        for(int i=0;i<width*heigth;++i)
        {
            data[i] = val;
        }
    }
    void mem_cpy(T* src)
    {
        for(int i=0;i<width*heigth;++i)
        {
            data[i] = src[i];
            //printf("%X\n",src[i]);
        }
    }
    void overwrite(unsigned char* src,int x0,int y0, int w,int h)
    {
        for(int y=0;y<h;++y)
        {
            for(int x=0;x<w;++x)
            {
                int new_x = x0+x;
                int new_y = y0+y;
                if(new_x>=width||new_y>=heigth)
                    continue;
                unsigned char col = src[x+y*w];
                data[new_x + new_y*width] = RGB(col,0,0).get_rgb_val();
            }
                
        }
    }
    T* get_buf()
    {
        return data;
    }  
    ~DrawBuffer()
    {
        delete[] data;
    }
};



#endif




