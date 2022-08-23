/*
 * Basis UI-Komponente
 * - Abgeleitete Klassen sind konkrete UI-Elemente wie Geschwindigkeitsanzeigen, Buttons, Bilder etc.
 */

#ifndef GUIBaseComponent_h
#define GUIBaseComponent_h

class GUIBaseComponent
{
public:
	virtual ~GUIBaseComponent();
	virtual void draw();
};

#endif

