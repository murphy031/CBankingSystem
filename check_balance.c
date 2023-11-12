#include "auth.h"

int check_balance(node *authenticated_user, const char *filename)
{
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL)
    {
        // File doesn't exist, or there's an error opening it
        authenticated_user->stored_balance = 0;
        return authenticated_user->stored_balance;
    }

    // Read the existing content
    char line[100];

    if (fgets(line, sizeof(line), file_ptr) != NULL && strncmp(line, "Balance:", 8) == 0)
    {
        if (sscanf(line + 8, " $%d", &authenticated_user->stored_balance) != 1)
        {
            printf("Error reading the current balance.\n");
            fclose(file_ptr);
            return authenticated_user->stored_balance;
        }
    }
    else
    {
        printf("Unable to find the current balance in the file. Initializing balance to $0.\n");
        authenticated_user->stored_balance = 0;
    }

    fclose(file_ptr);
    
    return authenticated_user->stored_balance;
}