#ifndef CloudShader_hpp
#define CloudShader_hpp

#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "PhongShader.h"
#include "Matrix.h"
#include "Aabb.h"

class CloudShader : public PhongShader
{
public:
	virtual void activate(const BaseCamera& Cam) const;
	virtual void deactivate() const;

	static float TimeTranslation;

	CloudShader();

	virtual ~CloudShader();

	void detailTex(unsigned int dtk, const Texture* tex);
	void activateTex(const Texture* pTex, GLint Loc, int slot) const;
	void aabb(AABB* bounds);
	void noise(const Texture* n);

private:
	// Views
	GLint ViewMatrixLoc;
	GLint ProjectionMatrixLoc;

	// Tex
	GLint DetailTexLoc[WORLEY_AMOUNT];
	GLint NoiseLoc;

	GLint boundsMinLoc;
	GLint boundsMaxLoc;

	GLint TimeTranslationLoc;

	AABB* bounds;

	const Texture* DetailTex[WORLEY_AMOUNT];
	const Texture* Noise;
};

#endif
