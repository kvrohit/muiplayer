/* FMOD Wrapper Class */

#ifndef __CFMOD_HPP
#define __CFMOD_HPP

#include "fmod.h"
#include "fmod_errors.h"
#include <string>
#include <exception>
#include <iostream>

namespace FMOD
{
	const std::string libversion = "1.3.5";

	class Player;
	class FMODException // encapsulate FMOD exceptions
	{
		private:	
		std::string e;
				
		public:
		FMODException(std::string s): e(s) { };
		std::string what() { return e; }
		
	}; 
	
	class Player // FMOD API 
	{
		protected:
			FMOD_RESULT result;
			FMOD_SYSTEM *system;
			FMOD_SOUND *sound;			
			FMOD_CHANNEL *channel;
			float volume;
			std::string getLastError(); // last error in the FMOD system		
		
		public:			
			
			Player(); // Calls System_Create and System_Init
			~Player(); // Releases FMOD resources
			
			void renderFile(std::string) throw (FMODException); // render a file, must call before any other function
			
			void play(); // starts playing, equivalent to calling resume after rendering
			void pause(); // pauses the stream
			void resume(); // resumes playback
			void stop(); // stops playback, must call renderFile to re-play
			
			unsigned int getPosition(); // returns the current position, milliseconds
			void setPosition(unsigned int) throw (FMODException); // sets the current position, milliseconds
			unsigned int getLength(); // returns length of current stream, milliseconds
			void setVolume(float) throw (FMODException); //sets the volume
			
			void getSpectrum(float *, int, int = 0, FMOD_DSP_FFT_WINDOW = FMOD_DSP_FFT_WINDOW_TRIANGLE); // obtains the spectrum
			void getWaveData(float*, int, int = 0); // obtains current waveform data			
			
			unsigned int getLengthFromName(std::string);
	};
}

#endif /* __CFMOD_HPP */
