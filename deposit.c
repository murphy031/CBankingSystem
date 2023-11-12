#include "auth.h"

int deposit(node *authenticated_user, const char *filename)
{
    int deposited_money;

    printf("How much money would you like to deposit? ");
    if (scanf("%d", &deposited_money) != 1)
    {
        printf("Deposit amount can only consist of numbers.\n");
        clear_input_buffer();
        return 0;                           // Return 0 to indicate a failure.
    }
    clear_input_buffer();

    FILE *file_ptr = fopen(filename, "r+"); // Open the file in read+write mode
    if (file_ptr == NULL)
    {
        authenticated_user->stored_balance = 0;
        // File doesn't exist, create it
        file_ptr = fopen(filename, "w");
        if (file_ptr == NULL)
        {
            printf("Unable to create the file.\n");
            return 0;                       // Return 0 to indicate a failure.
        }
    }

    // Read the existing content
    char line[100];

    // Try to read the existing balance
    if (fgets(line, sizeof(line), file_ptr) != NULL && strncmp(line, "Balance:", 8) == 0) 
    {
        if (sscanf(line + 8, " $%d", &authenticated_user->stored_balance) != 1)
        {
            printf("Error reading the current balance.\n");
            fclose(file_ptr);
            return 0;
        }
    }
    else
    {
        printf("Unable to find the current balance in the file. Initializing balance to  $0.\n");
    }

    // Calculate the new balance
    if ((authenticated_user->stored_balance + deposited_money) > __INT32_MAX__)
    {
        printf("Exceeds max amount of money that an acocunt can hold. Max amount is $%d.\n", __INT32_MAX__);
        return authenticated_user->stored_balance;
    }
    else
    {
        authenticated_user->stored_balance += deposited_money;
    }

    // Rewind to the beginning of the file
    fseek(file_ptr, 0, SEEK_SET);

    // Update the balance in the file
    fprintf(file_ptr, "Balance: $%d\n\n", authenticated_user->stored_balance);

    // Go to the end of the file to append
    fseek(file_ptr, 0, SEEK_END);

    // Append a record of the deposit
    fprintf(file_ptr, "Deposit: $%d\n", deposited_money);

    fclose(file_ptr);

    return authenticated_user->stored_balance; // Return the updated balance.
}