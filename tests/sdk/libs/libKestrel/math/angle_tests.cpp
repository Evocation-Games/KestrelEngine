// Copyright (c) 2022 Tom Hancocks
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <libTesting/testing.hpp>
#include <libKestrel/math/angle.hpp>
#include <libKestrel/math/angular_difference.hpp>
#include <libKestrel/math/point.hpp>

using namespace kestrel::math;

TEST(math_angle_constructFromTheta)
{
    angle sut(50.f);
    test::equal(sut.degrees(), 50.f);
}

TEST(math_angle_constructFromTheta_normalisesCorrectly)
{
    angle sut(370.f);
    test::equal(sut.degrees(), 10.f);
}

TEST(math_angle_degreesReturnsExpectedValue)
{
    angle sut(50.f);
    test::equal(sut.degrees(), 50.f);
}

TEST(math_angle_radiansReturnsExpectedValue)
{
    angle sut(90.f);
    test::equal(sut.radians(), 1.571f, 0.1f);
}

TEST(math_angle_sinReturnsExpectedValue)
{
    angle sut(90.f);
    test::equal(sut.sin(), 1);
}

TEST(math_angle_fastSinReturnsExpectedValue)
{
    angle sut(90.f);
    test::equal(sut.fsin(), 1);
}

TEST(math_angle_cosReturnsExpectedValue)
{
    angle sut(150.f);
    test::equal(sut.cos(), -0.866023f, 0.0001f);
}

TEST(math_angle_fastCosReturnsExpectedValue)
{
    angle sut(150.f);
    test::equal(sut.fcos(), -0.866025388f, 0.0001f);
}

TEST(math_angle_addAnglesReturnsExpectedValue)
{
    angle sut(90.f);
    test::equal((sut + sut).degrees(), 180.f);
}

TEST(math_angle_subtractAnglesReturnsExpectedValue)
{
    angle sut(90.f);
    test::equal((sut - angle(20.f)).degrees(), 70.f);
}

TEST(math_angle_addAngularDifferenceReturnsExpectedValue)
{
    angle sut(45.f);
    angular_difference diff(10.f);
    test::equal((sut + diff).degrees(), 55.f);
    test::equal(sut.add_angular_difference(diff).degrees(), 55.f);
}

TEST(math_angle_subtractAngularDifferenceReturnsExpectedValue)
{
    angle sut(45.f);
    angular_difference diff(10.0);
    test::equal((sut - diff).degrees(), 35.f);
    test::equal(sut.subtract_angular_difference(diff).degrees(), 35.f);
}

TEST(math_angle_vectorReturnsExpectedPoint)
{
    test::equal(angle(0).vector(1.0), point(1, 0));
    test::equal(angle(90).vector(1.0), point(0, 1));
    test::equal(angle(180).vector(1.0), point(-1, 0));
    test::equal(angle(270).vector(1.0), point(0, -1));
}

TEST(math_angle_oppositeReturnsExpectedAngle)
{
    test::equal(angle(90).opposite().degrees(), 270.f);
    test::equal(angle(0).opposite().degrees(), 180.f);
    test::equal(angle(270).opposite().degrees(), 90.f);
    test::equal(angle(180).opposite().degrees(), 0.f);
    test::equal(angle(360).opposite().degrees(), 180.f);
}