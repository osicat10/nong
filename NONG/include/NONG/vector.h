#ifndef NONG_VECTOR_H
#define NONG_VECTOR_H

#include <string>

namespace NONG {
    class Vector2
    {
    public:
        double x, y;

        Vector2();
        Vector2(double x, double y);
        Vector2(const Vector2& v);

        static Vector2 right();
        static Vector2 left();
        static Vector2 up();
        static Vector2 down();
        static Vector2 zero();
        static Vector2 one();

        double magnitude() const;
        
        Vector2 normalized() const;
        Vector2& normalize();

        Vector2& operator+=(const Vector2& rhs);
        Vector2& operator-=(const Vector2& rhs);
        // Vector2& operator*=(const Vector2& rhs);
        Vector2& operator*=(const double& rhs);
        // Vector2& operator/=(const Vector2& rhs);        
        Vector2& operator/=(const double& rhs);

        Vector2 operator+(const Vector2& rhs) const;
        Vector2 operator-(const Vector2& rhs) const;
        // Vector2 operator*(const Vector2& rhs) const;
        Vector2 operator*(const double& rhs) const;
        // Vector2 operator/(const Vector2& rhs) const;
        Vector2 operator/(const double& rhs) const;

        bool operator==(const Vector2& rhs) const;
        bool operator!=(const Vector2& rhs) const;
        bool operator<=(const Vector2& rhs) const;
        bool operator>=(const Vector2& rhs) const;
        bool operator<(const Vector2& rhs) const;
        bool operator>(const Vector2& rhs) const;

        std::string ToString() const;
    };

    class Vector3
    {
    public:
        double x, y, z;

        Vector3();
        Vector3(double x, double y, double z);
        Vector3(const Vector3& v);

        static Vector3 right();
        static Vector3 left();
        static Vector3 up();
        static Vector3 down();
        static Vector3 forward();
        static Vector3 back();
        static Vector3 zero();
        static Vector3 one();

        double magnitude() const;
        
        Vector3 normalized() const;
        Vector3& normalize();

        Vector3& operator+=(const Vector3& rhs);
        Vector3& operator-=(const Vector3& rhs);
        Vector3& operator*=(const double& rhs);
        Vector3& operator/=(const double& rhs);

        Vector3 operator+(const Vector3& rhs) const;
        Vector3 operator-(const Vector3& rhs) const;
        Vector3 operator*(const double& rhs) const;
        Vector3 operator/(const double& rhs) const;

        bool operator==(const Vector3& rhs) const;
        bool operator!=(const Vector3& rhs) const;
        bool operator<=(const Vector3& rhs) const;
        bool operator>=(const Vector3& rhs) const;
        bool operator<(const Vector3& rhs) const;
        bool operator>(const Vector3& rhs) const;

        std::string ToString() const;
    };
}

#endif