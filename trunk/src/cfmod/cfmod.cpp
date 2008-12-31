/* Implementation of the CFMOD Library */

#include "../../inc/cfmod.hpp"

FMOD::Player::Player()
{
	result = FMOD_System_Create( &system );
	result = FMOD_System_Init( system, 32, FMOD_INIT_NORMAL, 0 );
	volume = 1.0f;
}

FMOD::Player::~Player()
{
    result = FMOD_System_Close(system);
	result = FMOD_System_Release( system );
}

void FMOD::Player::renderFile(std::string file) throw (FMOD::FMODException)
{
	result = FMOD_System_CreateStream( system, file.c_str(), FMOD_2D | FMOD_SOFTWARE, 0, &sound );		
	if( result != FMOD_OK )
	{		
		throw FMOD::FMODException(getLastError());
	}

	result = FMOD_System_PlaySound( system, FMOD_CHANNEL_FREE, sound, 1, &channel );
	if( result != FMOD_OK )
	{			
		throw FMOD::FMODException(getLastError());
	}
}

void FMOD::Player::play()
{
	resume();
}


std::string FMOD::Player::getLastError()
{
	std::string e;
	if( result != FMOD_OK )
		e = FMOD_ErrorString(result); 
	else
		e = "Ok";
	return e; 
}

void FMOD::Player::pause()
{     result = FMOD_System_Close(system);
	result = FMOD_Channel_SetPaused( channel, 1 );
}

void FMOD::Player::resume()
{
	result = FMOD_Channel_SetPaused( channel, 0 );
}

void FMOD::Player::stop()
{
	result = FMOD_Channel_Stop( channel );
	result = FMOD_Sound_Release( sound );
}


unsigned int FMOD::Player::getPosition()
{ 
	unsigned int position;
	result = FMOD_Channel_GetPosition( channel, &position, FMOD_TIMEUNIT_MS );
	return position;
}

void FMOD::Player::setPosition(unsigned int position) throw (FMOD::FMODException)
{
	if( getLength() < position )
	{
		throw FMOD::FMODException("Position exceeds length.");
	}
	result = FMOD_Channel_SetPosition( channel, position, FMOD_TIMEUNIT_MS );
}

unsigned int FMOD::Player::getLength()
{
  unsigned int length;
  result = FMOD_Sound_GetLength( sound, &length, FMOD_TIMEUNIT_MS );
  return length;
}

void FMOD::Player::setVolume(float volume) throw (FMODException)
{
	if( volume > 1.0f || volume < 0.0f) 
	{
		throw FMODException("Volume out of range");
	}
	
	this->volume = volume;
	result = FMOD_Channel_SetVolume( channel, volume );
}

void FMOD::Player::getSpectrum(float *spectrum, int numvalues, int channel, FMOD_DSP_FFT_WINDOW window)
{
	result = FMOD_Channel_GetSpectrum( this->channel, spectrum, numvalues, channel, window );
}

void FMOD::Player::getWaveData(float *wavedata, int numvalues, int channel)
{
	result = FMOD_Channel_GetWaveData( this->channel, wavedata, numvalues, channel );
}

unsigned int FMOD::Player::getLengthFromName(std::string filepath)
{
	
	FMOD_SOUND *sound;
	unsigned int length;   
	result = FMOD_System_CreateStream( system, filepath.c_str(), FMOD_2D | FMOD_SOFTWARE, 0, &sound );		
	result = FMOD_Sound_GetLength( sound, &length, FMOD_TIMEUNIT_MS );
	result = FMOD_Sound_Release( sound );
	return length;
}
