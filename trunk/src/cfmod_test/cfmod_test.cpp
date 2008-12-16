/* test app for the cfmod wrapper */

#include "../../inc/cfmod.hpp"
#include<iostream>
#include<ctime>

#ifdef _WIN
#include <windows.h>
#endif

int main(int argc, char **argv)
{
	FMOD::Player p;
	int i, c;
	float pos, len;   
	float wavedata[20];
	
	if( argc != 2 )
	{
		std::cout<<"Usage: cfmod_test <file-name>"<<std::endl;
		return 1;
	}
	
	std::cout<<"CFMOD Library version: "<<FMOD::libversion<<std::endl<<std::endl;		
	try
	{
		std::cout<<"Attempting to render file: "<<argv[1]<<std::endl;
		p.renderFile( argv[1] );

		std::cout<<"Length of file: "<<p.getLength()<<"ms"<<std::endl;
		p.play();

		std::cout<<"Playing..."<<std::endl<<std::endl;		
		
		while(p.getPosition() < p.getLength())
		{
			
			pos = (float)p.getPosition();
			len = (float)p.getLength();			
			c = (int)((pos/len)*50);			
			
			std::cout<<"[";
			for(i=0;i<c;i++) std::cout<<"-";
			
			std::cout<<"|";
			
			for(i=0;i<50-c-1;i++) std::cout<<"-";
			std::cout<<"] "<<c*2<<"%\r"; 
			
			#ifdef _WIN
			Sleep( 1000 );
			#endif
			
			#ifdef _NIX
			sleep( 1 );
			#endif
		}
		std::cout<<std::endl<<"Done."<<std::endl;
	}
	catch(FMOD::FMODException &fmex)
	{
		std::cout<<"FMOD Exception!: "<<fmex.what()<<std::endl;
	}		
	
}
