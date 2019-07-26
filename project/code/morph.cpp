
// Master include
#include "morph.h"

int main(int argc, char *argv[])
{
    // Check arguments
    if(argc != 8)
    {
        printf("Morph: morph.exe input_path1 input_path2 cutoff output_path FX parameter amplitude");
        return 0;
    }

    // Set file paths
    const char *DirectoryA      = argv[1];
    const char *DirectoryB      = argv[2];
    size_t InputCutoff          = atoi(argv[3]);
    const char *DirectoryOutput = argv[4];
    u8 InputFX                  = atoi(argv[5]);
    f32 Parameter               = atof(argv[6]);
    f32 OutputAmplitude         = atof(argv[7]);

    // Set WAV and Source databases
    MORPH_SOURCES Sources = {};
    WAV_FILES WAVs = {};

    // Load input directories
    if(!ParseDirectories(DirectoryA, DirectoryB, &Sources, &WAVs, InputCutoff))
    {
        printf("Morph: Error parsing directories! %s\t%s", DirectoryA, DirectoryB);
        return 0;        
    }    

    // Reallocate according to largest input file
    ResizeBuffers(&Sources);

    // Process
    // Get FX type
    switch(InputFX)
    {
        case CROSS_FADE:
        {
            //TODO: Add option for A-B markers
            // Find largest sample count
            u64 MaxOutputCount = 0;
            for(size_t i = 0; i < Sources.Count; ++i)
            {
                MaxOutputCount = MAX(Sources.TotalSampleLength[i], MaxOutputCount);
            }

            // Allocate output
            f32 *OutputBuffer = (f32 *) malloc(sizeof(f32) * MaxOutputCount);

            // Used for output file numbering
            size_t FileTracker = 0;
            for(size_t i = 0; i < Sources.Count; i += 2)
            {
                ++FileTracker;
                
                // Get the output count
                u64 OutputCount = MAX(Sources.TotalSampleLength[i], Sources.TotalSampleLength[i+1]);

                // Reset output buffer
                memset(OutputBuffer, 0, (sizeof(f32) * OutputCount));

                // DSP 
                DSPCrossfade(OutputBuffer, OutputCount, &Sources, i, OutputAmplitude, Parameter);

                // Write output file
                char Path[512];
                snprintf(Path, 512, "%s//Output_AB_Crossfade_%zu.wav", DirectoryOutput, FileTracker);
                WriteOutputToWAV(Path, OutputBuffer, OutputCount, 2, 192000, 32);
            }            

            // Cleanup
            free(OutputBuffer);

            break;            
        }
    }    

    // Free data
    UnloadWAVFromSource(&Sources, &WAVs);

    return 0;
}
