#include "../../inc/caudiotags.hpp"
#include<iostream>

int main(int argc, char *argv[])
{
	if( argc !=2 )
	{
		std::cout<<"Usage: caudiotags_test <mp3 file>";
		return 1;
	}
	
	AudioTag::GenericTag tag;
	AudioTag::TagReader tr;
	
	std::cout<<"caudiotags version: "<<AudioTag::libversion<<std::endl;
	
	try
	{
		tr.renderFile(argv[1]);
		tag = tr.getTag();
		std::cout<<"Found Tag: "<<std::endl;
		std::cout<<"Title: "<<tag.title<<std::endl;
		std::cout<<"Artist: "<<tag.artist<<std::endl;
		std::cout<<"Album: "<<tag.album<<std::endl;
		std::cout<<"Comment: "<<tag.comment<<std::endl;
		std::cout<<"Track #"<<tag.track<<std::endl;
		std::cout<<"Year: "<<tag.year<<std::endl;

		if(tag.artfile == "")
			std::cout<<"No album art found."<<std::endl;
		else
			std::cout<<"Album art written to file: "<<tag.artfile<<std::endl;		
	}
	catch(AudioTag::TagException &tex)
	{
		std::cout<<"AudioTag exception #"<<tex.errorCode()<<std::endl;
	}
}
