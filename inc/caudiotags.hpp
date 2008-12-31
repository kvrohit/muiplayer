/**

ctags - Classes to read/write tags from/to audio files
author: ksk

*/

#ifndef __CAUDIOTAG_HPP
#define __CAUDIOTAG_HPP

#include<string>
#include<fstream>
#include<cctype>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<new>

#ifdef _NIX
	#include<strings.h>
	#define stricmp strcasecmp
#endif

namespace AudioTag
{
	const std::string libversion = "1.1.2";
	
	// tag descriptors
	typedef unsigned int TAG_DESCRIPTOR;
	const TAG_DESCRIPTOR _TITLE   = 0x00000001;
	const TAG_DESCRIPTOR ARTIST  = 0x00000002;
	const TAG_DESCRIPTOR ALBUM   = 0x00000004;
	const TAG_DESCRIPTOR COMMENT = 0x00000008;
	const TAG_DESCRIPTOR ARTFILE = 0x00000010;
	const TAG_DESCRIPTOR TRACK   = 0x00000020;
	const TAG_DESCRIPTOR YEAR	 = 0x00000040;
	
	// Generic Tag Structure
	typedef struct __generic_tag
	{
		std::string title, artist, album;
		std::string comment;
		std::string artfile;
		unsigned int track, year;
		
		__generic_tag(): title(""), artist(""), album(""), comment(""), artfile(""), track(0), year(0) { }
			
		__generic_tag(std::string title, std::string artist, std::string album,
					  std::string comment, std::string artfile, unsigned int track, unsigned int year):
					  title(title), artist(artist), album(album),
					  comment(comment), artfile(artfile), track(track), year(year) { }			  
		
	} GenericTag;
	
	// exception descriptors
	typedef unsigned int EXCEPTION_DESCRIPTOR;
	const EXCEPTION_DESCRIPTOR FILE_NOT_FOUND  = 0x10000000;
	const EXCEPTION_DESCRIPTOR NO_TAG 		   = 0x20000000;
	const EXCEPTION_DESCRIPTOR INVALID_TAG	   = 0x40000000;
	const EXCEPTION_DESCRIPTOR UNSUPPORTED_TAG = 0x80000000;
	
	class TagException
	{
		private:			
			unsigned int er;
		public:			
			TagException(EXCEPTION_DESCRIPTOR er): er(er) { }
			std::string what() 
			{ 
				switch(er)
				{
					case FILE_NOT_FOUND:  return "File not found";
					case NO_TAG: 		  return "No Tag present";
					case INVALID_TAG: 	  return "Invalid tag";
					case UNSUPPORTED_TAG: return "Unsupported tag";
					default:			  return "Unknown exception";
				}
			}
			EXCEPTION_DESCRIPTOR errorCode() { return er; }		
			
	};
	
	// generic tag reader class, base of all tag reader classes
	class __generic_tag_reader
	{
		protected:
			std::ifstream instream;
			
		public:
			virtual void renderFile(std::string) = 0;
			virtual GenericTag getTag() = 0;
			virtual std::string getTag(TAG_DESCRIPTOR) = 0;
			virtual ~__generic_tag_reader() { }
	};
	
	// ID3 tag reader class	
	class ID3v23TagReader: public __generic_tag_reader
	{
		private:
			
			AudioTag::GenericTag tag;			
			
			std::string readTXXXFrame();
			std::string readPICFrame();
			std::string readCOMMFrame();			
			std::string readAPICFrame();
			
			unsigned int frame_size;
			
		public:
			ID3v23TagReader(): tag(){ }					
			~ID3v23TagReader() { }
			
			void renderFile(std::string) throw(TagException);			
			GenericTag getTag();
			std::string getTag(TAG_DESCRIPTOR);
			
	};
	
	// generic Tag Reader Class
	class TagReader
	{
		private:
			ID3v23TagReader id3v23_reader;				
			__generic_tag_reader *reader;
		
		public:		
			void renderFile(std::string) throw(TagException);
			GenericTag getTag();
			std::string getTag(TAG_DESCRIPTOR);
		
	};
	
}

#endif /* __CAUDIOTAG_HPP  */
