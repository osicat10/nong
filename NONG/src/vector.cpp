#include "NONG/vector.h"

#include <cmath>

namespace NONG {
#pragma region Vector2
    Vector2::Vector2() : x(0), y(0) { }
    Vector2::Vector2(double x, double y) : x(x), y(y) { }
    Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y) { }

    Vector2 Vector2::right()    {return Vector2(1, 0);}
    Vector2 Vector2::left()     {return Vector2(-1, 0);}
    Vector2 Vector2::up()       {return Vector2(0, 1);}
    Vector2 Vector2::down()     {return Vector2(0, -1);}
    Vector2 Vector2::zero()     {return Vector2(0, 0);}
    Vector2 Vector2::one()      {return Vector2(1, 1);}

    double Vector2::magnitude() const {return std::sqrt(x*x + y*y);}

    Vector2 Vector2::normalized() const
    {
        double m = magnitude();
        if (m == 0) return Vector2(0, 0); 
        return Vector2(x / m, y / m);
    }

    Vector2& Vector2::normalize() 
    {
        double m = magnitude();
        if (m != 0)
        {
            x /= m;
            y /= m;
        }
        return *this;
    }

    Vector2& Vector2::operator+=(const Vector2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    
    Vector2& Vector2::operator-=(const Vector2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    Vector2& Vector2::operator*=(const double& rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    
    Vector2& Vector2::operator/=(const double& rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }


    
    Vector2 Vector2::operator+(const Vector2& rhs) const
    {
       return Vector2(x + rhs.x, y + rhs.y);
    }
    
    Vector2 Vector2::operator-(const Vector2& rhs) const
    {
       return Vector2(x - rhs.x, y - rhs.y);
    }
    
    Vector2 Vector2::operator*(const double& rhs) const
    {
       return Vector2(x * rhs, y * rhs);
    }
    
    Vector2 Vector2::operator/(const double& rhs) const
    {
       return Vector2(x / rhs, y / rhs);
    }


    bool Vector2::operator==(const Vector2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool Vector2::operator!=(const Vector2& rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }
    
    bool Vector2::operator<(const Vector2& rhs) const
    {
        return x < rhs.x || (x == rhs.x && y < rhs.y);
    }

    bool Vector2::operator<=(const Vector2& rhs) const
    {
        return *this < rhs || *this == rhs;
    }

    bool Vector2::operator>(const Vector2& rhs) const
    {
        return rhs < *this;
    }

    bool Vector2::operator>=(const Vector2& rhs) const
    {
        return rhs <= *this;
    }

    std::string Vector2::ToString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
#pragma endregion

#pragma region Vector3
    Vector3::Vector3() : x(0), y(0), z(0) { }
    Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) { }
    Vector3::Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) { }

    Vector3 Vector3::right()    { return Vector3(1, 0, 0); }
    Vector3 Vector3::left()     { return Vector3(-1, 0, 0); }
    Vector3 Vector3::up()       { return Vector3(0, 1, 0); }
    Vector3 Vector3::down()     { return Vector3(0, -1, 0); }
    Vector3 Vector3::forward()  { return Vector3(0, 0, 1); }
    Vector3 Vector3::back()     { return Vector3(0, 0, -1); }
    Vector3 Vector3::zero()     { return Vector3(0, 0, 0); }
    Vector3 Vector3::one()      { return Vector3(1, 1, 1); }

    double Vector3::magnitude() const { return std::sqrt(x*x + y*y + z*z); }

    Vector3 Vector3::normalized() const 
    {
        double m = magnitude();
        if (m == 0) return Vector3(0, 0, 0);
        return Vector3(x / m, y / m, z / m);
    }

    Vector3& Vector3::normalize() 
    {
        double m = magnitude();
        if (m != 0) {
            x /= m;
            y /= m;
            z /= m;
        }
        return *this;
    }

    Vector3& Vector3::operator+=(const Vector3& rhs)
    {
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }
    
    Vector3& Vector3::operator-=(const Vector3& rhs)
    {
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
        return *this;
    }
    
    Vector3& Vector3::operator*=(const double& rhs)
    {
        x *= rhs; y *= rhs; z *= rhs;
        return *this;
    }
    
    Vector3& Vector3::operator/=(const double& rhs)
    {
        x /= rhs; y /= rhs; z /= rhs;
        return *this;
    }

    Vector3 Vector3::operator+(const Vector3& rhs) const
    {
       return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    
    Vector3 Vector3::operator-(const Vector3& rhs) const
    {
       return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    
    Vector3 Vector3::operator*(const double& rhs) const
    {
       return Vector3(x * rhs, y * rhs, z * rhs);
    }
    
    Vector3 Vector3::operator/(const double& rhs) const
    {
       return Vector3(x / rhs, y / rhs, z / rhs);
    }

    bool Vector3::operator==(const Vector3& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
    
    bool Vector3::operator!=(const Vector3& rhs) const
    {
        return !(*this == rhs);
    }
    
    bool Vector3::operator<(const Vector3& rhs) const
    {
        if (x != rhs.x) return x < rhs.x;
        if (y != rhs.y) return y < rhs.y;
        return z < rhs.z;
    }

    bool Vector3::operator<=(const Vector3& rhs) const
    {
        return *this < rhs || *this == rhs;
    }

    bool Vector3::operator>(const Vector3& rhs) const
    {
        return rhs < *this;
    }

    bool Vector3::operator>=(const Vector3& rhs) const
    {
        return rhs <= *this;
    }

    std::string Vector3::ToString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }
#pragma endregion
}