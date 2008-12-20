/** cplaylist_test.cpp

purpose: Demonstration of MUIPlaylist::Playlist
author: ksk

*/

#include "../../inc/cplaylist.hpp"

int main(int argc, char *argv[])
{
	MUIPlaylist::Playlist p;
	
	if( argc != 3 )
	{
		std::cout<<"Usage: cplaylist_test <playlist1> <playlist2>"<<std::endl;
		std::cout<<"Converts playlist1 to playlist2"<<std::endl;
		return 1;
	}	
		
	try
	{
		std::cout<<"cplaylist version: "<<MUIPlaylist::libversion<<std::endl;
		p.renderPlaylist(argv[1]);
		p.createPlaylist(argv[2], false);
		
		std::string title, path;
		int length;
		while(p.hasNextEntry())
		{
			if(p.getNextEntry(length, title, path))
			{
				std::cout<<length<<" "<<title<<std::endl<<path<<std::endl;				
				p.writeNextEntry(length, title, path);
			}
		}
		p.endList();
	}
	catch(MUIPlaylist::PlaylistException &plex)
	{
		std::cout<<plex.what()<<std::endl;
	}	
	
}
