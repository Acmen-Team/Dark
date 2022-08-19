// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../CoreTypes.h"
#include "MathUtility.h"
#include "Vector.h"
//#include "VectorRegister.h"
#include "Rotator.h"
#include "Matrix.h"

/**
 * Floating point quaternion that can represent a rotation about an axis in 3-D space.
 * The X, Y, Z, W components also double as the Axis/Angle format.
 *
 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
 * first applies B then A to any subsequent transformation (right first, then left).
 * Note that this is the opposite order of FTransform multiplication.
 *
 * Example: LocalToWorld = (LocalToWorld * DeltaRotation) will change rotation in local space by DeltaRotation.
 * Example: LocalToWorld = (DeltaRotation * LocalToWorld) will change rotation in world space by DeltaRotation.
 */
/*MS_ALIGN(16)*/ struct Quat
{
public:

	/** The quaternion's X-component. */
	float X;

	/** The quaternion's Y-component. */
	float Y;

	/** The quaternion's Z-component. */
	float Z;

	/** The quaternion's W-component. */
	float W;

public:

	/** Identity quaternion. */
	static const Quat Identity;

public:

	/** Default constructor (no initialization). */
	FORCEINLINE Quat() { }

	/**
	 * Creates and initializes a new quaternion, with the W component either 0 or 1.
	 *
	 * @param EForceInit Force init enum: if equal to ForceInitToZero then W is 0, otherwise W = 1 (creating an identity transform)
	 */
	//explicit FORCEINLINE Quat(EForceInit);

	/**
	 * Constructor.
	 *
	 * @param InX X component of the quaternion
	 * @param InY Y component of the quaternion
	 * @param InZ Z component of the quaternion
	 * @param InW W component of the quaternion
	 */
	FORCEINLINE Quat(float InX, float InY, float InZ, float InW);

	/**
	 * Creates and initializes a new quaternion from the XYZW values in the given VectorRegister.
	 *
	 * @param V XYZW components of the quaternion packed into a single VectorRegister
	 */
	explicit FORCEINLINE Quat(const VectorRegister& V);

	/**
	 * Creates and initializes a new quaternion from the given matrix.
	 *
	 * @param M The rotation matrix to initialize from.
	 */
	explicit Quat(const Matrix& M);

	/**
	 * Creates and initializes a new quaternion from the given rotator.
	 *
	 * @param R The rotator to initialize from.
	 */
	explicit Quat(const Rotator& R);

	/**
	 * Creates and initializes a new quaternion from the a rotation around the given axis.
	 *
	 * @param Axis assumed to be a normalized vector
	 * @param Angle angle to rotate above the given axis (in radians)
	 */
	Quat(Vector Axis, float AngleRad);

public:

#ifdef IMPLEMENT_ASSIGNMENT_OPERATOR_MANUALLY
	/**
	 * Copy another Quat into this one
	 *
	 * @return reference to this Quat
	 */
	//FORCEINLINE Quat& operator=(const Quat& Other);
#endif

	/**
	 * Gets the result of adding a Quaternion to this.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to add.
	 * @return The result of addition.
	 */
	FORCEINLINE Quat operator+(const Quat& Q) const;

	/**
	 * Adds to this quaternion.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Other The quaternion to add to this.
	 * @return Result after addition.
	 */
	FORCEINLINE Quat operator+=(const Quat& Q);

	/**
	 * Gets the result of subtracting a Quaternion to this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to subtract.
	 * @return The result of subtraction.
	 */
	FORCEINLINE Quat operator-(const Quat& Q) const;

	/**
	 * Checks whether another Quaternion is equal to this, within specified tolerance.
	 *
	 * @param Q The other Quaternion.
	 * @param Tolerance Error tolerance for comparison with other Quaternion.
	 * @return true if two Quaternions are equal, within specified tolerance, otherwise false.
	 */
	FORCEINLINE bool Equals(const Quat& Q, float Tolerance = KINDA_SMALL_NUMBER) const;

	/**
	 * Checks whether this Quaternion is an Identity Quaternion.
	 * Assumes Quaternion tested is normalized.
	 *
	 * @param Tolerance Error tolerance for comparison with Identity Quaternion.
	 * @return true if Quaternion is a normalized Identity Quaternion.
	 */
	FORCEINLINE bool IsIdentity(float Tolerance = SMALL_NUMBER) const;

	/**
	 * Subtracts another quaternion from this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param Q The other quaternion.
	 * @return reference to this after subtraction.
	 */
	FORCEINLINE Quat operator-=(const Quat& Q);

	/**
	 * Gets the result of multiplying this by another quaternion (this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q The Quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
	FORCEINLINE Quat operator*(const Quat& Q) const;

	/**
	 * Multiply this by a quaternion (this = this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q the quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
	FORCEINLINE Quat operator*=(const Quat& Q);

	/**
	 * Rotate a vector by this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation
	 * @see RotateVector
	 */
	Vector operator*(const Vector& V) const;

	/**
	 * Multiply this by a matrix.
	 * This matrix conversion came from
	 * http://www.m-hikari.com/ija/ija-password-2008/ija-password17-20-2008/aristidouIJA17-20-2008.pdf
	 * used for non-uniform scaling transform.
	 *
	 * @param M Matrix to multiply by.
	 * @return Matrix result after multiplication.
	 */
	Matrix operator*(const Matrix& M) const;

	/**
	 * Multiply this quaternion by a scaling factor.
	 *
	 * @param Scale The scaling factor.
	 * @return a reference to this after scaling.
	 */
	FORCEINLINE Quat operator*=(const float Scale);

	/**
	 * Get the result of scaling this quaternion.
	 *
	 * @param Scale The scaling factor.
	 * @return The result of scaling.
	 */
	FORCEINLINE Quat operator*(const float Scale) const;

	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return a reference to this after scaling.
	 */
	FORCEINLINE Quat operator/=(const float Scale);

	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return new Quaternion of this after division by scale.
	 */
	FORCEINLINE Quat operator/(const float Scale) const;

	/**
	 * Identical implementation for Quat properties.
	 * Avoids intrinsics to remain consistent with previous per-property comparison.
	 */
	bool Identical(const Quat* Q, const uint32 PortFlags) const;

	/**
	 * Checks whether two quaternions are identical.
	 * This is an exact comparison per-component;see Equals() for a comparison
	 * that allows for a small error tolerance and flipped axes of rotation.
	 *
	 * @param Q The other quaternion.
	 * @return true if two quaternion are identical, otherwise false.
	 * @see Equals
	 */
	bool operator==(const Quat& Q) const;

	/**
	 * Checks whether two quaternions are not identical.
	 *
	 * @param Q The other quaternion.
	 * @return true if two quaternion are not identical, otherwise false.
	 */
	bool operator!=(const Quat& Q) const;

	/**
	 * Calculates dot product of two quaternions.
	 *
	 * @param Q The other quaternions.
	 * @return The dot product.
	 */
	float operator|(const Quat& Q) const;

public:

	/**
	 * Convert a vector of floating-point Euler angles (in degrees) into a Quaternion.
	 *
	 * @param Euler the Euler angles
	 * @return constructed Quat
	 */
	static Quat MakeFromEuler(const Vector& Euler);

	/** Convert a Quaternion into floating-point Euler angles (in degrees). */
	Vector Euler() const;

	/**
	 * Normalize this quaternion if it is large enough.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param Tolerance Minimum squared length of quaternion for normalization.
	 */
	FORCEINLINE void Normalize(float Tolerance = SMALL_NUMBER);

	/**
	 * Get a normalized copy of this quaternion.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param Tolerance Minimum squared length of quaternion for normalization.
	 */
	FORCEINLINE Quat GetNormalized(float Tolerance = SMALL_NUMBER) const;

	// Return true if this quaternion is normalized
	bool IsNormalized() const;

	/**
	 * Get the length of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
	FORCEINLINE float Size() const;

	/**
	 * Get the length squared of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
	FORCEINLINE float SizeSquared() const;


	/** Get the angle of this quaternion */
	FORCEINLINE float GetAngle() const;

	/**
	 * get the axis and angle of rotation of this quaternion
	 *
	 * @param Axis{out] vector of axis of the quaternion
	 * @param Angle{out] angle of the quaternion
	 * @warning : assumes normalized quaternions.
	 */
	void ToAxisAndAngle(Vector& Axis, float& Angle) const;

	/**
	 * Get the swing and twist decomposition for a specified axis
	 *
	 * @param InTwistAxis Axis to use for decomposition
	 * @param OutSwing swing component quaternion
	 * @param OutTwist Twist component quaternion
	 * @warning assumes normalized quaternion and twist axis
	 */
	void ToSwingTwist(const Vector& InTwistAxis, Quat& OutSwing, Quat& OutTwist) const;

	/**
	 * Get the twist angle (in radians) for a specified axis
	 *
	 * @param TwistAxis Axis to use for decomposition
	 * @return Twist angle (in radians)
	 * @warning assumes normalized quaternion and twist axis
	 */
	float GetTwistAngle(const Vector& TwistAxis) const;

	/**
	 * Rotate a vector by this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation
	 */
	Vector RotateVector(Vector V) const;

	/**
	 * Rotate a vector by the inverse of this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation by the inverse of this quaternion.
	 */
	Vector UnrotateVector(Vector V) const;

	/**
	 * @return quaternion with W=0 and V=theta*v.
	 */
	Quat Log() const;

	/**
	 * @note Exp should really only be used after Log.
	 * Assumes a quaternion with W=0 and V=theta*v (where |v| = 1).
	 * Exp(q) = (sin(theta)*v, cos(theta))
	 */
	Quat Exp() const;

	/**
	 * @return inverse of this quaternion
	 */
	FORCEINLINE Quat Inverse() const;

	/**
	 * Enforce that the delta between this Quaternion and another one represents
	 * the shortest possible rotation angle
	 */
	void EnforceShortestArcWith(const Quat& OtherQuat);

	/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
	FORCEINLINE Vector GetAxisX() const;

	/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
	FORCEINLINE Vector GetAxisY() const;

	/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
	FORCEINLINE Vector GetAxisZ() const;

	/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
	FORCEINLINE Vector GetForwardVector() const;

	/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
	FORCEINLINE Vector GetRightVector() const;

	/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
	FORCEINLINE Vector GetUpVector() const;

	/** Convert a rotation into a unit vector facing in its direction. Equivalent to GetForwardVector(). */
	FORCEINLINE Vector ToVector() const;

	/** Get the Rotator representation of this Quaternion. */
	Rotator ToRotator() const;

	/**
	 * Get the axis of rotation of the Quaternion.
	 * This is the axis around which rotation occurs to transform the canonical coordinate system to the target orientation.
	 * For the identity Quaternion which has no such rotation, Vector(1,0,0) is returned.
	 */
	FORCEINLINE Vector GetRotationAxis() const;

	/** Find the angular distance between two rotation quaternions (in radians) */
	FORCEINLINE float AngularDistance(const Quat& Q) const;

	/**
	 * Serializes the vector compressed for e.g. network transmission.
	 * @param Ar Archive to serialize to/ from.
	 * @return false to allow the ordinary struct code to run (this never happens).
	 */
	//bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this Quat.
	 *
	 * @return true if there are any non-finite values in this Quaternion, otherwise false.
	 */
	bool ContainsNaN() const;

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	//FString ToString() const;

	/**
	 * Initialize this Quat from a FString.
	 * The string is expected to contain X=, Y=, Z=, W=, otherwise
	 * this Quat will have indeterminate (invalid) values.
	 *
	 * @param InSourceString FString containing the quaternion values.
	 * @return true if the Quat was initialized; false otherwise.
	 */
	//bool InitFromString(const FString& InSourceString);

public:

	FORCEINLINE void DiagnosticCheckNaN() const {}

public:

	/**
	 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	 */
	static FORCEINLINE Quat FindBetween(const Vector& Vector1, const Vector& Vector2)
	{
		return FindBetweenVectors(Vector1, Vector2);
	}

	/**
	 * Generates the 'smallest' (geodesic) rotation between two normals (assumed to be unit length).
	 */
	static Quat FindBetweenNormals(const Vector& Normal1, const Vector& Normal2);

	/**
	 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	 */
	static Quat FindBetweenVectors(const Vector& Vector1, const Vector& Vector2);

	/**
	 * Error measure (angle) between two quaternions, ranged [0..1].
	 * Returns the hypersphere-angle between two quaternions; alignment shouldn't matter, though
	 * @note normalized input is expected.
	 */
	static FORCEINLINE float Error(const Quat& Q1, const Quat& Q2);

	/**
	 * Quat::Error with auto-normalization.
	 */
	static FORCEINLINE float ErrorAutoNormalize(const Quat& A, const Quat& B);

	/**
	 * Fast Linear Quaternion Interpolation.
	 * Result is NOT normalized.
	 */
	static FORCEINLINE Quat FastLerp(const Quat& A, const Quat& B, const float Alpha);

	/**
	 * Bi-Linear Quaternion interpolation.
	 * Result is NOT normalized.
	 */
	static FORCEINLINE Quat FastBilerp(const Quat& P00, const Quat& P10, const Quat& P01, const Quat& P11, float FracX, float FracY);


	/** Spherical interpolation. Will correct alignment. Result is NOT normalized. */
	static Quat Slerp_NotNormalized(const Quat& Quat1, const Quat& Quat2, float Slerp);

	/** Spherical interpolation. Will correct alignment. Result is normalized. */
	static FORCEINLINE Quat Slerp(const Quat& Quat1, const Quat& Quat2, float Slerp)
	{
		return Slerp_NotNormalized(Quat1, Quat2, Slerp).GetNormalized();
	}

	/**
	 * Simpler Slerp that doesn't do any checks for 'shortest distance' etc.
	 * We need this for the cubic interpolation stuff so that the multiple Slerps dont go in different directions.
	 * Result is NOT normalized.
	 */
	static Quat SlerpFullPath_NotNormalized(const Quat& quat1, const Quat& quat2, float Alpha);

	/**
	 * Simpler Slerp that doesn't do any checks for 'shortest distance' etc.
	 * We need this for the cubic interpolation stuff so that the multiple Slerps dont go in different directions.
	 * Result is normalized.
	 */
	static FORCEINLINE Quat SlerpFullPath(const Quat& quat1, const Quat& quat2, float Alpha)
	{
		return SlerpFullPath_NotNormalized(quat1, quat2, Alpha).GetNormalized();
	}

	/**
	 * Given start and end quaternions of quat1 and quat2, and tangents at those points tang1 and tang2, calculate the point at Alpha (between 0 and 1) between them. Result is normalized.
	 * This will correct alignment by ensuring that the shortest path is taken.
	 */
	static Quat Squad(const Quat& quat1, const Quat& tang1, const Quat& quat2, const Quat& tang2, float Alpha);

	/**
	 * Simpler Squad that doesn't do any checks for 'shortest distance' etc.
	 * Given start and end quaternions of quat1 and quat2, and tangents at those points tang1 and tang2, calculate the point at Alpha (between 0 and 1) between them. Result is normalized.
	 */
	static Quat SquadFullPath(const Quat& quat1, const Quat& tang1, const Quat& quat2, const Quat& tang2, float Alpha);

	/**
	 * Calculate tangents between given points
	 *
	 * @param PrevP quaternion at P-1
	 * @param P quaternion to return the tangent
	 * @param NextP quaternion P+1
	 * @param Tension @todo document
	 * @param OutTan Out control point
	 */
	static void CalcTangents(const Quat& PrevP, const Quat& P, const Quat& NextP, float Tension, Quat& OutTan);

public:

	/**
	 * Serializes the quaternion.
	 *
	 * @param Ar Reference to the serialization archive.
	 * @param F Reference to the quaternion being serialized.
	 * @return Reference to the Archive after serialization.
	 */
	/*friend FArchive& operator<<(FArchive& Ar, Quat& F)
	{
		return Ar << F.X << F.Y << F.Z << F.W;
	}*/

	/*bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}*/

} /*GCC_ALIGN(16)*/;


/* Quat inline functions
 *****************************************************************************/

inline Quat::Quat(const Matrix& M)
{
	// If Matrix is NULL, return Identity quaternion. If any of them is 0, you won't be able to construct rotation
	// if you have two plane at least, we can reconstruct the frame using cross product, but that's a bit expensive op to do here
	// for now, if you convert to matrix from 0 scale and convert back, you'll lose rotation. Don't do that. 
	if (M.GetScaledAxis(EAxis::X).IsNearlyZero() || M.GetScaledAxis(EAxis::Y).IsNearlyZero() || M.GetScaledAxis(EAxis::Z).IsNearlyZero())
	{
		*this = Quat::Identity;
		return;
	}

	//const MeReal *const t = (MeReal *) tm;
	float	s;

	// Check diagonal (trace)
	const float tr = M.M[0][0] + M.M[1][1] + M.M[2][2];

	if (tr > 0.0f)
	{
		float InvS = Math::InvSqrt(tr + 1.f);
		this->W = 0.5f * (1.f / InvS);
		s = 0.5f * InvS;

		this->X = (M.M[1][2] - M.M[2][1]) * s;
		this->Y = (M.M[2][0] - M.M[0][2]) * s;
		this->Z = (M.M[0][1] - M.M[1][0]) * s;
	}
	else
	{
		// diagonal is negative
		int32 i = 0;

		if (M.M[1][1] > M.M[0][0])
			i = 1;

		if (M.M[2][2] > M.M[i][i])
			i = 2;

		static const int32 nxt[3] = { 1, 2, 0 };
		const int32 j = nxt[i];
		const int32 k = nxt[j];

		s = M.M[i][i] - M.M[j][j] - M.M[k][k] + 1.0f;

		float InvS = Math::InvSqrt(s);

		float qt[4] = {0.0f, 0.0f, 0.0f, 0.0f };
		qt[i] = 0.5f * (1.f / InvS);

		s = 0.5f * InvS;

		qt[3] = (M.M[j][k] - M.M[k][j]) * s;
		qt[j] = (M.M[i][j] + M.M[j][i]) * s;
		qt[k] = (M.M[i][k] + M.M[k][i]) * s;

		this->X = qt[0];
		this->Y = qt[1];
		this->Z = qt[2];
		this->W = qt[3];

		DiagnosticCheckNaN();
	}
}


FORCEINLINE Quat::Quat(const Rotator& R)
{
	*this = R.Quaternion();
	DiagnosticCheckNaN();
}


FORCEINLINE Vector Quat::operator*(const Vector& V) const
{
	return RotateVector(V);
}


inline Matrix Quat::operator*(const Matrix& M) const
{
	Matrix Result;
	Quat VT, VR;
	Quat Inv = Inverse();
	for (int32 I = 0; I < 4; ++I)
	{
		Quat VQ(M.M[I][0], M.M[I][1], M.M[I][2], M.M[I][3]);
		VectorQuaternionMultiply(&VT, this, &VQ);
		VectorQuaternionMultiply(&VR, &VT, &Inv);
		Result.M[I][0] = VR.X;
		Result.M[I][1] = VR.Y;
		Result.M[I][2] = VR.Z;
		Result.M[I][3] = VR.W;
	}

	return Result;
}


/* Quat inline functions
 *****************************************************************************/

//FORCEINLINE Quat::Quat(EForceInit ZeroOrNot)
//	: X(0), Y(0), Z(0), W(ZeroOrNot == ForceInitToZero ? 0.0f : 1.0f)
//{ }


FORCEINLINE Quat::Quat(float InX, float InY, float InZ, float InW)
	: X(InX)
	, Y(InY)
	, Z(InZ)
	, W(InW)
{
	DiagnosticCheckNaN();
}


FORCEINLINE Quat::Quat(const VectorRegister& V)
{
	VectorStoreAligned(V, this);
	DiagnosticCheckNaN();
}


//FORCEINLINE FString Quat::ToString() const
//{
//	return FString::Printf(TEXT("X=%.9f Y=%.9f Z=%.9f W=%.9f"), X, Y, Z, W);
//}

//inline bool Quat::InitFromString(const FString& InSourceString)
//{
//	X = Y = Z = 0.f;
//	W = 1.f;
//
//	const bool bSuccess
//		= FParse::Value(*InSourceString, TEXT("X="), X)
//		&& FParse::Value(*InSourceString, TEXT("Y="), Y)
//		&& FParse::Value(*InSourceString, TEXT("Z="), Z)
//		&& FParse::Value(*InSourceString, TEXT("W="), W);
//	DiagnosticCheckNaN();
//	return bSuccess;
//}


FORCEINLINE Quat::Quat(Vector Axis, float AngleRad)
{
	const float half_a = 0.5f * AngleRad;
	float s, c;
	Math::SinCos(&s, &c, half_a);

	X = s * Axis.X;
	Y = s * Axis.Y;
	Z = s * Axis.Z;
	W = c;

	DiagnosticCheckNaN();
}


FORCEINLINE Quat Quat::operator+(const Quat& Q) const
{
	return Quat(X + Q.X, Y + Q.Y, Z + Q.Z, W + Q.W);
}


FORCEINLINE Quat Quat::operator+=(const Quat& Q)
{
	this->X += Q.X;
	this->Y += Q.Y;
	this->Z += Q.Z;
	this->W += Q.W;

	DiagnosticCheckNaN();

	return *this;
}


FORCEINLINE Quat Quat::operator-(const Quat& Q) const
{
	return Quat(X - Q.X, Y - Q.Y, Z - Q.Z, W - Q.W);
}


FORCEINLINE bool Quat::Equals(const Quat& Q, float Tolerance) const
{
	return (Math::Abs(X - Q.X) <= Tolerance && Math::Abs(Y - Q.Y) <= Tolerance && Math::Abs(Z - Q.Z) <= Tolerance && Math::Abs(W - Q.W) <= Tolerance)
		|| (Math::Abs(X + Q.X) <= Tolerance && Math::Abs(Y + Q.Y) <= Tolerance && Math::Abs(Z + Q.Z) <= Tolerance && Math::Abs(W + Q.W) <= Tolerance);
}

FORCEINLINE bool Quat::IsIdentity(float Tolerance) const
{
	return Equals(Quat::Identity, Tolerance);
}

FORCEINLINE Quat Quat::operator-=(const Quat& Q)
{
	this->X -= Q.X;
	this->Y -= Q.Y;
	this->Z -= Q.Z;
	this->W -= Q.W;

	DiagnosticCheckNaN();

	return *this;
}


FORCEINLINE Quat Quat::operator*(const Quat& Q) const
{
	Quat Result;
	VectorQuaternionMultiply(&Result, this, &Q);

	Result.DiagnosticCheckNaN();

	return Result;
}


FORCEINLINE Quat Quat::operator*=(const Quat& Q)
{
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorLoadAligned(&Q);
	VectorRegister Result;
	VectorQuaternionMultiply(&Result, &A, &B);
	VectorStoreAligned(Result, this);

	DiagnosticCheckNaN();

	return *this;
}


FORCEINLINE Quat Quat::operator*=(const float Scale)
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorSetFloat1(Scale);
	VectorStoreAligned(VectorMultiply(A, B), this);
#else
	X *= Scale;
	Y *= Scale;
	Z *= Scale;
	W *= Scale;
#endif // PLATFORM_ENABLE_VECTORINTRINSICS

	DiagnosticCheckNaN();

	return *this;
}


FORCEINLINE Quat Quat::operator*(const float Scale) const
{
	return Quat(Scale * X, Scale * Y, Scale * Z, Scale * W);
}


FORCEINLINE Quat Quat::operator/=(const float Scale)
{
	const float Recip = 1.0f / Scale;
	X *= Recip;
	Y *= Recip;
	Z *= Recip;
	W *= Recip;

	DiagnosticCheckNaN();

	return *this;
}


FORCEINLINE Quat Quat::operator/(const float Scale) const
{
	const float Recip = 1.0f / Scale;
	return Quat(X * Recip, Y * Recip, Z * Recip, W * Recip);
}

FORCEINLINE bool Quat::Identical(const Quat* Q, const uint32 PortFlags) const
{
	return X == Q->X && Y == Q->Y && Z == Q->Z && W == Q->W;
}

FORCEINLINE bool Quat::operator==(const Quat& Q) const
{
	return X == Q.X && Y == Q.Y && Z == Q.Z && W == Q.W;
}


FORCEINLINE bool Quat::operator!=(const Quat& Q) const
{
	return X != Q.X || Y != Q.Y || Z != Q.Z || W != Q.W;
}


FORCEINLINE float Quat::operator|(const Quat& Q) const
{
	return X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
}


FORCEINLINE void Quat::Normalize(float Tolerance)
{
	const float SquareSum = X * X + Y * Y + Z * Z + W * W;

	if (SquareSum >= Tolerance)
	{
		const float Scale = Math::InvSqrt(SquareSum);

		X *= Scale;
		Y *= Scale;
		Z *= Scale;
		W *= Scale;
	}
	else
	{
		*this = Quat::Identity;
	}
}


FORCEINLINE Quat Quat::GetNormalized(float Tolerance) const
{
	Quat Result(*this);
	Result.Normalize(Tolerance);
	return Result;
}



FORCEINLINE bool Quat::IsNormalized() const
{
	return (Math::Abs(1.f - SizeSquared()) < THRESH_QUAT_NORMALIZED);
}


FORCEINLINE float Quat::Size() const
{
	return Math::Sqrt(X * X + Y * Y + Z * Z + W * W);
}


FORCEINLINE float Quat::SizeSquared() const
{
	return (X * X + Y * Y + Z * Z + W * W);
}

FORCEINLINE float Quat::GetAngle() const
{
	return 2.f * Math::Acos(W);
}


FORCEINLINE void Quat::ToAxisAndAngle(Vector& Axis, float& Angle) const
{
	Angle = GetAngle();
	Axis = GetRotationAxis();
}

FORCEINLINE Vector Quat::GetRotationAxis() const
{
	const float SquareSum = X * X + Y * Y + Z * Z;
	if (SquareSum < SMALL_NUMBER)
	{
		return Vector::XAxisVector;
	}
	const float Scale = Math::InvSqrt(SquareSum);
	return Vector(X * Scale, Y * Scale, Z * Scale);
}

float Quat::AngularDistance(const Quat& Q) const
{
	float InnerProd = X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
	return Math::Acos((2 * InnerProd * InnerProd) - 1.f);
}


FORCEINLINE Vector Quat::RotateVector(Vector V) const
{
	// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
	// V' = V + 2w(Q x V) + (2Q x (Q x V))
	// refactor:
	// V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
	// T = 2(Q x V);
	// V' = V + w*(T) + (Q x T)

	const Vector Q(X, Y, Z);
	const Vector T = 2.f * Vector::CrossProduct(Q, V);
	const Vector Result = V + (W * T) + Vector::CrossProduct(Q, T);
	return Result;
}

FORCEINLINE Vector Quat::UnrotateVector(Vector V) const
{
	const Vector Q(-X, -Y, -Z); // Inverse
	const Vector T = 2.f * Vector::CrossProduct(Q, V);
	const Vector Result = V + (W * T) + Vector::CrossProduct(Q, T);
	return Result;
}


FORCEINLINE Quat Quat::Inverse() const
{
	//checkSlow(IsNormalized());

	return Quat(-X, -Y, -Z, W);
}


FORCEINLINE void Quat::EnforceShortestArcWith(const Quat& OtherQuat)
{
	const float DotResult = (OtherQuat | *this);
	const float Bias = Math::FloatSelect(DotResult, 1.0f, -1.0f);

	X *= Bias;
	Y *= Bias;
	Z *= Bias;
	W *= Bias;
}


FORCEINLINE Vector Quat::GetAxisX() const
{
	return RotateVector(Vector(1.f, 0.f, 0.f));
}


FORCEINLINE Vector Quat::GetAxisY() const
{
	return RotateVector(Vector(0.f, 1.f, 0.f));
}


FORCEINLINE Vector Quat::GetAxisZ() const
{
	return RotateVector(Vector(0.f, 0.f, 1.f));
}


FORCEINLINE Vector Quat::GetForwardVector() const
{
	return GetAxisX();
}

FORCEINLINE Vector Quat::GetRightVector() const
{
	return GetAxisY();
}

FORCEINLINE Vector Quat::GetUpVector() const
{
	return GetAxisZ();
}

FORCEINLINE Vector Quat::ToVector() const
{
	return GetAxisX();
}


FORCEINLINE float Quat::Error(const Quat& Q1, const Quat& Q2)
{
	const float cosom = Math::Abs(Q1.X * Q2.X + Q1.Y * Q2.Y + Q1.Z * Q2.Z + Q1.W * Q2.W);
	return (Math::Abs(cosom) < 0.9999999f) ? Math::Acos(cosom) * (1.f / PI) : 0.0f;
}


FORCEINLINE float Quat::ErrorAutoNormalize(const Quat& A, const Quat& B)
{
	Quat Q1 = A;
	Q1.Normalize();

	Quat Q2 = B;
	Q2.Normalize();

	return Quat::Error(Q1, Q2);
}

/**
 * Fast Linear Quaternion Interpolation.
 * Result is NOT normalized.
 */
FORCEINLINE Quat Quat::FastLerp(const Quat& A, const Quat& B, const float Alpha)
{
	// To ensure the 'shortest route', we make sure the dot product between the both rotations is positive.
	const float DotResult = (A | B);
	const float Bias = Math::FloatSelect(DotResult, 1.0f, -1.0f);
	return (B * Alpha) + (A * (Bias * (1.f - Alpha)));
}


FORCEINLINE Quat Quat::FastBilerp(const Quat& P00, const Quat& P10, const Quat& P01, const Quat& P11, float FracX, float FracY)
{
	return Quat::FastLerp(
		Quat::FastLerp(P00, P10, FracX),
		Quat::FastLerp(P01, P11, FracX),
		FracY
	);
}


FORCEINLINE bool Quat::ContainsNaN() const
{
	return (!Math::IsFinite(X) ||
		!Math::IsFinite(Y) ||
		!Math::IsFinite(Z) ||
		!Math::IsFinite(W)
		);
}


//template<> struct TIsPODType<Quat> { enum { Value = true }; };

/* Math inline functions
 *****************************************************************************/

//template<class U>
//FORCEINLINE_DEBUGGABLE Quat Math::Lerp(const Quat& A, const Quat& B, const U& Alpha)
//{
//	return Quat::Slerp(A, B, Alpha);
//}

//template<class U>
//FORCEINLINE_DEBUGGABLE Quat Math::BiLerp(const Quat& P00, const Quat& P10, const Quat& P01, const Quat& P11, float FracX, float FracY)
//{
//	Quat Result;
//
//	Result = Lerp(
//		Quat::Slerp_NotNormalized(P00, P10, FracX),
//		Quat::Slerp_NotNormalized(P01, P11, FracX),
//		FracY
//	);
//
//	return Result;
//}
//
//template<class U>
//FORCEINLINE_DEBUGGABLE Quat Math::CubicInterp(const Quat& P0, const Quat& T0, const Quat& P1, const Quat& T1, const U& A)
//{
//	return Quat::Squad(P0, T0, P1, T1, A);
//}

