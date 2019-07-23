
void DSPCrossfade(f32 *OutputBuffer, u64 OutputCount, MORPH_SOURCES *Sources, size_t Index, f32 OutputAmplitude, f32 Parameter)
{
    for(u64 j = 0; j < OutputCount; ++j)
    {
        f32 Sample      = 0.0f;
        Sample          = Sources->Data[Index+1][j] * Parameter + Sources->Data[Index][j] * (1 -Parameter);
        Sample          *= OutputAmplitude;
        OutputBuffer[j] = Sample;
    }
}