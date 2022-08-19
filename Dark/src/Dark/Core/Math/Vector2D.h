// Copyright LiuD, Inc. All Rights Reserved.

#pragma once
#include "MathUtility.h"

struct Vector2D
{
	/** Vector's X component. */
	float X;

	/** Vector's Y component. */
	float Y;

public:

	/** Global 2D zero vector constant (0,0) */
	static const Vector2D ZeroVector;

	/**
	* Global 2D one vector (poorly named) constant (1,1).
	*
	* @note Incorrectly named "unit" vector though its magnitude/length/size is not one. Would fix, though likely used all over the world. Use `Unit45Deg` below for an actual unit vector.
	*/
	static const Vector2D UnitVector;

	/**
	* Global 2D unit vector constant along the 45 degree angle or symmetrical positive axes (sqrt(.5),sqrt(.5)) or (.707,.707). https://en.wikipedia.org/wiki/Unit_vector
	*
	* @note The `UnitVector` above is actually a value with axes of 1 rather than a magnitude of one.
	*/
	static const Vector2D Unit45Deg;

public:

	/** Default constructor (no initialization). */
	FORCEINLINE Vector2D() { }

	/**
	 * Constructor using initial values for each component.
	 *
	 * @param InX X coordinate.
	 * @param InY Y coordinate.
	 */
	FORCEINLINE Vector2D(float InX, float InY) : X(InX), Y(InY) { }

	/**
	 * Constructor initializing both components to a single float value.
	 *
	 * @param InF Value to set both components to.
	 */
	explicit FORCEINLINE Vector2D(float InF) : X(InF), Y(InF) { }

	/**
	 * Constructs a vector from an FIntPoint.
	 *
	 * @param InPos Integer point used to set this vector.
	 */
	FORCEINLINE Vector2D(IntPoint InPos) { X = (float)InPos.X; Y = (float)InPos.Y; }

	/**
	 * Constructor which initializes all components to zero.
	 *
	 * @param EForceInit Force init enum
	 */
	//explicit FORCEINLINE Vector2D(EForceInit);

	/**
	 * Constructs a vector from an Vector.
	 * Copies the X and Y components from the Vector.
	 *
	 * @param V Vector to copy from.
	 */
	explicit FORCEINLINE Vector2D(const Vector& V);

	/**
	 * Constructs a vector from an Vector4.
	 * Copies the X and Y components from the Vector4.
	 *
	 * @param V Vector to copy from.
	 */
	explicit FORCEINLINE Vector2D(const Vector4& V);

public:

	/**
	 * Gets the result of adding two vectors together.
	 *
	 * @param V The other vector to add to this.
	 * @return The result of adding the vectors together.
	 */
	FORCEINLINE Vector2D operator+(const Vector2D& V) const { return Vector2D(X + V.X, Y + V.Y); }

	/**
	 * Gets the result of subtracting a vector from this one.
	 *
	 * @param V The other vector to subtract from this.
	 * @return The result of the subtraction.
	 */
	FORCEINLINE Vector2D operator-(const Vector2D& V) const { return Vector2D(X - V.X, Y - V.Y); }

	/**
	 * Gets the result of scaling the vector (multiplying each component by a value).
	 *
	 * @param Scale How much to scale the vector by.
	 * @return The result of scaling this vector.
	 */
	FORCEINLINE Vector2D operator*(float Scale) const { return Vector2D(X * Scale, Y * Scale); }

	/**
	 * Gets the result of dividing each component of the vector by a value.
	 *
	 * @param Scale How much to divide the vector by.
	 * @return The result of division on this vector.
	 */
	FORCEINLINE Vector2D operator/(float Scale) const
	{
		const float RScale = 1.f / Scale;
		return Vector2D(X * RScale, Y * RScale);
	}

	/**
	 * Gets the result of adding A to each component of the vector.
	 *
	 * @param A Float to add to each component.
	 * @return The result of adding A to each component.
	 */
	FORCEINLINE Vector2D operator+(float A) const { return Vector2D(X + A, Y + A); }

	/**
	 * Gets the result of subtracting A from each component of the vector.
	 *
	 * @param A Float to subtract from each component
	 * @return The result of subtracting A from each component.
	 */
	FORCEINLINE Vector2D operator-(float A) const { return Vector2D(X - A, Y - A); }

	/**
	 * Gets the result of component-wise multiplication of this vector by another.
	 *
	 * @param V The other vector to multiply this by.
	 * @return The result of the multiplication.
	 */
	FORCEINLINE Vector2D operator*(const Vector2D& V) const { return Vector2D(X * V.X, Y * V.Y); }

	/**
	 * Gets the result of component-wise division of this vector by another.
	 *
	 * @param V The other vector to divide this by.
	 * @return The result of the division.
	 */
	FORCEINLINE Vector2D operator/(const Vector2D& V) const { return Vector2D(X / V.X, Y / V.Y); }

	/**
	 * Calculates dot product of this vector and another.
	 *
	 * @param V The other vector.
	 * @return The dot product.
	 */
	FORCEINLINE float operator|(const Vector2D& V) const { return X * V.X + Y * V.Y; }

	/**
	 * Calculates cross product of this vector and another.
	 *
	 * @param V The other vector.
	 * @return The cross product.
	 */
	FORCEINLINE float operator^(const Vector2D& V) const { return X * V.Y - Y * V.X; }

public:

	/**
	 * Compares this vector against another for equality.
	 *
	 * @param V The vector to compare against.
	 * @return true if the two vectors are equal, otherwise false.
	 */
	FORCEINLINE bool operator==(const Vector2D& V) const { return X == V.X && Y == V.Y; }

	/**
	 * Compares this vector against another for inequality.
	 *
	 * @param V The vector to compare against.
	 * @return true if the two vectors are not equal, otherwise false.
	 */
	FORCEINLINE bool operator!=(const Vector2D& V) const { return X != V.X || Y != V.Y; }

	/**
	 * Checks whether both components of this vector are less than another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this is the smaller vector, otherwise false.
	 */
	FORCEINLINE bool operator<(const Vector2D& Other) const { return X < Other.X&& Y < Other.Y; }

	/**
	 * Checks whether both components of this vector are greater than another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this is the larger vector, otherwise false.
	 */
	FORCEINLINE bool operator>(const Vector2D& Other) const { return X > Other.X && Y > Other.Y; }

	/**
	 * Checks whether both components of this vector are less than or equal to another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this vector is less than or equal to the other vector, otherwise false.
	 */
	FORCEINLINE bool operator<=(const Vector2D& Other) const { return X <= Other.X && Y <= Other.Y; }

	/**
	 * Checks whether both components of this vector are greater than or equal to another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this vector is greater than or equal to the other vector, otherwise false.
	 */
	FORCEINLINE bool operator>=(const Vector2D& Other) const { return X >= Other.X && Y >= Other.Y; }

	/**
	 * Gets a negated copy of the vector.
	 *
	 * @return A negated copy of the vector.
	 */
	FORCEINLINE Vector2D operator-() const { return Vector2D(-X, -Y); }

	/**
	 * Adds another vector to this.
	 *
	 * @param V The other vector to add.
	 * @return Copy of the vector after addition.
	 */
	FORCEINLINE Vector2D operator+=(const Vector2D& V) { X += V.X; Y += V.Y; return *this; }

	/**
	 * Subtracts another vector from this.
	 *
	 * @param V The other vector to subtract.
	 * @return Copy of the vector after subtraction.
	 */
	FORCEINLINE Vector2D operator-=(const Vector2D& V) { X -= V.X; Y -= V.Y; return *this; }

	/**
	 * Scales this vector.
	 *
	 * @param Scale The scale to multiply vector by.
	 * @return Copy of the vector after scaling.
	 */
	FORCEINLINE Vector2D operator*=(float Scale) { X *= Scale; Y *= Scale; return *this; }

	/**
	 * Divides this vector.
	 *
	 * @param V What to divide vector by.
	 * @return Copy of the vector after division.
	 */
	FORCEINLINE Vector2D operator/=(float V)
	{
		const float RV = 1.f / V;
		X *= RV; Y *= RV;
		return *this;
	}

	/**
	 * Multiplies this vector with another vector, using component-wise multiplication.
	 *
	 * @param V The vector to multiply with.
	 * @return Copy of the vector after multiplication.
	 */
	FORCEINLINE Vector2D operator*=(const Vector2D& V) { X *= V.X; Y *= V.Y; return *this; }
	
	/**
	 * Divides this vector by another vector, using component-wise division.
	 *
	 * @param V The vector to divide by.
	 * @return Copy of the vector after division.
	 */
	FORCEINLINE Vector2D operator/=(const Vector2D& V) { X /= V.X; Y /= V.Y; return *this; }
	
	/**
	 * Gets specific component of the vector.
	 *
	 * @param Index the index of vector component
	 * @return reference to component.
	 */
	FORCEINLINE float& operator[](int32 Index)
	{
		//check(Index >= 0 && Index < 2);
		return ((Index == 0) ? X : Y);
	}

	/**
	 * Gets specific component of the vector.
	 *
	 * @param Index the index of vector component
	 * @return copy of component value.
	 */
	FORCEINLINE float operator[](int32 Index) const
	{
		//check(Index >= 0 && Index < 2);
		return ((Index == 0) ? X : Y);
	}

	/**
	* Gets a specific component of the vector.
	*
	* @param Index The index of the component required.
	* @return Reference to the specified component.
	*/
	FORCEINLINE float& Component(int32 Index) { return (&X)[Index]; }

	/**
	* Gets a specific component of the vector.
	*
	* @param Index The index of the component required.
	* @return Copy of the specified component.
	*/
	FORCEINLINE float Component(int32 Index) const { return (&X)[Index]; }

public:

	/**
	 * Calculates the dot product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The dot product.
	 */
	FORCEINLINE static float DotProduct(const Vector2D& A, const Vector2D& B) { return A | B; }

	/**
	 * Squared distance between two 2D points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The squared distance between two 2D points.
	 */
	FORCEINLINE static float DistSquared(const Vector2D& V1, const Vector2D& V2) { return Math::Square(V2.X - V1.X) + Math::Square(V2.Y - V1.Y); }

	/**
	 * Distance between two 2D points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The distance between two 2D points.
	 */
	FORCEINLINE static float Distance(const Vector2D& V1, const Vector2D& V2) { return Math::Sqrt(Vector2D::DistSquared(V1, V2)); }

	/**
	 * Calculate the cross product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The cross product.
	 */
	FORCEINLINE static float CrossProduct(const Vector2D& A, const Vector2D& B) { return A ^ B; }

	/**
	 * Returns a vector with the maximum component for each dimension from the pair of vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The max vector.
	 */
	FORCEINLINE static Vector2D Max(const Vector2D& A, const Vector2D& B) { return Vector2D(Math::Max(A.X, B.X), Math::Max(A.Y, B.Y)); }

	/**
	 * Returns a vector with the minimum component for each dimension from the pair of vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The min vector.
	 */
	FORCEINLINE static Vector2D Min(const Vector2D& A, const Vector2D& B) { return Vector2D(Math::Min(A.X, B.X), Math::Min(A.Y, B.Y)); }

	/**
	 * Checks for equality with error-tolerant comparison.
	 *
	 * @param V The vector to compare.
	 * @param Tolerance Error tolerance.
	 * @return true if the vectors are equal within specified tolerance, otherwise false.
	 */
	FORCEINLINE bool Equals(const Vector2D& V, float Tolerance = KINDA_SMALL_NUMBER) const { return Math::Abs(X - V.X) <= Tolerance && Math::Abs(Y - V.Y) <= Tolerance; }

	/**
	 * Set the values of the vector directly.
	 *
	 * @param InX New X coordinate.
	 * @param InY New Y coordinate.
	 */
	FORCEINLINE void Set(float InX, float InY) { X = InX; Y = InY; }

	/**
	 * Get the maximum value of the vector's components.
	 *
	 * @return The maximum value of the vector's components.
	 */
	FORCEINLINE float GetMax() const { return Math::Max(X, Y); }

	/**
	 * Get the maximum absolute value of the vector's components.
	 *
	 * @return The maximum absolute value of the vector's components.
	 */
	FORCEINLINE float GetAbsMax() const{ return Math::Max(Math::Abs(X), Math::Abs(Y)); }

	/**
	 * Get the minimum value of the vector's components.
	 *
	 * @return The minimum value of the vector's components.
	 */
	FORCEINLINE float GetMin() const { return Math::Min(X, Y); }

	/**
	 * Get the length (magnitude) of this vector.
	 *
	 * @return The length of this vector.
	 */
	FORCEINLINE float Size() const { return Math::Sqrt(X * X + Y * Y); }

	/**
	 * Get the squared length of this vector.
	 *
	 * @return The squared length of this vector.
	 */
	FORCEINLINE float SizeSquared() const { return X * X + Y * Y; }

	/**
	 * Rotates around axis (0,0,1)
	 *
	 * @param AngleDeg Angle to rotate (in degrees)
	 * @return Rotated Vector
	 */
	FORCEINLINE Vector2D GetRotated(float AngleDeg) const
	{
		// Based on Vector::RotateAngleAxis with Axis(0,0,1)

		float S, C;
		Math::SinCos(&S, &C, Math::DegreesToRadians(AngleDeg));

		//const float OMC = 1.0f - C;

		return Vector2D(
			C * X - S * Y,
			S * X + C * Y);
	}

	/**
	 * Gets a normalized copy of the vector, checking it is safe to do so based on the length.
	 * Returns zero vector if vector length is too small to safely normalize.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @return A normalized copy of the vector if safe, (0,0) otherwise.
	 */
	FORCEINLINE Vector2D GetSafeNormal(float Tolerance = SMALL_NUMBER) const
	{
		const float SquareSum = X * X + Y * Y;
		if (SquareSum > Tolerance)
		{
			const float Scale = Math::InvSqrt(SquareSum);
			return Vector2D(X * Scale, Y * Scale);
		}
		return Vector2D(0.f, 0.f);
	}

	/**
	 * Normalize this vector in-place if it is large enough, set it to (0,0) otherwise.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @see GetSafeNormal()
	 */
	FORCEINLINE void Normalize(float Tolerance = SMALL_NUMBER)
	{
		const float SquareSum = X * X + Y * Y;
		if (SquareSum > Tolerance)
		{
			const float Scale = Math::InvSqrt(SquareSum);
			X *= Scale;
			Y *= Scale;
			return;
		}
		X = 0.0f;
		Y = 0.0f;
	}

	/**
	 * Checks whether vector is near to zero within a specified tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 * @return true if vector is in tolerance to zero, otherwise false.
	 */
	FORCEINLINE bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const { return	Math::Abs(X) <= Tolerance && Math::Abs(Y) <= Tolerance; }
	

	/**
	 * Util to convert this vector into a unit direction vector and its original length.
	 *
	 * @param OutDir Reference passed in to store unit direction vector.
	 * @param OutLength Reference passed in to store length of the vector.
	 */
	FORCEINLINE void ToDirectionAndLength(Vector2D& OutDir, float& OutLength) const
	{
		OutLength = Size();
		if (OutLength > SMALL_NUMBER)
		{
			float OneOverLength = 1.0f / OutLength;
			OutDir = Vector2D(X * OneOverLength, Y * OneOverLength);
		}
		else
		{
			OutDir = Vector2D::ZeroVector;
		}
	}


	/**
	 * Checks whether all components of the vector are exactly zero.
	 *
	 * @return true if vector is exactly zero, otherwise false.
	 */
	FORCEINLINE bool IsZero() const { return X == 0.f && Y == 0.f; }

	/**
	 * Get this vector as an Int Point.
	 *
	 * @return New Int Point from this vector.
	 */
	FORCEINLINE IntPoint IntPoint() const { return IntVector2D(Math::RoundToInt(X), Math::RoundToInt(Y)); }

	/**
	 * Get this vector as a vector where each component has been rounded to the nearest int.
	 *
	 * @return New Vector2D from this vector that is rounded.
	 */
	FORCEINLINE Vector2D RoundToVector() const { return Vector2D(Math::RoundToFloat(X), Math::RoundToFloat(Y)); }

	/**
	 * Creates a copy of this vector with both axes clamped to the given range.
	 * @return New vector with clamped axes.
	 */
	FORCEINLINE Vector2D ClampAxes(float MinAxisVal, float MaxAxisVal) const
	{
		return Vector2D(Math::Clamp(X, MinAxisVal, MaxAxisVal), Math::Clamp(Y, MinAxisVal, MaxAxisVal));
	}

	/**
	* Get a copy of the vector as sign only.
	* Each component is set to +1 or -1, with the sign of zero treated as +1.
	*
	* @param A copy of the vector with each component set to +1 or -1
	*/
	FORCEINLINE Vector2D GetSignVector() const
	{
		return Vector2D
		(
			Math::FloatSelect(X, 1.f, -1.f),
			Math::FloatSelect(Y, 1.f, -1.f)
		);
	}

	/**
	* Get a copy of this vector with absolute value of each component.
	*
	* @return A copy of this vector with absolute value of each component.
	*/
	FORCEINLINE Vector2D GetAbs() const { return Vector2D(Math::Abs(X), Math::Abs(Y)); }

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	//FString ToString() const;

	/**
	 * Initialize this Vector based on an FString. The String is expected to contain X=, Y=.
	 * The Vector2D will be bogus when InitFromString returns false.
	 *
	 * @param	InSourceString	FString containing the vector values.
	 * @return true if the X,Y values were read successfully; false otherwise.
	 */
	//bool InitFromString(const FString& InSourceString);

	// Serialize
	//Todo


	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this vector.
	 *
	 * @return true if there are any non-finite values in this vector, false otherwise.
	 */
	FORCEINLINE bool ContainsNaN() const { return (!Math::IsFinite(X) || !Math::IsFinite(Y)); }

	/** Converts spherical coordinates on the unit sphere into a Cartesian unit length vector. */
	inline Vector SphericalToUnitCartesian() const;
};


/* Vector2D inline functions
 *****************************************************************************/

FORCEINLINE Vector2D operator*(float Scale, const Vector2D& V)
{
	return V.operator*(Scale);
}