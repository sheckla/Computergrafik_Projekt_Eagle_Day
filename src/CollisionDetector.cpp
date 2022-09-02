#include "CollisionDetector.h"

EnemyPlane* CollisionDetector::enemyPlane = nullptr;
std::list<AABB*> CollisionDetector::Hitboxes = {};

