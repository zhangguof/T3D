
#ifndef _H_DRAW_BUFFER_
#define _H_DRAW_BUFFER_
#include <iostream>
#include <cassert>
#include <map>
#include <vector>



template<typename T, unsigned int SIZE>
class Vec
{
    T vec[SIZE];
    const static int size = SIZE;
public:
    Vec(){}
    Vec(T *a)
    {
        mem_cpy(a,SIZE);
    }
    Vec(T a,T b)
    {
        vec[0] = a;
        vec[1] = b;
    }
    Vec(T a,T b,T c)
    {
        vec[0] = a;
        vec[1] = b;
        vec[2] = c;
    }
    Vec(T a,T b,T c,T d)
    {
        vec[0] = a;
        vec[1] = b;
        vec[2] = c;
        vec[3] = d;
    }
    void mem_cpy(T* a, int size)
    {
        for(int i=0;i<size;++i)
        {
            vec[i] = a[i];
        }
    }
    T get_rgb_val()
    {
        assert(SIZE>=3);
        return (vec[0]<<24)|(vec[1]<<16)|(vec[2]<<8);
    }
    T get_rbga_val()
    {
        assert(SIZE>=4);
        return get_rgb_val()|(vec[3]);
    }
    T &operator[](unsigned int index)
    {
        return vec[index];
    }

};

typedef Vec<int,2> Vec2i;
typedef Vec<int,3> Vec3i;
typedef Vec<int,4> Vec4i;

typedef Vec<unsigned int,2> Vec2u;
typedef Vec<unsigned int,3> Vec3u;
typedef Vec<unsigned int,4> Vec4u;

typedef Vec<float,2> Vec2f;
typedef Vec<float,3> Vec3f;
typedef Vec<float,4> Vec4f;

typedef Vec3u RGB;
typedef Vec4u RGBA;

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




