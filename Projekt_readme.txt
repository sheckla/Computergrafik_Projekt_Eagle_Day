*#####################################################################################################################################################################*

## Hochschule Osnabrück - Computergrafik SoSe 2022 - Projektabgabe

*#####################################################################################################################################################################*

# Teilnehmer: 		- Daniel Graf & Jannis Welkener

# Spielkonzept: 	- Dogfighter im WW2 Stil

# Spielmechaniken:	- Flugzeug in der Spielwelt steuern und fliegen.
			- Mit den Gewehren am eigenen Flugzeugen ein KI-Flugzeug abschiessen (Oder Player2 im Mehrspielermodus, über Sockets).
			- Spieler sowie Gegner verfügen über eine HP-Wert.
			- Abschiessen des Gegners mit den eigenen Waffen um seine HP zu verringern damit er abstürzt.
			- Das eigene Flugzeug hat ein Gewehr, welches eine verfügbare Schießzeit hat, welche beim Schießen verringert wird.
			  Fällt diese Schießzeit auf 0 verliert der Spieler leben, da die Gewehre überhitzen und das Flugzeug Schaden nimmt.
			- Altitude < 0   -> Spieler ist im Wasser und stirbt.
			- Altitude > 300 -> verliert Leben da er die kritische Hoehe erreicht hat.

# Getestet unter:	- Windows 10
			- RTX 3070 Ti: GeForce Game Ready-Driver 512.15

*#####################################################################################################################################################################*
# UI Hilfe:		

# Startbildschirm:	- Optionen lassen mit dem Klicken Zahnrädchens im oberen linken Bildschirmbereich öffnen (ESCAPE_KEY zum schließen dieser Einstellungen).
			- Spiel lässt sich hier Beenden sowie auch in den Gameplay-Modus starten.

# Im Gameplay:		- Speed & Altitude 'Meter' welches die aktuelle Geschwindigkeit sowie Höhe wiedergibt.
			- Lebensanzeige sowie Waffenanzeige geben Auskunft über die aktuelle Flugzeugvitalität sowie Waffenauslastung.
			- Simpler Kompass der die Distanz zum gegnerischen Flugzeug wiedergibt.
			- EscapeMenu -> ESCAPE-KEY auf der Tastatur, bietet auch Möglichkeit zum Neustarten des aktuellen Spielstands.
			  Optionen lassen sich hier auch verstellen indem das Zahnrädchen oben links erneut angeklickt wird.

*#####################################################################################################################################################################*

# Steuerung:		- Tastaturmodus:	W - Flugzeug nach oben ausrichten
						S - Flugzeug nach unten ausrichten 
						A - Rolle (links)
						D - Rolle (rechts)
						Q - Flugzeug nach links steuern
						E - Flugzeug nach rechts steuern
			- Mausmodus:		Mit der Maus lässt sich das Flugzeug hier steuern. Dazu Maus auf die gewünsche Zielposition im Bildschirm platzieren
						und das Flugzeug wird entsprechende Lenkbewegungen einrichten (einstellbar in den Optionen)

			- Flugzeugmotor:	SHIFT - Flugzeug beschleunigen
						CTRL  - Flugzeug abremsen
					
			- Flugzeuggewehre:	SPACE - schießen (Gewehrzustand beachten! Regeneriert sich sobald nicht geschossen wird)

*#####################################################################################################################################################################*

# Online Modus:		- Festlegen der Lokalen IP/Port sowie die IP/Port des Gegners in der 'master/assets/settings.txt'.
			- Java-Server starten ('master/server/Server_Runtime.java')
			- Online Modus im Spielbetrieb anstellen (Optionen -> "Online Mode") -> falls nicht schon an, neustarten

*#####################################################################################################################################################################*
## Feinheiten im Code:	

Die Einteilung der Projekt-Codestruktur erfolgte in einer sinngemäßen Einteilung: (application, controls, graphics, gui, network, particles, sound, util)

*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*

- Plane.cpp: Wrapper Klasse welche die einzelnen Modelle des Spitfire-Flugzeuges beinhält und diese verwaltet.
  So lassen sich je nach Lenkeinschlag verschiedene 'Flaps' am Flugzeug bewegen und das Flugzeug verhält sich wie ein richtiges Flugzeug.

- PostProcessingBuffer.cpp: Buffereinheit welche die vorgerenderte Szene per Framebuffer erhält, diese im PostProcessingShader.cpp je nach Einstellung modifiziert 
  und dann letztlich im ScreenQuadModel rendert. 
  Das ScreenQuadModel ist eine simples Objekt welches sich über die normalisierten Bildschirdmkoordinaten erstreckt und die vorgerenderte Szene als Textur kriegt.

- Texture.cpp: Erlaubt nun das Laden von nativen SkyBox-Images und splittet diese dementsprechend in eine Cubemap, so das dieses genutzt werden kann.
  Dazu werden die einzelnen Faces von der Skybox gesplitted und einzeln gespeichert.
  Klassen wie der PhongShader nutzen diese CubeMap um Modelle von dieser CubeMap reflektieren zu lassen.

*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*

- Volumetric Clouds: siehe 'master/assets/shader/cloud/vsCloud.glsl' & 'master/assets/shader/cloud/fsCloud.glsl'
		     Inspiriert anhand von https://www.guerrilla-games.com/media/News/Files/The-Real-time-Volumetric-Cloudscapes-of-Horizon-Zero-Dawn.pdf
- Ocean: siehe 'master/assets/shader/ocean/vsOcean.glsl' & 'master/assets/shader/ocean/fsOcean.glsl'
	 Ocean wurde auf Basis des Terrains im Praktikum erstellt.
			
*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*

# GUI:			- GUI für die Anwendung wurde objektorientiert erstellt und liefert im aktuellen Umfang ein Framework zum einfachen erstellen von UI-Elementen.
			- Darunter auch der PressListener welche als Listener-Einheit Keyboard- oder Mouseinputs abhört um z. B. ein Mouseoverhighlight zu ermöglichen,
			  sowie auch zur realisierung von UI-Logik. Es laesst sich abhören, wann bestimmte Buttons/Slider/Switches angeklickt wurden.

* core components:	- GUIBaseComponent als Oberklasse wovon alle anderen Unterklassen sich ableiten
			- GUIConstantQuad als implementierte Oberklasse fuer UI-Elemente. (Skalierbar, verschiebbar, MouseoverHighlight, Farbmodus, Mousepress)

* font & text:		- FNTManager als Ladeeinheit für gegebene .fnt-Dateien sowie der entsprechenden Fontmap.
			- GUIChar: einzelnes Zeichen welches mit den Daten aus den .fnt-Dateien das entsprechende Zeichen mit TexCoord-Verschiebung erreicht.
			  (Wrapperklasse GUIText verwaltet dann eine Ansammlung von GUIChar's)

* components: 		- GUIButton, GUIButtonSwitch, GUISlider, GUILoadingMeter und GUINumericPointerMeter arbeiten mit diesen Core-Components um bekannte
			  UI-Elemente nachzubilden.

*#####################################################################################################################################################################*
[Graphics Sources]: 
ScreenQuad.cpp: https://learnopengl.com/Getting-started/Textures
PhongShader.glsl: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
PhongShader.glsl: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
PhongShader.glsl: https://learnopengl.com/Advanced-OpenGL/Cubemaps

[Image Sources]
cogwheel icon: https://www.iconfinder.com/icons/216602/cogwheel_icon
plane vector icon: https://www.kindpng.com/downpng/oRbiii_plane-clipart-outline-ww2-aircraft-icon-hd-png/
spitfire texture: https://pixers.de/
battleship texture: https://pixers.de/

[Audio Sources]
Plane Engine Sounds & Gun Shooting Sounds: https://www.soundjay.com
Audio Sound Engine: IrrKlang Library

[Font Sources]
.fnt & fontmap Generation: Hiero Font Generator
WW2 Font : https://www.dafont.com/stencil-ww-ii.font
Typewriter Font: https://www.dafont.com/jmh-typewriter.font

Alles andere Bilder, Modelle & Sounddateien wurde vom initialen Praktikumsprojekt übernommen oder wurde von selber erstellt.

*#####################################################################################################################################################################*