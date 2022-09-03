#include "GUIConstantQuadWrapper.h"

GUIConstantQuadWrapper::GUIConstantQuadWrapper()
{
}

GUIConstantQuadWrapper::~GUIConstantQuadWrapper()
{
	for (auto component : Components)
	{
		delete component;
	}
}

void GUIConstantQuadWrapper::draw()
{
	bool wrapperHit = false;
	for (auto component : Components)
	{
		if (component->mouseInside()) wrapperHit = true;
	}

	for (auto component : Components)
	{
		if (wrapperHit) component->forceMouseoverHighlight(true);
		component->draw();
		if (wrapperHit) component->forceMouseoverHighlight(false);
	}
}

void GUIConstantQuadWrapper::update(float delta)
{
	for (auto component : Components) component->update(delta);
}

void GUIConstantQuadWrapper::add(GUIConstantQuad* component)
{
	Components.push_back(component);
}

std::vector<GUIConstantQuad*> GUIConstantQuadWrapper::components()
{
	return Components;
}

std::vector<GUIConstantQuad*> GUIConstantQuadWrapper::extractQuadsFromComponentList(std::vector<GUIBaseComponent*> list)
{
	std::vector<GUIConstantQuad*> extractedQuads;
	for (GUIBaseComponent* component : list) 
		for (GUIConstantQuad* quad : extractQuadsFromComponent(component)) extractedQuads.push_back(quad);
	return extractedQuads;
}

std::vector<GUIConstantQuad*> GUIConstantQuadWrapper::extractQuadsFromComponent(GUIBaseComponent* component)
{
	std::vector<GUIConstantQuad*> extractedQuads;

	// GUIBaseComponent = GUIConstantQuad
	GUIConstantQuad* quad = dynamic_cast<GUIConstantQuad*>(component);
	if (quad) extractedQuads.push_back(quad);

	// GUIBaseComponent = GUIConstantQuadWrapper
	GUIConstantQuadWrapper* wrapper = dynamic_cast<GUIConstantQuadWrapper*>(component);
	if (wrapper)
	{
		for (GUIConstantQuad* wrapperQuad : wrapper->components()) extractedQuads.push_back(wrapperQuad);
	}
	return extractedQuads;
}
