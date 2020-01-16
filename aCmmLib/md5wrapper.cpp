/*
 *	This is part of my wrapper-class to create
 *	a MD5 Hash from a string and a file.
 *
 *	This code is completly free, you 
 *	can copy it, modify it, or do 
 *	what ever you want with it.
 *
 *	Feb. 2005
 *	Benjamin Grüdelbach
 */

#include "stdafx.h"

//---------------------------------------------------------------------- 
//basic includes
#include <fstream>
#include <iostream>

//my includes
#include "md5wrapper.h"
#include "md5.h"

//---------privates--------------------------

/*
 * internal hash function, calling
 * the basic methods from md5.h
 */	
std::string md5wrapper::hashit(std::string text)
{
	MD5_CTX ctx;
	MD5 md5;
	//init md5
	md5.MD5Init(&ctx);
	//update with our string
	md5.MD5Update(&ctx,
		 (unsigned char*)text.c_str(),
		 text.length());
	
	//create the hash
	unsigned char buff[16] = "";	
	md5.MD5Final(&ctx);
	memcpy(buff, ctx.digest, 16);	//add by zfq,2013.01.11

	//converte the hash to a string and return it
	return convToString(buff);	
}

/*
 * converts the numeric hash to
 * a valid std::string.
 * (based on Jim Howard's code;
 * http://www.codeproject.com/cpp/cmd5.asp)
 */
std::string md5wrapper::convToString(unsigned char *bytes)
{
	char asciihash[33];

	int p = 0;
	for(int i=0; i<16; i++)
	{
		::sprintf(&asciihash[p],"%02X",bytes[i]);
		p += 2;
	}	
	asciihash[32] = '\0';
	return std::string(asciihash);
}

//---------publics--------------------------

//constructor
md5wrapper::md5wrapper()
{
}


//destructor
md5wrapper::~md5wrapper()
{
}

/*
 * creates a MD5 hash from
 * "text" and returns it as
 * string
 */	
std::string md5wrapper::getHashFromString(std::string text)
{
	return this->hashit(text); 
}


/*
 * creates a MD5 hash from
 * a file specified in "filename" and 
 * returns it as string
 * (based on Ronald L. Rivest's code
 * from RFC1321 "The MD5 Message-Digest Algorithm")
 */	
std::string md5wrapper::getHashFromFile(std::string filename)	
{
	FILE *file;
  	MD5_CTX context;
	MD5 md5;
	int len;
  	unsigned char buffer[1024];

	//open file
  	if ((file = fopen (filename.c_str(), "rb")) == NULL)
	{
		return "-1";
	}

	//init md5
 	md5.MD5Init (&context);
 	
	//read the filecontent
	while ( (len = fread (buffer, 1, 1024, file)) )
   	{
		md5.MD5Update (&context, buffer, len);
	}
	
	/*
	generate hash, close the file and return the
	hash as std::string
	*/
	md5.MD5Final (&context);
 	fclose (file);
	return convToString(context.digest);
 }	

/*
 * EOF
 */
