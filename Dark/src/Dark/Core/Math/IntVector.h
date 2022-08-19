// Copyright LiuD, Inc. All Rights Reserved.

#pragma once

#include "MathUtility.h"


/**
 * Structure for integer vectors in 3-d space.
 */
struct IntVector
{
	/** Holds the point's x-coordinate. */
	int32 X;

	/** Holds the point's y-coordinate. */
	int32 Y;

	/**  Holds the point's z-coordinate. */
	int32 Z;
};


struct IntVector2D
{
	/** Holds the point's x-coordinate. */
	int32 X;

	/** Holds the point's y-coordinate. */
	int32 Y;

public:

	/** An integer point with zeroed values. */
	static const IntPoint ZeroValue;

	/** An integer point with INDEX_NONE values. */
	static const IntPoint NoneValue;

public:

	/** Default constructor (no initialization). */
	IntVector2D() {}

	/**
	 * Create and initialize a new instance with the specified coordinates.
	 *
	 * @param InX The x-coordinate.
	 * @param InY The y-coordinate.
	 */
	IntVector2D(int32 InX, int32 InY) : X(InX), Y(InY) { }

	/**
	 * Create and initialize a new instance with a single int.
	 * Both X and Y will be initialized to this value
	 *
	 * @param InXY The x and y-coordinate.
	 */
	IntVector2D(int32 InXY) : X(InXY), Y(InXY) { }

	/**
	 * Create and initialize a new instance to zero.
	 *
	 * @param EForceInit Force init enum
	 */
	 //explicit FORCEINLINE IntPoint(EForceInit);

};