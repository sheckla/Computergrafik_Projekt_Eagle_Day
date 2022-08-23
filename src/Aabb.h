/* 
 * Axis Aligned Bounding Box
 * - Huellkoerper der komplexe oder simple 3-D Modelle einschließt
 *
 * Weiterentwickelt auf Basis vom Praktikum:
 *      Created by Philipp Lensing on 16.09.16.
 *      Copyright © 2016 Philipp Lensing. All rights reserved.
 */

#ifndef Aabb_h
#define Aabb_h

#include "vector.h"
#include "matrix.h"

class AABB
{
public:
    Vector Min;
    Vector Max;
    AABB();
    AABB(const Vector& min, const Vector& max);
    AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
    Vector size() const;
    AABB transform(const Matrix& m) const;
    AABB merge(const AABB& a, const AABB& b) const;
    AABB& merge(const AABB& a);
    Vector center() const;
    void corners(Vector c[8]) const;
    void fromPoints(const Vector* Points, unsigned int PointCount);
    static const AABB& unitBox();
protected:
    static AABB UnitBox;
};



#endif 
