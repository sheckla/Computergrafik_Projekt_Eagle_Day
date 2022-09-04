#include "CollisionDetector.h"

CollisionDetector* CollisionDetector::pCollisionDetector = nullptr;
EnemyPlane* CollisionDetector::enemyPlane = nullptr;
std::list<const AABB*> CollisionDetector::Hitboxes = {};

