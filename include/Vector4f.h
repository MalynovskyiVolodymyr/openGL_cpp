#ifndef VECTOR4F
#define VECTOR4F

#include "Vector3f.h"

struct Vector4f
{
    float x;
    float y;
    float z;
    float w;

    Vector4f()
    {
    }

    Vector4f(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    Vector4f(const Vector3f& v, float _w)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = _w;
    }

    void Print(bool endl = true) const
    {
        printf("(%f, %f, %f, %f)", x, y, z, w);

        if (endl) {
            printf("\n");
        }
    }

    Vector3f to3f() const
    {
        Vector3f v(x, y, z);
        return v;
    }

    float Length() const
    {
        float len = sqrtf(x * x + y * y + z * z + w * w);
        return len;
    }

    Vector4f& Normalize();

    float Dot(const Vector4f& v) const
    {
        float ret = x * v.x + y * v.y + z * v.z + w * v.w;
        return ret;
    }
};

inline Vector4f operator+(const Vector4f& l, const Vector4f& r)
{
    Vector4f Ret(l.x + r.x,
                 l.y + r.y,
                 l.z + r.z,
                 l.w + r.w);

    return Ret;
}


inline Vector4f operator/(const Vector4f& l, float f)
{
    Vector4f Ret(l.x / f,
                 l.y / f,
                 l.z / f,
                 l.w / f);

    return Ret;
}


inline Vector4f operator*(const Vector4f& l, float f)
{
    Vector4f Ret(l.x * f,
                 l.y * f,
                 l.z * f,
                 l.w * f);

    return Ret;
}


inline Vector4f operator*(float f, const Vector4f& l)
{
    Vector4f Ret(l.x * f,
                 l.y * f,
                 l.z * f,
                 l.w * f);

    return Ret;
}

#endif
