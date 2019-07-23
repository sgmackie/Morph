
bool CountDirectories(const char *PathA, const char *PathB)
{
    // Check paths match
    DIR *DirectoryA = opendir(PathA);
    if(!DirectoryA)
    {
        printf("Morph: Directory doesn't exist! %s\n", PathA);
		return false;
    }

    DIR *DirectoryB = opendir(PathB);
    if(!DirectoryB)
    {
        printf("Morph: Directory doesn't exist! %s\n", PathB);
		return false;
    }    

    // Get count of files in directories
    struct dirent *DirectoryAParser;
    struct dirent *DirectoryBParser;    
    size_t DirectoryACount = 0;
    size_t DirectoryBCount = 0;
    while((DirectoryAParser = readdir(DirectoryA))) 
    {
        // There's a file - increment the counter
        ++DirectoryACount;
    }
    while((DirectoryBParser = readdir(DirectoryB))) 
    {        
        // There's a file - increment the counter
        ++DirectoryBCount;
    }

    // Check counts match
    if(DirectoryACount != DirectoryBCount)
    {
        printf("Morph: Input file counts don't match!\n");
		return false;        
    }

    // Close the paths
    closedir(DirectoryA);
    closedir(DirectoryB);

    // Counts match
    return true;
}

bool ParseDirectories(const char *PathA, const char *PathB, MORPH_SOURCES *Sources, WAV_FILES *WAVs, size_t Cutoff)
{
    // Get count of files in directories
    if(!CountDirectories(PathA, PathB))
    {
        return false;
    }

    // Temp string buffers
    char FullPathA[512];
    char FullPathB[512];
	const char *Slash = "//";

    // Open paths and check they match
    DIR *DirectoryA = opendir(PathA);
    if(!DirectoryA)
    {
        printf("Morph: Directory doesn't exist! %s\n", PathA);
		return false;
    }

    DIR *DirectoryB = opendir(PathB);
    if(!DirectoryB)
    {
        printf("Morph: Directory doesn't exist! %s\n", PathB);
		return false;
    }    

    // Parse all files in the directory, alternating between A and B to make source pairs
    // TODO: This doesn't handle subfolders well (exits the top loop to early) - fix that
    struct dirent *DirectoryAParser;
    struct dirent *DirectoryBParser;
    while((DirectoryAParser = readdir(DirectoryA)) && (DirectoryBParser = readdir(DirectoryB))) 
    {
        // Check that we're not reading another directory
        // From A
        if(!strcmp(DirectoryAParser->d_name, "."))
            continue;
        if(!strcmp(DirectoryAParser->d_name, ".."))    
            continue;

        // From B
        if(!strcmp(DirectoryBParser->d_name, "."))
            continue;
        if(!strcmp(DirectoryBParser->d_name, ".."))    
            continue;

        // Directory A route
        // Concatenate strings to build the full file path
        memset(FullPathA, 0, sizeof(*FullPathA)); // Reset buffer for each loop
        strcat(FullPathA, PathA);
        strcat(FullPathA, Slash);
        strcat(FullPathA, DirectoryAParser->d_name);

        // Load into databases
        LoadWAVIntoSource(FullPathA, Sources, WAVs);

        // Directory B route
        // Concatenate strings to build the full file path
        memset(FullPathB, 0, sizeof(*FullPathB)); // Reset buffer for each loop
        strcat(FullPathB, PathB);
        strcat(FullPathB, Slash);
        strcat(FullPathB, DirectoryBParser->d_name);

        // Load into databases
        LoadWAVIntoSource(FullPathB, Sources, WAVs);

        // Check if we've reached the cutoff point
        if(Sources->Count == (Cutoff * 2))
        {
            // TODO: Better printf with compiler flags for verbosity settings (check previous logging functions)
            // printf("Morph: Cutoff reached - not reading past this point %zu\n", Cutoff);
			
			// Close the path
			closedir(DirectoryA);
			closedir(DirectoryB);
            return true;
        }   
    }

    // Close the path
    closedir(DirectoryA);
    closedir(DirectoryB);

    // Fully parsed
    return true;
}

