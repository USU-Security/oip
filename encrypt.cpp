#include "encrypt.h"
#include <cryptopp/integer.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>
#ifdef WIN32 //for some reason these arent defined in the lib
#include <windows.h>
#include <time.h>
NAMESPACE_BEGIN(CryptoPP)

const NullNameValuePairs g_nullNameValuePairs;


TimerWord ThreadUserTimer::TicksPerSecond()
{
#if defined(CRYPTOPP_WIN32_AVAILABLE)
	return 10*1000*1000;
#elif defined(CRYPTOPP_UNIX_AVAILABLE)
	static const long ticksPerSecond = sysconf(_SC_CLK_TCK);
	return ticksPerSecond;
#else
	return CLOCKS_PER_SEC;
#endif
}


TimerWord ThreadUserTimer::GetCurrentTimerValue()
{
#if defined(CRYPTOPP_WIN32_AVAILABLE)
	static bool getCurrentThreadImplemented = true;
	if (getCurrentThreadImplemented)
	{
		FILETIME now, ignored;
		if (!GetThreadTimes(GetCurrentThread(), &ignored, &ignored, &ignored, &now))
		{
			DWORD lastError = GetLastError();
			if (lastError == ERROR_CALL_NOT_IMPLEMENTED)
			{
				getCurrentThreadImplemented = false;
				goto GetCurrentThreadNotImplemented;
			}
			throw Exception(Exception::OTHER_ERROR, "ThreadUserTimer: GetThreadTimes failed with error " + IntToString(lastError));
		}
		return now.dwLowDateTime + ((TimerWord)now.dwHighDateTime << 32);
	}
GetCurrentThreadNotImplemented:
	return (TimerWord)clock() * (10*1000*1000 / CLOCKS_PER_SEC);
#elif defined(CRYPTOPP_UNIX_AVAILABLE)
	tms now;
	times(&now);
	return now.tms_utime;
#else
	return clock();
#endif
}


NAMESPACE_END

#endif


#include "config.h"

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

