#include "ApplicationSettings.h"

#include <fstream>
#include <sstream>
#include <string>

#include "Globals.h"

ApplicationSettings* ApplicationSettings::pApplicationSettings = nullptr;
float ApplicationSettings::HEIGHT = 1080;
float ApplicationSettings::WIDTH = 1920;
bool ApplicationSettings::SHUT_DOWN = false;

// ASSETS settings.txt
bool ApplicationSettings::FULL_SCREEN;
bool ApplicationSettings::MOUSE_CONTROLS;
float ApplicationSettings::AUDIO_VALUE;
bool ApplicationSettings::SEPIA_POST_PROCESSING;
bool ApplicationSettings::ONLINE_MODE;
std::string ApplicationSettings::LOCAL_IP;
std::string ApplicationSettings::LOCAL_PORT;
std::string ApplicationSettings::ENEMY_IP;
std::string ApplicationSettings::ENEMY_PORT;

std::string readString(std::string line)
{
	return line.substr(line.find_first_of('=')+1, line.length());
}

bool readBool(std::string line)
{
	line = readString(line);
	bool b = false;
	(line[0] == '0') ? b = false : b = true;
	return b;
}

float readFloat(std::string line)
{
	line = readString(line);
	return std::atof(line.c_str());

}

template<typename T>
std::string stringifyVal(std::string name, T val)
{
	std::stringstream ss;
	ss << name << "=" << val << "\n";
	return ss.str();
}

ApplicationSettings& ApplicationSettings::instance()
{
	if (!pApplicationSettings) pApplicationSettings = new ApplicationSettings();
	return *pApplicationSettings;
}

void ApplicationSettings::readSettings()
{
	std::ifstream file;
	file.open(ASSETS "settings.txt");
	std::string line;

	std::getline(file, line);
	FULL_SCREEN = readBool(line);

	std::getline(file, line);
	MOUSE_CONTROLS = readBool(line);

	std::getline(file, line);
	AUDIO_VALUE = readFloat(line);

	std::getline(file, line);
	SEPIA_POST_PROCESSING = readBool(line);

	std::getline(file, line);
	ONLINE_MODE = readBool(line);

	std::getline(file, line);
	LOCAL_IP = readString(line);

	std::getline(file, line);
	LOCAL_PORT = readString(line);

	std::getline(file, line);
	ENEMY_IP = readString(line);

	std::getline(file, line);
	ENEMY_PORT = readString(line);

	print("FULLSCREEN", FULL_SCREEN);
	print("MOUSECONTROLS", MOUSE_CONTROLS);
	print("AUDIOVALUE", AUDIO_VALUE);
	print("SEPIAPOSTPROCESSING", SEPIA_POST_PROCESSING);
	print("ONLINEMODE", ONLINE_MODE);
	print("LOCALIP", LOCAL_IP+"|");
	print("LOCALPORT", LOCAL_PORT + "|");
	print("ENEMYIP", ENEMY_IP + "|");
	print("ENEMYPORT", ENEMY_PORT + "|");

	file.close();

	if (LOCAL_IP.size() < 1) LOCAL_IP = "127.0.0.1";
	if (LOCAL_PORT.size() < 1) LOCAL_PORT = "14911";
	if (ENEMY_IP.size() < 1) ENEMY_IP = "127.0.0.1";
	if (ENEMY_PORT.size() < 1) ENEMY_PORT = "19413";
}

void ApplicationSettings::writeSettings()
{
	std::ofstream file;
	file.open(ASSETS "settings.txt");

	file << stringifyVal("FULL_SCREEN", FULL_SCREEN);
	file << stringifyVal("MOUSE_CONTROLS", MOUSE_CONTROLS);
	file << stringifyVal("AUDIO_VALUE", AUDIO_VALUE);
	file << stringifyVal("SEPIA_POST_PROCESSING", SEPIA_POST_PROCESSING);
	file << stringifyVal("ONLINE_MODE", ONLINE_MODE);
	file << stringifyVal("LOCAL_IP", LOCAL_IP);
	file << stringifyVal("LOCAL_PORT", LOCAL_PORT);
	file << stringifyVal("ENEMY_IP", ENEMY_IP);
	file << stringifyVal("ENEMY_PORT", ENEMY_PORT);
	file.close();
}