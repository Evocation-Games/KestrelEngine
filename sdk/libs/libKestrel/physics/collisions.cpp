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

#include <libKestrel/physics/collisions.hpp>
#include <libSIMD/float32.hpp>

// MARK: - Helpers

namespace kestrel::physics::collisions
{
    inline auto orient(const math::vec2& a, const math::vec2& b, const math::vec2& c) -> double
    {
        auto n = simd::float32(a.x(), b.y(), a.y(), b.x());
        auto m = simd::float32(c.x(), c.y(), c.y(), c.x());
        n -= m;
        m = simd::float32(n[0], n[2]) * simd::float32(n[1], n[3]);
        return m[0] - m[1];
    }

    inline auto test_vertex_intersection(const math::vec2& p1, const math::vec2& q1, const math::vec2& r1, const math::vec2& p2, const math::vec2& q2, const math::vec2& r2) -> bool
    {
        if (orient(r2,p2,q1) >= 0.0) {
            if (orient(r2, q2, q1) <= 0.0) {
                if (orient(p1, p2, q1) > 0.0) {
                    if (orient(p1, q2, q1) <= 0.0) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    if (orient(p1, p2, r1) >= 0.0) {
                        if (orient(q1, r1, p2) >= 0.0) {
                            return true;
                        } else {
                            return false;
                        }
                    } else {
                        return false;
                    }
                }
            } else {
                if (orient(p1, q2, q1) <= 0.0) {
                    if (orient(r2, q2, r1) <= 0.0) {
                        if (orient(q1, r1, q2) >= 0.0) {
                            return true;
                        } else {
                            return false;
                        }
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
        else {
            if (orient(r2,p2,r1) >= 0.0) {
                if (orient(q1,r1,r2) >= 0.0) {
                    if (orient(p1,p2,r1) >= 0.0) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else {
                    if (orient(q1,r1,q2) >= 0.0) {
                        if (orient(r2,r1,q2) >= 0.0) {
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                    else {
                        return false;
                    }
                }
            }
            else {
                return false;
            }
        }
    }

    inline auto test_edge_intersection(const math::vec2& p1, const math::vec2& q1, const math::vec2& r1, const math::vec2& p2, const math::vec2& q2, const math::vec2& r2) -> bool
    {
        if (orient(r2, p2, q1) >= 0.0) {
            if (orient(p1, p2, q1) >= 0.0) {
                if (orient(p1, q1, r2) >= 0.0) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                if (orient(q1, r1, p2) >= 0.0) {
                    if (orient(r1, p1, p2) >= 0.0) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else {
                    return false;
                }
            }
        }
        else {
            if (orient(r2, p2, r1) >= 0.0) {
                if (orient(p1, p2, r1) >= 0.0) {
                    if (orient(p1, r1, r2) >= 0.0) {
                        return true;
                    }
                    else {
                        if (orient(q1, r1, r2) >= 0.0) {
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
    }
}

// MARK: - Triangle - Triangle Collisions

namespace kestrel::physics::collisions
{
    inline auto ccw_triangle_triangle_intersection(const math::vec2& p1, const math::vec2& q1, const math::vec2& r1, const math::vec2& p2, const math::vec2& q2, const math::vec2& r2) -> bool
    {
        if (orient(p2, q2, p1) >= 0.0) {
            if (orient(q2, r2, p1) >= 0.0) {
                if (orient(r2, p2, p1) >= 0.0) {
                    return true;
                }
                else {
                    return test_edge_intersection(p1, q1, r1, p2, q2, r2);
                }
            }
            else {
                if (orient(r2, p2, p1) >= 0.0) {
                    return test_edge_intersection(p1, q1, r1, r2, p2, q2);
                }
                else {
                    return test_vertex_intersection(p1, q1, r1, p2, q2, r2);
                }
            }
        }
        else {
            if (orient(q2, r2, p1) >= 0.0) {
                if (orient(r2, p2, p1) >= 0.0) {
                    return test_edge_intersection(p1, q1, r1, q2, r2, p2);
                }
                else {
                    return test_vertex_intersection(p1, q1, r1, q2, r2, p2);
                }
            }
            else {
                return test_vertex_intersection(p1, q1, r1, r2, p2, q2);
            }
        }
    }

    auto test(const math::triangle &a, const math::triangle &b) -> bool
    {
        if (orient(a.a, a.b, a.c) < 0.0) {
            if (orient(b.a, b.b, b.c) < 0.0) {
                return ccw_triangle_triangle_intersection(a.a, a.c, a.b, b.a, b.c, b.b);
            }
            else {
                return ccw_triangle_triangle_intersection(a.a, a.c, a.b, b.a, b.b, b.c);
            }
        }
        else {
            if (orient(b.a, b.b, b.c) < 0.0) {
                return ccw_triangle_triangle_intersection(a.a, a.b, a.c, b.a, b.c, b.b);
            }
            else {
                return ccw_triangle_triangle_intersection(a.a, a.b, a.c, b.a, b.b, b.c);
            }
        }
    }
}


