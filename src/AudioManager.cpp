#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment( lib, "Winmm.lib" )

#include "AudioManager.h"
#include <thread>

AudioManager::AudioManager() 
{
	std::cout << "[AUDIO] active" << std::endl;
}

void AudioManager::PlayAsync(const wchar_t* file)
{	
	std::thread t1(&AudioManager::PlaySoundAsyncThread, this, file);
	t1.detach();
}

void AudioManager::PlaySoundAsyncThread(const wchar_t* file)
{
	std::cout << "[AUDIO] playing in thread..." << std::endl;

	PlaySound(file, 0, SND_FILENAME);
}