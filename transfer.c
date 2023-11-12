#include "auth.h"

int transfer(node *authenticated_user, const char* filename, hash_table *table)
{
    int transfer_money;                     // Buffer to store the amount of money to transfer
    printf("How much money would you like to transfer? ");
    if (scanf("%d", &transfer_money) != 1)
    {
        printf("Transfer amount can only consist of numbers.\n");
        clear_input_buffer();
        return 0;                           // Return 0 to indicate a failure.
    }
    clear_input_buffer();

    char username[LENGTH + 2];              // Buffer to store username to transfer money to
    printf("Username to transfer money to: ");
    if (scanf("%31s", username) != 1)       // Gets username from user ONLY UP TO 31 CHARACTERS
    {
        printf("Error reading input. Please try again.\n");
        clear_input_buffer();
        return 0;
    }            
    clear_input_buffer();                   // Clear input buffer after scanf

    char username_file[LENGTH * 2];
    snprintf(username_file, sizeof(username_file), "%s.txt", username); // Creates the username provided to be in .txt format

    int account_number;
    printf("Account number: ");
    if (scanf("%d", &account_number) != 1)
    {
        printf("Account number only consists of numbers.\n");
        clear_input_buffer();
        return 0;                           // Return 0 to indicate a failure.
    }
    clear_input_buffer();

    for (int i = 0; i < N; i++)
    {
        node *cursor = table->buckets[i];
        while (cursor != NULL)
        {
            if (strncasecmp(cursor->stored_username, username, sizeof(cursor->stored_username)) == 0)          // If one of the users in the hash table exists and has an account you can start the transfer process
            {
                if (cursor->stored_account_number != account_number)
                {
                    printf("Account number does not match.\n");
                    return 0;
                }
                FILE *transfer_from_file_ptr = fopen(filename, "r+"); // Open the file in read+write mode
                if (transfer_from_file_ptr == NULL)
                {
                    perror("Filename does not exist, or does not have a value.\n");
                    return 0;                           // Return 0 to indicate a failure.
                }

                char account_from_line[100];
                // Try to read the existing balance
                if (fgets(account_from_line, sizeof(account_from_line), transfer_from_file_ptr) != NULL && strncmp(account_from_line, "Balance:", 8) == 0) 
                {
                    if (sscanf(account_from_line + 8, " $%d", &authenticated_user->stored_balance) != 1)
                    {
                        printf("Error reading the current balance.\n");
                        fclose(transfer_from_file_ptr);
                        return 0;
                    }
                }

                if (transfer_money > authenticated_user->stored_balance)
                {
                    printf("Transfer will result in an overdraft. You balance is $%d.\n", authenticated_user->stored_balance);
                    return 0;
                }

                authenticated_user->stored_balance -= transfer_money;
                // Rewind to the beginning of the file
                fseek(transfer_from_file_ptr, 0, SEEK_SET);

                // Update the balance in the file
                fprintf(transfer_from_file_ptr, "Balance: $%d\n", authenticated_user->stored_balance);

                // Go to the end of the file to append
                fseek(transfer_from_file_ptr, 0, SEEK_END);

                // Append a record of the deposit
                fprintf(transfer_from_file_ptr, "Transfer out: $%d\n", transfer_money);

                fclose(transfer_from_file_ptr);

                FILE *transfer_to_file_ptr = fopen(username_file, "r+");       // Open the file to transfer into
                if (transfer_to_file_ptr == NULL)                              // If it is NULL that means the user has not logged in.
                {
                    printf("Transfer to user does have an account.\n");
                    return 0;
                }
                else
                {
                    char account_to_line[100];                              // Buffer to store what is read from the transfer to file
                    int transfer_to_balance;                                // Buffer to store the balance in the transfer to file

                    // Try to read the existing balance
                    if (fgets(account_to_line, sizeof(account_to_line), transfer_to_file_ptr) != NULL && strncmp(account_to_line, "Balance:", 8) == 0) 
                    {
                        if (sscanf(account_to_line + 8, " $%d", &transfer_to_balance) != 1)
                        {
                            printf("Error reading the current balance.\n");
                            fclose(transfer_to_file_ptr);
                            return 0;
                        }
                    }

                    // Calculate the new balance
                    transfer_to_balance += transfer_money;

                    // Rewind to the beginning of the file
                    fseek(transfer_to_file_ptr, 0, SEEK_SET);

                    // Update the balance in the file
                    fprintf(transfer_to_file_ptr, "Balance: $%d\n", transfer_to_balance);

                    // Go to the end of the file to append
                    fseek(transfer_to_file_ptr, 0, SEEK_END);

                    // Append a record of the deposit
                    fprintf(transfer_to_file_ptr, "Transfer in: $%d\n", transfer_money);

                    // Close the file
                    fclose(transfer_to_file_ptr);
                   
                    return transfer_money;      // Return the transfer amount
                }
                fclose(transfer_to_file_ptr);   // Close the file
            }
            cursor = cursor->next; // Move to the next node in the linked list of the hashtable
        }
    }
    return 0;
}