#ifndef CloudShader_hpp
#define CloudShader_hpp

#include <stdio.h>
#include <assert.h>
#include "PhongShader.h"

class CloudShader : public PhongShader
{
public:
	virtual void activate(const BaseCamera& Cam) const;
	virtual void deactivate() const;

	CloudShader(const std::string& AssetDirectory);

	virtual ~CloudShader() { std::cout << "DELETED!" << std::endl; }

	void SetWorleyTexture(const Texture* pTex) { WorleyTexture = pTex; }

	const Texture* worleyTexture() const { return WorleyTexture; }

private:

	GLint WorleyLoc;
	const Texture* WorleyTexture;
};

#endif
