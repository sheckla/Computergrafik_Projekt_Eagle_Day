#include "AIPlaneController.h"
#include "ModelLoader.h"

AIPlaneController::AIPlaneController()
{
	
}

void AIPlaneController::update(float delta)
{
	if (ModelLoader::pAIPlane) aiPlane = ModelLoader::pAIPlane;
	if (ModelLoader::pPlayerPlane) playerPlane = ModelLoader::pPlayerPlane;
	if (!aiPlane || !playerPlane)  return;
	aiPlane->Enemy_Speed = 455;

	// Common vectors
	// Calculate angle between Player & AI
	Vector playerPos, aiPos, playerPosNorm, aiPosNorm;
	playerPos = playerPlane->getParts()[0]->transform().translation().normalize();
	aiPos = aiPlane->transform().translation().normalize();

	Vector xzPlayer = Vector(playerPos.X, playerPos.Z, 0).normalize();
	Vector xzEnemy = Vector(aiPos.X, playerPos.Z, 0).normalize();

	float angle = acos(xzPlayer.dot(xzEnemy));
	Vector rotAxis = xzEnemy.cross(xzPlayer);
	rotAxis.normalize();

	Vector diff(0, 0, (aiPos - playerPos).length());
	Vector rotDiff = Camera::rotateAxisAngle(diff, rotAxis, angle);

	Vector cdir = aiPos - playerPos;
	cdir.normalize();
	Vector cup = ModelLoader::pPlayerPlane->getParts()[0]->transform().up().normalize();
	Vector cright = cdir.cross(cup);
	cright.normalize();
	cup = cright.cross(cdir);

	Vector rotDiffW;
	rotDiffW.X = cright.X * rotDiff.X + cup.X * rotDiff.Y + -cdir.X * rotDiff.Z;
	rotDiffW.Y = cright.Y * rotDiff.X + cup.Y * rotDiff.Y + -cdir.Y * rotDiff.Z;
	rotDiffW.Z = cright.Z * rotDiff.X + cup.Z * rotDiff.Y + -cdir.Z * rotDiff.Z;

	if (rotDiffW.Y != rotDiffW.Y) rotDiffW.Y = 0;
	if (rotDiffW.X != rotDiffW.X) rotDiffW.X = 0;
	if (rotDiffW.Z != rotDiffW.Z) rotDiffW.Z = 0;

	// Prepare transformations according to angleDiff
	Matrix yaw = Matrix().rotationY(-(rotDiffW.X) * delta / 3);
	Matrix pitch = Matrix().rotationX(-(rotDiffW.Y) * delta / 3);
	aiPlane->transform(aiPlane->transform() * yaw * pitch);

	// AIPlane dead -> drop it down
	if (aiPlane->hp <= 0) {
		Matrix fall = Matrix().translation(0, -10 * delta * deadCounter / 100,0);
		deadCounter++;
		aiPlane->transform(aiPlane->transform() * fall);
	} else
	{
		deadCounter = 0;
	}

	aiPlane->update(delta);
}
