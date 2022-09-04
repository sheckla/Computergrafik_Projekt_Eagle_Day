#ifndef CollisionDetector_hpp
#define CollisionDetector_hpp

#include <stdio.h>
#include "vector.h"
#include "matrix.h"
#include "EnemyPlane.h"
#include "Aabb.h"
#include "ModelLoader.h"

class CollisionDetector
{
public:
    static CollisionDetector* pCollisionDetector;
    static CollisionDetector& instance()
    {
        if (!pCollisionDetector) pCollisionDetector = new CollisionDetector();
        return *pCollisionDetector;
    }

    static bool CollisionDetector::BulletCollision(Vector v) 
    {
            AABB aabb;

            if (ModelLoader::pEnemyPlane) {
                aabb.Max = ModelLoader::pEnemyPlane->aabb().Max;
                aabb.Min = ModelLoader::pEnemyPlane->aabb().Min;
            }
            if (ModelLoader::pAIPlane) {
                aabb.Max = ModelLoader::pAIPlane->aabb().Max;
                aabb.Min = ModelLoader::pAIPlane->aabb().Min;
            }
            if ((v.X > aabb.Min.X &&
                 v.Y > aabb.Min.Y &&
                 v.Z > aabb.Min.Z)
                &&
                (v.X < aabb.Max.X &&
                 v.Y < aabb.Max.Y &&
                 v.Z < aabb.Max.Z))   
            {
                if (ModelLoader::pEnemyPlane) ModelLoader::pEnemyPlane->hit();
                if (ModelLoader::pAIPlane) ModelLoader::pAIPlane->hit();
                return true;
            }
        return false;
    }

    static void CollisionDetector::setCollisionTarget(EnemyPlane* enemy)
    {
        instance().enemyPlane = enemy;
    }

    static bool CollisionDetector::CheckPlaneCollision(AABB aabb) 
    {
        //Hitbox loop
        for (std::list<const AABB*>::iterator it = instance().Hitboxes.begin(); it != instance().Hitboxes.end(); ++it) 
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

        if (aabb.Min.Y < -3.0f) {
                std::cout << "Collision" << std::endl;
            return true;
        }

        return false;
    }

    static void CollisionDetector::AddHitbox(const AABB* hitbox)
    {
        instance().Hitboxes.push_back(hitbox);
    }

protected:
    static EnemyPlane* CollisionDetector::enemyPlane;
    static std::list<const AABB*> Hitboxes;
};


#endif /* CollisionDetector_hpp */
