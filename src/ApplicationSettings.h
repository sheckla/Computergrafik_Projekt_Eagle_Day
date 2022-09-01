/*
 * ApplicationSettings
 * - statische Instanz zum speichern zur Laufzeit variablen Programmeinstellungen (Sound, Grafik etc.)
 */

#ifndef ApplicationSettings_h
#define ApplicationSettings_h

class ApplicationSettings
{
public:
	static ApplicationSettings* pApplicationSettings;
	static float AUDIO_VALUE;
	static bool MOUSE_CONTROLS;

	static ApplicationSettings& instance();
};

#endif