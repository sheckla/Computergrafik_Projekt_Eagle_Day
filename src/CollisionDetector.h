#ifndef CollisionDetector_hpp
#define CollisionDetector_hpp

#include <stdio.h>
#include "vector.h"
#include "matrix.h"
#include "EnemyPlane.h"


class CollisionDetector
{
public:
    
    static bool CollisionDetector::IsColliding(Vector v) 
    {
        if (CollisionDetector::enemyPlane != nullptr) 
        { 
            AABB aabb = CollisionDetector::enemyPlane->boundingBox();

            if ((v.X > aabb.Min.X &&
                 v.Y > aabb.Min.Y &&
                 v.Z > aabb.Min.Z)
                &&
                (v.X < aabb.Max.X &&
                 v.Y < aabb.Max.Y &&
                 v.Z < aabb.Max.Z))   
            {
                CollisionDetector::enemyPlane->hit();
                return true;
            }
        }
        return false;
    }

    static void CollisionDetector::setCollisionTarget(EnemyPlane* enemy)
    {
        CollisionDetector::enemyPlane = enemy;
    }

protected:
    static EnemyPlane* CollisionDetector::enemyPlane;
};



#endif /* CollisionDetector_hpp */
