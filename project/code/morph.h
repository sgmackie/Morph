
// CRT
#include <stdlib.h>
#include <stdio.h>
#include <string.h> //memset
#include <assert.h>
#include <math.h>

// External
#define MEOW_FFT_IMPLEMENTATION
#include "meow_fft.h"

// TODO: This is a linux portover - replace with native win32 code?
#if _WIN32
    #include "dirent.h"
    #include "winsock.h"
#else
    #include <arpa/inet.h>
    #include <dirent.h>
#endif

// Custom
#include "types.h"

#define PI64 3.14159265358979323846

bool IsPowerOf2(uintptr_t x) 
{
	return (x & (x-1)) == 0;
}

//Assertion with logging
static bool AssertQuit = true;
static i64 AssertFailuresCounter = 0;

//! Add logger
#define Assert(Expression, ...) do {                            \
  if(!(Expression))                                             \
  {                                                             \
    ++AssertFailuresCounter;                                    \
    printf("%s, %d", __FILE__, __LINE__);                       \
    if(AssertQuit)                                              \
    {                                                           \
        abort();                                                \
    }                                                           \
  }                                                             \
} while(0);                                                     \

u64 LowerPowerOf2U64(u64 A)
{
    u64 v = A; 

    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    v++;

    u64 x = v >> 1;

    return x;
}

// Maxes
#define MAX_WAV_FILE_SIZE       ((192000 * 2) * 300)
//TODO: Check this on win/mac/linux 
#define MAX_PATH_SIZE           260
#define MAX_SOURCES             128 // Always double the intended amount

// Defaults
#define DEFAULT_CHANNELS        2
#define DEFAULT_SAMPLE_RATE     48000
#define DEFAULT_BIT_RATE        32

// WAV formats
#define WAV_FORMAT_UNKNOWN	    0x0000
#define WAV_FORMAT_PCM	 	    0x0001
#define WAV_FORMAT_IEEE_FLOAT   0x0003

typedef enum MORPH_TYPE
{
    CROSS_FADE = 1,
    CROSS_SYNTHESIS = 2,
    STFT = 3,
} MORPH_TYPE; 

typedef struct WAV_HEADER 
{
    u32 ChunkID;
    u32 ChunkSize;
    u32 Format;
    u32 Subchunk1ID;
    u32 Subchunk1Size;
    u16 AudioFormat;
    u16 Channels;
    u32 SampleRate;
    u32 ByteRate;
    u16 BlockAlign;
    u16 BitsPerSample;
    u32 Subchunk2ID;
    u32 Subchunk2Size;
} WAV_HEADER;

// AoS
typedef struct WAV_FILE 
{
    WAV_HEADER  Header;
    const char  *Path; //TODO: Max this a static allocation [MAX_PATH_SIZE]
    size_t      ID;
} WAV_FILE;

// SoA
typedef struct MORPH_SOURCES
{
    size_t      Count;
    size_t      WAVID[MAX_SOURCES];
    u64         TotalSampleLength[MAX_SOURCES];
    f32         *Data[MAX_SOURCES];
} MORPH_SOURCES;

typedef struct WAV_FILES
{
    WAV_HEADER  Header[MAX_SOURCES];
    size_t      ID[MAX_SOURCES];
    const char  *Path[MAX_SOURCES];    
} WAV_FILES;

// Macros
#define MAX(a, b)         ((a) > (b) ? (a) : (b))

// Source files
#include "wav.cpp"
#include "file.cpp"
#include "source.cpp"
#include "dsp.cpp"

