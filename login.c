#include "auth.h"

node *login(hash_table *table)
{
    load_table(table, "username_storage.txt");    // Load the .txt file to search for username and password to validate. The load funciton can be found in table.c
    char login_username[LENGTH + 2];              // Buffer to store login username in. must be LENGTH + 2 because you want scanf to read 31 characters so you can always check if username is over 30 characters
    do                                            // if you only read 30 in scanf then it will ignore anything after 30 characters, which is not good.
    {
        printf("Username: ");
        if (scanf("%31s", login_username) != 1)   // Gets username from user ONLY UP TO 30 CHARACTERS
        {
            printf("Error reading input. Please try again.\n");
            clear_input_buffer();
            continue;                             // Skip the rest of the loop and restart
        }            
        clear_input_buffer();                     // Clear input buffer after scanf
        if (strlen(login_username) > LENGTH) 
        {
            printf("Username length can not be greater than 30.\n");
            continue;                             // Skip the rest of the loop and reprompt for input
        }
        if (!check(table, login_username))        // If user does not have an account of they mistyped then reprompt
        {
            printf("Username does not exist.\n");
        }
    }
    while(strlen(login_username) > LENGTH || !check(table, login_username));   // Repeat loop until a username does exist that is stored in file. Check funciton can be found in table.c
    printf("Username accepted.\n");

    bool authenticated = false;
    char login_password[LENGTH + 1];                                           // Buffer to store provided password
    int password_attempts = 1;
    while (password_attempts <= PASSWORD_ATTEMPTS && !authenticated)           // Asks user to enter password until it matches or until 3 incorrect password entries
    {
        printf("Password: ");
        if (scanf("%30s", login_password) != 1)                                // Gets password from user ONLY UP TO 30 CHARACTERS
        {
            printf("Error reading input. Please try again.\n");
            clear_input_buffer();
            continue;                                                          // Skip the rest of the loop and restart
        }            
        clear_input_buffer();                                                  // Clear input buffer after scanf

        int provided_account_number;
        printf("Account number: ");
        if (scanf("%d", &provided_account_number) != 1)
        {
            printf("Account number can only consist of numbers. Please try again.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        node *authenticated_node = authenticate(table, login_username, login_password, provided_account_number);   // Searches hash table to see if username and password match
        if (authenticated_node != NULL)
        {
            return authenticated_node;
        }
        else
        {
            printf ("Password or Account number %d out of %d\n", password_attempts, PASSWORD_ATTEMPTS);
            password_attempts++;
            password_attempts = 1;
        }
    }
    return NULL;
}

// When logging in makes sure that username matches password stored in .txt file after loading the hash table.
node *authenticate(hash_table *table, const char *username, const char *password, const int provided_account_number)
{
    int index = hash(username);            // The hash function gives a bucket to put the node information in. The hash function can be found in table.c
    node *cursor = table->buckets[index];  // Cursor at table index to check the stored node information.
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->stored_username, username) == 0)
        {
            // Username found, compare hashed password
            char *encrypted_password = encrypt(password);
            if (encrypted_password == NULL)
            {
                return NULL;              // Handle memory allocation error
            }

            if (strcmp(cursor->stored_password, encrypted_password) == 0 && cursor->stored_account_number == provided_account_number)
            {
                // Passwords match, authentication successful
                free(encrypted_password); // Free memory allocated for hashed input
                return cursor;
            }

            free(encrypted_password);    // Free memory allocated for hashed input
            return NULL;                 // Passwords don't match
        }
        cursor = cursor->next;
    }
    printf ("Username not found\n");
    return NULL;                         // Username not found
}