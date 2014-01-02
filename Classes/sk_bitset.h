#pragma once

#ifndef _SK_BITSET_H__
#define _SK_BITSET_H__

#include "sk_assert.h"

#include <vector>
#include <algorithm>

namespace sk
{
	class bitset
	{
	public:
		typedef unsigned int uint;

		bitset(int n = 0, bool Clear = true)
		{
			init(n, Clear);
		}

		void init(int n, bool Clear = true)
		{
			if (0 == n)
			{
				return;
			}
			numBits = n;
			numUints = (numBits >> 5) + 1;
			data.resize(numUints);
			if (Clear)
			{
				clear();
			}
		}

		inline uint static getUintInd(uint ind)
		{
			return ind >> 5;
		}

		inline uint static getBitInd(uint ind)
		{
			return ind & 31;
		}

		inline uint static getBitMask(uint ind)
		{
			return 1 << ind;
		}

		inline bool isSet(uint ind) const
		{
			return (getBitMask(getBitInd(ind)) & data[getUintInd(ind)]) != 0;
		}

		inline void set(uint ind)
		{
			data[getUintInd(ind)] |= getBitMask(getBitInd(ind));
		}

		inline void unset(uint ind)
		{
			data[getUintInd(ind)] &= (~getBitMask(getBitInd(ind)));
		}

		inline void set(uint ind, bool v)
		{
			if (v)
			{
				set(ind);
			}
			else
			{
				unset(ind);
			}
		}

		inline void setIfTrue(uint ind, bool condition)
		{
			if (! condition)
			{
				return;
			}
			set(ind);
		}

		inline void setIfFalse(uint ind, bool condition)
		{
			if (condition)
			{
				return;
			}
			set(ind);
		}

		inline void unsetIfTrue(uint ind, bool condition)
		{
			if (!condition)
			{
				return;
			}
			unset(ind);
		}

		inline void unsetIfFalse(uint ind, bool condition)
		{
			if (condition)
			{
				return;
			}
			unset(ind);
		}
		void clear()
		{
			std::fill(data.begin(), data.end(), 0);
		}

		// this function is unsafe - all bitest must have the same nuber of bits
		void setOR(const bitset& bs0, const bitset& bs1)
		{
			for (int i = 0; i < numUints; ++i)
			{
				data[i] = bs0.data[i] | bs1.data[i];
			}
		}
		void setAND(const bitset& bs0, const bitset& bs1)
		{
			for (int i = 0; i < numUints; ++i)
			{
				data[i] = bs0.data[i] & bs1.data[i];
			}
		}
		void setANDNOT(const bitset& bs0, const bitset& bs1)
		{
			for (int i = 0; i < numUints; ++i)
			{
				data[i] = bs0.data[i] & (~bs1.data[i]);
			}
		}
		// number terminationg zeros for non non zero x !!!
		// should not be used with zero x
		template<typename index_type>
		index_type ntzFNZ(uint x)
		{
			SK_ASSERT(x);

			index_type res = 31;
			index_type y;
			y = x << 16; if (y) {res -= 16; x = y;}
			y = x <<  8; if (y) {res -= 8;  x = y;}
			y = x <<  4; if (y) {res -= 4;  x = y;}
			y = x <<  2; if (y) {res -= 2;  x = y;}
			y = x <<  1; if (y) {res -= 1;}
			return res;
		}

		template<typename index_type>
		int setANDNOT(const bitset& bs0, const bitset& bs1, std::vector<index_type>& res)
		{
			int found = 0;
			for (int i = 0; i < numUints; ++i)
			{
				uint curr = bs0.data[i] & (~bs1.data[i]);
				data[i] = curr;
				if (curr)
				{
					index_type offset = (i << 5);
					uint mask;
					index_type ind;

					bool go = true;

					while(go)
					{
						uint unsetLast = curr & (curr - 1);
						if (unsetLast)
						{
							mask = curr ^ unsetLast;
							ind = ntzFNZ<index_type>(mask);
							curr = unsetLast;
						}
						else
						{
							ind = ntzFNZ<index_type>(curr);
							go = false;
						}
						if (found < (int)res.size())
						{
							res[found] = offset + ind;
						}
						else
						{
							res.push_back(offset + ind);
						}
						++found;
					}
				}
			}
			return found;
		}

		void setXOR(const bitset& bs0, const bitset& bs1)
		{
			for (int i = 0; i < numUints; ++i)
			{
				data[i] = bs0.data[i] ^ bs1.data[i];
			}
		}
		void setNOT(const bitset& bs0)
		{
			for (int i = 0; i < numUints; ++i)
			{
				data[i] = ~bs0.data[i];
			}
		}
		void negateSelf()
		{
			for (int i = 0; i < numUints; ++i)
			{
				data[i] = ~data[i];
			}
		}

		bool operator==(const bitset & other) const
		{
			if (this->numBits == other.numBits
			&&	this->data == other.data)
			{
				return true;
			}
			return false;
		}
		bool operator!=(const bitset & other) const
		{
			return !(*this == other);
		}

		template<typename index_type>
		std::vector<index_type> dump(const bool set) const
		{
			std::vector<index_type> out;
			for (uint i = 0; i < numBits; ++i)
			{
				if (set == isSet(i))
				{
					out.push_back(i);
				}
			}
			return out;
		}

		std::vector<uint> data;
		int numBits;
		int numUints;

	};	// class bitset

} // namespace sk;

#endif // _SK_BITSET_H__
