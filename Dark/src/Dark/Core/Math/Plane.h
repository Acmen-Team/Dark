// Copyright LiuD, Inc. All Rights Reserved.

#pragma once

#include "Vector.h"

/**
 * Structure for three dimensional planes.
 *
 * Stores the coeffecients as Xx+Yy+Zz=W.
 * Note that this is different from many other Plane classes that use Xx+Yy+Zz+W=0.
 */
struct Plane: public Vector
{
public:

	/** The w-component. */
	float W;

public:

	/** Default constructor (no initialization). */
	//FORCEINLINE FPlane();

	/**
	 * Constructor.
	 *
	 * @param V 4D vector to set up plane.
	 */
	FORCEINLINE Plane(const Vector4& V) : Vector(V), W(V.W) {}
	
	/**
	 * Constructor.
	 *
	 * @param InX X-coefficient.
	 * @param InY Y-coefficient.
	 * @param InZ Z-coefficient.
	 * @param InW W-coefficient.
	 */
	FORCEINLINE Plane(float InX, float InY, float InZ, float InW) 
		: Vector(InX, InY, InZ), W(InW) {}

	/**
	 * Constructor.
	 *
	 * @param InNormal Plane Normal Vector.
	 * @param InW Plane W-coefficient.
	 */
	//FORCEINLINE Plane(Vector InNormal, float InW);

	/**
	 * Constructor.
	 *
	 * @param InBase Base point in plane.
	 * @param InNormal Plane Normal Vector.
	 */
	FORCEINLINE Plane(Vector InBase, const Vector& InNormal)
		: Vector(InNormal), W(InBase | InNormal) {}

	/**
	 * Constructor.
	 *
	 * @param A First point in the plane.
	 * @param B Second point in the plane.
	 * @param C Third point in the plane.
	 */
	FORCEINLINE Plane(Vector A, Vector B, Vector C)
		: Vector(((B - A) ^ (C - A)).GetSafeNormal())
	{
		W = A | (Vector)(*this);
	}

	/**
	 * Constructor
	 *
	 * @param EForceInit Force Init Enum.
	 */
	//explicit FORCEINLINE FPlane(EForceInit);

	// Functions.





	FORCEINLINE float PlaneDot(const Vector& P) const
	{
		return X * P.X + Y * P.Y + Z * P.Z - W;
	}



	/**
	 * Get the result of transforming the plane by a Matrix.
	 *
	 * @param M The matrix to transform plane with.
	 * @return The result of transform.
	 */
	Plane TransformBy(const Matrix& M) const;

	/**
	 * You can optionally pass in the matrices transpose-adjoint, which save it recalculating it.
	 * MSM: If we are going to save the transpose-adjoint we should also save the more expensive
	 * determinant.
	 *
	 * @param M The Matrix to transform plane with.
	 * @param DetM Determinant of Matrix.
	 * @param TA Transpose-adjoint of Matrix.
	 * @return The result of transform.
	 */
	Plane TransformByUsingAdjointT(const Matrix& M, float DetM, const Matrix& TA) const;
};


/* Vector inline functions
 *****************************************************************************/

inline Vector Vector::MirrorByPlane(const Plane& Plane) const
{
	return *this - Plane * (2.f * Plane.PlaneDot(*this));
}

inline Vector Vector::PointPlaneProject(const Vector& Point, const Plane& Plane)
{
	//Find the distance of X from the plane
	//Add the distance back along the normal from the point
	return Point - Plane.PlaneDot(Point) * Plane;
}

inline Vector Vector::PointPlaneProject(const Vector& Point, const Vector& A, const Vector& B, const Vector& C)
{
	//Compute the plane normal from ABC
	Plane Plane(A, B, C);

	//Find the distance of X from the plane
	//Add the distance back along the normal from the point
	return Point - Plane.PlaneDot(Point) * Plane;
}