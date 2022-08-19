// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Vector.h"
#include "Rotator.h"
#include "Matrix.h"
#include "Quat.h"

/** Combined rotation and translation matrix */
class RotationTranslationMatrix
	: public Matrix
{
public:

	/**
	 * Constructor.
	 *
	 * @param Rot rotation
	 * @param Origin translation to apply
	 */
	RotationTranslationMatrix(const Rotator& Rot, const Vector& Origin);

	/** Matrix factory. Return an Matrix so we don't have type conversion issues in expressions. */
	static Matrix Make(const Rotator& Rot, const Vector& Origin)
	{
		return RotationTranslationMatrix(Rot, Origin);
	}
};

FORCEINLINE RotationTranslationMatrix::RotationTranslationMatrix(const Rotator& Rot, const Vector& Origin)
{
#if 0

	const VectorRegister Angles = MakeVectorRegister(Rot.Pitch, Rot.Yaw, Rot.Roll, 0.0f);
	const VectorRegister HalfAngles = VectorMultiply(Angles, GlobalVectorConstants::DEG_TO_RAD);

	union { VectorRegister v; float f[4]; } SinAngles, CosAngles;
	VectorSinCos(&SinAngles.v, &CosAngles.v, &HalfAngles);

	const float	SP = SinAngles.f[0];
	const float	SY = SinAngles.f[1];
	const float	SR = SinAngles.f[2];
	const float	CP = CosAngles.f[0];
	const float	CY = CosAngles.f[1];
	const float	CR = CosAngles.f[2];

#else

	float SP, SY, SR;
	float CP, CY, CR;
	Math::SinCos(&SP, &CP, Math::DegreesToRadians(Rot.Pitch));
	Math::SinCos(&SY, &CY, Math::DegreesToRadians(Rot.Yaw));
	Math::SinCos(&SR, &CR, Math::DegreesToRadians(Rot.Roll));

#endif // PLATFORM_ENABLE_VECTORINTRINSICS

	M[0][0] = CP * CY;
	M[0][1] = CP * SY;
	M[0][2] = SP;
	M[0][3] = 0.f;

	M[1][0] = SR * SP * CY - CR * SY;
	M[1][1] = SR * SP * SY + CR * CY;
	M[1][2] = -SR * CP;
	M[1][3] = 0.f;

	M[2][0] = -(CR * SP * CY + SR * SY);
	M[2][1] = CY * SR - CR * SP * SY;
	M[2][2] = CR * CP;
	M[2][3] = 0.f;

	M[3][0] = Origin.X;
	M[3][1] = Origin.Y;
	M[3][2] = Origin.Z;
	M[3][3] = 1.f;
}

/** Rotation and translation matrix using quaternion rotation */
class QuatRotationTranslationMatrix
	: public Matrix
{
public:

	/** Constructor
	*
	* @param Q rotation
	* @param Origin translation to apply
	*/
	QuatRotationTranslationMatrix(const Quat& Q, const Vector& Origin);

	/** Matrix factory. Return an Matrix so we don't have type conversion issues in expressions. */
	static Matrix Make(const Quat& Q, const Vector& Origin)
	{
		return QuatRotationTranslationMatrix(Q, Origin);
	}
};


/** Rotation matrix using quaternion rotation */
class QuatRotationMatrix
	: public QuatRotationTranslationMatrix
{
public:

	/** Constructor
	*
	* @param Q rotation
	*/
	QuatRotationMatrix(const Quat& Q)
		: QuatRotationTranslationMatrix(Q, Vector::ZeroVector)
	{
	}

	/** Matrix factory. Return an Matrix so we don't have type conversion issues in expressions. */
	static Matrix Make(const Quat& Q)
	{
		return QuatRotationMatrix(Q);
	}
};

FORCEINLINE QuatRotationTranslationMatrix::QuatRotationTranslationMatrix(const Quat& Q, const Vector& Origin)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) && WITH_EDITORONLY_DATA
	// Make sure Quaternion is normalized
	check(Q.IsNormalized());
#endif
	const float x2 = Q.X + Q.X;  const float y2 = Q.Y + Q.Y;  const float z2 = Q.Z + Q.Z;
	const float xx = Q.X * x2;   const float xy = Q.X * y2;   const float xz = Q.X * z2;
	const float yy = Q.Y * y2;   const float yz = Q.Y * z2;   const float zz = Q.Z * z2;
	const float wx = Q.W * x2;   const float wy = Q.W * y2;   const float wz = Q.W * z2;

	M[0][0] = 1.0f - (yy + zz);	M[1][0] = xy - wz;				M[2][0] = xz + wy;			M[3][0] = Origin.X;
	M[0][1] = xy + wz;			M[1][1] = 1.0f - (xx + zz);		M[2][1] = yz - wx;			M[3][1] = Origin.Y;
	M[0][2] = xz - wy;			M[1][2] = yz + wx;				M[2][2] = 1.0f - (xx + yy);	M[3][2] = Origin.Z;
	M[0][3] = 0.0f;				M[1][3] = 0.0f;					M[2][3] = 0.0f;				M[3][3] = 1.0f;
}


/** Rotation matrix no translation */
class RotationMatrix
	: public RotationTranslationMatrix
{
public:

	/**
	 * Constructor.
	 *
	 * @param Rot rotation
	 */
	RotationMatrix(const Rotator& Rot)
		: RotationTranslationMatrix(Rot, Vector::ZeroVector)
	{ }

	/** Matrix factory. Return an Matrix so we don't have type conversion issues in expressions. */
	static Matrix Make(Rotator const& Rot)
	{
		return RotationMatrix(Rot);
	}

	/** Matrix factory. Return an Matrix so we don't have type conversion issues in expressions. */
	static Matrix Make(Quat const& Rot);

	/** Builds a rotation matrix given only a XAxis. Y and Z are unspecified but will be orthonormal. XAxis need not be normalized. */
	static Matrix MakeFromX(Vector const& XAxis);

	/** Builds a rotation matrix given only a YAxis. X and Z are unspecified but will be orthonormal. YAxis need not be normalized. */
	static Matrix MakeFromY(Vector const& YAxis);

	/** Builds a rotation matrix given only a ZAxis. X and Y are unspecified but will be orthonormal. ZAxis need not be normalized. */
	static Matrix MakeFromZ(Vector const& ZAxis);

	/** Builds a matrix with given X and Y axes. X will remain fixed, Y may be changed minimally to enforce orthogonality. Z will be computed. Inputs need not be normalized. */
	static Matrix MakeFromXY(Vector const& XAxis, Vector const& YAxis);

	/** Builds a matrix with given X and Z axes. X will remain fixed, Z may be changed minimally to enforce orthogonality. Y will be computed. Inputs need not be normalized. */
	static Matrix MakeFromXZ(Vector const& XAxis, Vector const& ZAxis);

	/** Builds a matrix with given Y and X axes. Y will remain fixed, X may be changed minimally to enforce orthogonality. Z will be computed. Inputs need not be normalized. */
	static Matrix MakeFromYX(Vector const& YAxis, Vector const& XAxis);

	/** Builds a matrix with given Y and Z axes. Y will remain fixed, Z may be changed minimally to enforce orthogonality. X will be computed. Inputs need not be normalized. */
	static Matrix MakeFromYZ(Vector const& YAxis, Vector const& ZAxis);

	/** Builds a matrix with given Z and X axes. Z will remain fixed, X may be changed minimally to enforce orthogonality. Y will be computed. Inputs need not be normalized. */
	static Matrix MakeFromZX(Vector const& ZAxis, Vector const& XAxis);

	/** Builds a matrix with given Z and Y axes. Z will remain fixed, Y may be changed minimally to enforce orthogonality. X will be computed. Inputs need not be normalized. */
	static Matrix MakeFromZY(Vector const& ZAxis, Vector const& YAxis);
};
