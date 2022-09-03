#include "AIPlaneController.h"

#include "MathUtil.h"
#include "ModelLoader.h"

AIPlaneController::AIPlaneController()
{
}

void AIPlaneController::update(float delta)
{
	if (ModelLoader::pAIPlane) aiPlane = ModelLoader::pAIPlane;
	if (ModelLoader::pPlayerPlane) playerPlane = ModelLoader::pPlayerPlane;
	if (!aiPlane || !playerPlane)  return;
	aiPlane->update(delta);


	// Common vectors
	Vector playerPos, aiPos, playerPosNorm, aiPosNorm;
	playerPos = playerPlane->getParts()[0]->transform().translation();
	aiPos = aiPlane->getParts()[0]->transform().translation();
	playerPosNorm = playerPlane->getParts()[0]->transform().translation().normalize();
	aiPosNorm = aiPlane->getParts()[0]->transform().translation().normalize();
	Vector aiPlayerCross = aiPosNorm.cross(playerPosNorm);

	// Speed
	float AISpeedVal = playerPlane->speedPercentage();

	aiPlane->accelerate(delta * AISpeedVal);
	aiPlane->update(delta);
}
