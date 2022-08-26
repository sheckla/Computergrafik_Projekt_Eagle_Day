#include "ApplicationSettings.h"

ApplicationSettings* ApplicationSettings::pApplicationSettings = nullptr;
float ApplicationSettings::AUDIO_VALUE = 0.0;

ApplicationSettings& ApplicationSettings::instance()
{
	if (!pApplicationSettings) pApplicationSettings = new ApplicationSettings();
	return *pApplicationSettings;
}
