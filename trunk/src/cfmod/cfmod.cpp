/* Implementation of the CFMOD Library */

#include "../../inc/cfmod.hpp"

FMOD::Player::Player()
{
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::Player(): About to call System_Create..."<<std::endl;
	#endif

	result = FMOD_System_Create( &system );

	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::Player(): "<<getLastError()<<std::endl;
		std::cout<<"DEBUG: FMOD::Player::Player(): About to call System_Init..."<<std::endl;
	#endif

	result = FMOD_System_Init( system, 32, FMOD_INIT_NORMAL, 0 );

	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::Player(): "<<getLastError()<<std::endl;
	#endif

	volume = 1.0f;
}

FMOD::Player::~Player()
{
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::~Player(): About to call System_Release"<<std::endl;
	#endif

	result = FMOD_System_Release( system );

	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::~Player(): "<<getLastError()<<std::endl;
	#endif
}

void FMOD::Player::renderFile(std::string file) throw (FMOD::FMODException)
{
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::renderFile(std::string): file="<<file<<std::endl;
		std::cout<<"DEBUG: FMOD::Player::renderFile(std::string): About to call System_CreateStream..."<<std::endl;
	#endif

	result = FMOD_System_CreateStream( system, file.c_str(), FMOD_2D | FMOD_SOFTWARE, 0, &sound );		

	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::renderFile(std::string): "<<getLastError()<<std::endl;
	#endif

	if( result != FMOD_OK )
	{		
		throw FMOD::FMODException(getLastError());
	}

	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::renderFile(std::string): About to call System_PlaySound..."<<std::endl;
	#endif

	result = FMOD_System_PlaySound( system, FMOD_CHANNEL_FREE, sound, 1, &channel );

	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::renderFile(std::string): "<<getLastError()<<std::endl;
	#endif

	if( result != FMOD_OK )
	{			
		throw FMOD::FMODException(getLastError());
	}
}

void FMOD::Player::play()
{
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::play(): About to call resume..."<<std::endl;
	#endif

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
{ 
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::pause(): About to call Channel_SetPaused..."<<std::endl;
	#endif

	result = FMOD_Channel_SetPaused( channel, 1 );

	#ifdef DBUG
			std::cout<<"DEBUG: FMOD::Player::pause(): "<<getLastError()<<std::endl;
	#endif

}

void FMOD::Player::resume()
{
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::resume(): About to call Channel_SetPaused..."<<std::endl;
	#endif

	result = FMOD_Channel_SetPaused( channel, 0 );

	#ifdef DBUG
			std::cout<<"DEBUG: FMOD::Player::resume(): "<<getLastError()<<std::endl;
	#endif
}

void FMOD::Player::stop()
{
	#ifdef DBUG
			std::cout<<"DEBUG: FMOD::Player::stop(): About to call Channel_Stop..."<<std::endl;
	#endif

	result = FMOD_Channel_Stop( channel );

	#ifdef DBUG
			std::cout<<"DEBUG: FMOD::Player::stop(): "<<getLastError()<<std::endl;
	#endif
	
	result = FMOD_Sound_Release( sound );

}


unsigned int FMOD::Player::getPosition()
{ 
	unsigned int position;
	
	#ifdef DBUG
			std::cout<<"DEBUG: FMOD::Player::getPosition(): About to call Channel_GetPosition..."<<std::endl;
	#endif

	result = FMOD_Channel_GetPosition( channel, &position, FMOD_TIMEUNIT_MS );

	#ifdef DBUG
			std::cout<<"DEBUG: FMOD::Player::getPosition(): position="<<position<<std::endl;
			std::cout<<"DEBUG: FMOD::Player::getPosition(): "<<getLastError()<<std::endl;
	#endif

	return position;
}

void FMOD::Player::setPosition(unsigned int position) throw (FMOD::FMODException)
{
	#ifdef DBUG
			std::cout<<"DEBUG: FMOD::Player::setPosition(unsigned int): position="<<position<<std::endl;
			std::cout<<"DEBUG: FMOD::Player::setPosition(unsigned int): About to call Channel_SetPosition..."<<std::endl;
	#endif

	if( getLength() < position )
	{
		#ifdef DBUG
				std::cout<<"DEBUG: FMOD::Player::setPosition(unsigned int): (exception)position exceeds length, position="<<position<<std::endl;
		#endif
		throw FMOD::FMODException("Position exceeds length.");
	}
	
	#ifdef DBUG
			std::cout<<"DEBUG: FMOD::Player::setPosition(unsigned int): About to call Channel_SetPosition..."<<std::endl;

	#endif

	result = FMOD_Channel_SetPosition( channel, position, FMOD_TIMEUNIT_MS );

	#ifdef DBUG
			std::cout<<"DEBUG: FMOD::Player::setPosition(unsigned int): "<<getLastError()<<std::endl;
	#endif
}

unsigned int FMOD::Player::getLength()
{
  unsigned int length;

  #ifdef DBUG
          std::cout<<"DEBUG: FMOD::Player::getLength(): About to call Sound_GetLength..."<<std::endl;
  #endif

  result = FMOD_Sound_GetLength( sound, &length, FMOD_TIMEUNIT_MS );

  #ifdef DBUG
          std::cout<<"DEBUG: FMOD::Player::getLength(): length="<<length<<std::endl;
          std::cout<<"DEBUG: FMOD::Player::getLength(): "<<getLastError()<<std::endl;
  #endif

  return length;
}

void FMOD::Player::setVolume(float volume) throw (FMODException)
{
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::setVolume(float): volume="<<volume<<std::endl;
		std::cout<<"DEBUG: FMOD::Player::setVolume(float): About to call Channel_SetVolume..."<<std::endl;
	#endif
	
	if( volume > 1.0f || volume < 0.0f) 
	{
		#ifdef DBUG
			std::cout<<"DEBUG: FMOD::Player::setVolume(float): (exception) Volume out of range: volume="<<volume<<std::endl;
		#endif
		throw FMODException("Volume out of range");
	}
	
	this->volume = volume;
	result = FMOD_Channel_SetVolume( channel, volume );
	
	#ifdef DBUG		
		std::cout<<"DEBUG: FMOD::Player::setVolume(float): "<<getLastError()<<std::endl;
	#endif
}

void FMOD::Player::getSpectrum(float *spectrum, int numvalues, int channel, FMOD_DSP_FFT_WINDOW window)
{
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::getSpectrum(float*, int, int, FMOD_DSP_FFT_WINDOW): numvalues="<<numvalues<<std::endl;
		std::cout<<"DEBUG: FMOD::Player::getSpectrum(float*, int, int, FMOD_DSP_FFT_WINDOW): About to call Channel_GetSpectrum..."<<std::endl;
	#endif
	
	result = FMOD_Channel_GetSpectrum( this->channel, spectrum, numvalues, channel, window );
	
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::getSpectrum(float*, int, int, FMOD_DSP_FFT_WINDOW): "<<getLastError()<<std::endl;
	#endif
}

void FMOD::Player::getWaveData(float *wavedata, int numvalues, int channel)
{
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::getWaveData(float*, int, int): numvalues="<<numvalues<<std::endl;
		std::cout<<"DEBUG: FMOD::Player::getWaveData(float*, int, int): About to call Channel_GetWaveData..."<<std::endl;
	#endif
	
	result = FMOD_Channel_GetWaveData( this->channel, wavedata, numvalues, channel );
	
	#ifdef DBUG
		std::cout<<"DEBUG: FMOD::Player::getWaveData(float*, int, int): "<<getLastError()<<std::endl;
	#endif
}
