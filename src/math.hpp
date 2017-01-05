
#ifndef _H_MATH_
#define _H_MATH_
#include <iostream>
#include <cassert>
#include <map>
#include <vector>
#include <cmath>

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
    const static int size = SIZE;
    typedef T value_type;
    typedef Vec<T,SIZE> vec_type;
    typedef vec_type & vec_reference;
    typedef vec_type const & vec_reference_const; 
public:
    Vec(){}
    Vec(T *a)
    {
        mem_cpy(a,SIZE);
    }
    Vec(vec_type const &v)
    {
        for(int i=0;i<size;++i)
        {
            this->vec[i] = v.vec[i];
        }
    }
    Vec(T a,T b)
    {
        this->vec[0] = a;
        this->vec[1] = b;
    }
    Vec(T a,T b,T c)
    {
        this->vec[0] = a;
        this->vec[1] = b;
        this->vec[2] = c;
    }
    Vec(T a,T b,T c,T d)
    {
        this->vec[0] = a;
        this->vec[1] = b;
        this->vec[2] = c;
        this->vec[3] = d;
    }
    void mem_cpy(T* a, int size)
    {
        for(int i=0;i<size;++i)
        {
            this->vec[i] = a[i];
        }
    }
    T get_rgb_val()
    {
        assert(SIZE>=3);
        return (this->vec[0]<<24)|(this->vec[1]<<16)|(this->vec[2]<<8);
    }
    T get_rgba_val()
    {
        assert(SIZE>=4);
        return get_rgb_val()|(this->vec[3]);
    }
    T &operator[](unsigned int index)
    {
        return this->vec[index];
    }

    vec_reference operator+=(T scala)
    {
        for(int i=0;i<size;++i)
            this->vec[i]+=scala;
        return *this;
    }
    vec_reference operator+=(vec_type const &v)
    {
        for (int i = 0; i < size; ++i)
        {
            this->vec[i]+=v[i];
        }
        return *this;
    }
    vec_type operator+(vec_type const &v)
    {
        vec_type r;
        for(int i=0;i<size;++i)
        {
            r[i] = this->vec[i] + v.vec[i];
        }
        return r;
    }
    vec_type operator-(vec_type const &v)
    {
        vec_type r;
        for(int i=0;i<size;++i)
        {
            r[i] = this->vec[i] - v.vec[i];
        }
        return r;
    }


    T dot(vec_reference_const &v)
    {
        T r = static_cast<T>(0);
        for(int i=0;i<size;++i)
        {
            r += this->vec[i] * v.vec[i];
        }
        return r;
    }

    double dist()
    {
        double d = 0;
        for(int i=0;i<size;++i)
        {
            d+= this->vec[i] * this->vec[i];
        }
        return std::sqrt(d);
    }

    vec_type normal()
    {
        vec_type v(&this);
        double d = this->dist();
        
        assert(d>0);
        v *= 1.0/d;
        return v;
    }

    void print_vec()
    {
        for(int i=0;i<size;++i)
        {
            std::cout<<this->vec[i]<<" ";
        }
        std::cout<<std::endl;
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

template<typename T>
class Mat2x2
{
    const static int size = 2*2;
    const static int row_num = 2;
    const static int col_num = 2;
    
public:
    T mat[size];
    Mat2x2(){}
    Mat2x2(T m00, T m01, T m10, T m11)
    {
        mat[0] = m00;
        mat[1] = m01;
        mat[2] = m10;
        mat[3] = m11;
    }
    Mat2x2(Mat2x2<T> const & m)
    {
        for(int i=0;i<size;++i)
        {
            mat[i] = m.mat[i];
        }
    }
};

template<typename T,unsigned int N,unsigned int M>
class MatNxM
{
    const static int size = N*M;
    const static int row_num = N;
    const static int col_num = M;
    typedef MatNxM<T,N,M> mat_type;
    typedef mat_type & mat_reference;
    typedef mat_type const & mat_reference_const;
    typedef Vec<T,col_num> row_type;
    typedef Vec<T,row_num> col_type;
    
public:
    T mat[size];
    MatNxM(){}
    MatNxM(T *a)
    {
        for (int i = 0; i < size; ++i)
        {
            mat[i]=a[i];
        }
    }

    MatNxM(mat_reference_const m)
    {
        for(int i=0;i<size;++i)
        {
            mat[i] = m.mat[i];
        }
    }

    T get(int n,int m)
    {
        assert(n<row_num && m<col_num);
        return mat[n*col_num + m];
    }
    void set(int n,int m, T val)
    {
        assert(n<row_num && m<col_num);
        mat[n*col_num + m] = val;
    }
    row_type get_row(int n)
    {
        assert(n<row_num);
        row_type v(&mat[n*col_num]);
        return v;
    }
    col_type get_col(int m)
    {
        assert(m<col_num);
        col_type v;
        for(int i=0;i<row_num;++i)
        {
            //v[i] = this->get(i,m);
            v[i] = mat[i*col_num + m];
        }
        return v;
    }

    mat_reference operator+=(mat_reference_const &m)
    {
        assert(this->row_num == m.row_num && this->col_num==m.col_num);
        for(int i=0;i<size;++i)
        {
            this->mat[i] += m.mat[i];
        }
        return *this;
        
    }


    mat_reference operator-=(mat_reference_const &m)
    {
        assert(this->row_num == m.row_num && this->col_num==m.col_num);
        for(int i=0;i<size;++i)
        {
            this->mat[i] -= m.mat[i];
        }
        return *this;
        
    }

    mat_type operator+(mat_reference_const &m)
    {
        assert(this->row_num == m.row_num && this->col_num==m.col_num);
        mat_type r;
        for (int i = 0; i < size; ++i)
        {
            r.mat[i] = this->mat[i] + m.mat[i];
        }
    }


    mat_type operator-(mat_reference_const &m)
    {
        assert(this->row_num == m.row_num && this->col_num==m.col_num);
        mat_type r;
        for (int i = 0; i < size; ++i)
        {
            r.mat[i] = this->mat[i] - m.mat[i];
        }
    }

    template<unsigned int R>
    MatNxM<T,N,R> mul(MatNxM<T,M,R> &m)
    {
        //std::cout<<"do mul:"<<N<<" "<<M<<" "<<R<<std::endl;
        MatNxM<T,N,R> r_mat;
        for(int i=0;i<N;++i)
        {
            Vec<T,M> row_i = this->get_row(i);
            //std::cout<<"in row:"<<i<<std::endl;
            //row_i.print_vec();
            for(int j=0;j<R;++j)
            {
                //std::cout<<"in col:"<<j<<std::endl;
                //m.get_col(j).print_vec();
                T cij = row_i.dot(m.get_col(j));
                //std::cout<<"sum:"<<cij<<std::endl;
                r_mat.set(i,j,cij);
            }
        }
        return r_mat;
    }

    MatNxM<T,N,1> operator*(MatNxM<T,M,1> &m)
    {
        return this->mul<1>(m);
    }
    MatNxM<T,N,N> operator*(MatNxM<T,M,N> &m)
    {
        return this->mul<N>(m);
    }

    void print_mat()
    {
        for (int i = 0; i < row_num; ++i)
        {
            for(int j=0;j<col_num;++j)
            {
                std::cout<<this->get(i,j)<<" ";
            }
            std::cout<<std::endl;
        }
    }
};



#endif




