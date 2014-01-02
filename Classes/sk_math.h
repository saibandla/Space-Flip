#pragma once

#ifndef _SK_MATH_H_
#define _SK_MATH_H_

#include "sk_assert.h"

#include <cmath>

#ifndef M_PI
#define M_PI 3.141526 
#endif

namespace sk
{

#ifndef SK_NO_SK_MIN_MAX	
	template <typename T>
	inline T min(T a, T b)
	{
		if (a < b)
		{
			return a;
		}
		return b;
	}
	
	template <typename T>
	inline T max(T a, T b)
	{
		if (a < b)
		{
			return b;
		}
		return a;
	}
#endif	
	
	template <typename T>
	struct int_frac
	{
		int integer;
		T frac;
	};
	
	template<typename T>
	inline int_frac<T> get_int_frac(T v)
	{
		int_frac<T> res;
		double int_part;
		res.frac = static_cast<T>(modf((double)v, &int_part));
		res.integer = static_cast<int>(int_part);
		return res;
	}
	
	inline double toRadians(double degrees)
	{
		return degrees * (M_PI / 180.0);
	}

	inline double toDegrees(double radians)
	{
		return radians * (180.0 / M_PI);
	}

	inline float toRadians(float degrees)
	{
		return degrees * (float(M_PI) / 180.0f);
	}

	inline float toDegrees(float radians)
	{
		return radians * (180.0f / float(M_PI));
	}

	template <typename T>
	inline T abs(T v)
	{
		if (v >= 0)
		{
			return v;
		}
		return -v;
	}



	template <typename T>
	inline T clamp(T min, T max, T v)
	{
		SK_ASSERT(min <= max);
		if (v <= min)
		{
			return min;
		}
		if (v >= max)
		{
			return max;
		}
		return v;
	}
	
	template<typename T>
	inline void sk_swap(T& lhv, T&rhv)
	{
		T temp = lhv;
		lhv = rhv;
		rhv = temp;		
	}
	
	enum IntervalClassification
	{
		kICLeft,
		kICLeftBorder,
		kICMiddle,
		kICRightBorder,
		kICRight
	};
	
	template<typename T>
	inline IntervalClassification classify(T pt, T left, T right)
	{
		assert(left <= right);
		if (pt <= left)
		{
			if (pt == left)
			{
				return kICLeftBorder;
			}
			else
			{
				return kICLeft;
			}
		}
		if (pt < right)
		{
			return kICMiddle;			
		}
		else
		{
			if (pt == right)
			{
				return kICRightBorder;
			}
			else
			{
				return kICRight;
			}
		}
	}
	template<typename T>
	inline T NEXT(T curr, T max)
	{
		++curr;
		if (curr >= max)
		{
			return T(0);
		}
		return curr;
	}
	
	template<typename T>
	inline T PREV(T curr, T max)
	{
		--curr;
		if (curr < 0)
		{
			return max - T(1);
		}
		return curr;
	}
	
	
	
	template <typename T>
	inline bool is_pow_2(T x)
	{
		return 0 == (x & (x - 1));
	}
	
	// sktodo : different versions for different sizeof(T)
	template <typename T>
	inline T set_all_bits(T x)
	{
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return x;
	}
	
	template <typename T>
	inline T floor_pow_2(T x)
	{
		x = set_all_bits(x);
		return x - (x >> 1);
	}
	
	template <typename T>
	inline T ceil_pow_2(T x)
	{        
		if (is_pow_2(x))
		{
			return x;
		}
		x = set_all_bits(x);
		return x + 1;
	}
	
	template <typename T>
	inline T num_trailing_zeros(T x)
	{
		int n = 0;
		x = ~x & (x - 1);
		while (x)
		{
			++n;
			x >>= 1;
		}
		return n;
	}
	

	inline int half(int i)
	{
		return i >> 1;
	}

	inline float half(float f)
	{
		return f * 0.5f;
	}

	inline double half(double d)
	{
		return d * 0.5;
	}
#if 0
	namespace tests
	{
		namespace math
		{
			static bool doIt()
			{
				SK_ASSERT_TEST(1 == clamp(1, 2, -2));
				SK_ASSERT_TEST(2 == clamp(1, 2, 3));
				SK_ASSERT_TEST(3 == clamp(1, 4, 3));
				return true;
			}
		}
	}
#endif
} // namespace sk


#endif
