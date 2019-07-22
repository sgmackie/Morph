
// void SetMarkers(MORPH_SOURCE *Source, WAV_FILE *WAV, f64 StartTime, f64 EndTime)
// {
//     // Find matching WAV
//     for(size_t i = 0; i < MAX_SOURCES; ++i)
//     {
//         if(Source->WAVID == WAV->ID)
//         {
//             // Set markers according to sample rate
//             Source->StartMarker = (WAV->Header.SampleRate * StartTime);
//             Source->EndMarker   = (WAV->Header.SampleRate * EndTime);
            
//             // Find new sample length
//             Source->MarkerSampleLength = Source->EndMarker - Source->StartMarker;

//             return;
//         }
//     }

//     printf("Morph: Failed to set markers! - No sources match the WAV files\n");
// }

void ResizeBuffers(MORPH_SOURCES *Sources)
{
    for(size_t i = 0; i < Sources->Count; i += 2)
    {
        // Source A is bigger - realloc Source B
        if(Sources->TotalSampleLength[i] > Sources->TotalSampleLength[i+1])
        {
            u64 Padding = Sources->TotalSampleLength[i] - Sources->TotalSampleLength[i+1];
            u64 NewSize = Sources->TotalSampleLength[i+1] + Padding;
	    	Sources->Data[i+1] = (f32 *) realloc(Sources->Data[i+1], sizeof(f32) * NewSize);
        }

        // Source B is bigger - realloc Source A
        else
        {
            u64 Padding = Sources->TotalSampleLength[i+1] - Sources->TotalSampleLength[i];
            u64 NewSize = Sources->TotalSampleLength[i]  + Padding;
	    	Sources->Data[i] = (f32 *) realloc(Sources->Data[i], sizeof(f32) * NewSize);
        }        
    }
}