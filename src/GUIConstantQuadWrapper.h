#ifndef GUIConstantQuadWrapper_h
#define GUIConstantQuadWrapper_h
#include <vector>
#include "GUIConstantQuad.h"

/*
 * Simple Wrapper Klasse welche mehrere GUIConstantQuads und deren ableitung als vector<> speichert
 */
class GUIConstantQuadWrapper : public GUIBaseComponent
{
	std::vector<GUIConstantQuad*> Components;
public:
	GUIConstantQuadWrapper();
	~GUIConstantQuadWrapper() override;
	void draw() override;

	void add(GUIConstantQuad* component);

	std::vector<GUIConstantQuad*> components();

	static std::vector<GUIConstantQuad*> extractQuadsFromComponentList(std::vector<GUIBaseComponent*> list);
	static std::vector<GUIConstantQuad*> extractQuadsFromComponent(GUIBaseComponent* component);
};

#endif
