/*
 * Copyright 2010-2018 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bx#license-bsd-2-clause
 */

#include "test.h"
#include <bx/math.h>
#include <bx/file.h>

#include <math.h>

#if !BX_COMPILER_MSVC || BX_COMPILER_MSVC >= 1800
TEST_CASE("isFinite, isInfinite, isNan", "")
{
	for (uint64_t ii = 0; ii < UINT32_MAX; ii += rand()%(1<<13)+1)
	{
		union { uint32_t ui; float f; } u = { uint32_t(ii) };
		REQUIRE(std::isnan(u.f)    == bx::isNan(u.f) );
		REQUIRE(std::isfinite(u.f) == bx::isFinite(u.f) );
		REQUIRE(std::isinf(u.f)    == bx::isInfinite(u.f) );
	}
}
#endif // !BX_COMPILER_MSVC || BX_COMPILER_MSVC >= 1800

bool log2_test(float _a)
{
	return bx::log2(_a) == bx::log(_a) * (1.0f / bx::log(2.0f) );
}

TEST_CASE("log2", "")
{
	log2_test(0.0f);
	log2_test(256.0f);
}

TEST_CASE("libm", "")
{
	bx::WriterI* writer = bx::getNullOut();

	REQUIRE(1389.0f == bx::abs(-1389.0f) );
	REQUIRE(1389.0f == bx::abs( 1389.0f) );
	REQUIRE(   0.0f == bx::abs(-0.0f) );
	REQUIRE(   0.0f == bx::abs( 0.0f) );

	REQUIRE(389.0f == bx::mod(1389.0f, 1000.0f) );
	REQUIRE(bx::isNan(bx::mod(0.0f, 0.0f) ) );

	REQUIRE( 13.0f == bx::floor( 13.89f) );
	REQUIRE(-14.0f == bx::floor(-13.89f) );
	REQUIRE( 14.0f == bx::ceil(  13.89f) );
	REQUIRE(-13.0f == bx::ceil( -13.89f) );

	REQUIRE( 13.0f == bx::trunc( 13.89f) );
	REQUIRE(-13.0f == bx::trunc(-13.89f) );
	REQUIRE(bx::equal( 0.89f, bx::fract( 13.89f), 0.000001f) );
	REQUIRE(bx::equal(-0.89f, bx::fract(-13.89f), 0.000001f) );

	for (int32_t yy = -10; yy < 10; ++yy)
	{
		for (float xx = -100.0f; xx < 100.0f; xx += 0.1f)
		{
			bx::writePrintf(writer, "ldexp(%f, %d) == %f (expected: %f)\n", xx, yy, bx::ldexp(xx, yy), ::ldexpf(xx, yy) );
			REQUIRE(bx::equal(bx::ldexp(xx, yy), ::ldexpf(xx, yy), 0.00001f) );
		}
	}

	for (float xx = -80.0f; xx < 80.0f; xx += 0.1f)
	{
		bx::writePrintf(writer, "exp(%f) == %f (expected: %f)\n", xx, bx::exp(xx), ::expf(xx) );
		REQUIRE(bx::equal(bx::exp(xx), ::expf(xx), 0.00001f) );
	}

	for (float xx = 0.0f; xx < 100.0f; xx += 0.1f)
	{
		bx::writePrintf(writer, "rsqrt(%f) == %f (expected: %f)\n", xx, bx::rsqrt(xx), 1.0f/::sqrtf(xx) );
		REQUIRE(bx::equal(bx::rsqrt(xx), 1.0f/::sqrtf(xx), 0.00001f) );
	}

	for (float xx = 0.0f; xx < 100.0f; xx += 0.1f)
	{
		bx::writePrintf(writer, "sqrt(%f) == %f (expected: %f)\n", xx, bx::sqrt(xx), ::sqrtf(xx) );
		REQUIRE(bx::equal(bx::sqrt(xx), ::sqrtf(xx), 0.00001f) );
	}

	for (float xx = -100.0f; xx < 100.0f; xx += 0.1f)
	{
		bx::writePrintf(writer, "pow(1.389f, %f) == %f (expected: %f)\n", xx, bx::pow(1.389f, xx), ::powf(1.389f, xx) );
		REQUIRE(bx::equal(bx::pow(1.389f, xx), ::powf(1.389f, xx), 0.00001f) );
	}

	for (float xx = -1.0f; xx < 1.0f; xx += 0.001f)
	{
		bx::writePrintf(writer, "asin(%f) == %f (expected: %f)\n", xx, bx::asin(xx), ::asinf(xx) );
		REQUIRE(bx::equal(bx::asin(xx), ::asinf(xx), 0.0001f) );
	}

	for (float xx = -100.0f; xx < 100.0f; xx += 0.1f)
	{
		bx::writePrintf(writer, "sin(%f) == %f (expected: %f)\n", xx, bx::sin(xx), ::sinf(xx) );
		REQUIRE(bx::equal(bx::sin(xx), ::sinf(xx), 0.00001f) );
	}

	for (float xx = -1.0f; xx < 1.0f; xx += 0.1f)
	{
		bx::writePrintf(writer, "sinh(%f) == %f (expected: %f)\n", xx, bx::sinh(xx), ::sinhf(xx) );
		REQUIRE(bx::equal(bx::sinh(xx), ::sinhf(xx), 0.00001f) );
	}

	for (float xx = -1.0f; xx < 1.0f; xx += 0.001f)
	{
		bx::writePrintf(writer, "acos(%f) == %f (expected: %f\n)", xx, bx::acos(xx), ::acosf(xx) );
		REQUIRE(bx::equal(bx::acos(xx), ::acosf(xx), 0.0001f) );
	}

	for (float xx = -100.0f; xx < 100.0f; xx += 0.1f)
	{
		bx::writePrintf(writer, "cos(%f) == %f (expected: %f)\n", xx, bx::cos(xx), ::cosf(xx) );
		REQUIRE(bx::equal(bx::cos(xx), ::cosf(xx), 0.00001f) );
	}

	for (float xx = -100.0f; xx < 100.0f; xx += 0.1f)
	{
		bx::writePrintf(writer, "tan(%f) == %f (expected: %f)\n", xx, bx::tan(xx), ::tanf(xx) );
		REQUIRE(bx::equal(bx::tan(xx), ::tanf(xx), 0.001f) );
	}

	for (float xx = -1.0f; xx < 1.0f; xx += 0.1f)
	{
		bx::writePrintf(writer, "tanh(%f) == %f (expected: %f\n", xx, bx::tanh(xx), ::tanhf(xx) );
		REQUIRE(bx::equal(bx::tanh(xx), ::tanhf(xx), 0.00001f) );
	}

	for (float xx = -100.0f; xx < 100.0f; xx += 0.1f)
	{
		bx::writePrintf(writer, "atan(%f) == %f (expected: %f)\n", xx, bx::atan(xx), ::atanf(xx) );
		REQUIRE(bx::equal(bx::atan(xx), ::atanf(xx), 0.00001f) );
	}

	for (float yy = -100.0f; yy < 100.0f; yy += 0.1f)
	{
		for (float xx = -100.0f; xx < 100.0f; xx += 0.1f)
		{
			bx::writePrintf(writer, "atan2(%f, %f) == %f (expected: %f)\n", yy, xx, bx::atan2(yy, xx), ::atan2f(yy, xx) );
			REQUIRE(bx::equal(bx::atan2(yy, xx), ::atan2f(yy, xx), 0.00001f) );
		}
	}

	REQUIRE(bx::equal(bx::atan2(0.0f, 0.0f), ::atan2f(0.0f, 0.0f), 0.00001f) );
}

TEST_CASE("ToBits", "")
{
	REQUIRE(UINT32_C(0x12345678)         == bx::floatToBits( bx::bitsToFloat( UINT32_C(0x12345678) ) ) );
	REQUIRE(UINT64_C(0x123456789abcdef0) == bx::doubleToBits(bx::bitsToDouble(UINT32_C(0x123456789abcdef0) ) ) );
}

void mtxCheck(const float* _a, const float* _b)
{
	if (!bx::equal(_a, _b, 16, 0.01f) )
	{
		DBG("\n"
			"A:\n"
			"%10.4f %10.4f %10.4f %10.4f\n"
			"%10.4f %10.4f %10.4f %10.4f\n"
			"%10.4f %10.4f %10.4f %10.4f\n"
			"%10.4f %10.4f %10.4f %10.4f\n"
			"B:\n"
			"%10.4f %10.4f %10.4f %10.4f\n"
			"%10.4f %10.4f %10.4f %10.4f\n"
			"%10.4f %10.4f %10.4f %10.4f\n"
			"%10.4f %10.4f %10.4f %10.4f\n"
			, _a[ 0], _a[ 1], _a[ 2], _a[ 3]
			, _a[ 4], _a[ 5], _a[ 6], _a[ 7]
			, _a[ 8], _a[ 9], _a[10], _a[11]
			, _a[12], _a[13], _a[14], _a[15]
			, _b[ 0], _b[ 1], _b[ 2], _b[ 3]
			, _b[ 4], _b[ 5], _b[ 6], _b[ 7]
			, _b[ 8], _b[ 9], _b[10], _b[11]
			, _b[12], _b[13], _b[14], _b[15]
			);

		CHECK(false);
	}
}

TEST_CASE("quaternion", "")
{
	float mtxQ[16];
	float mtx[16];

	float quat[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	bx::mtxQuat(mtxQ, quat);
	bx::mtxIdentity(mtx);
	mtxCheck(mtxQ, mtx);

	float ax = bx::kPi/27.0f;
	float ay = bx::kPi/13.0f;
	float az = bx::kPi/7.0f;

	bx::quatRotateX(quat, ax);
	bx::mtxQuat(mtxQ, quat);
	bx::mtxRotateX(mtx, ax);
	mtxCheck(mtxQ, mtx);

	float euler[3];
	bx::quatToEuler(euler, quat);
	CHECK(bx::equal(euler[0], ax, 0.001f) );

	bx::quatRotateY(quat, ay);
	bx::mtxQuat(mtxQ, quat);
	bx::mtxRotateY(mtx, ay);
	mtxCheck(mtxQ, mtx);

	bx::quatToEuler(euler, quat);
	CHECK(bx::equal(euler[1], ay, 0.001f) );

	bx::quatRotateZ(quat, az);
	bx::mtxQuat(mtxQ, quat);
	bx::mtxRotateZ(mtx, az);
	mtxCheck(mtxQ, mtx);

	bx::quatToEuler(euler, quat);
	CHECK(bx::equal(euler[2], az, 0.001f) );
}
