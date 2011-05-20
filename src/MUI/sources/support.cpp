/**

Unicode support for MUI

*/

#include "support.h"
#include<QDebug>
#include<string>


QString convertToUnicode(const std::string &data)
{
    QString unicode;

    // determine encoding
    unsigned char c1, c2, c3;
    unsigned char header;

    header = data[0] - 0x01;

    qDebug()<<"Data:"<<data.length()<<"Enc: "<<(int)data[0];
    int u = 0;
    if(header == 0x00)
    {
        return QString(data.substr(1).c_str());
    }
    else if(header == 0x01)
    {
        // UTF-16/UCS-2
        c1 = data[1];
        c2 = data[2];
        unsigned int enc = (c1 == (unsigned char)'\xFF' ? 1 : (c1 == (unsigned char)'\xFE' ? 2 : 0));
        for(unsigned int i=3; i<data.length()-1; i+=2)
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
    else if(header == 0x03)
    {
        // UTF-8
        for(unsigned int i=1; i<data.length(); i++)
        {
            c1 = data[i];
            if( c1 == 0x00 )
                break;
            else if(c1 <= 0x7F)
            {
                u = c1;
            }
            else if( c1>= 0xC2 && c1 <= 0xDF)
            {
                // 2-byte UTF-8
                c2 = data[++i];
                c1 &= 0x03;
                c1 <<=0x02;
                c1 |= ((c2 & 0x30) >> 4);
                u = c1<<4;
                u |= (c2 & 0x0F);

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
    else
    {
        unicode = "?";
    }
    return unicode;
}

QString formatTimeToQString(quint32 duration)
{
    QString strTime;
    strTime.sprintf("%02d:%02d", duration / 1000 / 60, duration / 1000 % 60);
    return strTime;
}
