

// TODO: This isn't bulletproof - some files we're getting cutoff halfway (labeled MS/ORTF) - need to test this with a bunch of use cases
void LoadWAVIntoSource(const char *Path, MORPH_SOURCES *Sources, WAV_FILES *WAVFiles)
{
    // Get current index
    size_t Index = Sources->Count;

    // Clear header
    memset(&WAVFiles->Header[Index], 0, sizeof(WAV_HEADER)); 

    // Open file
    // memcpy((char *) WAVFiles->Path[Index], Path, sizeof(*Path));
    WAVFiles->Path[Index] = Path;
    FILE *File;
    if(fopen_s(&File, WAVFiles->Path[Index], "rb") != 0) 
    {
        printf("Morph: Failed to open file %s\n", WAVFiles->Path[Index]);
        return;
    }   

    // Read header - check chunks
    size_t BytesReturned = fread(&WAVFiles->Header[Index], sizeof(WAV_HEADER), 1, File);
    assert(BytesReturned > 0);
    assert(WAVFiles->Header[Index].ChunkID      == htonl(0x52494646)); // "RIFF"
    assert(WAVFiles->Header[Index].Format       == htonl(0x57415645)); // "WAVE"
    assert(WAVFiles->Header[Index].Subchunk1ID  == htonl(0x666d7420)); // "fmt "

    // Seek to "data" chunk
    while(WAVFiles->Header[Index].Subchunk2ID   != htonl(0x64617461)) // "data"
    {
        fseek(File, 4, SEEK_CUR);
        fread(&WAVFiles->Header[Index].Subchunk2ID, 4, 1, File);
    }    
    assert(WAVFiles->Header[Index].Subchunk2ID  == htonl(0x64617461)); // "data"
    fread(&WAVFiles->Header[Index].Subchunk2Size, 4, 1, File);

    // Set properties
    Sources->TotalSampleLength[Index] = WAVFiles->Header[Index].Subchunk2Size / (WAVFiles->Header[Index].Channels * 4);

    if(Sources->TotalSampleLength[Index] < MAX_WAV_FILE_SIZE)
    {
        //TODO: Handle different bitrates
        if(WAVFiles->Header[Index].BitsPerSample == 16)
        {
        }
		else if(WAVFiles->Header[Index].BitsPerSample == 24)
		{
		}
		else if(WAVFiles->Header[Index].BitsPerSample == 32)
        {
            // Allocate
            Sources->Data[Index] = (f32 *) malloc(sizeof(f32) * Sources->TotalSampleLength[Index]);

            // Copy sample data
            u64 SamplesRead = 0;
            SamplesRead = fread(Sources->Data[Index] , sizeof(f32), Sources->TotalSampleLength[Index], File);
        }
		else
		{
			printf("Morph: Unsupported bitrate %d! %s\n", WAVFiles->Header[Index].BitsPerSample,  WAVFiles->Path[Index]);
			return;
		}
    }

	else
	{
		printf("Morph: File too big! %s\n", WAVFiles->Path[Index]);
		return;
	}

    // Assign ID for lookups
    Sources->Count++;
    WAVFiles->ID[Index]     = Sources->Count;
    Sources->WAVID[Index]   = WAVFiles->ID[Index];

    // Close File
    fclose(File);
    File = 0;
}

void UnloadWAVFromSource(MORPH_SOURCES *Sources, WAV_FILES *WAVFiles)
{
    for(size_t i = 0; i < Sources->Count; ++i)
    {
        // Free
        if(Sources->Data[i])
        {
            free(Sources->Data[i]);
        }
    
        // Reset
        // memset(WAVFile, 0, sizeof(WAV_FILE));
    }
}



void WriteCC(u32 Value, u8 **Array) 
{
	*(*Array)++ = Value >> 24;
	*(*Array)++ = Value >> 16;
	*(*Array)++ = Value >> 8;
	*(*Array)++ = Value;
}

void Write4Bytes(u32 Value, u8 **Array) 
{
	*(*Array)++ = Value;
	*(*Array)++ = Value >> 8;
	*(*Array)++ = Value >> 16;
	*(*Array)++ = Value >> 24;
}

void Write2Bytes(u32 Value, u8 **Array) 
{
	*(*Array)++ = Value;
	*(*Array)++ = Value >> 8;
}

void WriteOutputToWAV(const char *Path, f32 *OutputBuffer, u64 SampleCount, u16 Channels = DEFAULT_CHANNELS, u32 SampleRate = DEFAULT_SAMPLE_RATE, u16 BitsPerSample = DEFAULT_BIT_RATE)
{    
    // Open file
    FILE *File;
    //TODO: If the output directory doesn't exist then this will fail - mkdir? Complain to user?
    if(fopen_s(&File, Path, "wb") != 0) 
    {
        printf("Morph: Failed to open file %s\n", Path);
        return;
    }   

    // Check file is open for writing
    assert(File != NULL);

    // Set header (chunk sizes filled on file close)
    size_t BytesPerSample           = 4;
    u64 DataSize                    = SampleCount * BytesPerSample;

    // Byte array for filling out properties
	u8 Header[58];
	u8 *HeaderPtr = Header;
    
    //TODO: Not even using the struct for the header - how to merge the two concepts?
    WriteCC             ('RIFF', &HeaderPtr);
    Write4Bytes         (4 + 26 + 12 + 8 + DataSize, &HeaderPtr);
    WriteCC             ('WAVE', &HeaderPtr);
    WriteCC             ('fmt ', &HeaderPtr);
    Write4Bytes         (18, &HeaderPtr);
    Write2Bytes         (WAV_FORMAT_IEEE_FLOAT, &HeaderPtr);
    Write2Bytes         (Channels, &HeaderPtr);
    Write4Bytes         (SampleRate, &HeaderPtr);
    Write4Bytes         (SampleRate * Channels * BytesPerSample, &HeaderPtr);
    Write2Bytes         (Channels * BytesPerSample, &HeaderPtr);
    Write2Bytes         (BytesPerSample * 8, &HeaderPtr);
    Write2Bytes         (0, &HeaderPtr);
    WriteCC             ('fact', &HeaderPtr);
    Write4Bytes         (4, &HeaderPtr);
    Write4Bytes         (Channels * SampleCount, &HeaderPtr);
	WriteCC             ('data', &HeaderPtr);
    Write4Bytes         (SampleCount * Channels * BytesPerSample, &HeaderPtr);
    
    // Write the header
    fwrite(Header, 1, sizeof(Header), File);
    
    // Write the data
    fwrite(OutputBuffer, sizeof(*OutputBuffer), SampleCount, File);
    
    // Close File
    fclose(File);
    File = 0;
    
    return;
}

