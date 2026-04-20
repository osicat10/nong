#ifndef NONG_TRANSFORM_H
#define NONG_TRANSFORM_H

#include "NONG/component.h"
#include "NONG/vector.h"

namespace NONG {
    class Transform : public Component
    {
        DECLARE_COMPONENT(Transform)
    public:
        Transform() { }
        Transform(double x, double y, double z) : position(Vector3(x, y, z)) { }
        Transform(const Vector3& position) : position(position) { }
        Transform(const Vector3& position, const Vector3& rotation) : position(position), rotation(rotation) { }
        Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale) : position(position), rotation(rotation), scale(scale) { }

        Vector3 position = Vector3::zero();
        Vector3 rotation = Vector3::zero();
        Vector3 scale = Vector3::one();
    };
}

#endif