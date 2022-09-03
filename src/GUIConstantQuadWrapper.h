/*
 * Simple Wrapper Klasse welche mehrere GUIConstantQuads und deren ableitung als vector<> speichert
 */
#ifndef GUIConstantQuadWrapper_h
#define GUIConstantQuadWrapper_h

#include <vector>
#include "GUIConstantQuad.h"

class GUIConstantQuadWrapper : public GUIBaseComponent
{
	std::vector<GUIConstantQuad*> Components;
public:
	GUIConstantQuadWrapper();
	~GUIConstantQuadWrapper() override;

	void draw() override;
	void update(float delta);

	void add(GUIConstantQuad* component);
	std::vector<GUIConstantQuad*> components();

	// Quads Component von vector extrahieren
	static std::vector<GUIConstantQuad*> extractQuadsFromComponentList(std::vector<GUIBaseComponent*> list);

	// Quad Components von Wrapper extrahieren
	static std::vector<GUIConstantQuad*> extractQuadsFromComponent(GUIBaseComponent* component);
};

#endif
