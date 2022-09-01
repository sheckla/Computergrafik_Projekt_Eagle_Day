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



	// Common vectors
	Vector playerPos, aiPos, playerPosNorm, aiPosNorm;
	playerPos = playerPlane->getParts()[0]->transform().translation();
	aiPos = aiPlane->getParts()[0]->transform().translation();
	playerPosNorm = playerPlane->getParts()[0]->transform().translation().normalize();
	aiPosNorm = aiPlane->getParts()[0]->transform().translation().normalize();
	Vector aiPlayerCross = aiPosNorm.cross(playerPosNorm);

	// Speed
	float AISpeedVal = 0.5;

	// Wingflap Val
	float AIWingflapVal = 0;
	Vector yzPlayer = Vector(0, playerPosNorm.Y, playerPosNorm.Z);
	Vector yzAi = Vector(0, aiPosNorm.Y, aiPosNorm.Z);
	float yzCos = acos(yzPlayer.dot(yzAi));
	if (yzCos != yzCos) yzCos = 0;
	float yzCross = Vector(1, 0, 0).dot(aiPlayerCross);
	if (yzCross != yzCross) yzCross = 0; // NaN
	if (yzCross > 0) yzCos = -yzCos;

	float flapDiff = playerPlane->totalRudderRotation - aiPlane->totalRudderRotation;
	float flapDiffFactor = MathUtil::remapBounds(abs(yzCross), 0, 0.005, 1, 30);
	flapDiffFactor = 12 - flapDiffFactor;
	flapDiffFactor = MathUtil::clamp(flapDiffFactor, 1, 100);

	//aiPlane->tiltLeftWingflaps(delta * yzCos * 10);
	//aiPlane->tiltRightWingflaps(delta * yzCos * 10);

	/*print("leftDiff", leftWingDiff);
	print("rightWingDiff", rightWingDiff);*/

	/*float leftWingDiff = playerPlane->totalLeftWingflapRotation - aiPlane->totalLeftWingflapRotation;
	float rightWingDiff = playerPlane->totalRightWingflapRotation - aiPlane->totalRightWingflapRotation;
	if (leftWingDiff == rightWingDiff)
	{
		aiPlane->tiltLeftWingflaps(delta * leftWingDiff * flapDiffFactor);
		aiPlane->tiltRightWingflaps(delta * rightWingDiff * flapDiffFactor);
	}
	aiPlane->tiltLeftWingflaps(delta * yzCos);
	aiPlane->tiltRightWingflaps(delta * yzCos);*/


	// Rudder Val
	Vector xzAi = Vector(aiPos.X, 0, aiPos.Z);
	Vector xzPlayer = Vector(playerPos.X, 0, playerPos.Z);
	double xzCos = acos(xzPlayer.dot(xzAi) / (xzPlayer.length() * xzAi.length()));
	if (xzCos != xzCos) xzCos = 0; // NaN
	float xzCross = Vector(0, 1, 0).dot(aiPlayerCross);
	if (xzCross != xzCross) xzCross = 0; // NaN
	if (xzCross < 0) xzCos = -xzCos;
	float xyDiff = xzPlayer.length() - xzAi.length();
	if (xyDiff != xyDiff) xyDiff = 0;

	float counterSteer = 0;
	float rudderDiffFactor = MathUtil::remapBounds(abs(xzCross), 0, 0.005, 0, 30);
	rudderDiffFactor = 12 - rudderDiffFactor;
	rudderDiffFactor = MathUtil::clamp(rudderDiffFactor, 0, 100);
	if (xzCos < 0) rudderDiffFactor = -rudderDiffFactor;
	counterSteer = delta * rudderDiffFactor / 20;

	if ((xzCross > 0 && prevXZCross < 0))
	{
		print("switch", "");
		stop = true;
	} 

	prevXZCross = xzCross;

	// -xzCos link->rechts
	// xzCos rechts->links
	// Ziel: rudderDiff == 0 && yCross == 0

	if (stop)
	{
		//aiPlane->tiltRudder(counterSteer);
		//print("xz", xzCross);
		if (xzCross > max)
		{
			max = xzCross;
			//print("new max", max);
		}

	}
	//float rudderDiff = playerPlane->totalRudderRotation - aiPlane->totalRudderRotation;
	//float rudderDiffFactor = MathUtil::remapBounds(abs(xzCross), 0, 0.005, 1, 30);
	//rudderDiffFactor = 12 - rudderDiffFactor;
	//rudderDiffFactor = MathUtil::clamp(rudderDiffFactor, 1, 100);
	//aiPlane->tiltRudder(delta * rudderDiffFactor*5 * -rudderDiff);
	if (abs(xzCross) < 0.01)
	{
		(xzCross < 0) ? aiPlane->tiltRudder(delta * xzCos * 40) : aiPlane->tiltRudder(delta * -xzCos * 40);
	} else
	{
		float rudderSteer = delta * -xzCos * 40;
		aiPlane->tiltRudder(rudderSteer);
	}


	aiPlane->accelerate(delta * AISpeedVal);
	aiPlane->update(delta);
}
