/**

caudiotags.cpp: implementation of the AudioTag component
author: ksk

*/
#include "../../inc/caudiotags.hpp"

// checks for a valid ID3 frame
// valid ID3 frames are alphanums
inline bool isValidFrame(char frame[])
{
	return( isalnum(frame[0]) && isalnum(frame[1]) && isalnum(frame[2]) && isalnum(frame[3]) );
}

// compares a non-terminated char* with a null-terminated char*
inline bool compareFrame(char data[], char frame[])
{
	return( data[0]==frame[0] && data[1]==frame[1] && data[2]==frame[2] && data[3]==frame[3] );
}

// converts an encoded std::string to an unsigned int
unsigned int toUnsigned(std::string data)
{
	unsigned int value;
	sscanf(data.c_str(), "%u", &value);
	return value;
}

void AudioTag::ID3v23TagReader::renderFile(std::string file) throw (TagException)
{
	char data[4], size[4], flags[2], temp;
	unsigned int tag_size = 0; // tag size
	
	// ID3v2.3 frames
	char FRAME_COMM[] = "COMM", FRAME_TALB[] = "TALB", FRAME_TPE1[] = "TPE1",
	     FRAME_TIT2[] = "TIT2", FRAME_TRCK[] = "TRCK", FRAME_TYER[] = "TYER",
	     FRAME_APIC[] = "APIC";
	
	// reset tag
	tag.title = "";
	tag.artist = "";
	tag.album = "";
	tag.track = 0;
	tag.year = 0;
	tag.artfile = "";	
	
	instream.open( file.c_str(), std::ios::in | std::ios::binary );
	if( !instream.is_open() )
		throw TagException(AudioTag::FILE_NOT_FOUND);
	
	// read ID3 header
	instream.read(data, 3); 	
	if( !(data[0] == 'I' && data[1] == 'D' && data[2] == '3') )
	{
		instream.close();
		throw TagException(AudioTag::NO_TAG);
	}
	
	// read version
	// must be 2.3.0 or 2.4.0
	instream.read(flags, 2);	
	if( (int)flags[0] != 3 && (int)flags[0] != 4 && (int)flags[1] != 0 )
	{
		instream.close();
		throw TagException(AudioTag::INVALID_TAG);
	}
	
	// read flags	
	instream.read(&temp, 1);
	// check for extended header or experimental header
	// and for a valid flag
	if( temp & (1<<6) || temp & (1<<5) || temp & 0x1F) 
	{
		instream.close();
		throw TagException(AudioTag::UNSUPPORTED_TAG);
	}
		
	// read size of tag
	// buggy
	instream.read(size, 4); 
	for(int i=3; i>=0; i--)
	{	
		unsigned char _size = (unsigned char)size[i];
		if( _size > 0x80 )
		{
		    instream.close();
			throw TagException(AudioTag::INVALID_TAG);
		}
		tag_size += (_size)<<(7*i);	
	}	
	
	// read frame	
	while(tag_size--)
	{
		instream.read(data, 4); // obtain next frame
		if( !isValidFrame(data) ) break; // break if no valid frame was found
		
		instream.read(size, 4); // read frame size
		instream.read(flags, 2); // read flags
		
		// calculate frame size in bytes
		frame_size = 0;
		for(int i=0; i<=3; i++)		
		{
			unsigned char _size = (unsigned char) size[i];
			frame_size |= (_size)<<(8 * (3-i));		
		}
		
		// process frame
		if( compareFrame(data, FRAME_COMM) ) // comment
			tag.comment = readCOMMFrame();
		else if( compareFrame(data, FRAME_TALB) ) // album
			tag.album = readTXXXFrame();
		else if( compareFrame(data, FRAME_TPE1) ) // artist
			tag.artist = readTXXXFrame();
		else if( compareFrame(data, FRAME_TIT2) ) // title
			tag.title = readTXXXFrame();
		else if( compareFrame(data, FRAME_TRCK) ) // track
			tag.track = toUnsigned(readTXXXFrame());		
		else if( compareFrame(data, FRAME_TYER) ) // year					
			tag.year = toUnsigned(readTXXXFrame());
		else if( compareFrame(data, FRAME_APIC) ) // album art
			tag.artfile = readAPICFrame();
		else
		{
			// unknown/boring frame
			while(frame_size--)
			{
				instream.read(&temp, 1);
				tag_size--;
				if( !tag_size )
				{
					tag_size++;
					break;
				}
			}
		}
	}
	instream.close();
}

// frame processing functions

std::string AudioTag::ID3v23TagReader::readCOMMFrame()
{
	
	char temp, buff[3], *comm;
	
	instream.read(&temp, 1); // text-encoding
	instream.read(buff, 3); // language
	instream.read(&temp, 1); // has to be 0x00
	
	try
	{
		comm = new char[frame_size - 5 + 1];
	}
	catch(std::bad_alloc &ba)
	{
		// skip this frame
		while(frame_size--)
			instream.read(&temp, 1);
		return "";
	}	
	
	instream.read(comm, frame_size - 5);
	comm[frame_size - 5]='\0';
	
	std::string comment(comm, frame_size - 5 + 1);
	delete []comm;
	
	return comment;
}

std::string AudioTag::ID3v23TagReader::readTXXXFrame()
{
    char temp, *data;	
	instream.read(&temp, 1); //text-encoding
	
	try
	{
		data = new char[frame_size];		
	}
	catch(std::bad_alloc &ba)
	{
		// skip this frame
		while(frame_size--)
			instream.read(&temp, 1);
		return "";
	}	
	instream.read(data, frame_size - 1);
	data[frame_size - 1]='\0';
	
	std::string tx_data(data, frame_size);		
	delete []data;
	
	return tx_data;	
}

std::string AudioTag::ID3v23TagReader::readAPICFrame()
{
	char temp, *picture_data;
	unsigned int bytes_read = 0;
	
	instream.read(&temp, 1); // text-encoding
	bytes_read++;
	
	do
	{
		instream.read(&temp, 1); // get MIME type
		bytes_read++;
	}
	while( temp != 0 );
	
	instream.read(&temp, 1); // get picture type
	bytes_read++;
	if( temp != 0 )
	{
		do
		{
			instream.read(&temp, 1); // get picture description
			bytes_read++;
		}
		while(temp != 0);
	}
	
	try
	{
		picture_data = new char[frame_size - bytes_read]; // picture data
	}
	catch(std::bad_alloc &ba)
	{
		// skip this frame
		while(frame_size--)
			instream.read(&temp, 1);
		return "";

	}	
	
	instream.read(picture_data, frame_size - bytes_read);
	
	
	srand( time(NULL) );
	unsigned int _random_num = rand();
	char filename[20];
	sprintf(filename, "%u.jpg", _random_num);
	
	std::ofstream out(filename, std::ios::out | std::ios::binary );
	if( !out.is_open() ) return "";
	out.write( picture_data, frame_size - 14);
	out.close();
	
	delete []picture_data;
	return filename;
}

AudioTag::GenericTag AudioTag::ID3v23TagReader::getTag()
{
	return tag;
}
	
void AudioTag::TagReader::renderFile(std::string file) throw (TagException)
{
	std::ifstream in( file.c_str(), std::ios::in | std::ios::binary );
	if( !in.is_open() )
		throw TagException(AudioTag::FILE_NOT_FOUND);
	
	std::string ext = file.substr(file.length()-3);
	
	if( 0 == stricmp(ext.c_str(), "mp3") )
		reader = &id3v23_reader;
	else
		throw TagException(AudioTag::UNSUPPORTED_TAG);
		
	try
	{
		reader->renderFile(file);
	}
	catch(AudioTag::TagException &tex)
	{
		throw(tex);
	}
}

AudioTag::GenericTag AudioTag::TagReader::getTag()
{
	return reader->getTag();
}

