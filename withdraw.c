#include "auth.h"

int withdraw(node *authenticated_user, const char *filename)
{
    int withdrawn_money;

    printf("How much money would you like to withdraw? ");
    if (scanf("%d", &withdrawn_money) != 1)
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
            perror("Unable to create the file.\n");
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
        fprintf(file_ptr, "Balance: $0\n");
    }

    // Calculate the new balance unless it will result in a negative amount of money in the bank
    if (authenticated_user->stored_balance >= withdrawn_money)
    {
        authenticated_user->stored_balance -= withdrawn_money;
    }
    else
    {
        printf("A withdrawl of $%d will result in a overdraft. Unable to complete transaction.\n", withdrawn_money);
        return authenticated_user->stored_balance;
    }

    // Rewind to the beginning of the file
    fseek(file_ptr, 0, SEEK_SET);

    // Update the balance in the file
    fprintf(file_ptr, "Balance: $%d\n", authenticated_user->stored_balance);

    // Go to the end of the file to append
    fseek(file_ptr, 0, SEEK_END);

    // Append a record of the deposit
    fprintf(file_ptr, "Withdraw: $%d\n", withdrawn_money);

    // Close the file
    fclose(file_ptr);

    return authenticated_user->stored_balance; // Return the updated balance.
}