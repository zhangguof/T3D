#ifndef _H_MAT_TRANS
#define _H_MAT_TRANS
#include "math.hpp"


template<typename T,unsigned int N>
class MatTran
{
	typedef MatNxM<T,N,N> mat_type;
	typedef mat_type & mat_referent;
	typedef Vec<T,N> vec_type;
public:
	mat_type mat;
	MatTran(){
		mat.identity();
	}
	MatTran(mat_type m){
		mat = m;
	}
	mat_referent get_mat(){
		return mat;
	}
	mat_referent make_identity()
	{
		mat.identity();
		return mat;
	}
	mat_referent mat_trans(mat_referent &m)
	{
		mat *= m;
		return mat;
	}
	vec_type trans(vec_type &v)
	{
		return mat * v;
	}
	vec_type vertex_trans(mat_referent m, vec_type v)
	{
		return m * v;
	}

	void mat_move_to(vec_type v)
	{
		static_assert(N==3,"mat_move_to_xy,use in 3 dim;");
		T data[9] = {
			1,0,v[0],
			0,1,v[1],
			0,0,1,
		};
		mat_type m(data);
		mat *=m;
	}
	void mat_rotate_degree(float degree)
	{
		mat_rotate_radian(degree/180.0*PI);
	}
	void mat_rotate_radian(float radian)
	{
		static_assert(N==3,"use in 3 dim;");
		float sina = sin(radian);
		float cosa = cos(radian);

		float data[9] = {
			cosa,-sina,0,
			sina,cosa,0,
			0,0,1.0,
		};
		MatNxM<float,3,3> m(data);
		mat *=m;
	}
};








#endif