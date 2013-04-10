/*
	Copyright 2008 Utah State University    

	This file is part of OIP.

    OIP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OIP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OIP.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "encrypt.h"
#include <cryptopp/integer.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>
#ifdef WIN32 //for some reason these arent defined in the lib
#include <windows.h>
#include <time.h>
#endif

#include "myconfig.h"

using namespace CryptoPP;

/* 
 * im instantiating a new instance of configfile, because I can't control what
 * order the constructors get called for these global variables. 
 */
aesencrypt aes(configfile().value("secret").c_str());

aesencrypt::aesencrypt(const char* secret)
{

	int i;
	byte iv[AES::BLOCKSIZE];
	//initialize the iv. maybe this should be configuration option?
	for (i = 0; i < AES::BLOCKSIZE; i++)
		iv[i] = i % sizeof(byte);
	
	byte key[keylength];
	PKCS12_PBKDF<SHA256> keygenerator;
	keygenerator.DeriveKey(key, keylength, (byte)0, (byte*)secret, strlen(secret), (byte*)"aiowe jaz;se5u89", 16, 1000, 0);
	cfbEncryption = new ECB_Mode<AES>::Encryption(key, keylength);
	cfbDecryption = new ECB_Mode<AES>::Decryption(key, keylength);
}

aesencrypt::~aesencrypt()
{
	if (cfbEncryption)
		delete cfbEncryption;
	if (cfbDecryption)
		delete cfbDecryption;
}

