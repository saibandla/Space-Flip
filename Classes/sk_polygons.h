#pragma once

#ifndef _SK_POLYGONS_H__
#define _SK_POLYGONS_H__

#include "sk_point_2.h"
#include "sk_enums_geometry.h"
#include "sk_predicates_int.h"
#include "sk_math.h"
#include <vector>
#include <algorithm>
#include <ctime>

namespace sk
{

	template <typename ST>
	class polygon : public std::vector<Point_2<ST> >
	{
	public:
		typedef ST scalar_type;
		typedef Point_2<ST> point_type;
		typedef std::vector<point_type> vector_type;
		typedef polygon<ST> class_type;

		polygon()
		{}

		polygon(ST x, ST y)
		{
			push_back(x, y);
		}

		void push_back(ST x, ST y)
		{
			vector_type::push_back(point_type(x, y));
		}

		void pb(ST x, ST y)
		{
			vector_type::push_back(point_type(x, y));
		}
		void apb(ST x, ST y)
		{
			pb(back_x() + x, back_y() + y);
		}

		ST back_x() const
		{
			SK_ASSERT(this->size());
			return this->back().x;
		}
		ST back_y() const
		{
			SK_ASSERT(this->size());
			return this->back().y;
		}

		point_type offset_back(Direction45 d, scalar_type l = scalar_type(1)) const
		{
			assert(this->size());
			if (kLeftUp == d)
			{
				return point_type(this->back().x - l, this->back().y + l);
			}
			else
			if (kRightUp == d)
			{
				return point_type(this->back().x + l, this->back().y + l);
			}
			else 
			if(kLeftDown == d)
			{
				return point_type(this->back().x - l, this->back().y - l);
			}
			else
			{
				SK_ASSERT(kRightDown == d);
				return point_type(this->back().x + l, this->back().y - l);
			}			
		}

		point_type offset_back(DirectionOrpho d, scalar_type l = scalar_type(1)) const
		{
			SK_ASSERT(this->size());
			if (kLeft == d)
			{
				return point_type(this->back().x - l, this->back().y);
			}
			else
			if (kRight == d)
			{
				return point_type(this->back().x + l, this->back().y);
			}
			else 
			if(kUp == d)
			{
				return point_type(this->back().x, this->back().y + l);
			}
			else
			{
				SK_ASSERT(kDown == d);
				return point_type(this->back().x, this->back().y - l);
			}			
		}

		void push_back(const point_type& pt)
		{
			vector_type::push_back(pt);
		}

		void pb(const point_type& pt)
		{
			vector_type::push_back(pt);
		}

		void push_back(DirectionOrpho d)
		{
			vector_type::push_back(offset_back(d));
		}

		void pb(DirectionOrpho d)
		{
			vector_type::push_back(offset_back(d));
		}

		void push_back(Direction45 d)
		{
			vector_type::push_back(offset_back(d));
		}

		void pb(Direction45 d)
		{
			vector_type::push_back(offset_back(d));
		}

		void push_back(DirectionOrpho d, scalar_type k)
		{
			vector_type::push_back(offset_back(d, k));
		}

		void pb(DirectionOrpho d, scalar_type k)
		{
			vector_type::push_back(offset_back(d, k));
		}

		void push_back(Direction45 d, scalar_type k)
		{
			vector_type::push_back(offset_back(d, k));
		}

		void pb(Direction45 d, scalar_type k)
		{
			vector_type::push_back(offset_back(d, k));
		}
		

		bool is_ccw() const
		{
			SK_ASSERT(this->size() >= 3);
			SK_ASSERT(!has_consequtive_duplicates());
			int ind = 0;
			int s = int(this->size());
			for (int i = 1; i < s; ++i)
			{				
				if ((*this)[ind].x > (*this)[i].x)
				{
					ind = i;
				}
				else 
				if((*this)[ind].x == (*this)[i].x && (*this)[ind].y > (*this)[i].y)
				{
					ind = i;
				}
			}
			return are_ccw(PREV(ind, s), ind, NEXT(ind, s));
		}

		bool are_ccw(int i, int j, int k) const
		{
			return predicates::are_ccw((*this)[i], (*this)[j], (*this)[k]);
		}

		bool has_consequtive_duplicates() const
		{
			SK_ASSERT(this->size() >= 2);
			for (size_t i = 0, end = this->size() - 1; i < end; ++i)
			{
				if (predicates::points_equal((*this)[i], (*this)[i+1]))
				{
					return true;
				}
			}
			if (predicates::points_equal(this->front(), this->back()))
			{
				return true;
			}
			return false;
		}
		void assign(const point_type * ptr, int num)
		{
			assert(ptr && num >= 0);
			vector_type::assign(ptr, ptr + num);
		}

		static const int MAX_GEN_TYPES = 3;
		static class_type create_type(int ind, scalar_type h, scalar_type ox = 0, scalar_type oy = 0)
		{
			class_type res;
			if (ind < 0 || ind >= MAX_GEN_TYPES)
			{
				return res;
			}
			if (0 == ind) {return create_type_000(h, ox, oy);}
			if (1 == ind) {return create_type_001(h, ox, oy);}
			if (2 == ind) {return create_type_002(h, ox, oy);}
			return res;
		}

		static class_type create_convex_8_gon_45_ccw(scalar_type step = scalar_type(1), scalar_type ox = 0, scalar_type oy = 0)
		{
			assert(step > 0);
			class_type res(ox, oy);
			res.push_back(kLeftUp, step);
			res.push_back(kUp, step);
			res.push_back(kRightUp, step);
			res.push_back(kRight, step);
			res.push_back(kRightDown, step);
			res.push_back(kDown, step);
			res.push_back(kLeftDown, step);			
			return res;
		}

		static class_type create_type_000(scalar_type h, scalar_type ox = 0, scalar_type oy = 0)
		{
			assert(h > 0);
			class_type res(ox, oy);

			for (int i = 0; i < h; ++i)
			{
				res.push_back(kUp);
				res.push_back(kRight);
			}
			for (int i = 0; i < h - 1; ++i)
			{
				res.push_back(kDown);
				res.push_back(kRight);
			}
			res.push_back(kDown);
			for (int i = 0; i < 2 * h - 2; ++i)
			{
				res.push_back(kLeft);
			}
			assert(!res.is_closed());
			return res;
		}

		static class_type create_type_001(scalar_type h, scalar_type ox = 0, scalar_type oy = 0)
		{
			assert(h > 0);
			class_type res(ox, oy);

			for (int i = 0; i < h; ++i)
			{
				res.push_back(kUp);				
			}
			for (int i = 0; i < h; ++i)
			{
				res.push_back(kRight);				
			}
			for (int i = 0; i < h; ++i)
			{
				res.push_back(kDown);				
			}
			for (int i = 0; i < h - 1; ++i)
			{
				res.push_back(kLeft);				
			}
			assert(!res.is_closed());
			return res;
		}

		static class_type create_type_002(scalar_type h, scalar_type ox = 0, scalar_type oy = 0)
		{
			assert(h > 0);
			class_type res(ox, oy);

			for (int i = 0; i < h; ++i)
			{
				res.push_back(kRightUp);				
			}
			for (int i = 0; i < h; ++i)
			{
				res.push_back(kRightDown);				
			}
			for (int i = 0; i < h; ++i)
			{
				res.push_back(kLeftDown);				
			}
			for (int i = 0; i < h - 1; ++i)
			{
				res.push_back(kLeftUp);				
			}
			SK_ASSERT(!res.is_closed());
			return res;
		}

		bool is_closed() const
		{
			SK_ASSERT(this->size());
			return predicates::points_equal(this->front(), this->back());
		}

		struct X_ONLY_CMP
		{
			inline bool operator()(const point_type& lhv, const point_type& rhv) const
			{
				return lhv.x < rhv.x;
			}
		};

		struct X_AND_Y_ONLY_CMP
		{
			inline bool operator()(const point_type& lhv, const point_type& rhv) const
			{
				if (lhv.x == rhv.x)
				{
					return lhv.y < rhv.y;
				}
				else
				{
					return lhv.x < rhv.x;
				}
			}
		};

		void sort_by_x_only()
		{
			SK_ASSERT(this->size());
			std::sort(vector_type::begin(), vector_type::end(), X_ONLY_CMP());
		}

		void sort_by_x_and_y()
		{
			SK_ASSERT(this->size());
			std::sort(vector_type::begin(), vector_type::end(), X_AND_Y_ONLY_CMP());
		}

		bool has_duplicates()
		{
			int s = int(this->size());
			if (s <= 1)
			{
				return false;
			}
			class_type t = *this;
			t.sort_by_x_and_y();
			for (int i = 0; i < s - 1; ++i)
			{
				if (predicates::points_equal(t[i], t[i + 1]))
				{
					return true;
				}
			}
			return false;
		}

		bool ends_cw() const
		{
			int s = int(this->size());
			SK_ASSERT(s >= 3);
			int o = predicates::orientation_value((*this)[s - 3], (*this)[s - 2], (*this)[s - 1]);
			return o < 0;
		}

		bool are_ccw_from(int ind) const
		{
			int s = int(this->size());
			SK_ASSERT(s >= 3);
			int i1 = NEXT(ind, s);
			int i2 = NEXT(i1, s);
			return predicates::are_ccw((*this)[ind], (*this)[i1], (*this)[i2]);
		}

		bool any_three_consequtive_ccw() const
		{
			int s = int(this->size());
			SK_ASSERT(s >= 3);
			for (int i = 0; i < s; ++i)
			{
				if (!are_ccw_from(i))
				{
					return false;
				}
			}
			return true;
		}

		bool are_cw_from(int ind) const
		{
			int s = int(this->size());
			SK_ASSERT(s >= 3);
			int i1 = NEXT(ind, s);
			int i2 = NEXT(i1, s);
			return predicates::are_cw((*this)[ind], (*this)[i1], (*this)[i2]);
		}

		bool is_notch(int ind) const
		{
			int s = int(this->size());
			SK_ASSERT(s >= 3);
			int p = PREV(ind, s);
			int n = NEXT(ind, s);
			return predicates::are_ccw((*this)[p], (*this)[ind], (*this)[n]);
		}

		bool any_three_consequtive_cw() const
		{
			int s = int(this->size());
			SK_ASSERT(s >= 3);
			for (int i = 0; i < s; ++i)
			{
				if (!are_cw_from(i))
				{
					return false;
				}
			}
			return true;
		}

		point_type& second_back()
		{
			int s = int(this->size());
			SK_ASSERT(s >= 2);
			return (*this)[s - 2];
		}

		const point_type& second_back() const
		{
			int s = int(this->size());
			SK_ASSERT(s >= 2);
			return (*this)[s - 2];
		}

		void pop_second_back()
		{
			int s = int(this->size());
			SK_ASSERT(s >= 2);
			(*this)[s - 2] = (*this)[s - 1];
			vector_type::pop_back();
		}

		bool ends_ccw() const
		{
			int s = int(this->size());
			SK_ASSERT(s >= 3);
			return predicates::are_ccw((*this)[s - 3], (*this)[s - 2], (*this)[s - 1]);
		}

		void set_random(int n)
		{
			SK_ASSERT(n > 0);
			vector_type::resize(n);
			srand(time(NULL));
			for (int i = 0; i < n; ++i)
			{
				(*this)[i].x = scalar_type(rand());
				(*this)[i].y = scalar_type(rand());
			}
		}

		void remove_collinear_and_duplicates()
		{
			SK_ASSERT(this->size());
			int i = 1;
			int s = int(this->size());
			int num = 1;
			while (i < s && predicates::points_equal(this->front(), (*this)[i]))
			{
				++i;
			}
			if (s == i)
			{
				this->erase(this->begin() + num, this->end());
				SK_ASSERT(!has_collinear_or_duplicates());
				return;
			}
			else
			{
				(*this)[num] = (*this)[i];
				++num;
				++i;
			}
			while (i < s)
			{
				if (predicates::points_equal((*this)[num - 1], (*this)[i]) ||
					predicates::are_collinear((*this)[num - 2], (*this)[num - 1], (*this)[i]))
				{
					(*this)[num - 1] = (*this)[i];
					++i;
				}
				else
				{
					(*this)[num] = (*this)[i];
					++num;	
					++i;
				}
			}
			while (num > 1 && 
				(predicates::points_equal((*this)[num - 1], (*this)[0]) ||
				 predicates::are_collinear((*this)[num - 2], (*this)[num - 1], (*this)[0])))
			{
				--num;
			}
			this->erase(this->begin() + num, this->end());
			SK_ASSERT(!has_collinear_or_duplicates());
		}
		
		bool has_collinear_or_duplicates() const
		{
			int s = int(this->size());
			if (s < 2)
			{
				return false;
			}
			for (int i = 0; i < s; ++i)
			{
				int i1 = NEXT(i, s);
				int i2 = NEXT(i1, s);
				if (predicates::points_equal((*this)[i], (*this)[i1]))
				{
					return true;
				}
				if (predicates::are_collinear((*this)[i], (*this)[i1], (*this)[i2]))
				{
					return true;
				}
			}
			return false;			
		}

		void reverse()
		{
			std::reverse(vector_type::begin(), vector_type::end());
		}
		
	};
}

#endif _SK_POLYGONS_H__