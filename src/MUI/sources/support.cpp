/**

Unicode support for MUI

*/

#include "support.h"


QString convertToUnicode(std::string data)
{
	QString unicode;	
		
	
	// determine encoding
	unsigned char c1 = data[0], c2 = data[1], c3;
	unsigned int enc = (c1 == (unsigned char)'\xFF' ? 1 : (c1 == (unsigned char)'\xFE' ? 2 : 0));
	
	int u;
	if(enc)
	{		
		// UTF-16/UCS-2
		for(unsigned int i=2; i<data.length()-1; i+=2) 
		{	
			u = 0;					
			c1 = data[i];
			c2 = data[i+1];
			
			if(!c1 && !c2) 
				break; // reached unicode null = \0\0
			else if(c1 && c2) // 2-byte unicode
				u = c1<<(enc-1)*8 | c2<<(2-enc)*8;		
			else if(c1)			
				u = c1; // 1-byte unicode, LE				
			else
				u = c2; // 1-byte, BE			
			unicode.append(QChar(u));
		}	
	}
	else
	{
		// probably UTF-8 or ASCII
		for(unsigned int i=0; i<data.length(); i++)
		{
			c1 = data[i];			
			if( c1 == 0x00 )
				break;
			else if(c1 <= 0x7F)
				u = c1;
			else if( c1>= 0xC2 && c1 <= 0xDF)
			{
				// 2-byte UTF-8
				c2 = data[++i];
				c1 &= 0x03;
				u = ((c1 << 0x02 | c2 & 0x30)<<0x04) | (c2 & 0x0F);				
			}
			else if( c1 >= 0xE0 && c1 <= 0xEF)
			{
				// 3 byte UTF-8
				c2 = data[++i];
				c3 = data[++i];				
				
				c1 &= 0x0F;
				
				// high byte
				u = (c1<<4 | (c2&0x3C)>>2);
				u <<= 8;
				
				// low byte
				u |= ( (c2&0x03)<<2 | (c3&0x30)>>4 )<<4 ;
				u |= ( c3 & 0x0F );			
				
			}
			else
			{
				u = '?'; // unknown/invalid character
			}
			unicode.append(QChar(u)); 
			
		}
	}
	return unicode;
}
