#pragma once

#include "sk_assert.h"

namespace sk
{
	enum DirectionOrpho
	{
		kLeft,
		kRight,
		kUp,
		kDown,
		kNumDirectionsOrpho
	};

	enum Direction45
	{
		kRightUp,
		kRightDown,
		kLeftUp,
		kLeftDown,
		kNumDirections45
	};

	enum ResPointInSector
	{
		kPointInSectorOnOrigin,
		kPointInSectorOnFirstRay,
		kPointInSectorOnSecondRay,
		kPointInsideSector,				
		kPointOutsideSector,
		kPointOutsideSectorOnFirstRay,
		kPointOutsideSectorOnSecondRay,
		kInvalidConvexSector
	};

	enum ResPointInPolygon
	{
		kPointOutsidePolygon,
		kPointOnPolygonBorder,
		kPointInsidePolygon,
		kInvalidConvexPolygon

	};

	inline bool is_outside_sector(ResPointInSector pis)
	{
		SK_ASSERT(pis != kInvalidConvexSector);
		return long(pis) >= long(kPointOutsideSector);
	}

	enum Snap2GridMode
	{
		kSnap_XY,
		kSnap_xY,
		kSnap_xy,
		kSnap_Xy,
		kSnap_Nearest,
		kSnap_Center,
		kNumSnap2GridModes
	};

	enum AreaCode 
	{
		kAcLeft   = 1,
		kAcRight  = 1 << 1,
		kAcButtom = 1 << 2,
		kAcTop    = 1 << 3
	};

	enum PrimitiveType
	{
		kPtClosedPolygon,
		kPtClosedPolygonWithHoles,
		kNumPrimitiveTypes,
		kPtInvalid
	};

	enum OperationType
	{
		kOtConvexDecomposition,
		kNumOperationTypes,
		kOtInvalid
	};
}