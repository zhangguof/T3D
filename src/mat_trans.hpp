#ifndef _H_MAT_TRANS
#define _H_MAT_TRANS
#include "math.hpp"

template<typename T,unsigned int N>
Vec<T,N> mat_trans(MatNxM<T,N,N> &m, Vec<T,N> v)
{
	return m * v;
}





#endif