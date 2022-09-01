#include "ApplicationSettings.h"

ApplicationSettings* ApplicationSettings::pApplicationSettings = nullptr;
float ApplicationSettings::AUDIO_VALUE = 0.2;
bool ApplicationSettings::MOUSE_CONTROLS = false;

ApplicationSettings& ApplicationSettings::instance()
{
	if (!pApplicationSettings) pApplicationSettings = new ApplicationSettings();
	return *pApplicationSettings;
}
