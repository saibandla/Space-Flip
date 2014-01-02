#pragma once

#ifndef _SK_POINT_2_
#define _SK_POINT_2_

#include "sk_math.h"
#include "sk_assert.h"

#ifdef SK_QT
	 #include <QPoint>
	 #include <QPointF>
#endif

namespace sk
{

template <typename ST>
struct Point_2
{
public:
	typedef ST                            scalar_type;
	typedef sk::Point_2<scalar_type>      point_type;
	typedef point_type                    vector_type;

public:
	scalar_type x;
	scalar_type y;

public:

	inline Point_2()
	{
		// by default values are uninitialized
	}

	inline Point_2(scalar_type _s)
		:
	x(_s), y(_s)
	{
	}

    inline Point_2(const scalar_type _x, const scalar_type _y)
        :
    x(_x), y(_y)
    {
    }

public:

	inline scalar_type getX() const
	{
		return x;
	}

	inline scalar_type getY() const
	{
		return y;
	}

	void zero()
	{
		x = y = 0;
	}

	inline static Point_2 Zero()
	{
		return point_type(0, 0);
	}

    inline static Point_2 Up()
	{
		return point_type(0, 1);
	}

    inline static Point_2 Down()
	{
		return point_type(0, -1);
	}

    inline static Point_2 Left()
	{
		return point_type(-1, 0);
	}

    inline static Point_2 Right()
	{
		return point_type(1, 0);
	}

	inline static Point_2 RightUp()
	{
		return point_type(1, 1);
	}

	inline static Point_2 RightDown()
	{
		return point_type(1, -1);
	}

	inline static Point_2 LeftUp()
	{
		return point_type(-1, 1);
	}

	inline static Point_2 LeftDown()
	{
		return point_type(-1, -1);
	}

	inline static void order(point_type& pt0, point_type& pt1)
	{
		if (pt0.x == pt1.x)
		{
			if (pt0.y > pt1.y)
			{
				sk_swap(pt0, pt1);
			}
		}
		else
		{
			if (pt0.x > pt1.x)
			{
				sk_swap(pt0, pt1);
			}
		}
		assert(pt0.x <= pt1.x);
	}

	template<typename deg_type>
	inline static Point_2 fromPolar(scalar_type r, deg_type degrees)
	{
		scalar_type rad = toRadians(scalar_type(degrees));
		return fromPolarRadians(r, rad);
	}

	inline static Point_2 fromPolarRadians(scalar_type r, scalar_type rad)
	{
		scalar_type cs = cos(rad);
		scalar_type sn = sin(rad);
		return point_type(r * cs, r * sn);
	}

	// for vector this return angle in standard mathematic notation
	// for point this is operation has no sense
	inline scalar_type degrees() const
	{
		return toDegrees(radians());
	}
	inline scalar_type radians() const
	{
		if (this->y != 0)
		{
			if (this->x != 0)
			{
				const scalar_type _tg =
					((this->x > 0 && this->y > 0) || (this->x < 0 && this->y < 0))
					?	sk::abs(this->y / this->x)
					:	sk::abs(this->x / this->y);

				const scalar_type _atan = static_cast<scalar_type>(atan(_tg));
				scalar_type diff = 0;	// для первой четверти

				if ((this->x < 0) && (this->y > 0))
				{
					diff = toRadians(scalar_type(90));
				}
				else if ((this->x < 0) && (this->y < 0))
				{
					diff = toRadians(scalar_type(180));
				}
				else if ((this->x > 0) && (this->y < 0))
				{
					diff = toRadians(scalar_type(270));
				}
				return _atan + diff;
			}
			else
			{
				if (this->y >= 0)
				{
					return toRadians(scalar_type(90));
				}
				else
				{
					return toRadians(scalar_type(270));
				}
			}
		}
		else
		{
			if (this->x >= 0)
			{
				return scalar_type(0);
			}
			else
			{
				return toRadians(scalar_type(180));
			}
		}
	}

	inline void operator  *= (scalar_type m)
	{
		x *= m;
		y *= m;
	}

	inline void operator  /= (scalar_type denom)
	{
		scalar_type ooDenom = scalar_type(1) / denom;
		*this *= ooDenom;
	}

	// remember, that you MUST try to never call this function
	inline scalar_type length() const
	{
		return sqrt(x * x + y * y);
	}

	// try to use this function instead of length()
	inline scalar_type lengthSqr() const
	{
		return x * x + y * y;
	}

	inline void normalizeSelf()
	{
		scalar_type l = length();
		*this /= l;
	}

    inline void operator = (scalar_type all)
    {
        x = y = all;
    }

    inline void operator += (scalar_type c)
    {
        x += c;
        y += c;
    }

    inline void operator += (const point_type& pt)
    {
        x += pt.x;
        y += pt.y;
    }

	inline bool operator == (const point_type& pt) const
	{
		return x == pt.x && y == pt.y;
	}

	inline bool operator < (const point_type& pt) const
	{
		if (x == pt.x)
		{
			return y < pt.y;
		}
		return x < pt.x;
	}
/*
	inline operator size_t () const
	{
		int t = sizeof(*this) >> 2;
		assert(0 == (sizeof(*this) & 3));  // will not work with smaller types
		size_t hash = 0;
		const size_t* ptr = reinterpret_cast<const size_t*>(this);
		for (int i = 0; i < t; ++i)
		{
			hash ^= *ptr++;
		}
		return hash;
	}
*/
    inline static const point_type mirrorX(const point_type& pt)
    {
        return point_type(-pt.x, pt.y);
    }

    inline static const point_type mirrorY(const point_type& pt)
    {
        return point_type(pt.x, -pt.y);
    }

    inline static const point_type mirrorXY(const point_type& pt)
    {
        return point_type(-pt.x, -pt.y);
    }

	inline friend point_type operator - (const point_type& pt)
	{
		return point_type(-pt.x, -pt.y);
	}

	inline friend point_type operator + (const point_type& lhv, const point_type& rhv)
	{
		return point_type(lhv.x + rhv.x, lhv.y + rhv.y);
	}

	inline friend point_type operator - (const point_type& lhv, const point_type& rhv)
	{
		return point_type(lhv.x - rhv.x, lhv.y - rhv.y);
	}

	inline friend point_type operator * (const point_type& lhv, const point_type& rhv)
	{
		return point_type(lhv.x * rhv.x, lhv.y * rhv.y);
	}

	inline friend point_type operator * (const point_type& lhv, const scalar_type s)
	{
		return point_type(lhv.x * s, lhv.y * s);
	}

	inline friend point_type operator * (const scalar_type s, const point_type& lhv)
	{
		return lhv * s;
	}

	inline friend point_type operator / (const point_type& lhv, const point_type& rhv)
	{
		return point_type(lhv.x / rhv.x, lhv.y / rhv.y);
	}

	inline friend point_type operator / (const point_type& lhv, const scalar_type s)
	{
		ST denom = ST(1) / s;
		return lhv * denom;
	}

	static inline vector_type normalize(const vector_type& v)
	{
		vector_type res = v;
		res.normalizeSelf();
		return res;
	}

	inline bool abs_components_less(scalar_type v) const
	{
		assert(v > 0);
		if (abs(x) >= v)
		{
			return false;
		}
		return y < v;
	}


	static inline scalar_type distance_sqr(const point_type& lhv, const point_type& rhv)
	{
		vector_type d = lhv - rhv;
		return d.x * d.x + d.y * d.y;
	}

	static inline scalar_type dot(const vector_type& lhv, const scalar_type rhvX, const scalar_type rhvY)
	{
		return lhv.x * rhvX + lhv.y * rhvY;
	}

	static inline vector_type project(const vector_type& v, const vector_type& onWhat)
	{
		scalar_type d = dot(v, onWhat) / onWhat.lengthSqr();
		return d * onWhat;
	}

	static inline vector_type project_perp(const vector_type& v, const vector_type& onWhat)
	{
		return v - project(v, onWhat);
	}

	static inline vector_type project_unit(const vector_type& v, const vector_type& onWhatUnit)
	{
		scalar_type d = dot(v, onWhatUnit);
		return d * onWhatUnit;
	}

	static inline vector_type project_perp_unit(const vector_type& v, const vector_type& onWhatUnit)
	{
		return v - project_unit(v, onWhatUnit);
	}

	static inline vector_type perp_ccw(const vector_type& v)
	{
		return vector_type(-v.y, v.x);
	}

	static inline vector_type perp_cw(const vector_type& v)
	{
		return vector_type(v.y, -v.x);
	}

	static inline bool are_parallel(const vector_type& v0, const vector_type& v1, scalar_type eps)
	{
		scalar_type d = v0.x * v1.y - v0.y * v1.x;
		return sk::abs(d) < eps;
	}

	static inline bool are_othogonal(const vector_type& v0, const vector_type& v1, scalar_type eps)
	{
		scalar_type d = dot(v0, v1);
		return sk::abs(d) < eps;
	}
	// return signed area of parallelogram spanned of vectors,
	// are is positive it turn from v0 to v1 is CCW

	static inline scalar_type signed_area(const vector_type& v0, const vector_type& v1)
	{
		return v0.x * v1.y - v0.y * v1.x;
	}

	static inline scalar_type signed_area(scalar_type v0x, scalar_type v0y, scalar_type v1x, scalar_type v1y)
	{
		return signed_area(point_type(v0x, v0y), point_type(v1x, v1y));
	}

	static inline scalar_type signed_area(scalar_type v0x, scalar_type v0y, const vector_type& v1)
	{
		return signed_area(point_type(v0x, v0y), v1);
	}

	static inline scalar_type signed_area(const vector_type& v0, scalar_type v1x, scalar_type v1y)
	{
		return signed_area(v0, point_type(v1x, v1y));
	}

	static inline bool eq(const point_type& pt0, const point_type& pt1)
	{
		if (pt0.x != pt1.x)
		{
			return false;
		}
		if (pt0.y != pt1.y)
		{
			return false;
		}
		return true;
	}

	static inline bool eq_manh(const point_type& pt0, const point_type& pt1, scalar_type eps)
	{
		if (sk::abs(pt0.x - pt1.x) > eps)
		{
			return false;
		}
		if (sk::abs(pt0.y - pt1.y) > eps)
		{
			return false;
		}
		return true;
	}



	static inline bool eq(const point_type& pt0, scalar_type pt1x, scalar_type pt1y)
	{
		return eq(pt0, point_type(pt1x, pt1y));
	}

	static inline point_type point_at_ratio(const point_type& pt0, const point_type&pt1, scalar_type r0, scalar_type r1)
	{
		return point_at_ratio(pt0, pt1, r0 / (r0 + r1));
	}

	static inline point_type point_at_ratio(const point_type& pt0, const point_type&pt1, scalar_type r)
	{
		return pt0 + r * (pt1 - pt0);
	}

	inline scalar_type center() const
	{
		return half(x + y);
	}

	inline point_type inorder() const
	{
		if (x <= y)
		{
			return point_type(x, y);
		}
		return point_type(y, x);
	}

	inline point_type unorder() const
	{
		if (x > y)
		{
			return point_type(x, y);
		}
		return point_type(y, x);
	}

	inline bool is_inorder() const
	{
		return x <= y;
	}

	// unsafe function, proper order is implicitle assumed
	inline bool contains(scalar_type v) const
	{
		SK_ASSERT(is_inorder());
		if (v < x)
		{
			return false;
		}
		if (v > y)
		{
			return false;
		}
		return true;
	}

#ifdef SK_QT
	inline QPointF toQPointF() const
	{
		return QPointF(x, y);
	}
	inline QPoint toQPoint() const
	{
		return QPoint(x, y);
	}
#endif

	template<typename T>
	inline const T& raw_convert() const
	{
		return *(reinterpret_cast<const T*>(this));
	}

    inline Point_2<float> ptFloat() const
    {
        return Point_2<float>(float(x), float(y));
    }

	inline Point_2<double> ptDouble() const
	{
		return Point_2<double>(double(x), double(y));
	}

    template<typename converted_type>
    inline Point_2<converted_type> pt() const
    {
        return Point_2<converted_type>(converted_type(x), converted_type(y));
    }

}; // classPoint_2

template <typename T>
static inline T dot(const Point_2<T>& lhv, const Point_2<T>& rhv)
{
	return lhv.x * rhv.x + lhv.y * rhv.y;
}
	
template <typename T>
static inline T distance(const Point_2<T>& lhv, const Point_2<T>& rhv)
{
	const Point_2<T> d = rhv - lhv;
	return d.length();
}
	
typedef Point_2<double> point_2;
typedef Point_2<float>  point_2f;
typedef Point_2<int>    point_2i;

typedef point_2  vector_2;
typedef point_2f vector_2f;
typedef point_2i vector_2i;

#if 0

namespace tests
{
	namespace point_2
	{
		static bool doIt()
		{
			point_2i pt(1);
			point_2i pt1(1);
			point_2i pt2(2);
			SK_ASSERT_TEST( point_2i::eq(pt, pt1));
			SK_ASSERT_TEST(!point_2i::eq(pt, pt2));
			SK_ASSERT_TEST( point_2i::eq_manh(pt, pt2, 1));
			point_2f pt3(1.0f);
			SK_ASSERT_TEST( point_2f::eq(pt.ptFloat(), pt3));
			return true;
		}
	}
}

#endif

}  // namespace sk

#endif
