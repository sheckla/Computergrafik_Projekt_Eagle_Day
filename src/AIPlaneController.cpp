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

	float playerD = playerPlane->getParts()[0]->transform().translation().length();
	float aiD = aiPlane->getParts()[0]->transform().translation().length();
	float dist = playerD - aiD;


	float speedVal = 0;
	if (dist > 1) speedVal = 1;
	if (dist < 1) speedVal = -0.5;

	Vector playerPos, aiPos, playerPosNorm, aiPosNorm;
	playerPos = playerPlane->getParts()[0]->transform().translation();
	aiPos = aiPlane->getParts()[0]->transform().translation();
	playerPosNorm = playerPos.normalize();
	aiPosNorm = aiPos.normalize();

	//print("playerPos", playerPos);
	//print("aiPos", aiPos);

	float cos = playerPosNorm.dot(aiPosNorm);
	if (aiPosNorm.length() == 0 || playerPosNorm.length() == 0) cos = 1;
	if (cos != cos) cos = 1; // check for NaN
	cos = 1 - cos;
	float pureCos = cos;

	if (playerPos.Y < aiPos.Y) {
		cos = -cos;
	} else if (abs(playerPos.Y - aiPos.Y) < 2)
	{
		//cos = 0;
	}

	print("cos", cos);
	aiPlane->accelerate(delta * speedVal);
	aiPlane->tiltRudder(delta * pureCos);
	aiPlane->tiltLeftWingflaps(delta * cos * 3300);
	aiPlane->tiltRightWingflaps(delta * cos * 3300);
	aiPlane->update(delta);
}
