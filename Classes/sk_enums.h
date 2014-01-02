#pragma once

#ifndef _SK_ENUMS_H_
#define _SK_ENUMS_H_


namespace sk
{

    enum PrimitiveType 
    {
        PT_POLYGON, 
        PT_POLYLINE,
	    PT_RECTANGLE_SET,
	    PT_POINTS,
	    PT_SEGMENT_SET,
	    PT_POLYGON_SET,
		        
	    PT_COMPLEX_UNSPECIFIED,

        NUM_PrimitiveTypes
    };

	namespace toString
	{
	     const char * from(PrimitiveType primitiveType);		
	} // namespace toString

	enum Orientation
	{
		O_CCW = -1,
		O_COLLINEAR,
		O_CW
	};
	
} // namespace sk


#endif