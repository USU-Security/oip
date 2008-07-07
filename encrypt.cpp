#include "encrypt.h"
#include <cryptopp/integer.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>
#include "config.h"

using namespace CryptoPP;

/* 
 * im instantiating a new instance of configfile, because I can't control what
 * order the constructors get called for these global variables. 
 */
aesencrypt aes(configfile()["secret"].c_str());

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

