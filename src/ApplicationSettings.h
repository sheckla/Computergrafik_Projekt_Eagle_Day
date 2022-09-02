/*
 * ApplicationSettings
 * - statische Instanz zum speichern zur Laufzeit variablen Programmeinstellungen (Sound, Grafik etc.)
 */

#ifndef ApplicationSettings_h
#define ApplicationSettings_h
#include <fstream>

class ApplicationSettings
{
public:
	static ApplicationSettings* pApplicationSettings;
	static float WIDTH;
	static float HEIGHT;
	static bool SHUT_DOWN;

	// ASSETS settings.txt
	static bool FULL_SCREEN;
	static bool MOUSE_CONTROLS;
	static float AUDIO_VALUE;
	static bool SEPIA_POST_PROCESSING;
	static bool ONLINE_MODE;
	static std::string LOCAL_IP;
	static std::string LOCAL_PORT;
	static std::string ENEMY_IP;
	static std::string ENEMY_PORT;

	static ApplicationSettings& instance();
	static void readSettings();
	static void writeSettings();
};

#endif