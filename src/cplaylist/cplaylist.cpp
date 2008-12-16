/** cplaylist.cpp

purpose: Implementation of MUIPlaylist::Playlist
author: ksk

*/

#include "../../inc/cplaylist.hpp"


void MUIPlaylist::Playlist::renderPlaylist(std::string filePath) throw (PlaylistException)
{
    char buff[256];
	
    instream.open( filePath.c_str(), std::ifstream::in);
	
	// check if file is present
	if( !instream.is_open() ) 
		throw PlaylistException("File not found");
	
	// check for valid extension
	if( 0 != stricmp(filePath.substr( filePath.length()-3 ).c_str(), "m3u") )
	{
		instream.close();
		throw PlaylistException("Invalid file");
	}
	
	instream.getline( buff, 255 );
	
	// check for valid header
	if ( 0 == strcmp(buff, "#EXTM3U") )
		readHandler = &MUIPlaylist::Playlist::getNextM3UEntry;		
	else
	{
		instream.close();
		throw PlaylistException("Invalid header found");	
	}

}

void MUIPlaylist::Playlist::createPlaylist(std::string filePath, bool append) throw (PlaylistException)
{
	if( append )
		outstream.open(filePath.c_str(), std::ofstream::ate );
	else
		outstream.open(filePath.c_str(), std::ofstream::out | std::ofstream::ate );
	
	if( !outstream.is_open() )
		throw PlaylistException("Unable to create file");
		
	if( 0 == stricmp(filePath.substr( filePath.length()-3 ).c_str(), "m3u") )
	{
		writeHandler = &MUIPlaylist::Playlist::writeNextM3UEntry;
		if( !append ) 
			outstream<<"#EXTM3U"<<std::endl;
	}
	else
	{
		instream.close();
		throw PlaylistException("Invalid file");
	}
		
}	

bool MUIPlaylist::Playlist::getNextEntry(int &length, std::string &title, std::string &path)
{
	return (this->*readHandler)( length, title, path );
}

void MUIPlaylist::Playlist::writeNextEntry(int &length, std::string &title, std::string &path)
{
    (this->*writeHandler)(length, title, path);
}

bool MUIPlaylist::Playlist::hasNextEntry()
{
	return !instream.eof();
}

bool MUIPlaylist::Playlist::getNextM3UEntry(int &length, std::string &title, std::string &path)
{
    char buff[255], *tok;
	
	do
	{
		if(!instream.getline( buff, 255 ))
		{
			instream.close();
			return false;
		}
	}
	while( strlen(buff) == 0 );
	
	tok = strtok(buff, ":,"); // gets #EXTINF	
	if( 0 != strcmp(tok, "#EXTINF") ) return false;
	tok = strtok(NULL, ","); // gets length
	sscanf(tok, "%d", &length);

	tok = strtok(NULL, ""); // gets the title
	title = tok;
	
	if(!instream.getline(buff, 255))	
	{
		instream.close();
		return false;
	}
	if( strlen(buff) == 0 ) return false;
	path = buff;		
	return true;
}


void MUIPlaylist::Playlist::writeNextM3UEntry(int &length, std::string &title, std::string &path) 
{
	outstream<<"#EXTINF:"<<length<<","<<title.c_str()<<std::endl;
	outstream<<path.c_str()<<std::endl;
}

void MUIPlaylist::Playlist::endList()
{
	outstream.close();
}
