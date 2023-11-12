#include "auth.h"

bool statements(const char *filename)
{
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL)
    {
        perror("Unable to open file statements\n\n");
        return false;
    }

    char line[100];     // Buffer to store each line into
    // Read and print each line until EOF
    while (fgets(line, sizeof(line), file_ptr) != NULL) 
    {
        printf("%s", line); // Print the line
    }
    printf("\n");

    // Close the file
    fclose(file_ptr);
    
    return true;
}