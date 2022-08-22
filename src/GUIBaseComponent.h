#ifndef GUIBaseComponent_h
#define GUIBaseComponent_h

/*
 * Basis UI-Komponente
 * - Abgeleitete Klassen sind konkrete UI-Elemente wie Geschwindigkeitsanzeigen, Buttons, Bilder etc.
 */
class GUIBaseComponent
{
public:
	virtual ~GUIBaseComponent();
	virtual void draw();
};

#endif

