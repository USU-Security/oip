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
/*
 * maintains a global instace of an encryption/decryption module
 */

#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
using namespace CryptoPP;

class aesencrypt
{
private:
	//TODO: ECB mode is _not_ secure, it is basically a 1 to 1 mapping of 128
	//bit blocks. Should use CFB mode, but need to maintain a separate instance
	//for each client:port combination. im not ready for that yet. 
	ECB_Mode<AES>::Encryption* cfbEncryption;
	ECB_Mode<AES>::Decryption* cfbDecryption;
public:
	aesencrypt(const char* secret);
	~aesencrypt();

	//pad it out to the key length, and process it
#ifndef CRYPTOPP_NO_GLOBAL_BYTE
#define MY_BYTE byte
#else
#define MY_BYTE CryptoPP::byte
#endif

	void encrypt(void* txt, int len) {cfbEncryption->ProcessData((MY_BYTE*)txt,(MY_BYTE*)txt,len);}
	void decrypt(void* txt, int len) {cfbDecryption->ProcessData((MY_BYTE*)txt,(MY_BYTE*)txt,len);}

#ifndef WIN32
	static const int keylength = AES::DEFAULT_KEYLENGTH;
#else
	enum {keylength = AES::DEFAULT_KEYLENGTH}; //VC++ BUG
#endif
};

extern aesencrypt aes;

#endif //ENCRYPT_H
