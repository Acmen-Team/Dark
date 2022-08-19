#include "Math.h"

/*-----------------------------------------------------------------------------
	Globals
 -----------------------------------------------------------------------------*/

const Vector Vector::ZeroVector(0.0f, 0.0f, 0.0f);
const Vector Vector::OneVector(1.0f, 1.0f, 1.0f);
const Vector Vector::UpVector(0.0f, 0.0f, 1.0f);
const Vector Vector::DownVector(0.0f, 0.0f, -1.0f);
const Vector Vector::ForwardVector(1.0f, 0.0f, 0.0f);
const Vector Vector::BackwardVector(-1.0f, 0.0f, 0.0f);
const Vector Vector::RightVector(0.0f, 1.0f, 0.0f);
const Vector Vector::LeftVector(0.0f, -1.0f, 0.0f);
const Vector Vector::XAxisVector(1.0f, 0.0f, 0.0f);
const Vector Vector::YAxisVector(0.0f, 1.0f, 0.0f);
const Vector Vector::ZAxisVector(0.0f, 0.0f, 1.0f);

const Vector2D Vector2D::ZeroVector(0.0f, 0.0f);
const Vector2D Vector2D::UnitVector(1.0f, 1.0f);
const Vector2D Vector2D::Unit45Deg(_INV_SQRT_2, _INV_SQRT_2);

const Rotator Rotator::ZeroRotator(0.f, 0.f, 0.f);

Rotator Vector::ToOrientationRotator() const
{
	Rotator R;

	// Find yaw.
	R.Yaw = Math::Atan2(Y, X) * (180.f / PI);

	// Find pitch.
	R.Pitch = Math::Atan2(Z, Math::Sqrt(X * X + Y * Y)) * (180.f / PI);

	// Find roll.
	R.Roll = 0;

	//#if ENABLE_NAN_DIAGNOSTIC || (DO_CHECK && !UE_BUILD_SHIPPING)
	//		if (R.ContainsNaN())
	//		{
	//			logOrEnsureNanError(TEXT("Vector::Rotation(): Rotator result %s contains NaN! Input Vector = %s"), *R.ToString(), *this->ToString());
	//			R = FRotator::ZeroRotator;
	//		}
	//#endif

	return R;
}

void Vector::CreateOrthonormalBasis(Vector& XAxis, Vector& YAxis, Vector& ZAxis)
{
	// Project the X and Y axes onto the plane perpendicular to the Z axis.
	XAxis -= (XAxis | ZAxis) / (ZAxis | ZAxis) * ZAxis;
	YAxis -= (YAxis | ZAxis) / (ZAxis | ZAxis) * ZAxis;

	// If the X axis was parallel to the Z axis, choose a vector which is orthogonal to the Y and Z axes.
	if (XAxis.SizeSquared() < DELTA * DELTA)
	{
		XAxis = YAxis ^ ZAxis;
	}

	// If the Y axis was parallel to the Z axis, choose a vector which is orthogonal to the X and Z axes.
	if (YAxis.SizeSquared() < DELTA * DELTA)
	{
		YAxis = XAxis ^ ZAxis;
	}

	// Normalize the basis vectors.
	XAxis.Normalize();
	YAxis.Normalize();
	ZAxis.Normalize();
}

Rotator Vector4::ToOrientationRotator() const
{
	Rotator R;

	// Find yaw.
	R.Yaw = Math::Atan2(Y, X) * (180.f / PI);

	// Find pitch.
	R.Pitch = Math::Atan2(Z, Math::Sqrt(X * X + Y * Y)) * (180.f / PI);

	// Find roll.
	R.Roll = 0;

	return R;
}

Rotator Vector4::Rotation() const
{
	return ToOrientationRotator();
}

Matrix RotationMatrix::Make(Quat const& Rot)
{
	return QuatRotationTranslationMatrix(Rot, Vector::ZeroVector);
}

Rotator Matrix::GetRotator() const
{
	const Vector		XAxis = GetScaledAxis(EAxis::X);
	const Vector		YAxis = GetScaledAxis(EAxis::Y);
	const Vector		ZAxis = GetScaledAxis(EAxis::Z);

	Rotator	rotator = Rotator(
		Math::Atan2(XAxis.Z, Math::Sqrt(Math::Square(XAxis.X) + Math::Square(XAxis.Y))) * 180.f / PI,
		Math::Atan2(XAxis.Y, XAxis.X) * 180.f / PI,
		0
	);

	const Vector		SYAxis = RotationMatrix(rotator).GetScaledAxis(EAxis::Y);
	rotator.Roll = Math::Atan2(ZAxis | SYAxis, YAxis | SYAxis) * 180.f / PI;

	rotator.DiagnosticCheckNaN();
	return rotator;
}

const Quat Quat::Identity(0, 0, 0, 1);

Quat Matrix::ToQuat() const
{
	Quat Result(*this);
	return Result;
}


//Rotator Rotator::GetInverse() const
//{
//	return Quaternion().Inverse().Rotator();
//}

Vector Rotator::ToVector() const
{
	// Remove winding and clamp to [-360, 360]
	const float PitchNoWinding = Math::Fmod(Pitch, 360.0f);
	const float YawNoWinding = Math::Fmod(Yaw, 360.0f);

	float CP, SP, CY, SY;
	Math::SinCos(&SP, &CP, Math::DegreesToRadians(PitchNoWinding));
	Math::SinCos(&SY, &CY, Math::DegreesToRadians(YawNoWinding));
	Vector V = Vector(CP * CY, CP * SY, SP);

	return V;
}

Quat Rotator::Quaternion() const
{
	DiagnosticCheckNaN();

	const float DEG_TO_RAD = PI / (180.f);
	const float RADS_DIVIDED_BY_2 = DEG_TO_RAD / 2.f;
	float SP, SY, SR;
	float CP, CY, CR;

	const float PitchNoWinding = Math::Fmod(Pitch, 360.0f);
	const float YawNoWinding = Math::Fmod(Yaw, 360.0f);
	const float RollNoWinding = Math::Fmod(Roll, 360.0f);

	Math::SinCos(&SP, &CP, PitchNoWinding * RADS_DIVIDED_BY_2);
	Math::SinCos(&SY, &CY, YawNoWinding * RADS_DIVIDED_BY_2);
	Math::SinCos(&SR, &CR, RollNoWinding * RADS_DIVIDED_BY_2);

	Quat RotationQuat;
	RotationQuat.X = CR * SP * SY - SR * CP * CY;
	RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
	RotationQuat.Z = CR * CP * SY - SR * SP * CY;
	RotationQuat.W = CR * CP * CY + SR * SP * SY;

	return RotationQuat;
}

Vector Rotator::Euler() const
{
	return Vector(Roll, Pitch, Yaw);
}

Vector Rotator::RotateVector(const Vector& V) const
{
	return RotationMatrix(*this).TransformVector(V);
}

Vector Rotator::UnrotateVector(const Vector& V) const
{
	return RotationMatrix(*this).GetTransposed().TransformVector(V);
}

void Rotator::GetWindingAndRemainder(Rotator& Winding, Rotator& Remainder) const
{
	//// YAW
	Remainder.Yaw = NormalizeAxis(Yaw);

	Winding.Yaw = Yaw - Remainder.Yaw;

	//// PITCH
	Remainder.Pitch = NormalizeAxis(Pitch);

	Winding.Pitch = Pitch - Remainder.Pitch;

	//// ROLL
	Remainder.Roll = NormalizeAxis(Roll);

	Winding.Roll = Roll - Remainder.Roll;
}

Rotator Rotator::MakeFromEuler(const Vector& Euler)
{
	return Rotator(Euler.Y, Euler.Z, Euler.X);
}

//////////////////////////////////////////////////////////////////////////
// FQuat

Rotator Quat::ToRotator() const
{
	DiagnosticCheckNaN();
	const float SingularityTest = Z * X - W * Y;
	const float YawY = 2.f * (W * Z + X * Y);
	const float YawX = (1.f - 2.f * (Math::Square(Y) + Math::Square(Z)));

	// reference 
	// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

	// this value was found from experience, the above websites recommend different values
	// but that isn't the case for us, so I went through different testing, and finally found the case 
	// where both of world lives happily. 
	const float SINGULARITY_THRESHOLD = 0.4999995f;
	const float RAD_TO_DEG = (180.f) / PI;
	Rotator RotatorFromQuat;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = -90.f;
		RotatorFromQuat.Yaw = Math::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = Rotator::NormalizeAxis(-RotatorFromQuat.Yaw - (2.f * Math::Atan2(X, W) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = 90.f;
		RotatorFromQuat.Yaw = Math::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = Rotator::NormalizeAxis(RotatorFromQuat.Yaw - (2.f * Math::Atan2(X, W) * RAD_TO_DEG));
	}
	else
	{
		RotatorFromQuat.Pitch = Math::FastAsin(2.f * (SingularityTest)) * RAD_TO_DEG;
		RotatorFromQuat.Yaw = Math::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = Math::Atan2(-2.f * (W * X + Y * Z), (1.f - 2.f * (Math::Square(X) + Math::Square(Y)))) * RAD_TO_DEG;
	}

	return RotatorFromQuat;
}

Vector Quat::Euler() const
{
	return Rotator().Euler();
}

Quat Quat::MakeFromEuler(const Vector& Euler)
{
	return Rotator::MakeFromEuler(Euler).Quaternion();
}

void Quat::ToSwingTwist(const Vector& InTwistAxis, Quat& OutSwing, Quat& OutTwist) const
{
	// Vector part projected onto twist axis
	Vector Projection = Vector::DotProduct(InTwistAxis, Vector(X, Y, Z)) * InTwistAxis;

	// Twist quaternion
	OutTwist = Quat(Projection.X, Projection.Y, Projection.Z, W);

	// Singularity close to 180deg
	if (OutTwist.SizeSquared() == 0.0f)
	{
		OutTwist = Quat::Identity;
	}
	else
	{
		OutTwist.Normalize();
	}

	// Set swing
	OutSwing = *this * OutTwist.Inverse();
}

float Quat::GetTwistAngle(const Vector& TwistAxis) const
{
	float XYZ = Vector::DotProduct(TwistAxis, Vector(X, Y, Z));
	return Math::UnwindRadians(2.0f * Math::Atan2(XYZ, W));
}

//
// Based on:
// http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final
// http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
//
FORCEINLINE Quat FindBetween_Helper(const Vector& A, const Vector& B, float NormAB)
{
	float W = NormAB + Vector::DotProduct(A, B);
	Quat Result;

	if (W >= 1e-6f * NormAB)
	{
		//Axis = FVector::CrossProduct(A, B);
		Result = Quat(A.Y * B.Z - A.Z * B.Y,
			A.Z * B.X - A.X * B.Z,
			A.X * B.Y - A.Y * B.X,
			W);
	}
	else
	{
		// A and B point in opposite directions
		W = 0.f;
		Result = Math::Abs(A.X) > Math::Abs(A.Y)
			? Quat(-A.Z, 0.f, A.X, W)
			: Quat(0.f, -A.Z, A.Y, W);
	}

	Result.Normalize();
	return Result;
}

Quat Quat::FindBetweenNormals(const Vector& A, const Vector& B)
{
	const float NormAB = 1.f;
	return FindBetween_Helper(A, B, NormAB);
}

Quat Quat::FindBetweenVectors(const Vector& A, const Vector& B)
{
	const float NormAB = Math::Sqrt(A.SizeSquared() * B.SizeSquared());
	return FindBetween_Helper(A, B, NormAB);
}

Quat Quat::Log() const
{
	Quat Result;
	Result.W = 0.f;

	if (Math::Abs(W) < 1.f)
	{
		const float Angle = Math::Acos(W);
		const float SinAngle = Math::Sin(Angle);

		if (Math::Abs(SinAngle) >= SMALL_NUMBER)
		{
			const float Scale = Angle / SinAngle;
			Result.X = Scale * X;
			Result.Y = Scale * Y;
			Result.Z = Scale * Z;

			return Result;
		}
	}

	Result.X = X;
	Result.Y = Y;
	Result.Z = Z;

	return Result;
}

Quat Quat::Exp() const
{
	const float Angle = Math::Sqrt(X * X + Y * Y + Z * Z);
	const float SinAngle = Math::Sin(Angle);

	Quat Result;
	Result.W = Math::Cos(Angle);

	if (Math::Abs(SinAngle) >= SMALL_NUMBER)
	{
		const float Scale = SinAngle / Angle;
		Result.X = Scale * X;
		Result.Y = Scale * Y;
		Result.Z = Scale * Z;
	}
	else
	{
		Result.X = X;
		Result.Y = Y;
		Result.Z = Z;
	}

	return Result;
}

Quat Quat::Slerp_NotNormalized(const Quat& Quat1, const Quat& Quat2, float Slerp)
{
	// Get cosine of angle between quats.
	const float RawCosom =
		Quat1.X * Quat2.X +
		Quat1.Y * Quat2.Y +
		Quat1.Z * Quat2.Z +
		Quat1.W * Quat2.W;
	// Unaligned quats - compensate, results in taking shorter route.
	const float Cosom = Math::FloatSelect(RawCosom, RawCosom, -RawCosom);

	float Scale0, Scale1;

	if (Cosom < 0.9999f)
	{
		const float Omega = Math::Acos(Cosom);
		const float InvSin = 1.f / Math::Sin(Omega);
		Scale0 = Math::Sin((1.f - Slerp) * Omega) * InvSin;
		Scale1 = Math::Sin(Slerp * Omega) * InvSin;
	}
	else
	{
		// Use linear interpolation.
		Scale0 = 1.0f - Slerp;
		Scale1 = Slerp;
	}

	// In keeping with our flipped Cosom:
	Scale1 = Math::FloatSelect(RawCosom, Scale1, -Scale1);

	Quat Result;

	Result.X = Scale0 * Quat1.X + Scale1 * Quat2.X;
	Result.Y = Scale0 * Quat1.Y + Scale1 * Quat2.Y;
	Result.Z = Scale0 * Quat1.Z + Scale1 * Quat2.Z;
	Result.W = Scale0 * Quat1.W + Scale1 * Quat2.W;

	return Result;
}

Quat Quat::SlerpFullPath_NotNormalized(const Quat& quat1, const Quat& quat2, float Alpha)
{
	const float CosAngle = Math::Clamp(quat1 | quat2, -1.f, 1.f);
	const float Angle = Math::Acos(CosAngle);

	//UE_LOG(LogUnrealMath, Log,  TEXT("CosAngle: %f Angle: %f"), CosAngle, Angle );

	if (Math::Abs(Angle) < KINDA_SMALL_NUMBER)
	{
		return quat1;
	}

	const float SinAngle = Math::Sin(Angle);
	const float InvSinAngle = 1.f / SinAngle;

	const float Scale0 = Math::Sin((1.0f - Alpha) * Angle) * InvSinAngle;
	const float Scale1 = Math::Sin(Alpha * Angle) * InvSinAngle;

	return quat1 * Scale0 + quat2 * Scale1;
}

Quat Quat::Squad(const Quat& quat1, const Quat& tang1, const Quat& quat2, const Quat& tang2, float Alpha)
{
	// Always slerp along the short path from quat1 to quat2 to prevent axis flipping.
	// This approach is taken by OGRE engine, amongst others.
	const Quat Q1 = Quat::Slerp_NotNormalized(quat1, quat2, Alpha);
	const Quat Q2 = Quat::SlerpFullPath_NotNormalized(tang1, tang2, Alpha);
	const Quat Result = Quat::SlerpFullPath(Q1, Q2, 2.f * Alpha * (1.f - Alpha));

	return Result;
}

Quat Quat::SquadFullPath(const Quat& quat1, const Quat& tang1, const Quat& quat2, const Quat& tang2, float Alpha)
{
	const Quat Q1 = Quat::SlerpFullPath_NotNormalized(quat1, quat2, Alpha);
	const Quat Q2 = Quat::SlerpFullPath_NotNormalized(tang1, tang2, Alpha);
	const Quat Result = Quat::SlerpFullPath(Q1, Q2, 2.f * Alpha * (1.f - Alpha));

	return Result;
}

void Quat::CalcTangents(const Quat& PrevP, const Quat& P, const Quat& NextP, float Tension, Quat& OutTan)
{
	const Quat InvP = P.Inverse();
	const Quat Part1 = (InvP * PrevP).Log();
	const Quat Part2 = (InvP * NextP).Log();

	const Quat PreExp = (Part1 + Part2) * -0.5f;

	OutTan = P * PreExp.Exp();
}

/** RotatorMatrix*/
Matrix RotationMatrix::MakeFromX(Vector const& XAxis)
{
	Vector const NewX = XAxis.GetSafeNormal();

	// try to use up if possible
	Vector const UpVector = (Math::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);

	const Vector NewY = (UpVector ^ NewX).GetSafeNormal();
	const Vector NewZ = NewX ^ NewY;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

Matrix RotationMatrix::MakeFromY(Vector const& YAxis)
{
	Vector const NewY = YAxis.GetSafeNormal();

	// try to use up if possible
	Vector const UpVector = (Math::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);

	const Vector NewZ = (UpVector ^ NewY).GetSafeNormal();
	const Vector NewX = NewY ^ NewZ;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

Matrix RotationMatrix::MakeFromZ(Vector const& ZAxis)
{
	Vector const NewZ = ZAxis.GetSafeNormal();

	// try to use up if possible
	Vector const UpVector = (Math::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);

	const Vector NewX = (UpVector ^ NewZ).GetSafeNormal();
	const Vector NewY = NewZ ^ NewX;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

Matrix RotationMatrix::MakeFromXY(Vector const& XAxis, Vector const& YAxis)
{
	Vector NewX = XAxis.GetSafeNormal();
	Vector Norm = YAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewX | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewZ = (NewX ^ Norm).GetSafeNormal();
	const Vector NewY = NewZ ^ NewX;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

Matrix RotationMatrix::MakeFromXZ(Vector const& XAxis, Vector const& ZAxis)
{
	Vector const NewX = XAxis.GetSafeNormal();
	Vector Norm = ZAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewX | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewY = (Norm ^ NewX).GetSafeNormal();
	const Vector NewZ = NewX ^ NewY;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

Matrix RotationMatrix::MakeFromYX(Vector const& YAxis, Vector const& XAxis)
{
	Vector const NewY = YAxis.GetSafeNormal();
	Vector Norm = XAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewY | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewZ = (Norm ^ NewY).GetSafeNormal();
	const Vector NewX = NewY ^ NewZ;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

Matrix RotationMatrix::MakeFromYZ(Vector const& YAxis, Vector const& ZAxis)
{
	Vector const NewY = YAxis.GetSafeNormal();
	Vector Norm = ZAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewY | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewX = (NewY ^ Norm).GetSafeNormal();
	const Vector NewZ = NewX ^ NewY;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

Matrix RotationMatrix::MakeFromZX(Vector const& ZAxis, Vector const& XAxis)
{
	Vector const NewZ = ZAxis.GetSafeNormal();
	Vector Norm = XAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewZ | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewY = (NewZ ^ Norm).GetSafeNormal();
	const Vector NewX = NewY ^ NewZ;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

Matrix RotationMatrix::MakeFromZY(Vector const& ZAxis, Vector const& YAxis)
{
	Vector const NewZ = ZAxis.GetSafeNormal();
	Vector Norm = YAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewZ | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewX = (Norm ^ NewZ).GetSafeNormal();
	const Vector NewY = NewZ ^ NewX;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}