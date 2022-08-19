// Copyright LiuD, Inc. All Rights Reserved.

#pragma once
#include "MathUtility.h"
#include "Vector.h"
#include "Vector4.h"
#include "Plane.h"
#include "Rotator.h"
#include "Axis.h"

#include "MathSSE.h"
//#include "../HAL/Memory.h"

struct Matrix
{
public:
	union
	{
		float M[4][4];
	};

	//Identity matrix
	static const Matrix Identity;

	// Constructors.
	FORCEINLINE Matrix() {}

	/**
	 * Constructor.
	 *
	 * @param EForceInit Force Init Enum.
	 */
	//explicit FORCEINLINE Matrix(EForceInit) { Memory::Memzero(this, sizeof(*this)); }

	/**
	 * Constructor.
	 *
	 * @param InX X plane
	 * @param InY Y plane
	 * @param InZ Z plane
	 * @param InW W plane
	 */
	FORCEINLINE Matrix(const Plane& InX, const Plane& InY, const Plane& InZ, const Plane& InW);

	/**
	 * Constructor.
	 *
	 * @param InX X vector
	 * @param InY Y vector
	 * @param InZ Z vector
	 * @param InW W vector
	 */
	FORCEINLINE Matrix(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW);


	// Set this to the identity matrix
	inline void SetIdentity();

	/**
	 * Gets the result of multiplying a Matrix to this.
	 *
	 * @param Other The matrix to multiply this by.
	 * @return The result of multiplication.
	 */
	FORCEINLINE Matrix operator* (const Matrix& Other) const;

	/**
	 * Multiply this by a matrix.
	 *
	 * @param Other the matrix to multiply by this.
	 * @return reference to this after multiply.
	 */
	FORCEINLINE void operator*=(const Matrix& Other);

	/**
	 * Gets the result of adding a matrix to this.
	 *
	 * @param Other The Matrix to add.
	 * @return The result of addition.
	 */
	FORCEINLINE Matrix operator+ (const Matrix& Other) const;

	/**
	 * Adds to this matrix.
	 *
	 * @param Other The matrix to add to this.
	 * @return Reference to this after addition.
	 */
	FORCEINLINE void operator+=(const Matrix& Other);

	/**
	  * This isn't applying SCALE, just multiplying the value to all members - i.e. weighting
	  */
	FORCEINLINE Matrix operator* (float Other) const;

	/**
	 * Multiply this matrix by a weighting factor.
	 *
	 * @param other The weight.
	 * @return a reference to this after weighting.
	 */
	FORCEINLINE void operator*=(float Other);

	/**
	 * Checks whether two matrix are identical.
	 *
	 * @param Other The other matrix.
	 * @return true if two matrix are identical, otherwise false.
	 */
	inline bool operator==(const Matrix& Other) const;

	/**
	 * Checks whether another Matrix is equal to this, within specified tolerance.
	 *
	 * @param Other The other Matrix.
	 * @param Tolerance Error Tolerance.
	 * @return true if two Matrix are equal, within specified tolerance, otherwise false.
	 */
	inline bool Equals(const Matrix& Other, float Tolerance = KINDA_SMALL_NUMBER) const;

	/**
	 * Checks whether another Matrix is not equal to this, within specified tolerance.
	 *
	 * @param Other The other Matrix.
	 * @return true if two Matrix are not equal, within specified tolerance, otherwise false.
	 */
	inline bool operator!=(const Matrix& Other) const;

	// Homogeneous transform.
	FORCEINLINE Vector4 TransformVector4(const Vector4& V) const;

	/** Transform a location - will take into account translation part of the Matrix. */
	FORCEINLINE Vector4 TransformPosition(const Vector& V) const;

	/** Inverts the matrix and then transforms V - correctly handles scaling in this matrix. */
	FORCEINLINE Vector InverseTransformPosition(const Vector& V) const;

	/**
	 *	Transform a direction vector - will not take into account translation part of the Matrix.
	 *	If you want to transform a surface normal (or plane) and correctly account for non-uniform scaling you should use TransformByUsingAdjointT.
	 */
	FORCEINLINE Vector4 TransformVector(const Vector& V) const;

	/**
	 *	Transform a direction vector by the inverse of this matrix - will not take into account translation part.
	 *	If you want to transform a surface normal (or plane) and correctly account for non-uniform scaling you should use TransformByUsingAdjointT with adjoint of matrix inverse.
	 */
	FORCEINLINE Vector InverseTransformVector(const Vector& V) const;

	// Transpose.

	FORCEINLINE Matrix GetTransposed() const;

	// @return determinant of this matrix.

	inline float Determinant() const;

	/** @return the determinant of rotation 3x3 matrix */
	inline float RotDeterminant() const;

	/** Fast path, doesn't check for nil matrices in final release builds */
	inline Matrix InverseFast() const;

	/** Fast path, and handles nil matrices. */
	inline Matrix Inverse() const;

	inline Matrix TransposeAdjoint() const;

	// NOTE: There is some compiler optimization issues with WIN64 that cause FORCEINLINE to cause a crash
	// Remove any scaling from this matrix (ie magnitude of each row is 1) with error Tolerance
	inline void RemoveScaling(float Tolerance = SMALL_NUMBER);

	// Returns matrix after RemoveScaling with error Tolerance
	inline Matrix GetMatrixWithoutScale(float Tolerance = SMALL_NUMBER) const;

	/** Remove any scaling from this matrix (ie magnitude of each row is 1) and return the 3D scale vector that was initially present with error Tolerance */
	inline Vector ExtractScaling(float Tolerance = SMALL_NUMBER);

	/** return a 3D scale vector calculated from this matrix (where each component is the magnitude of a row vector) with error Tolerance. */
	inline Vector GetScaleVector(float Tolerance = SMALL_NUMBER) const;

	// Remove any translation from this matrix
	inline Matrix RemoveTranslation() const;

	/** Returns a matrix with an additional translation concatenated. */
	inline Matrix ConcatTranslation(const Vector& Translation) const;

	/** Returns true if any element of this matrix is NaN */
	inline bool ContainsNaN() const;

	/** Scale the translation part of the matrix by the supplied vector. */
	inline void ScaleTranslation(const Vector& Scale3D);

	/** @return the maximum magnitude of any row of the matrix. */
	inline float GetMaximumAxisScale() const;

	/** Apply Scale to this matrix **/
	inline Matrix ApplyScale(float Scale) const;

	// @return the origin of the co-ordinate system
	inline Vector GetOrigin() const;

	/**
	 * get axis of this matrix scaled by the scale of the matrix
	 *
	 * @param i index into the axis of the matrix
	 * @ return vector of the axis
	 */
	inline Vector GetScaledAxis(EAxis::Type Axis) const;

	/**
	 * get axes of this matrix scaled by the scale of the matrix
	 *
	 * @param X axes returned to this param
	 * @param Y axes returned to this param
	 * @param Z axes returned to this param
	 */
	inline void GetScaledAxes(Vector& X, Vector& Y, Vector& Z) const;

	/**
	 * get unit length axis of this matrix
	 *
	 * @param i index into the axis of the matrix
	 * @return vector of the axis
	 */
	inline Vector GetUnitAxis(EAxis::Type Axis) const;

	/**
	 * get unit length axes of this matrix
	 *
	 * @param X axes returned to this param
	 * @param Y axes returned to this param
	 * @param Z axes returned to this param
	 */
	inline void GetUnitAxes(Vector& X, Vector& Y, Vector& Z) const;

	/**
	 * set an axis of this matrix
	 *
	 * @param i index into the axis of the matrix
	 * @param Axis vector of the axis
	 */
	inline void SetAxis(int32 i, const Vector& Axis);

	// Set the origin of the coordinate system to the given vector
	inline void SetOrigin(const Vector& NewOrigin);

	/**
	 * Update the axes of the matrix if any value is NULL do not update that axis
	 *
	 * @param Axis0 set matrix row 0
	 * @param Axis1 set matrix row 1
	 * @param Axis2 set matrix row 2
	 * @param Origin set matrix row 3
	 */
	inline void SetAxes(Vector* Axis0 = NULL, Vector* Axis1 = NULL, Vector* Axis2 = NULL, Vector* Origin = NULL);


	/**
	 * get a column of this matrix
	 *
	 * @param i index into the column of the matrix
	 * @return vector of the column
	 */
	inline Vector GetColumn(int32 i) const;

	/**
	 * Set a column of this matrix
	 *
	 * @param i index of the matrix column
	 * @param Value new value of the column
	 */
	inline void SetColumn(int32 i, Vector Value);

	/** @return rotator representation of this matrix */
	Rotator GetRotator() const;

	/**
	 * Transform a rotation matrix into a quaternion.
	 *
	 * @warning rotation part will need to be unit length for this to be right!
	 */
	Quat ToQuat() const;

	// Frustum plane extraction.
	/** @param OutPlane the near plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumNearPlane(Plane& OutPlane) const;

	/** @param OutPlane the far plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumFarPlane(Plane& OutPlane) const;

	/** @param OutPlane the left plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumLeftPlane(Plane& OutPlane) const;

	/** @param OutPlane the right plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumRightPlane(Plane& OutPlane) const;

	/** @param OutPlane the top plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumTopPlane(Plane& OutPlane) const;

	/** @param OutPlane the bottom plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumBottomPlane(Plane& OutPlane) const;

	/**
	 * Utility for mirroring this transform across a certain plane, and flipping one of the axis as well.
	 */
	inline void Mirror(EAxis::Type MirrorAxis, EAxis::Type FlipAxis);

};

// Matrix Inline

FORCEINLINE Matrix::Matrix(const Plane& InX, const Plane& InY, const Plane& InZ, const Plane& InW)
{
	M[0][0] = InX.X; M[0][1] = InX.Y;  M[0][2] = InX.Z;  M[0][3] = InX.W;
	M[1][0] = InY.X; M[1][1] = InY.Y;  M[1][2] = InY.Z;  M[1][3] = InY.W;
	M[2][0] = InZ.X; M[2][1] = InZ.Y;  M[2][2] = InZ.Z;  M[2][3] = InZ.W;
	M[3][0] = InW.X; M[3][1] = InW.Y;  M[3][2] = InW.Z;  M[3][3] = InW.W;
}

FORCEINLINE Matrix::Matrix(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW)
{
	M[0][0] = InX.X; M[0][1] = InX.Y;  M[0][2] = InX.Z;  M[0][3] = 0.0f;
	M[1][0] = InY.X; M[1][1] = InY.Y;  M[1][2] = InY.Z;  M[1][3] = 0.0f;
	M[2][0] = InZ.X; M[2][1] = InZ.Y;  M[2][2] = InZ.Z;  M[2][3] = 0.0f;
	M[3][0] = InW.X; M[3][1] = InW.Y;  M[3][2] = InW.Z;  M[3][3] = 1.0f;
}

inline void Matrix::SetIdentity()
{
	M[0][0] = 1; M[0][1] = 0;  M[0][2] = 0;  M[0][3] = 0;
	M[1][0] = 0; M[1][1] = 1;  M[1][2] = 0;  M[1][3] = 0;
	M[2][0] = 0; M[2][1] = 0;  M[2][2] = 1;  M[2][3] = 0;
	M[3][0] = 0; M[3][1] = 0;  M[3][2] = 0;  M[3][3] = 1;
}

FORCEINLINE Matrix Matrix::operator*(const Matrix& Other) const
{
	Matrix Result;
	VectorMatrixMultiply(&Result, this, &Other);
	return Result;
}

FORCEINLINE void Matrix::operator*=(const Matrix& Other)
{
	VectorMatrixMultiply(this, this, &Other);
}

FORCEINLINE Matrix	Matrix::operator+(const Matrix& Other) const
{
	Matrix ResultMat;

	for (int32 X = 0; X < 4; X++)
	{
		for (int32 Y = 0; Y < 4; Y++)
		{
			ResultMat.M[X][Y] = M[X][Y] + Other.M[X][Y];
		}
	}

	return ResultMat;
}

FORCEINLINE void Matrix::operator+=(const Matrix& Other)
{
	*this = *this + Other;
}

FORCEINLINE Matrix	Matrix::operator*(float Other) const
{
	Matrix ResultMat;

	for (int32 X = 0; X < 4; X++)
	{
		for (int32 Y = 0; Y < 4; Y++)
		{
			ResultMat.M[X][Y] = M[X][Y] * Other;
		}
	}

	return ResultMat;
}

FORCEINLINE void Matrix::operator*=(float Other)
{
	*this = *this * Other;
}


// Comparison operators.

inline bool Matrix::operator==(const Matrix& Other) const
{
	for (int32 X = 0; X < 4; X++)
	{
		for (int32 Y = 0; Y < 4; Y++)
		{
			if (M[X][Y] != Other.M[X][Y])
			{
				return false;
			}
		}
	}

	return true;
}

// Error-tolerant comparison.
inline bool Matrix::Equals(const Matrix& Other, float Tolerance/*=KINDA_SMALL_NUMBER*/) const
{
	for (int32 X = 0; X < 4; X++)
	{
		for (int32 Y = 0; Y < 4; Y++)
		{
			if (Math::Abs(M[X][Y] - Other.M[X][Y]) > Tolerance)
			{
				return false;
			}
		}
	}

	return true;
}

inline bool Matrix::operator!=(const Matrix& Other) const
{
	return !(*this == Other);
}

// Homogeneous transform.

FORCEINLINE Vector4 Matrix::TransformVector4(const Vector4& P) const
{
	Vector4 Result;
	VectorRegister VecP = VectorLoadAligned(&P);
	VectorRegister VecR = VectorTransformVector(VecP, this);
	VectorStoreAligned(VecR, &Result);
	return Result;
}

// Transform position

/** Transform a location - will take into account translation part of the Matrix. */
FORCEINLINE Vector4 Matrix::TransformPosition(const Vector& V) const
{
	return TransformVector4(Vector4(V.X, V.Y, V.Z, 1.0f));
}

/** Inverts the matrix and then transforms V - correctly handles scaling in this matrix. */
FORCEINLINE Vector Matrix::InverseTransformPosition(const Vector& V) const
{
	Matrix InvSelf = this->InverseFast();
	return InvSelf.TransformPosition(V);
}

// Transform vector

/**
 *	Transform a direction vector - will not take into account translation part of the Matrix.
 *	If you want to transform a surface normal (or plane) and correctly account for non-uniform scaling you should use TransformByUsingAdjointT.
 */
FORCEINLINE Vector4 Matrix::TransformVector(const Vector& V) const
{
	return TransformVector4(Vector4(V.X, V.Y, V.Z, 0.0f));
}

/** Faster version of InverseTransformVector that assumes no scaling. WARNING: Will NOT work correctly if there is scaling in the matrix. */
FORCEINLINE Vector Matrix::InverseTransformVector(const Vector& V) const
{
	Matrix InvSelf = this->InverseFast();
	return InvSelf.TransformVector(V);
}

// Transpose.

FORCEINLINE Matrix Matrix::GetTransposed() const
{
	Matrix	Result;

	Result.M[0][0] = M[0][0];
	Result.M[0][1] = M[1][0];
	Result.M[0][2] = M[2][0];
	Result.M[0][3] = M[3][0];

	Result.M[1][0] = M[0][1];
	Result.M[1][1] = M[1][1];
	Result.M[1][2] = M[2][1];
	Result.M[1][3] = M[3][1];

	Result.M[2][0] = M[0][2];
	Result.M[2][1] = M[1][2];
	Result.M[2][2] = M[2][2];
	Result.M[2][3] = M[3][2];

	Result.M[3][0] = M[0][3];
	Result.M[3][1] = M[1][3];
	Result.M[3][2] = M[2][3];
	Result.M[3][3] = M[3][3];

	return Result;
}

// Determinant.

inline float Matrix::Determinant() const
{
	return	M[0][0] * (
		M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
		M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
		M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
		) -
		M[1][0] * (
			M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
			M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
			M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
			) +
		M[2][0] * (
			M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
			M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
			M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			) -
		M[3][0] * (
			M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
			M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
			M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			);
}

/** Calculate determinant of rotation 3x3 matrix */
inline float Matrix::RotDeterminant() const
{
	return
		M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
		M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
		M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1]);
}

// Inverse.
/** Fast path, doesn't check for nil matrices in final release builds */
inline Matrix Matrix::InverseFast() const
{
	// If we're in non final release, then make sure we're not creating NaNs
//#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
//	// Check for zero scale matrix to invert
//	if (GetScaledAxis(EAxis::X).IsNearlyZero(SMALL_NUMBER) &&
//		GetScaledAxis(EAxis::Y).IsNearlyZero(SMALL_NUMBER) &&
//		GetScaledAxis(EAxis::Z).IsNearlyZero(SMALL_NUMBER))
//	{
//		//ErrorEnsure(TEXT("Matrix::InverseFast(), trying to invert a NIL matrix, this results in NaNs! Use Inverse() instead."));
//	}
//	else
//	{
//		const float	Det = Determinant();
//
//		if (Det == 0.0f || !Math::IsFinite(Det))
//		{
//			//ErrorEnsure(TEXT("Matrix::InverseFast(), trying to invert a non-invertible matrix, this results in NaNs! Use Inverse() instead."));
//		}
//	}
//#endif
	Matrix Result;
	VectorMatrixInverse(&Result, this);
	return Result;
}

// Inverse.
inline Matrix Matrix::Inverse() const
{
	Matrix Result;

	// Check for zero scale matrix to invert
	if (GetScaledAxis(EAxis::X).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(EAxis::Y).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(EAxis::Z).IsNearlyZero(SMALL_NUMBER))
	{
		// just set to zero - avoids unsafe inverse of zero and duplicates what QNANs were resulting in before (scaling away all children)
		Result = Matrix::Identity;
	}
	else
	{
		const float	Det = Determinant();

		if (Det == 0.0f)
		{
			Result = Matrix::Identity;
		}
		else
		{
			VectorMatrixInverse(&Result, this);
		}
	}

	return Result;
}

inline Matrix Matrix::TransposeAdjoint() const
{
	Matrix TA;

	TA.M[0][0] = this->M[1][1] * this->M[2][2] - this->M[1][2] * this->M[2][1];
	TA.M[0][1] = this->M[1][2] * this->M[2][0] - this->M[1][0] * this->M[2][2];
	TA.M[0][2] = this->M[1][0] * this->M[2][1] - this->M[1][1] * this->M[2][0];
	TA.M[0][3] = 0.f;

	TA.M[1][0] = this->M[2][1] * this->M[0][2] - this->M[2][2] * this->M[0][1];
	TA.M[1][1] = this->M[2][2] * this->M[0][0] - this->M[2][0] * this->M[0][2];
	TA.M[1][2] = this->M[2][0] * this->M[0][1] - this->M[2][1] * this->M[0][0];
	TA.M[1][3] = 0.f;

	TA.M[2][0] = this->M[0][1] * this->M[1][2] - this->M[0][2] * this->M[1][1];
	TA.M[2][1] = this->M[0][2] * this->M[1][0] - this->M[0][0] * this->M[1][2];
	TA.M[2][2] = this->M[0][0] * this->M[1][1] - this->M[0][1] * this->M[1][0];
	TA.M[2][3] = 0.f;

	TA.M[3][0] = 0.f;
	TA.M[3][1] = 0.f;
	TA.M[3][2] = 0.f;
	TA.M[3][3] = 1.f;

	return TA;
}

// NOTE: There is some compiler optimization issues with WIN64 that cause FORCEINLINE to cause a crash
// Remove any scaling from this matrix (ie magnitude of each row is 1)
inline void Matrix::RemoveScaling(float Tolerance/*=SMALL_NUMBER*/)
{
	// For each row, find magnitude, and if its non-zero re-scale so its unit length.
	const float SquareSum0 = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
	const float SquareSum1 = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
	const float SquareSum2 = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);
	const float Scale0 = Math::FloatSelect(SquareSum0 - Tolerance, Math::InvSqrt(SquareSum0), 1.0f);
	const float Scale1 = Math::FloatSelect(SquareSum1 - Tolerance, Math::InvSqrt(SquareSum1), 1.0f);
	const float Scale2 = Math::FloatSelect(SquareSum2 - Tolerance, Math::InvSqrt(SquareSum2), 1.0f);
	M[0][0] *= Scale0;
	M[0][1] *= Scale0;
	M[0][2] *= Scale0;
	M[1][0] *= Scale1;
	M[1][1] *= Scale1;
	M[1][2] *= Scale1;
	M[2][0] *= Scale2;
	M[2][1] *= Scale2;
	M[2][2] *= Scale2;
}

// Returns matrix without scale information
inline Matrix Matrix::GetMatrixWithoutScale(float Tolerance/*=SMALL_NUMBER*/) const
{
	Matrix Result = *this;
	Result.RemoveScaling(Tolerance);
	return Result;
}

/** Remove any scaling from this matrix (ie magnitude of each row is 1) and return the 3D scale vector that was initially present. */
inline Vector Matrix::ExtractScaling(float Tolerance/*=SMALL_NUMBER*/)
{
	Vector Scale3D(0, 0, 0);

	// For each row, find magnitude, and if its non-zero re-scale so its unit length.
	const float SquareSum0 = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
	const float SquareSum1 = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
	const float SquareSum2 = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);

	if (SquareSum0 > Tolerance)
	{
		float Scale0 = Math::Sqrt(SquareSum0);
		Scale3D[0] = Scale0;
		float InvScale0 = 1.f / Scale0;
		M[0][0] *= InvScale0;
		M[0][1] *= InvScale0;
		M[0][2] *= InvScale0;
	}
	else
	{
		Scale3D[0] = 0;
	}

	if (SquareSum1 > Tolerance)
	{
		float Scale1 = Math::Sqrt(SquareSum1);
		Scale3D[1] = Scale1;
		float InvScale1 = 1.f / Scale1;
		M[1][0] *= InvScale1;
		M[1][1] *= InvScale1;
		M[1][2] *= InvScale1;
	}
	else
	{
		Scale3D[1] = 0;
	}

	if (SquareSum2 > Tolerance)
	{
		float Scale2 = Math::Sqrt(SquareSum2);
		Scale3D[2] = Scale2;
		float InvScale2 = 1.f / Scale2;
		M[2][0] *= InvScale2;
		M[2][1] *= InvScale2;
		M[2][2] *= InvScale2;
	}
	else
	{
		Scale3D[2] = 0;
	}

	return Scale3D;
}

/** return a 3D scale vector calculated from this matrix (where each component is the magnitude of a row vector). */
inline Vector Matrix::GetScaleVector(float Tolerance/*=SMALL_NUMBER*/) const
{
	Vector Scale3D(1, 1, 1);

	// For each row, find magnitude, and if its non-zero re-scale so its unit length.
	for (int32 i = 0; i < 3; i++)
	{
		const float SquareSum = (M[i][0] * M[i][0]) + (M[i][1] * M[i][1]) + (M[i][2] * M[i][2]);
		if (SquareSum > Tolerance)
		{
			Scale3D[i] = Math::Sqrt(SquareSum);
		}
		else
		{
			Scale3D[i] = 0.f;
		}
	}

	return Scale3D;
}

// Remove any translation from this matrix
inline Matrix Matrix::RemoveTranslation() const
{
	Matrix Result = *this;
	Result.M[3][0] = 0.0f;
	Result.M[3][1] = 0.0f;
	Result.M[3][2] = 0.0f;
	return Result;
}

FORCEINLINE Matrix Matrix::ConcatTranslation(const Vector& Translation) const
{
	Matrix Result;

	float* RESTRICT Dest = &Result.M[0][0];
	const float* RESTRICT Src = &M[0][0];
	const float* RESTRICT Trans = &Translation.X;

	Dest[0] = Src[0];
	Dest[1] = Src[1];
	Dest[2] = Src[2];
	Dest[3] = Src[3];
	Dest[4] = Src[4];
	Dest[5] = Src[5];
	Dest[6] = Src[6];
	Dest[7] = Src[7];
	Dest[8] = Src[8];
	Dest[9] = Src[9];
	Dest[10] = Src[10];
	Dest[11] = Src[11];
	Dest[12] = Src[12] + Trans[0];
	Dest[13] = Src[13] + Trans[1];
	Dest[14] = Src[14] + Trans[2];
	Dest[15] = Src[15];

	return Result;
}

/** Returns true if any element of this matrix is not finite */
inline bool Matrix::ContainsNaN() const
{
	for (int32 i = 0; i < 4; i++)
	{
		for (int32 j = 0; j < 4; j++)
		{
			if (!Math::IsFinite(M[i][j]))
			{
				return true;
			}
		}
	}

	return false;
}

inline void Matrix::ScaleTranslation(const Vector& InScale3D)
{
	M[3][0] *= InScale3D.X;
	M[3][1] *= InScale3D.Y;
	M[3][2] *= InScale3D.Z;
}

/** @return the maximum magnitude of any row of the matrix. */
inline float Matrix::GetMaximumAxisScale() const
{
	const float MaxRowScaleSquared = Math::Max(
		GetScaledAxis(EAxis::X).SizeSquared(),
		Math::Max(
			GetScaledAxis(EAxis::Y).SizeSquared(),
			GetScaledAxis(EAxis::Z).SizeSquared()
		)
	);
	return Math::Sqrt(MaxRowScaleSquared);
}

/**
 * Apply Scale to this matrix
 */
inline Matrix Matrix::ApplyScale(float Scale) const
{
	Matrix ScaleMatrix(
		Plane(Scale, 0.0f, 0.0f, 0.0f),
		Plane(0.0f, Scale, 0.0f, 0.0f),
		Plane(0.0f, 0.0f, Scale, 0.0f),
		Plane(0.0f, 0.0f, 0.0f, 1.0f)
	);
	return ScaleMatrix * (*this);
}

// GetOrigin

inline Vector Matrix::GetOrigin() const
{
	return Vector(M[3][0], M[3][1], M[3][2]);
}

inline Vector Matrix::GetScaledAxis(EAxis::Type InAxis) const
{
	switch (InAxis)
	{
	case EAxis::X:
		return Vector(M[0][0], M[0][1], M[0][2]);

	case EAxis::Y:
		return Vector(M[1][0], M[1][1], M[1][2]);

	case EAxis::Z:
		return Vector(M[2][0], M[2][1], M[2][2]);

	default:
		//ensure(0);
		return Vector::ZeroVector;
	}
}

inline void Matrix::GetScaledAxes(Vector& X, Vector& Y, Vector& Z) const
{
	X.X = M[0][0]; X.Y = M[0][1]; X.Z = M[0][2];
	Y.X = M[1][0]; Y.Y = M[1][1]; Y.Z = M[1][2];
	Z.X = M[2][0]; Z.Y = M[2][1]; Z.Z = M[2][2];
}

inline Vector Matrix::GetUnitAxis(EAxis::Type InAxis) const
{
	return GetScaledAxis(InAxis).GetSafeNormal();
}

inline void Matrix::GetUnitAxes(Vector& X, Vector& Y, Vector& Z) const
{
	GetScaledAxes(X, Y, Z);
	X.Normalize();
	Y.Normalize();
	Z.Normalize();
}

inline void Matrix::SetAxis(int32 i, const Vector& Axis)
{
	//checkSlow(i >= 0 && i <= 2);
	M[i][0] = Axis.X;
	M[i][1] = Axis.Y;
	M[i][2] = Axis.Z;
}

inline void Matrix::SetOrigin(const Vector& NewOrigin)
{
	M[3][0] = NewOrigin.X;
	M[3][1] = NewOrigin.Y;
	M[3][2] = NewOrigin.Z;
}

inline void Matrix::SetAxes(Vector* Axis0, Vector* Axis1, Vector* Axis2, Vector* Origin)
{
	if (Axis0 != NULL)
	{
		M[0][0] = Axis0->X;
		M[0][1] = Axis0->Y;
		M[0][2] = Axis0->Z;
	}
	if (Axis1 != NULL)
	{
		M[1][0] = Axis1->X;
		M[1][1] = Axis1->Y;
		M[1][2] = Axis1->Z;
	}
	if (Axis2 != NULL)
	{
		M[2][0] = Axis2->X;
		M[2][1] = Axis2->Y;
		M[2][2] = Axis2->Z;
	}
	if (Origin != NULL)
	{
		M[3][0] = Origin->X;
		M[3][1] = Origin->Y;
		M[3][2] = Origin->Z;
	}
}

inline Vector Matrix::GetColumn(int32 i) const
{
	//checkSlow(i >= 0 && i <= 3);
	return Vector(M[0][i], M[1][i], M[2][i]);
}

inline void Matrix::SetColumn(int32 i, Vector Value)
{
	//checkSlow(i >= 0 && i <= 3);
	M[0][i] = Value.X;
	M[1][i] = Value.Y;
	M[2][i] = Value.Z;
}


FORCEINLINE bool MakeFrustumPlane(float A, float B, float C, float D, Plane& OutPlane)
{
	const float	LengthSquared = A * A + B * B + C * C;
	if (LengthSquared > DELTA * DELTA)
	{
		const float	InvLength = Math::InvSqrt(LengthSquared);
		OutPlane = Plane(-A * InvLength, -B * InvLength, -C * InvLength, D * InvLength);
		return 1;
	}
	else
		return 0;
}


// Frustum plane extraction.
FORCEINLINE bool Matrix::GetFrustumNearPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][2],
		M[1][2],
		M[2][2],
		M[3][2],
		OutPlane
	);
}

FORCEINLINE bool Matrix::GetFrustumFarPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] - M[0][2],
		M[1][3] - M[1][2],
		M[2][3] - M[2][2],
		M[3][3] - M[3][2],
		OutPlane
	);
}

FORCEINLINE bool Matrix::GetFrustumLeftPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] + M[0][0],
		M[1][3] + M[1][0],
		M[2][3] + M[2][0],
		M[3][3] + M[3][0],
		OutPlane
	);
}

FORCEINLINE bool Matrix::GetFrustumRightPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] - M[0][0],
		M[1][3] - M[1][0],
		M[2][3] - M[2][0],
		M[3][3] - M[3][0],
		OutPlane
	);
}

FORCEINLINE bool Matrix::GetFrustumTopPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] - M[0][1],
		M[1][3] - M[1][1],
		M[2][3] - M[2][1],
		M[3][3] - M[3][1],
		OutPlane
	);
}

FORCEINLINE bool Matrix::GetFrustumBottomPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] + M[0][1],
		M[1][3] + M[1][1],
		M[2][3] + M[2][1],
		M[3][3] + M[3][1],
		OutPlane
	);
}

/**
 * Utility for mirroring this transform across a certain plane,
 * and flipping one of the axis as well.
 */
inline void Matrix::Mirror(EAxis::Type MirrorAxis, EAxis::Type FlipAxis)
{
	if (MirrorAxis == EAxis::X)
	{
		M[0][0] *= -1.f;
		M[1][0] *= -1.f;
		M[2][0] *= -1.f;

		M[3][0] *= -1.f;
	}
	else if (MirrorAxis == EAxis::Y)
	{
		M[0][1] *= -1.f;
		M[1][1] *= -1.f;
		M[2][1] *= -1.f;

		M[3][1] *= -1.f;
	}
	else if (MirrorAxis == EAxis::Z)
	{
		M[0][2] *= -1.f;
		M[1][2] *= -1.f;
		M[2][2] *= -1.f;

		M[3][2] *= -1.f;
	}

	if (FlipAxis == EAxis::X)
	{
		M[0][0] *= -1.f;
		M[0][1] *= -1.f;
		M[0][2] *= -1.f;
	}
	else if (FlipAxis == EAxis::Y)
	{
		M[1][0] *= -1.f;
		M[1][1] *= -1.f;
		M[1][2] *= -1.f;
	}
	else if (FlipAxis == EAxis::Z)
	{
		M[2][0] *= -1.f;
		M[2][1] *= -1.f;
		M[2][2] *= -1.f;
	}
}


// Serializer.
//inline FArchive& operator<<(FArchive& Ar, Matrix& M)
//{
//	Ar << M.M[0][0] << M.M[0][1] << M.M[0][2] << M.M[0][3];
//	Ar << M.M[1][0] << M.M[1][1] << M.M[1][2] << M.M[1][3];
//	Ar << M.M[2][0] << M.M[2][1] << M.M[2][2] << M.M[2][3];
//	Ar << M.M[3][0] << M.M[3][1] << M.M[3][2] << M.M[3][3];
//	return Ar;
//}


/**
 * FPlane inline functions.
 */

inline Plane Plane::TransformBy(const Matrix& M) const
{
	const Matrix tmpTA = M.TransposeAdjoint();
	const float DetM = M.Determinant();
	return this->TransformByUsingAdjointT(M, DetM, tmpTA);
}

inline Plane Plane::TransformByUsingAdjointT(const Matrix& M, float DetM, const Matrix& TA) const
{
	Vector newNorm = TA.TransformVector(*this).GetSafeNormal();

	if (DetM < 0.f)
	{
		newNorm *= -1.0f;
	}

	return Plane(M.TransformPosition(*this * W), newNorm);
}


struct BasisVectorMatrix : Matrix
{
	// Create Basis matrix from 3 axis vectors and the origin
	BasisVectorMatrix(const Vector& XAxis, const Vector& YAxis, const Vector& ZAxis, const Vector& Origin);
};



struct LookFromMatrix : Matrix
{
	/**
	 * Creates a view matrix given an eye position, a direction to look in, and an up vector.
	 * Direction or up vectors need not be normalized.
	 * This does the same thing as FLookAtMatrix, except without completely destroying precision when position is large,
	 * Always use this instead of e.g., FLookAtMatrix(Pos, Pos + Dir,...);
	 */
	LookFromMatrix(const Vector& EyePosition, const Vector& LookDirection, const Vector& UpVector);
};


struct LookAtMatrix : LookFromMatrix
{
	/**
	 * Creates a view matrix given an eye position, a position to look at, and an up vector.
	 * Equivalent of FLookFromMatrix(EyePosition, LookAtPosition - EyePosition, UpVector)
	 * The up vector need not be normalized.
	 * This does the same thing as D3DXMatrixLookAtLH.
	 */
	LookAtMatrix(const Vector& EyePosition, const Vector& LookAtPosition, const Vector& UpVector);
};

FORCEINLINE BasisVectorMatrix::BasisVectorMatrix(const Vector& XAxis, const Vector& YAxis, const Vector& ZAxis, const Vector& Origin)
{
	for (uint32 RowIndex = 0; RowIndex < 3; RowIndex++)
	{
		M[RowIndex][0] = (&XAxis.X)[RowIndex];
		M[RowIndex][1] = (&YAxis.X)[RowIndex];
		M[RowIndex][2] = (&ZAxis.X)[RowIndex];
		M[RowIndex][3] = 0.0f;
	}
	M[3][0] = Origin | XAxis;
	M[3][1] = Origin | YAxis;
	M[3][2] = Origin | ZAxis;
	M[3][3] = 1.0f;
}


FORCEINLINE LookFromMatrix::LookFromMatrix(const Vector& EyePosition, const Vector& LookDirection, const Vector& UpVector)
{
	const Vector ZAxis = LookDirection.GetSafeNormal();
	const Vector XAxis = (UpVector ^ ZAxis).GetSafeNormal();
	const Vector YAxis = ZAxis ^ XAxis;

	for (uint32 RowIndex = 0; RowIndex < 3; RowIndex++)
	{
		M[RowIndex][0] = (&XAxis.X)[RowIndex];
		M[RowIndex][1] = (&YAxis.X)[RowIndex];
		M[RowIndex][2] = (&ZAxis.X)[RowIndex];
		M[RowIndex][3] = 0.0f;
	}
	M[3][0] = -EyePosition | XAxis;
	M[3][1] = -EyePosition | YAxis;
	M[3][2] = -EyePosition | ZAxis;
	M[3][3] = 1.0f;
}



FORCEINLINE LookAtMatrix::LookAtMatrix(const Vector& EyePosition, const Vector& LookAtPosition, const Vector& UpVector) :
	LookFromMatrix(EyePosition, LookAtPosition - EyePosition, UpVector)
{}
