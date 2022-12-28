#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>  
#include <math.h>
#include <inttypes.h>
#include <time.h>
#include <sys/timeb.h>
#include <pthread.h>
#include <errno.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <cryptopp/cryptlib.h>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/ccm.h>
#include <cryptopp/osrng.h>
#include <mutex>
#include "defines.h"


using CryptoPP::Exception;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::AES;
using CryptoPP::CBC_Mode;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::AutoSeededRandomPool;

#endif
