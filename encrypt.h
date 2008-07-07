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
	void encrypt(void* txt, int len) {cfbEncryption->ProcessData((byte*)txt,(byte*)txt,len);}
	void decrypt(void* txt, int len) {cfbDecryption->ProcessData((byte*)txt,(byte*)txt,len);}

	static const int keylength = AES::DEFAULT_KEYLENGTH;
};

extern aesencrypt aes;

#endif //ENCRYPT_H
