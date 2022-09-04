#include "CollisionDetector.h"

EnemyPlane* CollisionDetector::enemyPlane = nullptr;
std::list<const AABB*> CollisionDetector::Hitboxes = {};

