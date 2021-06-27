#include "audio.h"

std::map<std::string, Audio*> Audio::sLoadedAudios;

HCHANNEL Audio::play(){
    BASS_ChannelPlay( channel, true );
	return channel;
}

Audio::Audio(){std::map<std::string, Audio*> sLoadedAudios; sample = 0;}

Audio::~Audio(){BASS_SampleFree(sample);}

bool Audio::load(const char* filename )
{
	sample = BASS_SampleLoad( false, filename,0,0,5,0 ); 
    if(sample == 0 )
    {
        return false;//file not found
    }
    channel = BASS_SampleGetChannel(sample, false);
	std::cout<<"LOADED AUDIO"<<std::endl;
	return true;
}

Audio* Audio::Get(const char* filename)
{
	std::map<std::string,Audio*>::iterator it = sLoadedAudios.find(filename);
	if (it != sLoadedAudios.end())
		return it->second;

	Audio* audio = new Audio();
	if (!audio->load(filename))
		return NULL;
	sLoadedAudios[filename] = audio;
	return audio;
}

HCHANNEL Audio::Play( const char* filename ){
	Audio* audio = Audio::Get(filename);
	return audio->play();
}

HCHANNEL Audio::PlayParallel( const char* filename ){
	Audio* audio = Audio::Get(filename);
	HCHANNEL ch = BASS_SampleGetChannel(audio->sample, false);
	BASS_ChannelPlay( ch, true );
	return ch;
}