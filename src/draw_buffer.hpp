
#ifndef _H_DRAW_BUFFER_
#define _H_DRAW_BUFFER_
#include "math.hpp"


const int COLORBITS = 8;
template<typename T>
class DrawBuffer
{
    T *data;
    int width;
    int heigth;
public:
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
    void set_point(int x, int y, T val)
    {
        assert(x<width && y<heigth);
        data[x+y*width] = val;
    }
    void clear_color(RGB color)
    {
        T val = color.get_rgb_val();
        for(int i=0;i<width*heigth;++i)
        {
            data[i]=val;
        }
    }
    void mem_cpy(T* src)
    {
        for(int i=0;i<width*heigth;++i)
        {
            data[i] = src[i];
            printf("%X\n",src[i]);
        }
    }
    void overwrite(void* src,int x0,int y0, int w,int h)
    {
        for(int y=0;y<h;++y)
        {
            for(int x=0;x<w;++x)
            {
                int new_x = x0+x;
                int new_y = y0+y;
                if(new_x>=width||new_y>=heigth)
                    continue;
                data[new_x + new_y*width] = RGB(255,0,0).get_rgb_val();//(((unsigned char*)src)[x+y*w])<<24;
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




