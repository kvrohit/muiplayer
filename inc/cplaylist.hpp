/** cplaylist.hpp

purpose: Header for the MUIPlaylist namespace
author: ksk

*/

#ifndef __CPLAYLIST_HPP
#define __CPLAYLIST_HPP

#include<iostream>
#include<string>
#include<cstring>
#include<fstream>

#ifdef _NIX
	#include<strings.h>
	#define stricmp strcasecmp
#endif

namespace MUIPlaylist
{
    const std::string libversion = "1.3.3";

	class PlaylistException
	{
		private:
			std::string e;

		public:
			PlaylistException(std::string s): e(s) { }
			std::string what() { return e; }
	};



	class Playlist
	{
		private:
			std::ifstream instream;					
			std::ofstream outstream;

			bool getNextM3UEntry(int &length, std::string &title, std::string &path);
			void writeNextM3UEntry(int &length, std::string &title, std::string &path);

			bool getNextPLSEntry(int &length, std::string &title, std::string &path);
			void writeNextPLSEntry(int length, std::string title, std::string path) { }	
			
			bool (MUIPlaylist::Playlist::*readHandler)(int &length, std::string &title, std::string &path);
			void (MUIPlaylist::Playlist::*writeHandler)(int &length, std::string &title, std::string &path);

		public:
			void renderPlaylist(std::string filePath) throw (PlaylistException); // readies the playlist for reading
			void createPlaylist(std::string filePath, bool) throw (PlaylistException); // creates a new playlist
			
			bool getNextEntry(int &length, std::string &title, std::string &path); // reads next entry, returns true if a valid entry was read
			void writeNextEntry(int &length, std::string &title, std::string &path); // writes next entry

			bool hasNextEntry(); // returns true if the playlist has another entry
			void endList(); // stop writing to playlist
	};
}


#endif // __CPLAYLIST_HPP
