#ifndef AudioManager_HPP
#define AudioManager_HPP

#include <iostream>

class AudioManager
{
public:
	AudioManager::AudioManager();
	AudioManager::~AudioManager() {}

	void AudioManager::PlayAsync(const wchar_t* file);

private:
	void AudioManager::PlaySoundAsyncThread(const wchar_t* file);
};
#endif /* AudioManager_hpp */