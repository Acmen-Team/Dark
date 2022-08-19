// Copyright LiuD, Inc. All Rights Reserved.

#pragma once

#include "MathUtility.h"
#include "Color.h"
#include "IntVector.h"
#include "Vector2D.h"
#include "Axis.h"

/**
 * A vector in 3-D space composed of components (X, Y, Z) with floating point precision.
 */
struct Vector
{

public:

	/** Vector's X component. */
	float X;

	/** Vector's Y component. */
	float Y;

	/** Vector's Z component. */
	float Z;

public:

	/** A zero vector (0,0,0) */
	static const Vector ZeroVector;

	/** One vector (1,1,1) */
	static const Vector OneVector;

	/** up vector (0,0,1) */
	static const Vector UpVector;

	/** down vector (0,0,-1) */
	static const Vector DownVector;

	/** forward vector (1,0,0) */
	static const Vector ForwardVector;

	/** backward vector (-1,0,0) */
	static const Vector BackwardVector;

	/** right vector (0,1,0) */
	static const Vector RightVector;

	/** left vector (0,-1,0) */
	static const Vector LeftVector;

	/** Unit X axis vector (1,0,0) */
	static const Vector XAxisVector;

	/** Unit Y axis vector (0,1,0) */
	static const Vector YAxisVector;

	/** Unit Z axis vector (0,0,1) */
	static const Vector ZAxisVector;

public:
	FORCEINLINE void DiagnosticCheckNaN() const {}

	/** Default constructor (no initialization). */
	FORCEINLINE Vector() {}

	/**
	 * Constructor initializing all components to a single float value.
	 *
	 * @param InF Value to set all components to.
	 */
	explicit FORCEINLINE Vector(float InF) : X(InF), Y(InF), Z(InF)
	{
		DiagnosticCheckNaN();
	}

	/**
	 * Constructor using initial values for each component.
	 *
	 * @param InX X Coordinate.
	 * @param InY Y Coordinate.
	 * @param InZ Z Coordinate.
	 */
	FORCEINLINE Vector(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ)
	{
		DiagnosticCheckNaN();
	}

	/**
	 * Constructs a vector from an Vector2D and Z value.
	 *
	 * @param V Vector to copy from.
	 * @param InZ Z Coordinate.
	 */
	explicit FORCEINLINE Vector(const Vector2D V, float InZ) : X(V.X), Y(V.Y), Z(InZ)
	{
		DiagnosticCheckNaN();
	}

	/**
	 * Constructor using the XYZ components from a 4D vector.
	 *
	 * @param V 4D Vector to copy from.
	 */
	FORCEINLINE Vector(const Vector4& V);
	/**
	 * Constructs a vector from an FLinearColor.
	 *
	 * @param InColor Color to copy from.
	 */
	explicit Vector(const LinearColor& InColor) : X(InColor.R), Y(InColor.G), Z(InColor.B)
	{
		DiagnosticCheckNaN();
	}

	/**
	 * Constructs a vector from an FIntVector.
	 *
	 * @param InVector FIntVector to copy from.
	 */
	explicit Vector(IntVector InVector) : X((float)InVector.X), Y((float)InVector.Y), Z((float)InVector.Z)
	{
		DiagnosticCheckNaN();
	}

	/**
	 * Constructs a vector from an FIntPoint.
	 *
	 * @param A Int Point used to set X and Y coordinates, Z is set to zero.
	 */
	explicit Vector(IntPoint A) : X((float)A.X), Y((float)A.Y), Z(0.f)
	{
		DiagnosticCheckNaN();
	}

	/**
	 * Constructor which initializes all components to zero.
	 *
	 * @param EForceInit Force init enum
	 */
	//explicit FORCEINLINE Vector(EForceInit);

	/**
	 * Calculate cross product between this and another vector.
	 *
	 * @param V The other vector.
	 * @return The cross product.
	 */
	FORCEINLINE Vector operator^(const Vector& V) const
	{
		return Vector
		(
			Y * V.Z - Z * V.Y,
			Z * V.X - X * V.Z,
			X * V.Y - Y * V.X
		);
	}

	/**
	 * Calculate the cross product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The cross product.
	 */
	FORCEINLINE static Vector CrossProduct(const Vector& A, const Vector& B) { return A ^ B; }

	/**
	 * Calculate the dot product between this and another vector.
	 *
	 * @param V The other vector.
	 * @return The dot product.
	 */
	FORCEINLINE float operator|(const Vector& V) const { return X * V.X + Y * V.Y + Z * V.Z; }

	/**
	 * Calculate the dot product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The dot product.
	 */
	FORCEINLINE static float DotProduct(const Vector& A, const Vector& B) { return A | B; }

	/**
	 * Gets the result of component-wise addition of this and another vector.
	 *
	 * @param V The vector to add to this.
	 * @return The result of vector addition.
	 */
	FORCEINLINE Vector operator+(const Vector& V) const { return Vector(X + V.X, Y + V.Y, Z + V.Z); }

	/**
	 * Gets the result of component-wise subtraction of this by another vector.
	 *
	 * @param V The vector to subtract from this.
	 * @return The result of vector subtraction.
	 */
	FORCEINLINE Vector operator-(const Vector& V) const { return Vector(X - V.X, Y - V.Y, Z - V.Z); }

	/**
	 * Gets the result of subtracting from each component of the vector.
	 *
	 * @param Bias How much to subtract from each component.
	 * @return The result of subtraction.
	 */
	FORCEINLINE Vector operator-(float Bias) const { return Vector(X - Bias, Y - Bias, Z - Bias); }

	/**
	 * Gets the result of adding to each component of the vector.
	 *
	 * @param Bias How much to add to each component.
	 * @return The result of addition.
	 */
	FORCEINLINE Vector operator+(float Bias) const { return Vector(X + Bias, Y + Bias, Z + Bias); }

	/**
	 * Gets the result of scaling the vector (multiplying each component by a value).
	 *
	 * @param Scale What to multiply each component by.
	 * @return The result of multiplication.
	 */
	FORCEINLINE Vector operator*(float Scale) const { return Vector(X * Scale, Y * Scale, Z * Scale); }

	/**
	 * Gets the result of dividing each component of the vector by a value.
	 *
	 * @param Scale What to divide each component by.
	 * @return The result of division.
	 */
	Vector operator/(float Scale) const
	{
		const float RScale = 1.f / Scale;
		return Vector(X * RScale, Y * RScale, Z * RScale);
	}

	/**
	 * Gets the result of component-wise multiplication of this vector by another.
	 *
	 * @param V The vector to multiply with.
	 * @return The result of multiplication.
	 */
	FORCEINLINE Vector operator*(const Vector& V) const { return Vector(X * V.X, Y * V.Y, Z * V.Z); }

	/**
	 * Gets the result of component-wise division of this vector by another.
	 *
	 * @param V The vector to divide by.
	 * @return The result of division.
	 */
	FORCEINLINE Vector operator/(const Vector& V) const { return Vector(X / V.X, Y / V.Y, Z / V.Z); }

	// Binary comparison operators.

	/**
	 * Check against another vector for equality.
	 *
	 * @param V The vector to check against.
	 * @return true if the vectors are equal, false otherwise.
	 */
	bool operator==(const Vector& V) const { return X == V.X && Y == V.Y && Z == V.Z; }

	/**
	 * Check against another vector for inequality.
	 *
	 * @param V The vector to check against.
	 * @return true if the vectors are not equal, false otherwise.
	 */
	bool operator!=(const Vector& V) const { return X != V.X || Y != V.Y || Z != V.Z; }

	/**
	 * Check against another vector for equality, within specified error limits.
	 *
	 * @param V The vector to check against.
	 * @param Tolerance Error tolerance.
	 * @return true if the vectors are equal within tolerance limits, false otherwise.
	 */
	bool Equals(const Vector& V, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return Math::Abs(X - V.X) <= Tolerance && Math::Abs(Y - V.Y) <= Tolerance && Math::Abs(Z - V.Z) <= Tolerance;
	}

	/**
	 * Checks whether all components of this vector are the same, within a tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 * @return true if the vectors are equal within tolerance limits, false otherwise.
	 */
	bool AllComponentsEqual(float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return Math::Abs(X - Y) <= Tolerance && Math::Abs(X - Z) <= Tolerance && Math::Abs(Y - Z) <= Tolerance;
	}

	/**
	 * Get a negated copy of the vector.
	 *
	 * @return A negated copy of the vector.
	 */
	FORCEINLINE Vector operator-() const { return Vector(-X, -Y, -Z); }

	/**
	 * Adds another vector to this.
	 * Uses component-wise addition.
	 *
	 * @param V Vector to add to this.
	 * @return Copy of the vector after addition.
	 */
	FORCEINLINE Vector operator+=(const Vector& V)
	{
		X += V.X; Y += V.Y; Z += V.Z;
		DiagnosticCheckNaN();
		return *this;
	}

	/**
	 * Subtracts another vector from this.
	 * Uses component-wise subtraction.
	 *
	 * @param V Vector to subtract from this.
	 * @return Copy of the vector after subtraction.
	 */
	FORCEINLINE Vector operator-=(const Vector& V)
	{
		X -= V.X; Y -= V.Y; Z -= V.Z;
		DiagnosticCheckNaN();
		return *this;
	}

	/**
	 * Scales the vector.
	 *
	 * @param Scale Amount to scale this vector by.
	 * @return Copy of the vector after scaling.
	 */
	FORCEINLINE Vector operator*=(float Scale)
	{
		X *= Scale; Y *= Scale; Z *= Scale;
		DiagnosticCheckNaN();
		return *this;
	}

	/**
	 * Divides the vector by a number.
	 *
	 * @param V What to divide this vector by.
	 * @return Copy of the vector after division.
	 */
	FORCEINLINE Vector operator/=(float V)
	{
		const float RV = 1.f / V;
		X *= RV; Y *= RV; Z *= RV;
		DiagnosticCheckNaN();
		return *this;
	}

	/**
	 * Multiplies the vector with another vector, using component-wise multiplication.
	 *
	 * @param V What to multiply this vector with.
	 * @return Copy of the vector after multiplication.
	 */
	FORCEINLINE Vector operator*=(const Vector& V)
	{
		X *= V.X; Y *= V.Y; Z *= V.Z;
		DiagnosticCheckNaN();
		return *this;
	}

	/**
	 * Divides the vector by another vector, using component-wise division.
	 *
	 * @param V What to divide vector by.
	 * @return Copy of the vector after division.
	 */
	FORCEINLINE Vector operator/=(const Vector& V)
	{
		X /= V.X; Y /= V.Y; Z /= V.Z;
		DiagnosticCheckNaN();
		return *this;
	}

	/**
	 * Gets specific component of the vector.
	 *
	 * @param Index the index of vector component
	 * @return reference to component.
	 */
	FORCEINLINE float& operator[](int32 Index)
	{
		//checkSlow(Index >= 0 && Index < 3);
		return (&X)[Index];
	}

	/**
	 * Gets specific component of the vector.
	 *
	 * @param Index the index of vector component
	 * @return Copy of the component.
	 */
	FORCEINLINE float operator[](int32 Index)const
	{
		//checkSlow(Index >= 0 && Index < 3);
		return (&X)[Index];
	}

	/**
	* Gets a specific component of the vector.
	*
	* @param Index The index of the component required.
	*
	* @return Reference to the specified component.
	*/
	FORCEINLINE float& Component(int32 Index)
	{
		return (&X)[Index];
	}

	/**
	* Gets a specific component of the vector.
	*
	* @param Index The index of the component required.
	* @return Copy of the specified component.
	*/
	FORCEINLINE float Component(int32 Index) const
	{
		return (&X)[Index];
	}

	/** Get a specific component of the vector, given a specific axis by enum */
	float GetComponentForAxis(EAxis::Type Axis) const
	{
		switch (Axis)
		{
		case EAxis::X:
			return X;
		case EAxis::Y:
			return Y;
		case EAxis::Z:
			return Z;
		default:
			return 0.f;
		}
	}

	/** Set a specified componet of the vector, given a specific axis by enum */
	void SetComponentForAxis(EAxis::Type Axis, float Component)
	{
		switch (Axis)
		{
		case EAxis::X:
			X = Component;
			break;
		case EAxis::Y:
			Y = Component;
			break;
		case EAxis::Z:
			Z = Component;
			break;
		}
	}

	// Simple functions.

	/**
	 * Set the values of the vector directly.
	 *
	 * @param InX New X coordinate.
	 * @param InY New Y coordinate.
	 * @param InZ New Z coordinate.
	 */
	FORCEINLINE void Set(float InX, float InY, float InZ) { X = InX; Y = InY; Z = InZ; DiagnosticCheckNaN(); }

	/**
	 * Get the maximum value of the vector's components.
	 *
	 * @return The maximum value of the vector's components.
	 */
	FORCEINLINE float GetMax() const { return Math::Max(Math::Max(X, Y), Z); }

	/**
	 * Get the maximum absolute value of the vector's components.
	 *
	 * @return The maximum absolute value of the vector's components.
	 */
	FORCEINLINE float GetAbsMax() const { return Math::Max(Math::Max(Math::Abs(X), Math::Abs(Y)), Math::Abs(Z)); }

	/**
	 * Get the minimum value of the vector's components.
	 *
	 * @return The minimum value of the vector's components.
	 */
	FORCEINLINE float GetMin() const { return Math::Min(Math::Min(X, Y), Z); }

	/**
	 * Get the minimum absolute value of the vector's components.
	 *
	 * @return The minimum absolute value of the vector's components.
	 */
	FORCEINLINE float GetAbsMin() const { return Math::Min(Math::Min(Math::Abs(X), Math::Abs(Y)), Math::Abs(Z)); }

	/** Gets the component-wise min of two vectors. */
	FORCEINLINE Vector ComponentMin(const Vector& Other) const { return Vector(Math::Min(X, Other.X), Math::Min(Y, Other.Y), Math::Min(Z, Other.Z)); }

	/** Gets the component-wise max of two vectors. */
	FORCEINLINE Vector ComponentMax(const Vector& Other) const { return Vector(Math::Max(X, Other.X), Math::Max(Y, Other.Y), Math::Max(Z, Other.Z)); }

	/**
	 * Get a copy of this vector with absolute value of each component.
	 *
	 * @return A copy of this vector with absolute value of each component.
	 */
	FORCEINLINE Vector GetAbs() const { return Vector(Math::Abs(X), Math::Abs(Y), Math::Abs(Z)); }

	/**
	 * Get the length (magnitude) of this vector.
	 *
	 * @return The length of this vector.
	 */
	FORCEINLINE float Size() const { return Math::Sqrt(X * X + Y * Y + Z * Z); }

	/**
	 * Get the squared length of this vector.
	 *
	 * @return The squared length of this vector.
	 */
	FORCEINLINE float SizeSquared() const { return X * X + Y * Y + Z * Z; }

	/**
	 * Get the length of the 2D components of this vector.
	 *
	 * @return The 2D length of this vector.
	 */
	FORCEINLINE float Size2D() const { return Math::Sqrt(X * X + Y * Y); }

	/**
	 * Get the squared length of the 2D components of this vector.
	 *
	 * @return The squared 2D length of this vector.
	 */
	FORCEINLINE float SizeSquared2D() const { return X * X + Y * Y; }

	/**
	 * Checks whether vector is near to zero within a specified tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 * @return true if the vector is near to zero, false otherwise.
	 */
	FORCEINLINE bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const {
		return Math::Abs(X) <= Tolerance
			&& Math::Abs(Y) <= Tolerance
			&& Math::Abs(Z) <= Tolerance;
	}

	/**
	 * Checks whether all components of the vector are exactly zero.
	 *
	 * @return true if the vector is exactly zero, false otherwise.
	 */
	FORCEINLINE bool IsZero() const { return X == 0.f && Y == 0.f && Z == 0.f; }

	/**
	 * Check if the vector is of unit length, with specified tolerance.
	 *
	 * @param LengthSquaredTolerance Tolerance against squared length.
	 * @return true if the vector is a unit vector within the specified tolerance.
	 */
	FORCEINLINE bool IsUnit(float LengthSquaredTolerance = KINDA_SMALL_NUMBER) const { return Math::Abs(1.0f - SizeSquared()) < LengthSquaredTolerance; }

	/**
	 * Checks whether vector is normalized.
	 *
	 * @return true if normalized, false otherwise.
	 */
	FORCEINLINE bool IsNormalized() const { return (Math::Abs(1.f - SizeSquared()) < THRESH_VECTOR_NORMALIZED); }

	/**
	 * Normalize this vector in-place if it is larger than a given tolerance. Leaves it unchanged if not.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @return true if the vector was normalized correctly, false otherwise.
	 */
	FORCEINLINE bool Normalize(float Tolerance = SMALL_NUMBER)
	{
		const float SquareSum = X * X + Y * Y + Z * Z;
		if (SquareSum > Tolerance)
		{
			const float Scale = Math::InvSqrt(SquareSum);
			X *= Scale; Y *= Scale; Z *= Scale;
			return true;
		}
		return false;
	}

	/**
	 * Calculates normalized version of vector without checking for zero length.
	 *
	 * @return Normalized version of vector.
	 * @see GetSafeNormal()
	 */
	FORCEINLINE Vector GetUnsafeNormal() const
	{
		const float Scale = Math::InvSqrt(X * X + Y * Y + Z * Z);
		return Vector(X * Scale, Y * Scale, Z * Scale);
	}

	/**
	 * Gets a normalized copy of the vector, checking it is safe to do so based on the length.
	 * Returns zero vector if vector length is too small to safely normalize.
	 *
	 * @param Tolerance Minimum squared vector length.
	 * @return A normalized copy if safe, (0,0,0) otherwise.
	 */
	FORCEINLINE Vector GetSafeNormal(float Tolerance = SMALL_NUMBER) const
	{
		const float SquareSum = X * X + Y * Y + Z * Z;

		// Not sure if it's safe to add tolerance in there. Might introduce too many errors
		if (SquareSum == 1.f)
		{
			return *this;
		}
		else if (SquareSum < Tolerance)
		{
			return Vector::ZeroVector;
		}
		const float Scale = Math::InvSqrt(SquareSum);
		return Vector(X * Scale, Y * Scale, Z * Scale);
	}

	/**
	 * Gets a normalized copy of the 2D components of the vector, checking it is safe to do so. Z is set to zero.
	 * Returns zero vector if vector length is too small to normalize.
	 *
	 * @param Tolerance Minimum squared vector length.
	 * @return Normalized copy if safe, otherwise returns zero vector.
	 */
	FORCEINLINE Vector GetSafeNormal2D(float Tolerance = SMALL_NUMBER) const
	{
		const float SquareSum = X * X + Y * Y;

		// Not sure if it's safe to add tolerance in there. Might introduce too many errors
		if (SquareSum == 1.f)
		{
			if (Z == 0.f)
			{
				return *this;
			}
			else
			{
				return Vector(X, Y, 0.f);
			}
		}
		else if (SquareSum < Tolerance)
		{
			return Vector::ZeroVector;
		}

		const float Scale = Math::InvSqrt(SquareSum);
		return Vector(X * Scale, Y * Scale, 0.f);
	}

	/**
	 * Util to convert this vector into a unit direction vector and its original length.
	 *
	 * @param OutDir Reference passed in to store unit direction vector.
	 * @param OutLength Reference passed in to store length of the vector.
	 */
	FORCEINLINE void ToDirectionAndLength(Vector& OutDir, float& OutLength) const
	{
		OutLength = Size();
		if (OutLength > SMALL_NUMBER)
		{
			float OneOverLength = 1.0f / OutLength;
			OutDir = Vector(X * OneOverLength, Y * OneOverLength,
				Z * OneOverLength);
		}
		else
		{
			OutDir = Vector::ZeroVector;
		}
	}

	/**
	 * Get a copy of the vector as sign only.
	 * Each component is set to +1 or -1, with the sign of zero treated as +1.
	 *
	 * @param A copy of the vector with each component set to +1 or -1
	 */
	FORCEINLINE Vector GetSignVector() const
	{
		return Vector
		(
			Math::FloatSelect(X, 1.f, -1.f),
			Math::FloatSelect(Y, 1.f, -1.f),
			Math::FloatSelect(Z, 1.f, -1.f)
		);
	}

	/**
	 * Projects 2D components of vector based on Z.
	 *
	 * @return Projected version of vector based on Z.
	 */
	FORCEINLINE Vector Projection() const
	{
		const float RZ = 1.f / Z;
		return Vector(X * RZ, Y * RZ, 1);
	}

	/**
	* Calculates normalized 2D version of vector without checking for zero length.
	*
	* @return Normalized version of vector.
	* @see GetSafeNormal2D()
	*/
	FORCEINLINE Vector GetUnsafeNormal2D() const
	{
		const float Scale = Math::InvSqrt(X * X + Y * Y);
		return Vector(X * Scale, Y * Scale, 0);
	}

	/**
	 * Gets a copy of this vector snapped to a grid.
	 *
	 * @param GridSz Grid dimension.
	 * @return A copy of this vector snapped to a grid.
	 * @see Math::GridSnap()
	 */
	FORCEINLINE Vector GridSnap(const float& GridSz) const
	{
		return Vector(Math::GridSnap(X, GridSz), Math::GridSnap(Y, GridSz), Math::GridSnap(Z, GridSz));
	}

	/**
	 * Get a copy of this vector, clamped inside of a cube.
	 *
	 * @param Radius Half size of the cube.
	 * @return A copy of this vector, bound by cube.
	 */
	FORCEINLINE Vector BoundToCube(float Radius) const
	{
		return Vector
		(
			Math::Clamp(X, -Radius, Radius),
			Math::Clamp(Y, -Radius, Radius),
			Math::Clamp(Z, -Radius, Radius)
		);
	}

	/** Get a copy of this vector, clamped inside of a cube. */
	FORCEINLINE Vector BoundToBox(const Vector& Min, const Vector Max) const
	{
		return Vector
		(
			Math::Clamp(X, Min.X, Max.X),
			Math::Clamp(Y, Min.Y, Max.Y),
			Math::Clamp(Z, Min.Z, Max.Z)
		);
	}

	/** Create a copy of this vector, with its magnitude clamped between Min and Max. */
	Vector GetClampedToSize(float Min, float Max) const;

	/** Create a copy of this vector, with the 2D magnitude clamped between Min and Max. Z is unchanged. */
	FORCEINLINE Vector GetClampedToSize2D(float Min, float Max) const
	{
		float VecSize2D = Size2D();
		const Vector VecDir = (VecSize2D > SMALL_NUMBER) ? (*this / VecSize2D) : Vector::ZeroVector;

		VecSize2D = Math::Clamp(VecSize2D, Min, Max);

		return Vector(VecSize2D * VecDir.X, VecSize2D * VecDir.Y, Z);
	}

	/** Create a copy of this vector, with its maximum magnitude clamped to MaxSize. */
	FORCEINLINE Vector GetClampedToMaxSize(float MaxSize) const
	{
		if (MaxSize < KINDA_SMALL_NUMBER)
		{
			return Vector::ZeroVector;
		}

		const float VSq = SizeSquared();
		if (VSq > Math::Square(MaxSize))
		{
			const float Scale = MaxSize * Math::InvSqrt(VSq);
			return Vector(X * Scale, Y * Scale, Z * Scale);
		}
		else
		{
			return *this;
		}
	}

	/** Create a copy of this vector, with the maximum 2D magnitude clamped to MaxSize. Z is unchanged. */
	FORCEINLINE Vector GetClampedToMaxSize2D(float MaxSize) const
	{
		if (MaxSize < KINDA_SMALL_NUMBER)
		{
			return Vector(0.f, 0.f, Z);
		}

		const float VSq2D = SizeSquared2D();
		if (VSq2D > Math::Square(MaxSize))
		{
			const float Scale = MaxSize * Math::InvSqrt(VSq2D);
			return Vector(X * Scale, Y * Scale, Z);
		}
		else
		{
			return *this;
		}
	}

	/**
	 * Add a vector to this and clamp the result in a cube.
	 *
	 * @param V Vector to add.
	 * @param Radius Half size of the cube.
	 */
	FORCEINLINE void AddBounded(const Vector& V, float Radius = MAX_int16) { *this = (*this + V).BoundToCube(Radius); }

	/**
	 * Gets the reciprocal of this vector, avoiding division by zero.
	 * Zero components are set to BIG_NUMBER.
	 *
	 * @return Reciprocal of this vector.
	 */
	FORCEINLINE Vector Reciprocal() const
	{
		Vector RecVector;
		if (X != 0.f)
		{
			RecVector.X = 1.f / X;
		}
		else
		{
			RecVector.X = BIG_NUMBER;
		}
		if (Y != 0.f)
		{
			RecVector.Y = 1.f / Y;
		}
		else
		{
			RecVector.Y = BIG_NUMBER;
		}
		if (Z != 0.f)
		{
			RecVector.Z = 1.f / Z;
		}
		else
		{
			RecVector.Z = BIG_NUMBER;
		}

		return RecVector;
	}

	/**
	 * Check whether X, Y and Z are nearly equal.
	 *
	 * @param Tolerance Specified Tolerance.
	 * @return true if X == Y == Z within the specified tolerance.
	 */
	FORCEINLINE bool IsUniform(float Tolerance = KINDA_SMALL_NUMBER) const { return AllComponentsEqual(Tolerance); }

	/**
	 * Mirror a vector about a normal vector.
	 *
	 * @param MirrorNormal Normal vector to mirror about.
	 * @return Mirrored vector.
	 */
	FORCEINLINE Vector MirrorByVector(const Vector& MirrorNormal) const { return *this - MirrorNormal * (2.f * (*this | MirrorNormal)); }

	/**
	 * Mirrors a vector about a plane.
	 *
	 * @param Plane Plane to mirror about.
	 * @return Mirrored vector.
	 */
	Vector MirrorByPlane(const Plane& Plane) const;

	/**
	 * Rotates around Axis (assumes Axis.Size() == 1).
	 *
	 * @param Angle Angle to rotate (in degrees).
	 * @param Axis Axis to rotate around.
	 * @return Rotated Vector.
	 */
	inline Vector RotateAngleAxis(const float AngleDeg, const Vector& Axis) const
	{
		float S, C;
		Math::SinCos(&S, &C, Math::DegreesToRadians(AngleDeg));

		const float XX = Axis.X * Axis.X;
		const float YY = Axis.Y * Axis.Y;
		const float ZZ = Axis.Z * Axis.Z;

		const float XY = Axis.X * Axis.Y;
		const float YZ = Axis.Y * Axis.Z;
		const float ZX = Axis.Z * Axis.X;

		const float XS = Axis.X * S;
		const float YS = Axis.Y * S;
		const float ZS = Axis.Z * S;

		const float OMC = 1.f - C;

		return Vector(
			(OMC * XX + C) * X + (OMC * XY - ZS) * Y + (OMC * ZX + YS) * Z,
			(OMC * XY + ZS) * X + (OMC * YY + C) * Y + (OMC * YZ - XS) * Z,
			(OMC * ZX - YS) * X + (OMC * YZ + XS) * Y + (OMC * ZZ + C) * Z
		);
	}

	/**
	 * Returns the cosine of the angle between this vector and another projected onto the XY plane (no Z).
	 *
	 * @param B the other vector to find the 2D cosine of the angle with.
	 * @return The cosine.
	 */
	FORCEINLINE float CosineAngle2D(Vector B) const
	{
		Vector A(*this);
		A.Z = 0.0f;
		B.Z = 0.0f;
		A.Normalize();
		B.Normalize();
		return A | B;
	}

	/**
	 * Gets a copy of this vector projected onto the input vector.
	 *
	 * @param A	Vector to project onto, does not assume it is normalized.
	 * @return Projected vector.
	 */
	FORCEINLINE Vector ProjectOnTo(const Vector& A) const { return (A * ((*this | A) / (A | A))); }

	/**
	 * Gets a copy of this vector projected onto the input vector, which is assumed to be unit length.
	 *
	 * @param  Normal Vector to project onto (assumed to be unit length).
	 * @return Projected vector.
	 */
	FORCEINLINE Vector ProjectOnToNormal(const Vector& Normal) const { return (Normal * (*this | Normal)); }

	/**
	 * Return the FRotator orientation corresponding to the direction in which the vector points.
	 * Sets Yaw and Pitch to the proper numbers, and sets Roll to zero because the roll can't be determined from a vector.
	 *
	 * @return FRotator from the Vector's direction, without any roll.
	 * @see ToOrientationQuat()
	 */
	Rotator ToOrientationRotator() const;

	/**
	 * Return the Quaternion orientation corresponding to the direction in which the vector points.
	 * Similar to the FRotator version, returns a result without roll such that it preserves the up vector.
	 *
	 * @note If you don't care about preserving the up vector and just want the most direct rotation, you can use the faster
	 * 'FQuat::FindBetweenVectors(Vector::ForwardVector, YourVector)' or 'FQuat::FindBetweenNormals(...)' if you know the vector is of unit length.
	 *
	 * @return Quaternion from the Vector's direction, without any roll.
	 * @see ToOrientationRotator(), FQuat::FindBetweenVectors()
	 */
	//FORCEINLINE Quat ToOrientationQuat() const;

	/**
	 * Return the FRotator orientation corresponding to the direction in which the vector points.
	 * Sets Yaw and Pitch to the proper numbers, and sets Roll to zero because the roll can't be determined from a vector.
	 * @note Identical to 'ToOrientationRotator()' and preserved for legacy reasons.
	 * @return FRotator from the Vector's direction.
	 * @see ToOrientationRotator(), ToOrientationQuat()
	 */
	//FORCEINLINE Rotator Rotation() const;

	/**
	 * Find good arbitrary axis vectors to represent U and V axes of a plane,
	 * using this vector as the normal of the plane.
	 *
	 * @param Axis1 Reference to first axis.
	 * @param Axis2 Reference to second axis.
	 */
	//FORCEINLINE CORE_API void FindBestAxisVectors(Vector& Axis1, Vector& Axis2) const;

	/** When this vector contains Euler angles (degrees), ensure that angles are between +/-180 */
	//FORCEINLINE CORE_API void UnwindEuler();

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this vector.
	 *
	 * @return true if there are any non-finite values in this vector, false otherwise.
	 */
	FORCEINLINE bool ContainsNaN() const
	{
		return (!Math::IsFinite(X) ||
			!Math::IsFinite(Y) ||
			!Math::IsFinite(Z));
	}

	/**
	 * Get a textual representation of this vector.
	 *
	 * @return A string describing the vector.
	 */
	//FORCEINLINE FString ToString() const;

	/**
	* Get a locale aware textual representation of this vector.
	*
	* @return A string describing the vector.
	*/
	//FORCEINLINE FText ToText() const;

	/** Get a short textural representation of this vector, for compact readable logging. */
	//FORCEINLINE FString ToCompactString() const;

	/** Get a short locale aware textural representation of this vector, for compact readable logging. */
	//FORCEINLINE FText ToCompactText() const;

	/**
	 * Initialize this Vector based on an FString. The String is expected to contain X=, Y=, Z=.
	 * The Vector will be bogus when InitFromString returns false.
	 *
	 * @param	InSourceString	FString containing the vector values.
	 * @return true if the X,Y,Z values were read successfully; false otherwise.
	 */
	//FORCEINLINE bool InitFromString(const FString& InSourceString);

	/**
	 * Converts a Cartesian unit vector into spherical coordinates on the unit sphere.
	 * @return Output Theta will be in the range [0, PI], and output Phi will be in the range [-PI, PI].
	 */
	FORCEINLINE Vector2D UnitCartesianToSpherical() const
	{
		//checkSlow(IsUnit());
		const float Theta = Math::Acos(Z / Size());
		const float Phi = Math::Atan2(Y, X);
		return Vector2D(Theta, Phi);
	}

	/**
	 * Convert a direction vector into a 'heading' angle.
	 *
	 * @return 'Heading' angle between +/-PI. 0 is pointing down +X.
	 */
	FORCEINLINE float HeadingAngle() const
	{
		// Project Dir into Z plane.
		Vector PlaneDir = *this;
		PlaneDir.Z = 0.f;
		PlaneDir = PlaneDir.GetSafeNormal();

		float Angle = Math::Acos(PlaneDir.X);

		if (PlaneDir.Y < 0.0f)
		{
			Angle *= -1.0f;
		}

		return Angle;
	}

	/**
	 * Create an orthonormal basis from a basis with at least two orthogonal vectors.
	 * It may change the directions of the X and Y axes to make the basis orthogonal,
	 * but it won't change the direction of the Z axis.
	 * All axes will be normalized.
	 *
	 * @param XAxis The input basis' XAxis, and upon return the orthonormal basis' XAxis.
	 * @param YAxis The input basis' YAxis, and upon return the orthonormal basis' YAxis.
	 * @param ZAxis The input basis' ZAxis, and upon return the orthonormal basis' ZAxis.
	 */
	static void CreateOrthonormalBasis(Vector& XAxis, Vector& YAxis, Vector& ZAxis);

	/**
	 * Compare two points and see if they're the same, using a threshold.
	 *
	 * @param P First vector.
	 * @param Q Second vector.
	 * @return Whether points are the same within a threshold. Uses fast distance approximation (linear per-component distance).
	 */
	inline static bool PointsAreSame(const Vector& P, const Vector& Q)
	{
		float Temp;
		Temp = P.X - Q.X;
		if ((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
		{
			Temp = P.Y - Q.Y;
			if ((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
			{
				Temp = P.Z - Q.Z;
				if ((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
				{
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * Compare two points and see if they're within specified distance.
	 *
	 * @param Point1 First vector.
	 * @param Point2 Second vector.
	 * @param Dist Specified distance.
	 * @return Whether two points are within the specified distance. Uses fast distance approximation (linear per-component distance).
	 */
	inline static bool PointsAreNear(const Vector& Point1, const Vector& Point2, float Dist)
	{
		float Temp;
		Temp = (Point1.X - Point2.X); if (Math::Abs(Temp) >= Dist) return false;
		Temp = (Point1.Y - Point2.Y); if (Math::Abs(Temp) >= Dist) return false;
		Temp = (Point1.Z - Point2.Z); if (Math::Abs(Temp) >= Dist) return false;
		return true;
	}


	/**
	 * Calculate the signed distance (in the direction of the normal) between a point and a plane.
	 *
	 * @param Point The Point we are checking.
	 * @param PlaneBase The Base Point in the plane.
	 * @param PlaneNormal The Normal of the plane (assumed to be unit length).
	 * @return Signed distance between point and plane.
	 */
	inline static float PointPlaneDist(const Vector& Point, const Vector& PlaneBase, const Vector& PlaneNormal)
	{
		return (Point - PlaneBase) | PlaneNormal;
	}

	/**
	 * Calculate the projection of a point on the given plane.
	 *
	 * @param Point The point to project onto the plane
	 * @param Plane The plane
	 * @return Projection of Point onto Plane
	 */
	static Vector PointPlaneProject(const Vector& Point, const Plane& Plane);

	/**
	 * Calculate the projection of a point on the plane defined by counter-clockwise (CCW) points A,B,C.
	 *
	 * @param Point The point to project onto the plane
	 * @param A 1st of three points in CCW order defining the plane
	 * @param B 2nd of three points in CCW order defining the plane
	 * @param C 3rd of three points in CCW order defining the plane
	 * @return Projection of Point onto plane ABC
	 */
	static Vector PointPlaneProject(const Vector& Point, const Vector& A, const Vector& B, const Vector& C);

	/**
	* Calculate the projection of a point on the plane defined by PlaneBase and PlaneNormal.
	*
	* @param Point The point to project onto the plane
	* @param PlaneBase Point on the plane
	* @param PlaneNorm Normal of the plane (assumed to be unit length).
	* @return Projection of Point onto plane
	*/
	inline static Vector PointPlaneProject(const Vector& Point, const Vector& PlaneBase, const Vector& PlaneNormal);

	/**
	 * Calculate the projection of a vector on the plane defined by PlaneNormal.
	 *
	 * @param  V The vector to project onto the plane.
	 * @param  PlaneNormal Normal of the plane (assumed to be unit length).
	 * @return Projection of V onto plane.
	 */
	inline static Vector VectorPlaneProject(const Vector& V, const Vector& PlaneNormal) { return V - V.ProjectOnToNormal(PlaneNormal); }

	/**
	 * Euclidean distance between two points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The distance between two points.
	 */
	static FORCEINLINE float Dist(const Vector& V1, const Vector& V2) { return Math::Sqrt(Vector::DistSquared(V1, V2)); }
	static FORCEINLINE float Distance(const Vector& V1, const Vector& V2) { return Dist(V1, V2); }

	/**
	* Euclidean distance between two points in the XY plane (ignoring Z).
	*
	* @param V1 The first point.
	* @param V2 The second point.
	* @return The distance between two points in the XY plane.
	*/
	static FORCEINLINE float DistXY(const Vector& V1, const Vector& V2) { return Math::Sqrt(Vector::DistSquaredXY(V1, V2)); }
	static FORCEINLINE float Dist2D(const Vector& V1, const Vector& V2) { return DistXY(V1, V2); }

	/**
	 * Squared distance between two points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The squared distance between two points.
	 */
	static FORCEINLINE float DistSquared(const Vector& V1, const Vector& V2)
	{
		return Math::Square(V2.X - V1.X) + Math::Square(V2.Y - V1.Y) + Math::Square(V2.Z - V1.Z);
	}

	/**
	 * Squared distance between two points in the XY plane only.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The squared distance between two points in the XY plane
	 */
	static FORCEINLINE float DistSquaredXY(const Vector& V1, const Vector& V2) { return Math::Square(V2.X - V1.X) + Math::Square(V2.Y - V1.Y); }
	static FORCEINLINE float DistSquared2D(const Vector& V1, const Vector& V2) { return DistSquaredXY(V1, V2); }

	/**
	 * Compute pushout of a box from a plane.
	 *
	 * @param Normal The plane normal.
	 * @param Size The size of the box.
	 * @return Pushout required.
	 */
	static FORCEINLINE float BoxPushOut(const Vector& Normal, const Vector& Size)
	{
		return Math::Abs(Normal.X * Size.X) + Math::Abs(Normal.Y * Size.Y) + Math::Abs(Normal.Z * Size.Z);
	}

	/**
	 * See if two normal vectors are nearly parallel, meaning the angle between them is close to 0 degrees.
	 *
	 * @param  Normal1 First normalized vector.
	 * @param  Normal1 Second normalized vector.
	 * @param  ParallelCosineThreshold Normals are parallel if absolute value of dot product (cosine of angle between them) is greater than or equal to this. For example: cos(1.0 degrees).
	 * @return true if vectors are nearly parallel, false otherwise.
	 */
	inline static bool Parallel(const Vector& Normal1, const Vector& Normal2, float ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL)
	{
		const float NormalDot = Normal1 | Normal2;
		return Math::Abs(NormalDot) >= ParallelCosineThreshold;
	}


	/**
	 * See if two normal vectors are coincident (nearly parallel and point in the same direction).
	 *
	 * @param  Normal1 First normalized vector.
	 * @param  Normal2 Second normalized vector.
	 * @param  ParallelCosineThreshold Normals are coincident if dot product (cosine of angle between them) is greater than or equal to this. For example: cos(1.0 degrees).
	 * @return true if vectors are coincident (nearly parallel and point in the same direction), false otherwise.
	 */
	inline static bool Coincident(const Vector& Normal1, const Vector& Normal2, float ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL)
	{
		const float NormalDot = Normal1 | Normal2;
		return NormalDot >= ParallelCosineThreshold;
	}

	/**
	 * See if two normal vectors are nearly orthogonal (perpendicular), meaning the angle between them is close to 90 degrees.
	 *
	 * @param  Normal1 First normalized vector.
	 * @param  Normal2 Second normalized vector.
	 * @param  OrthogonalCosineThreshold Normals are orthogonal if absolute value of dot product (cosine of angle between them) is less than or equal to this. For example: cos(89.0 degrees).
	 * @return true if vectors are orthogonal (perpendicular), false otherwise.
	 */
	inline static bool Orthogonal(const Vector& Normal1, const Vector& Normal2, float OrthogonalCosineThreshold = THRESH_NORMALS_ARE_ORTHOGONAL)
	{
		const float NormalDot = Normal1 | Normal2;
		return Math::Abs(NormalDot) <= OrthogonalCosineThreshold;
	}

	/**
	 * See if two planes are coplanar. They are coplanar if the normals are nearly parallel and the planes include the same set of points.
	 *
	 * @param Base1 The base point in the first plane.
	 * @param Normal1 The normal of the first plane.
	 * @param Base2 The base point in the second plane.
	 * @param Normal2 The normal of the second plane.
	 * @param ParallelCosineThreshold Normals are parallel if absolute value of dot product is greater than or equal to this.
	 * @return true if the planes are coplanar, false otherwise.
	 */
	inline static bool Coplanar(const Vector& Base1, const Vector& Normal1, const Vector& Base2, const Vector& Normal2, float ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL)
	{
		if (!Vector::Parallel(Normal1, Normal2, ParallelCosineThreshold)) return false;
		else if (Math::Abs(Vector::PointPlaneDist(Base2, Base1, Normal1)) > THRESH_POINT_ON_PLANE) return false;
		else return true;
	}

	/**
	 * Triple product of three vectors: X dot (Y cross Z).
	 *
	 * @param X The first vector.
	 * @param Y The second vector.
	 * @param Z The third vector.
	 * @return The triple product: X dot (Y cross Z).
	 */
	inline static float Triple(const Vector& X, const Vector& Y, const Vector& Z)
	{
		return
			((X.X * (Y.Y * Z.Z - Y.Z * Z.Y))
			+ (X.Y * (Y.Z * Z.X - Y.X * Z.Z))
			+ (X.Z * (Y.X * Z.Y - Y.Y * Z.X)));
	}

	/**
	 * Generates a list of sample points on a Bezier curve defined by 2 points.
	 *
	 * @param ControlPoints	Array of 4 Vectors (vert1, controlpoint1, controlpoint2, vert2).
	 * @param NumPoints Number of samples.
	 * @param OutPoints Receives the output samples.
	 * @return The path length.
	 */
	//static float EvaluateBezier(const Vector* ControlPoints, int32 NumPoints, TArray<Vector>& OutPoints);

	/**
	 * Converts a vector containing radian values to a vector containing degree values.
	 *
	 * @param RadVector	Vector containing radian values
	 * @return Vector  containing degree values
	 */
	inline static Vector RadiansToDegrees(const Vector& RadVector) { return RadVector * (180.f / PI); }

	/**
	 * Converts a vector containing degree values to a vector containing radian values.
	 *
	 * @param DegVector	Vector containing degree values
	 * @return Vector containing radian values
	 */
	inline static Vector DegreesToRadians(const Vector& DegVector) { return DegVector * (PI / 180.f); }

	/**
	 * Given a current set of cluster centers, a set of points, iterate N times to move clusters to be central.
	 *
	 * @param Clusters Reference to array of Clusters.
	 * @param Points Set of points.
	 * @param NumIterations Number of iterations.
	 * @param NumConnectionsToBeValid Sometimes you will have long strings that come off the mass of points
	 * which happen to have been chosen as Cluster starting points.  You want to be able to disregard those.
	 */
	//static void GenerateClusterCenters(TArray<Vector>& Clusters, const TArray<Vector>& Points, int32 NumIterations, int32 NumConnectionsToBeValid);

	/**
	 * Structured archive slot serializer.
	 *
	 * @param Slot Structured archive slot.
	 * @param V Vector to serialize.
	 */
	//friend void operator<<(FStructuredArchive::FSlot Slot, Vector& V)
	//{
	//	// @warning BulkSerialize: Vector is serialized as memory dump
	//	// See TArray::BulkSerialize for detailed description of implied limitations.
	//	FStructuredArchive::FRecord Record = Slot.EnterRecord();
	//	Record << SA_VALUE(TEXT("X"), V.X);
	//	Record << SA_VALUE(TEXT("Y"), V.Y);
	//	Record << SA_VALUE(TEXT("Z"), V.Z);
	//}

	// Serialize
	//Todo
};



/* Vector inline functions
 *****************************************************************************/

 /**
  * Multiplies a vector by a scaling factor.
  *
  * @param Scale Scaling factor.
  * @param V Vector to scale.
  * @return Result of multiplication.
  */
FORCEINLINE Vector operator*(float Scale, const Vector& V)
{
	return V.operator*(Scale);
}


FORCEINLINE Vector Vector::GetClampedToSize(float Min, float Max) const
{
	float VecSize = Size();
	const Vector VecDir = (VecSize > SMALL_NUMBER) ? (*this / VecSize) : Vector::ZeroVector;

	VecSize = Math::Clamp(VecSize, Min, Max);

	return VecSize * VecDir;
}

inline Vector Vector::PointPlaneProject(const Vector& Point, const Vector& PlaneBase, const Vector& PlaneNormal)
{
	//Find the distance of X from the plane
	//Add the distance back along the normal from the point
	return Point - Vector::PointPlaneDist(Point, PlaneBase, PlaneNormal) * PlaneNormal;
}


/* Vector2D inline functions
 *****************************************************************************/

FORCEINLINE Vector2D::Vector2D(const Vector& V)
	: X(V.X), Y(V.Y)
{
}

inline Vector Vector2D::SphericalToUnitCartesian() const
{
	const float SinTheta = Math::Sin(X);
	return Vector(Math::Cos(Y) * SinTheta, Math::Sin(Y) * SinTheta, Math::Cos(X));
}