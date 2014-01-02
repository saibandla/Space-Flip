#pragma once

#ifndef _SK_PREDICATES_INT_H_
#define _SK_PREDICATES_INT_H_

#include "sk_point_2.h"
#include "sk_enums_geometry.h"
#include "sk_math.h"

#include <cassert> // TODO : replace !
#include <climits>


#define pt_i sk::Point_2<int>
namespace sk
{
	
	namespace predicates
	{

		inline bool points_equal(const sk::Point_2<int> & lhv, const sk::Point_2<int> & rhv)
		{
			if (lhv.x != rhv.x)
			{
				return false;
			}
			return lhv.y == rhv.y;
		}

		typedef signed long long sll;
		inline int orientation_value(const sk::Point_2<int> & a, const sk::Point_2<int> & b, const pt_i& c)
		{	
			// TODO : carefull testing		
			sk::Point_2<int> v0 = b - a;
			sk::Point_2<int> v1 = c - a;
			//assert(v0.abs_components_less(0xffff));
			//assert(v1.abs_components_less(0xffff));
#ifndef NDEBUG
			sll v = v0.x * v1.y - v0.y * v1.x;
			//assert(sk::abs(v) < INT_MAX);

#endif
			sll  aa = sll(v0.x) * sll(v1.y);
//			assert(sk::abs(aa) < INT_MAX);
			sll  bb = sll(v0.y) * sll(v1.x);
//			assert(sk::abs(bb) < INT_MAX);
			//if (aa >= 0 && bb < 0)
			//{
			//	return 1;
			//}
			//if (aa <= 0 && bb > 0)
			//{
			//	return -1;
			//}
			signed long long  r = aa - bb;
			assert(sk::abs(r) < LLONG_MAX);
			if (r < 0)
			{
				return -1;
			}
			if (r > 0)
			{
				return 1;
			}
			return 0;
		}

		inline bool are_collinear(const sk::Point_2<int> & a, const sk::Point_2<int> & b, const sk::Point_2<int>& c)
		{
			int ov = orientation_value(a, b, c);
			return 0 == ov;
		}

		inline bool are_cw(const sk::Point_2<int> & a, const sk::Point_2<int> & b, const sk::Point_2<int>& c)
		{
			assert(!points_equal(a, b));
			assert(!points_equal(a, c));
			assert(!points_equal(b, c));
			int ov = orientation_value(a, b, c);
			return ov < 0;
		}

		inline bool are_ccw(const sk::Point_2<int> & a, const sk::Point_2<int> & b, const sk::Point_2<int>& c)
		{
			assert(!points_equal(a, b));
			assert(!points_equal(a, c));
			assert(!points_equal(b, c));
			int ov = orientation_value(a, b, c);
			return ov > 0;
		}



		inline bool point_on_segment_apr_collinear(const sk::Point_2<int> & pt, const sk::Point_2<int> & seg_b, const sk::Point_2<int>& seg_e)
		{
			assert(are_collinear(pt, seg_b, seg_e));

			int min_x = sk::min(seg_b.x, seg_e.x);
			if (pt.x < min_x)
			{
				return false;
			}
			int max_x = sk::max(seg_b.x, seg_e.x);
			if (pt.x > max_x)
			{
				return false;
			}
			int min_y = sk::min(seg_b.y, seg_e.y);
			if (pt.y < min_y)
			{
				return false;
			}
			int max_y = sk::max(seg_b.y, seg_e.y);
			if (pt.y > max_y)
			{
				return false;
			}
			return true;
		}

		static bool point_on_segment(const pt_i & pt, const pt_i & seg_b, const pt_i& seg_e)
		{
			if (!are_collinear(pt, seg_b, seg_e))
			{
				return false;
			}
			return point_on_segment_apr_collinear(pt, seg_b, seg_e);
		}

		inline ResPointInSector classify_point_in_sector_cw(const pt_i& origin, const pt_i& first_ray, const pt_i& second_ray, const pt_i& point)
		{
			assert(are_cw(origin, first_ray, second_ray));			

			int fo = orientation_value(first_ray, origin, point);
			int so = orientation_value(point, origin, second_ray);

			if (0 == fo && 0 == so)
			{
				assert(points_equal(origin, point));
				return kPointInSectorOnOrigin; 
			};	

			if (0 != fo && 0 != so)
			{
				if (fo > 0 && so > 0)
				{
					return kPointInsideSector;					
				}
				return kPointOutsideSector;
			}

			if (0 == fo)
			{
				assert(0 != so);
				if (so > 0)
				{
					return kPointInSectorOnFirstRay;
				}
				else
				{
					assert(so < 0);
					return kPointOutsideSectorOnFirstRay;
				}
			}
			else
			{
				assert(0 == so);
				assert(0 != fo);
				if (fo > 0)
				{
					return kPointInSectorOnSecondRay;
				}
				else
				{
					assert(fo < 0);
					return kPointOutsideSectorOnSecondRay;
				}
			}
			assert(false);
			return kInvalidConvexSector;
		}

		inline ResPointInSector classify_point_in_sector_ccw(const pt_i& origin, const pt_i& first_ray, const pt_i& second_ray, const pt_i& point)
		{
			assert(are_ccw(origin, first_ray, second_ray));
			ResPointInSector r = classify_point_in_sector_cw(origin, second_ray, first_ray, point);

			const ResPointInSector _map[] =
			{
				kPointInSectorOnOrigin,				
				kPointInSectorOnSecondRay,
				kPointInSectorOnFirstRay,
				kPointInsideSector,		
				kPointOutsideSector,				
				kPointOutsideSectorOnSecondRay,
				kPointOutsideSectorOnFirstRay,
				kInvalidConvexSector 
			};

			return _map[r];
		}
		// generatl fuction for classification of point in sector
		// we can do better if we know sector orientation apriory

		inline ResPointInSector classify_point_in_collinear_sector(const pt_i& origin, const pt_i& first_ray, const pt_i& second_ray, const pt_i& point)
		{
			assert(0 == orientation_value(origin, first_ray, second_ray));
			assert(!sk::predicates::points_equal(origin, first_ray));
			assert(!sk::predicates::points_equal(origin, second_ray));
			assert(!sk::predicates::points_equal(first_ray, second_ray));

			int ofp = orientation_value(origin, first_ray, point);
			
			if (ofp > 0)
			{
				return kPointOutsideSector;
			}
			if (ofp < 0)
			{
				return kPointInsideSector;
			}
						
			pt_i df = first_ray - origin;
			pt_i ds = second_ray - origin;

			assert((ds.x >= 0 && df.x <= 0) || (ds.x <= 0 && df.x >= 0));
			assert((ds.y >= 0 && df.y <= 0) || (ds.y <= 0 && df.y >= 0));
			

			if (sk::predicates::points_equal(origin, point))
			{
				return kPointInSectorOnOrigin;
			}

			pt_i dp = point - origin;
			if (abs(df.x) >= abs(df.y))
			{
				assert(abs(ds.x) >= abs(ds.y));
				bool wasSwapped = false;
				if (df.x < ds.x)
				{
					sk_swap(df, ds);
					wasSwapped = true;
				}
				if (dp.x >= 0)
				{
					assert(df.x >= 0);
					if (dp.x <= df.x)
					{
						return !wasSwapped ? kPointInSectorOnFirstRay : kPointInSectorOnSecondRay;
					}
					else
					{
						return !wasSwapped ? kPointOutsideSectorOnFirstRay : kPointOutsideSectorOnSecondRay;
					}
				}
				else
				{
					assert((ds.y >=0 && df.y <= 0) || (ds.y <= 0 && df.y >= 0));
					assert(ds.x <= 0);
					if (dp.x >= ds.x)
					{
						return !wasSwapped ? kPointInSectorOnSecondRay : kPointInSectorOnFirstRay;
					}
					else
					{
						return !wasSwapped ? kPointOutsideSectorOnSecondRay : kPointOutsideSectorOnFirstRay;
					}
				}
			}
			else
			{
				assert(abs(ds.x) <= abs(ds.y));
				bool wasSwapped = false;
				if (df.y < ds.y)
				{
					sk_swap(df, ds);
					wasSwapped = true;
				}
				if (dp.y >= 0)
				{
					assert(df.y >= 0);
					if (dp.y <= df.y)
					{
						return !wasSwapped ? kPointInSectorOnFirstRay : kPointInSectorOnSecondRay;
					}
					else
					{
						return !wasSwapped ? kPointOutsideSectorOnFirstRay : kPointOutsideSectorOnSecondRay;
					}
				}
				else
				{
					assert(ds.y <= 0);
					if (dp.y >= ds.y)
					{
						return !wasSwapped ? kPointInSectorOnSecondRay : kPointInSectorOnFirstRay;
					}
					else
					{
						return !wasSwapped ? kPointOutsideSectorOnSecondRay : kPointOutsideSectorOnFirstRay;
					}
				}
			}
			return kInvalidConvexSector;
		}
		inline ResPointInSector classify_point_in_sector(const pt_i& origin, const pt_i& first_ray, const pt_i& second_ray, const pt_i& point)
		{
			int o = orientation_value(origin, first_ray, second_ray);
			if (0 == o)
			{
				return classify_point_in_collinear_sector(origin, first_ray, second_ray, point);				
			}
			assert(0 != o);
			if (o < 0)
			{
				return classify_point_in_sector_cw(origin, first_ray, second_ray, point);
			}
			else
			{
				return classify_point_in_sector_ccw(origin, first_ray, second_ray, point);
			}
		}

		inline bool is_distinct_of(const pt_i& pt, const pt_i& pt0, const pt_i& pt1, const pt_i& pt2)
		{
			if (points_equal(pt, pt0))
			{
				return false;
			}
			if (points_equal(pt, pt1))
			{
				return false;
			}
			if (points_equal(pt, pt2))
			{
				return false;
			}
			return true;
		}

	} // namespace predicates

	namespace tests
	{
		namespace predicates
		{
			static int num_tests = 0;

			static bool test_orientation_suite_cw(const pt_i * pts) // must be clockwise
			{
				assert(sk::predicates::are_cw(pts[0], pts[1], pts[2]));
				++num_tests;
				if (!sk::predicates::are_cw(pts[1], pts[2], pts[0]))
				{
					return false;
				}
				++num_tests;
				if (!sk::predicates::are_cw(pts[2], pts[0], pts[1]))
				{
					return false;
				}
				++num_tests;
				if (!sk::predicates::are_ccw(pts[0], pts[2], pts[1]))
				{
					return false;
				}
				++num_tests;
				if (!sk::predicates::are_ccw(pts[2], pts[1], pts[0]))
				{
					return false;
				}
				++num_tests;
				if (!sk::predicates::are_ccw(pts[1], pts[0], pts[2]))
				{
					return false;
				}

				return true;
			}

			static bool test_classify_point_in_sector_suite(pt_i * pts, pt_i * test_pts, const ResPointInSector * res, int num)
			{
				for (int i = 0; i < num; ++i)
				{
					if(res[i] != sk::predicates::classify_point_in_sector(pts[0], pts[1], pts[2], test_pts[i]))
					{
						return false;
					}
				}	
				for (int i = 0; i < 3; ++i){pts[i].y = -pts[i].y;} for (int i = 0; i < num; ++i){test_pts[i].y = -test_pts[i].y;}
				for (int i = 0; i < num; ++i)
				{
					if(res[i] != sk::predicates::classify_point_in_sector(pts[0], pts[1], pts[2], test_pts[i]))
					{
						return false;
					}
				}	
				for (int i = 0; i < 3; ++i){pts[i].x = -pts[i].x;} for (int i = 0; i < num; ++i){test_pts[i].x = -test_pts[i].x;}
				for (int i = 0; i < num; ++i)
				{
					if(res[i] != sk::predicates::classify_point_in_sector(pts[0], pts[1], pts[2], test_pts[i]))
					{
						return false;
					}
				}	
				for (int i = 0; i < 3; ++i){pts[i].y = -pts[i].y;} for (int i = 0; i < num; ++i){test_pts[i].y = -test_pts[i].y;}
				for (int i = 0; i < num; ++i)
				{
					if(res[i] != sk::predicates::classify_point_in_sector(pts[0], pts[1], pts[2], test_pts[i]))
					{
						return false;
					}
				}	
				return true;
			}

			static bool test_orientation()
			{
				pt_i pts[3];

				pts[0].x = 0;
				pts[0].y = 0;
				pts[1].x = 2;
				pts[1].y = 2;
				pts[2].x = 6;
				pts[2].y = 4;

				if (!test_orientation_suite_cw(pts))
				{
					return false;
				}

				pts[2].x = 2;
				pts[2].y = 0;				

				if (!test_orientation_suite_cw(pts))
				{
					return false;
				}

				pts[0].x = 6;
				pts[0].y = -2;
				pts[1].x = 0;
				pts[1].y = 0;

				if (!test_orientation_suite_cw(pts))
				{
					return false;
				}

				pts[0].x = 0;
				pts[0].y = 0;

				pts[1].x = -1;
				pts[1].y =  1;

				pts[2].x = -1;
				pts[2].y =  2;

				if (!test_orientation_suite_cw(pts))
				{
					return false;
				}

				pts[0].x = 10;
				pts[0].y = -10;

				pts[1].x = 1;
				pts[1].y = 6;

				pts[2].x = 5;
				pts[2].y =  7;

				if (!test_orientation_suite_cw(pts))
				{
					return false;
				}

				pts[0].x = 456;
				pts[0].y = 6731;

				pts[1].x = 16208;
				pts[1].y = 31478;

				pts[2].x = 29156;
				pts[2].y = 30134;

				pt_i p = pt_i(55781, -27075);

				bool is_cw = sk::predicates::are_cw(p, pts[0], pts[2]);
				bool is_cw1 = sk::predicates::are_cw(pts[0], pts[2], p);
				bool is_cw2 = sk::predicates::are_cw(pts[2],p , pts[0]);

				ResPointInSector res = sk::predicates::classify_point_in_sector(pts[0], pts[1], pts[2], p);
				if (res != kPointOutsideSector)
				{
					return false;
				}
				return true;
			}


			static bool test_classify_point_in_collinear_sector()
			{
				const int W = 2;
				pt_i origin(0, 0);
				pt_i up(0, W);
				pt_i up_h (0, W >> 1);
				pt_i down(0, -W);
				pt_i down_h (0, - (W >> 1));
				pt_i left(-W, 0);
				pt_i right(W, 0);
				ResPointInSector res;
				
				int num = 4;

				do 
				{
					res = sk::predicates::classify_point_in_collinear_sector(origin, up, down, right);
					if (kPointInsideSector != res)
					{
						return false;
					}
					res = sk::predicates::classify_point_in_collinear_sector(origin, up, down, left);
					if (kPointOutsideSector != res)
					{
						return false;
					}
					res = sk::predicates::classify_point_in_collinear_sector(origin, up, down, up_h);
					if (kPointInSectorOnFirstRay != res)
					{
						return false;
					}
					res = sk::predicates::classify_point_in_collinear_sector(origin, up, down, down_h);
					if (kPointInSectorOnSecondRay != res)
					{
						return false;
					}
					up     = up.perp_cw    (up);
					down   = down.perp_cw  (down);
					left   = left.perp_cw  (left);
					right  = right.perp_cw (right);
					up_h   = up_h.perp_cw  (up_h);
					down_h = down_h.perp_cw(down_h);
					--num;
				} while (num);
				
				return true;
			}

			static bool doIt()
			{
				if (!test_orientation())
				{
					return false;
				}
				pt_i pts[3];

				pts[0] = pt_i(0, 0); pts[1] = pt_i(2, 2); pts[2] = pt_i(6, -2);				
				pt_i p[] = {pts[0], pt_i(0, 2), pt_i(0, -2), pt_i(-2, 0), pt_i(2, 0), pt_i(1, 1), pt_i(3, -1), pt_i(-1, -1), pt_i(-3, 1), pts[1], pts[2]};
				ResPointInSector res[] = {kPointInSectorOnOrigin, kPointOutsideSector, kPointOutsideSector, kPointOutsideSector, kPointInsideSector, kPointInSectorOnFirstRay, kPointInSectorOnSecondRay, kPointOutsideSectorOnFirstRay, kPointOutsideSectorOnSecondRay, kPointInSectorOnFirstRay, kPointInSectorOnSecondRay};

				if (!test_classify_point_in_sector_suite(pts, p, res, sizeof(p) / sizeof(p[0])))
				{
					return false;
				}				

				pts[0] = pt_i(0, 1); pts[1] = pt_i(-1, 0); pts[2] = pt_i(1, -1);			
				pt_i pp[] = {pts[0], pt_i(0, 2), pt_i(0, -2), pt_i(-2, 0), pt_i(0, 0), pt_i(-2, -1), pt_i(2, -3), pt_i(1, 2), pt_i(-1, 3), pts[1], pts[2]};
				ResPointInSector pres[] = {kPointInSectorOnOrigin, kPointOutsideSector, kPointInsideSector, kPointOutsideSector, kPointInsideSector, kPointInSectorOnFirstRay, kPointInSectorOnSecondRay, kPointOutsideSectorOnFirstRay, kPointOutsideSectorOnSecondRay, kPointInSectorOnFirstRay, kPointInSectorOnSecondRay};

				if (!test_classify_point_in_sector_suite(pts, pp, pres, sizeof(pp) / sizeof(pp[0])))
				{
					return false;
				}				
				if (!test_classify_point_in_collinear_sector())
				{
					return false;
				}
				return true;
			}
		} // namespace predicates
	} // namespace tests

} // namespace predicates 

#endif