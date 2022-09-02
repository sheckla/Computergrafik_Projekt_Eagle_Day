#ifndef CollisionDetector_hpp
#define CollisionDetector_hpp

#include <stdio.h>
#include "vector.h"
#include "matrix.h"
#include "EnemyPlane.h"
#include "Aabb.h"

class CollisionDetector
{
public:
    
    static bool CollisionDetector::BulletCollision(Vector v) 
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

    static bool CollisionDetector::CheckPlaneCollision(AABB aabb) 
    {
        //Hitbox loop
        for (std::list<AABB*>::iterator it = CollisionDetector::Hitboxes.begin(); it != CollisionDetector::Hitboxes.end(); ++it) 
        {
            if (
                aabb.Min.X <= (*it)->Max.X &&
                aabb.Max.X >= (*it)->Min.X &&
                aabb.Min.Y <= (*it)->Max.Y &&
                aabb.Max.Y >= (*it)->Min.Y &&
                aabb.Min.Z <= (*it)->Max.Z &&
                aabb.Max.Z >= (*it)->Min.Z
                ) {
                std::cout << "Collision" << std::endl;
                return true;
            }
        }

        if (aabb.Min.Y < 1.3f) {
            return true;
        }

        return false;
    }

    static void CollisionDetector::AddHitbox(AABB* hitbox)
    {
        CollisionDetector::Hitboxes.push_back(hitbox);
    }

protected:
    static EnemyPlane* CollisionDetector::enemyPlane;
    static std::list<AABB*> Hitboxes;
};


#endif /* CollisionDetector_hpp */
