
#ifndef _H_MATH_
#define _H_MATH_
#include <iostream>
#include <cassert>
#include <map>
#include <vector>

template<typename T, unsigned int SIZE>
struct VecBase
{
    T vec[SIZE];
};

template<typename T>
struct VecBase<T,2>
{
    union{
        T vec[2];
        struct{ T x,y;};
        struct{ T s,t;};
    };
};

template<typename T>
struct VecBase<T,3>
{
    union{
        T vec[3];
        struct{ T x,y,z;};
        struct{ T r,g,b;};
        //struct{ T s,t,r;};
    };
};

template<typename T>
struct VecBase<T,4>
{
    union{
        T vec[4];
        struct{ T x,y,z,w;};
        struct{ T r,g,b,a;};
        //struct{ T s,t,r,q;};
    };
};

template<typename T, unsigned int SIZE>
class Vec:public VecBase<T, SIZE>{
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
    T get_rgba_val()
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



#endif




