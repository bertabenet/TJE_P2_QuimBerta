#include "game.h"
#include "extra/bass.h"

class Audio 
{
public:
	bool first_time = true;
	static std::map<std::string, Audio*> sLoadedAudios; //para nuestro manager
	HSAMPLE sample; //aqui guardamos el handler del sample que retorna BASS_SampleLoad
	HCHANNEL channel;
	
	Audio(); //importante poner sample a cero aqui
	~Audio(); //aqui deberiamos liberar el sample con BASS_SampleFree

	bool load(const char* filename, int loop = 0); //lanza el audio y retorna el channel donde suena
	HCHANNEL play(); //lanza el audio y retorna el channel donde suena

	static void Stop(const char* filename);
	static Audio* Get( const char* filename, int loop = 0); //manager de audios 
	static HCHANNEL Play( const char* filename, int loop = 0); //version estática para ir mas rapido
	static HCHANNEL PlayParallel( const char* filename );
};

