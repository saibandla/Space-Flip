#pragma once

#ifndef _SK_BITSET_2D_H__
#define _SK_BITSET_2D_H__

#include "sk_assert.h"
#include "sk_bitset.h"
#include "sk_point_2.h"

#include <vector>
#include <algorithm>

namespace sk
{

	class bitset_2d : protected bitset
	{
	public:
		size_t _w;
				size_t _h;
		typedef bool pixel_type;

		bitset_2d()
			:
		_w(0),
		_h(0)
		{
		}

		bitset_2d(size_t w, size_t h, bool Clear = true)
		{
			bitset_2d::init(w, h, Clear);
		}

		void init(size_t w, size_t h, bool Clear = true)
		{
			bitset::init(w * h, Clear);
			_w = w;
			_h = h;
		}

		inline size_t width() const
		{
			return _w;
		}

		inline size_t height() const
		{
			return _h;
		}

		inline bool is_empty() const
		{
			return _w == 0 && _h == 0;
		}

		inline size_t index(size_t x, size_t y) const
		{
			SK_ASSERT(x < _w);
			SK_ASSERT(y < _h);

			return 	_w * y + x;
		}

		inline bool at(size_t x, size_t y) const
		{
			return bitset::isSet(index(x, y));
		}
		
		inline bool at(int ind) const
		{
			return bitset::isSet(ind);
		}

		inline void set(size_t x, size_t y, bool v)
		{
			bitset::set(index(x, y), v);
		}
		
		inline void set(size_t ind, bool v)
		{
			bitset::set(ind, v);
		}
		
		inline bool is_valid_index(size_t x, size_t y)
		{
			if (x >= _w || y >= _h)
			{
				return false;
			}
			return true;
		}
		inline void toggle_safe(size_t x, size_t y)
		{
			if (!is_valid_index(x, y))
			{
				return;
			}
			set(x, y, !at(x, y));			
		}
		
		inline void toggle_cross_safe(size_t x, size_t y)
		{
			toggle_safe(x - 1, y);
			toggle_safe(x + 1, y);
			toggle_safe(x, y - 1);
			toggle_safe(x, y + 1);
			toggle_safe(x , y);
		}

		inline std::vector<sk::point_2i> dump(bool set) const
		{
			std::vector<sk::point_2i> out;
			for (size_t x = 0; x < width(); ++x)
			{
				for (size_t y = 0; y < height(); ++y)
				{
					if (set == at(x, y))
					{
						out.push_back(point_2i(x, y));
					}
				}
			}
			return out;
		}

		bool operator==(const sk::bitset_2d & other) const
		{
			if ((this->width() == other.width())
			&&	(this->height() == other.height())
			&&	(this->data == other.data))
			{
				return true;
			}
			return false;
		}

		bool operator!=(const sk::bitset_2d & other) const
		{
			return !(*this == other);
		}

	private:

	}; // bitset_2d

} // namespace sk;

#endif // _SK_BITSET_2D_H__
