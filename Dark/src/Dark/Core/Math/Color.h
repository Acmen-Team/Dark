// Copyright LiuD, Inc. All Rights Reserved.

#pragma once

#include "MathUtility.h"
#include "../Containers/String.h"

/**
 * A linear, 32-bit/component floating point RGBA color.
 */
struct LinearColor
{
	float	R,
			G,
			B,
			A;

	/** Static lookup table used for FColor -> FLinearColor conversion. Pow(2.2) */
	static float Pow22OneOver255Table[256];

	/** Static lookup table used for FColor -> FLinearColor conversion. sRGB */
	static float sRGBToLinearTable[256];

	FORCEINLINE LinearColor() {}
	constexpr FORCEINLINE LinearColor(float InR, float InG, float InB, float InA = 1.0f) : R(InR), G(InG), B(InB), A(InA) {}

	/**
	 * Converts an FColor which is assumed to be in sRGB space, into linear color space.
	 * @param Color The sRGB color that needs to be converted into linear space.
	 */
	LinearColor(const Color& Color);

	LinearColor(const Vector& Vector);

	explicit LinearColor(const Vector4& Vector);

	//explicit LinearColor(const FFloat16Color& C);

	// Serializer.
	//Todo

	// Conversions.
	Color ToRGBE() const;

	/**
	 * Converts an Color coming from an observed sRGB output, into a linear color.
	 * @param Color The sRGB color that needs to be converted into linear space.
	 */
	static LinearColor FromSRGBColor(const Color& Color);

	/**
	 * Converts an Color coming from an observed Pow(1/2.2) output, into a linear color.
	 * @param Color The Pow(1/2.2) color that needs to be converted into linear space.
	 */
	static LinearColor FromPow22Color(const Color& Color);

	// Operators.

	FORCEINLINE float& Component(int32 Index)
	{
		return (&R)[Index];
	}

	FORCEINLINE const float& Component(int32 Index) const
	{
		return (&R)[Index];
	}

	FORCEINLINE LinearColor operator+(const LinearColor& ColorB) const
	{
		return LinearColor(
			R + ColorB.R,
			G + ColorB.G,
			B + ColorB.B,
			A + ColorB.A
		);
	}
	FORCEINLINE LinearColor& operator+=(const LinearColor& ColorB)
	{
		R += ColorB.R;
		G += ColorB.G;
		B += ColorB.B;
		A += ColorB.A;
		return *this;
	}

	FORCEINLINE LinearColor operator-(const LinearColor& ColorB) const
	{
		return LinearColor(
			R - ColorB.R,
			G - ColorB.G,
			B - ColorB.B,
			A - ColorB.A
		);
	}
	FORCEINLINE LinearColor& operator-=(const LinearColor& ColorB)
	{
		R -= ColorB.R;
		G -= ColorB.G;
		B -= ColorB.B;
		A -= ColorB.A;
		return *this;
	}

	FORCEINLINE LinearColor operator*(const LinearColor& ColorB) const
	{
		return LinearColor(
			R * ColorB.R,
			G * ColorB.G,
			B * ColorB.B,
			A * ColorB.A
		);
	}
	FORCEINLINE LinearColor& operator*=(const LinearColor& ColorB)
	{
		R *= ColorB.R;
		G *= ColorB.G;
		B *= ColorB.B;
		A *= ColorB.A;
		return *this;
	}

	FORCEINLINE LinearColor operator*(float Scalar) const
	{
		return LinearColor(
			R * Scalar,
			G * Scalar,
			B * Scalar,
			A * Scalar
		);
	}

	FORCEINLINE LinearColor& operator*=(float Scalar)
	{
		R *= Scalar;
		G *= Scalar;
		B *= Scalar;
		A *= Scalar;
		return *this;
	}

	FORCEINLINE LinearColor operator/(const LinearColor& ColorB) const
	{
		return LinearColor(
			R / ColorB.R,
			G / ColorB.G,
			B / ColorB.B,
			A / ColorB.A
		);
	}
	FORCEINLINE LinearColor& operator/=(const LinearColor& ColorB)
	{
		R /= ColorB.R;
		G /= ColorB.G;
		B /= ColorB.B;
		A /= ColorB.A;
		return *this;
	}

	FORCEINLINE LinearColor operator/(float Scalar) const
	{
		const float	InvScalar = 1.0f / Scalar;
		return LinearColor(
			R * InvScalar,
			G * InvScalar,
			B * InvScalar,
			A * InvScalar
		);
	}
	FORCEINLINE LinearColor& operator/=(float Scalar)
	{
		const float	InvScalar = 1.0f / Scalar;
		R *= InvScalar;
		G *= InvScalar;
		B *= InvScalar;
		A *= InvScalar;
		return *this;
	}

	// clamped in 0..1 range
	FORCEINLINE LinearColor GetClamped(float InMin = 0.0f, float InMax = 1.0f) const
	{
		LinearColor Ret;

		Ret.R = Math::Clamp(R, InMin, InMax);
		Ret.G = Math::Clamp(G, InMin, InMax);
		Ret.B = Math::Clamp(B, InMin, InMax);
		Ret.A = Math::Clamp(A, InMin, InMax);

		return Ret;
	}

	/** Comparison operators */
	FORCEINLINE bool operator==(const LinearColor& ColorB) const
	{
		return this->R == ColorB.R && this->G == ColorB.G && this->B == ColorB.B && this->A == ColorB.A;
	}
	FORCEINLINE bool operator!=(const LinearColor& Other) const
	{
		return this->R != Other.R || this->G != Other.G || this->B != Other.B || this->A != Other.A;
	}

	// Error-tolerant comparison.
	FORCEINLINE bool Equals(const LinearColor& ColorB, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return Math::Abs(this->R - ColorB.R) < Tolerance && Math::Abs(this->G - ColorB.G) < Tolerance && Math::Abs(this->B - ColorB.B) < Tolerance && Math::Abs(this->A - ColorB.A) < Tolerance;
	}

	LinearColor CopyWithNewOpacity(float NewOpacicty) const
	{
		LinearColor NewCopy = *this;
		NewCopy.A = NewOpacicty;
		return NewCopy;
	}

	/**
	 * Converts byte hue-saturation-brightness to floating point red-green-blue.
	 */
	static LinearColor MakeFromHSV8(uint8 H, uint8 S, uint8 V);

	/**
	* Makes a random but quite nice color.
	*/
	static LinearColor MakeRandomColor();

	/**
	* Converts temperature in Kelvins of a black body radiator to RGB chromaticity.
	*/
	static LinearColor MakeFromColorTemperature(float Temp);

	/**
	 * Euclidean distance between two points.
	 */
	static inline float Dist(const LinearColor& V1, const LinearColor& V2)
	{
		return Math::Sqrt(Math::Square(V2.R - V1.R) + Math::Square(V2.G - V1.G) + Math::Square(V2.B - V1.B) + Math::Square(V2.A - V1.A));
	}

	/**
	 * Generates a list of sample points on a Bezier curve defined by 2 points.
	 *
	 * @param	ControlPoints	Array of 4 Linear Colors (vert1, controlpoint1, controlpoint2, vert2).
	 * @param	NumPoints		Number of samples.
	 * @param	OutPoints		Receives the output samples.
	 * @return					Path length.
	 */
	static float EvaluateBezier(const LinearColor* ControlPoints, int32 NumPoints, TArray<LinearColor>& OutPoints);

	/** Converts a linear space RGB color to an HSV color */
	LinearColor LinearRGBToHSV() const;

	/** Converts an HSV color to a linear space RGB color */
	LinearColor HSVToLinearRGB() const;

	/**
	 * Linearly interpolates between two colors by the specified progress amount.  The interpolation is performed in HSV color space
	 * taking the shortest path to the new color's hue.  This can give better results than Math::Lerp(), but is much more expensive.
	 * The incoming colors are in RGB space, and the output color will be RGB.  The alpha value will also be interpolated.
	 *
	 * @param	From		The color and alpha to interpolate from as linear RGBA
	 * @param	To			The color and alpha to interpolate to as linear RGBA
	 * @param	Progress	Scalar interpolation amount (usually between 0.0 and 1.0 inclusive)
	 * @return	The interpolated color in linear RGB space along with the interpolated alpha value
	 */
	static LinearColor LerpUsingHSV(const LinearColor& From, const LinearColor& To, const float Progress);

	/** Quantizes the linear color and returns the result as a FColor.  This bypasses the SRGB conversion. */
	Color Quantize() const;

	/** Quantizes the linear color with rounding and returns the result as a FColor.  This bypasses the SRGB conversion. */
	Color QuantizeRound() const;

	/** Quantizes the linear color and returns the result as a FColor with optional sRGB conversion and quality as goal. */
	Color ToFColor(const bool bSRGB) const;

	/**
	 * Returns a desaturated color, with 0 meaning no desaturation and 1 == full desaturation
	 *
	 * @param	Desaturation	Desaturation factor in range [0..1]
	 * @return	Desaturated color
	 */
	LinearColor Desaturate(float Desaturation) const;

	/** Computes the perceptually weighted luminance value of a color. */
	inline float ComputeLuminance() const
	{
		return R * 0.3f + G * 0.59f + B * 0.11f;
	}

	/**
	 * Returns the maximum value in this color structure
	 *
	 * @return The maximum color channel value
	 */
	FORCEINLINE float GetMax() const
	{
		return Math::Max(Math::Max(Math::Max(R, G), B), A);
	}

	/** useful to detect if a light contribution needs to be rendered */
	bool IsAlmostBlack() const
	{
		return Math::Square(R) < DELTA && Math::Square(G) < DELTA && Math::Square(B) < DELTA;
	}

	/**
	 * Returns the minimum value in this color structure
	 *
	 * @return The minimum color channel value
	 */
	FORCEINLINE float GetMin() const
	{
		return Math::Min(Math::Min(Math::Min(R, G), B), A);
	}

	FORCEINLINE float GetLuminance() const
	{
		return R * 0.3f + G * 0.59f + B * 0.11f;
	}

	// Common colors.
	static const LinearColor White;
	static const LinearColor Gray;
	static const LinearColor Black;
	static const LinearColor Transparent;
	static const LinearColor Red;
	static const LinearColor Green;
	static const LinearColor Blue;
	static const LinearColor Yellow;
};


FORCEINLINE LinearColor operator*(const float Scalar, const LinearColor& Color)
{
	return Color.operator*(Scalar);
}

//
//	FColor
//	Stores a color with 8 bits of precision per channel.
//	Note: Linear color values should always be converted to gamma space before stored in an FColor, as 8 bits of precision is not enough to store linear space colors!
//	This can be done with FLinearColor::ToFColor(true)
//

struct Color
{
	union { struct { uint8 B, G, R, A; }; uint32 AlignmentDummy; };

	uint32& DWColor(void) { return *((uint32*)this); }
	const uint32& DWColor(void) const { return *((uint32*)this); }

	// Constructors.
	//FORCEINLINE Color() {}

	Color()
	{
		// put these into the body for proper ordering with INTEL vs non-INTEL_BYTE_ORDER
		R = G = B = A = 0;
	}

	constexpr FORCEINLINE Color(uint8 InR, uint8 InG, uint8 InB, uint8 InA = 255)
		: A(InA), R(InR), G(InG), B(InB)
	{}

	FORCEINLINE explicit Color(uint32 InColor)
	{
		DWColor() = InColor;
	}

	// Serializer
	//Todo

	// Operators.
	FORCEINLINE bool operator==(const Color& C) const
	{
		return DWColor() == C.DWColor();
	}

	FORCEINLINE bool operator!=(const Color& C) const
	{
		return DWColor() != C.DWColor();
	}

	FORCEINLINE void operator+=(const Color& C)
	{
		R = (uint8)Math::Min((int32)R + (int32)C.R, 255);
		G = (uint8)Math::Min((int32)G + (int32)C.G, 255);
		B = (uint8)Math::Min((int32)B + (int32)C.B, 255);
		A = (uint8)Math::Min((int32)A + (int32)C.A, 255);
	}

	LinearColor FromRGBE() const;

	/**
	 * Creates a color value from the given hexadecimal string.
	 *
	 * Supported formats are: RGB, RRGGBB, RRGGBBAA, #RGB, #RRGGBB, #RRGGBBAA
	 *
	 * @param HexString - The hexadecimal string.
	 * @return The corresponding color value.
	 * @see ToHex
	 */
	//static Color FromHex(const FString& HexString);

	/**
	 * Makes a random but quite nice color.
	 */
	static Color MakeRandomColor();

	/**
	 * Makes a color red->green with the passed in scalar (e.g. 0 is red, 1 is green)
	 */
	static Color MakeRedToGreenColorFromScalar(float Scalar);

	/**
	* Converts temperature in Kelvins of a black body radiator to RGB chromaticity.
	*/
	static Color MakeFromColorTemperature(float Temp);

	/**
	 *	@return a new FColor based of this color with the new alpha value.
	 *	Usage: const FColor& MyColor = FColorList::Green.WithAlpha(128);
	 */
	Color WithAlpha(uint8 Alpha) const
	{
		return Color(R, G, B, Alpha);
	}

	/**
	 * Reinterprets the color as a linear color.
	 *
	 * @return The linear color representation.
	 */
	FORCEINLINE LinearColor ReinterpretAsLinear() const
	{
		return LinearColor(R / 255.f, G / 255.f, B / 255.f, A / 255.f);
	}

	/**
	 * Converts this color value to a hexadecimal string.
	 *
	 * The format of the string is RRGGBBAA.
	 *
	 * @return Hexadecimal string.
	 * @see FromHex, ToString
	 */
	/*FORCEINLINE FString ToHex() const
	{
		return FString::Printf(TEXT("%02X%02X%02X%02X"), R, G, B, A);
	}*/

	/**
	 * Converts this color value to a string.
	 *
	 * @return The string representation.
	 * @see ToHex
	 */
	/*FORCEINLINE FString ToString() const
	{
		return FString::Printf(TEXT("(R=%i,G=%i,B=%i,A=%i)"), R, G, B, A);
	}*/

	/**
	 * Initialize this Color based on an FString. The String is expected to contain R=, G=, B=, A=.
	 * The FColor will be bogus when InitFromString returns false.
	 *
	 * @param	InSourceString	FString containing the color values.
	 * @return true if the R,G,B values were read successfully; false otherwise.
	 */
	//bool InitFromString(const FString& InSourceString)
	//{
	//	R = G = B = 0;
	//	A = 255;

	//	// The initialization is only successful if the R, G, and B values can all be parsed from the string
	//	const bool bSuccessful = FParse::Value(*InSourceString, TEXT("R="), R) && FParse::Value(*InSourceString, TEXT("G="), G) && FParse::Value(*InSourceString, TEXT("B="), B);

	//	// Alpha is optional, so don't factor in its presence (or lack thereof) in determining initialization success
	//	FParse::Value(*InSourceString, TEXT("A="), A);

	//	return bSuccessful;
	//}

	/**
	 * Gets the color in a packed uint32 format packed in the order ARGB.
	 */
	FORCEINLINE uint32 ToPackedARGB() const
	{
		return (A << 24) | (R << 16) | (G << 8) | (B << 0);
	}

	/**
	 * Gets the color in a packed uint32 format packed in the order ABGR.
	 */
	FORCEINLINE uint32 ToPackedABGR() const
	{
		return (A << 24) | (B << 16) | (G << 8) | (R << 0);
	}

	/**
	 * Gets the color in a packed uint32 format packed in the order RGBA.
	 */
	FORCEINLINE uint32 ToPackedRGBA() const
	{
		return (R << 24) | (G << 16) | (B << 8) | (A << 0);
	}

	/**
	 * Gets the color in a packed uint32 format packed in the order BGRA.
	 */
	FORCEINLINE uint32 ToPackedBGRA() const
	{
		return (B << 24) | (G << 16) | (R << 8) | (A << 0);
	}

	/** Some pre-inited colors, useful for debug code */
	static const Color White;
	static const Color Black;
	static const Color Transparent;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Cyan;
	static const Color Magenta;
	static const Color Orange;
	static const Color Purple;
	static const Color Turquoise;
	static const Color Silver;
	static const Color Emerald;

private:
	/**
	 * Please use .ToFColor(true) on FLinearColor if you wish to convert from FLinearColor to FColor,
	 * with proper sRGB conversion applied.
	 *
	 * Note: Do not implement or make public.  We don't want people needlessly and implicitly converting between
	 * FLinearColor and FColor.  It's not a free conversion.
	 */
	explicit Color(const LinearColor& LinearColor);

};

FORCEINLINE uint32 GetTypeHash(const Color& Color)
{
	return Color.DWColor();
}

//FORCEINLINE uint32 GetTypeHash(const LinearColor& LinearColor)
//{
//	// Note: this assumes there's no padding in FLinearColor that could contain uncompared data.
//	//return FCrc::MemCrc_DEPRECATED(&LinearColor, sizeof(LinearColor));
//}

/** Computes a brightness and a fixed point color from a floating point color. */
extern void ComputeAndFixedColorAndIntensity(const LinearColor& InLinearColor, Color& OutColor, float& OutIntensity);