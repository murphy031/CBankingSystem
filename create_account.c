#include "auth.h"

bool create_account(hash_table *table)
{
    load_table(table, "username_storage.txt");  // Load the .txt file to store new account information
    char username[LENGTH + 2];                  // Buffer for scanf to read into. +2 for the null terminator and scanf to read up to 31. if it only reads up to 30 it will always result true
    do
    {
        printf("New User Name: ");
        if (scanf("%31s", username) != 1)       // Gets username from user ONLY UP TO 31 CHARACTERS
        {
            printf("Error reading input. Please try again.\n");
            clear_input_buffer();
            continue;                           // Skip the rest of the loop and restart
        }            
        clear_input_buffer();                   // Clear input buffer after scanf
        if (strlen(username) < MIN_USERNAME_LENGTH)
        {
            printf("Username must contain at least 5 characters.\n");
        }
        if (strlen(username) > LENGTH)          // If username >30 error message
        {   
            printf("Username can only consist of up to 30 characters.\n");
        }
        else if (check(table, username))
        {
            printf("Username already exisits.\n");
        }
    }
    // Username can not be < 5 characters, must be < 30 characters, and must not already exist
    while (strlen(username) < MIN_USERNAME_LENGTH || strlen(username) > LENGTH || check(table, username));
    printf("Username accepted.\n");

    char password[LENGTH + 2];
    do
    {
        printf("Password: ");
        if (scanf("%31s", password) != 1)       // Gets username from user ONLY UP TO 31 CHARACTERS
        {
            printf("Error reading input. Please try again.\n");
            clear_input_buffer();
            continue;                           // Skip the rest of the loop and restart
        }            
        clear_input_buffer();                   // Clear input buffer after scanf
    }
    while(!is_valide_password(password));    
    printf("Password accepted.\n\n");           // Checks password criteria

    char *encrypted_password = encrypt(password);
    int account_number = assign_account_number(table);
    printf("Your account number is: %d\n", account_number);
    store_new_user(username, encrypted_password, account_number);   // Stores new username and password in .txt file.
    free(encrypted_password);                       // Frees the allocated memory from the hashed function
    return true;
}

// Stores username and password it into .txt file
bool store_new_user(const char *username, const char *encrypted_password, int account_number)
{
    FILE *username_storage = fopen("username_storage.txt", "a");
    if (username_storage != NULL)
    {
        // Write the username, password, and account number to .txt file
        fprintf(username_storage, "%s,%s,%d\n", username, encrypted_password, account_number);
        // Close the file
        fclose(username_storage);
    }
    else
    {
        printf("Error opening the file for writing.\n");
        return false;
    }
    return true;
}

int assign_account_number(hash_table *table)
{
    int new_account_number = 0;
    for (int i = 0; i < N; i++)
    {
        node *cursor = table->buckets[i];
        while (cursor != NULL)
        {
            if (cursor->stored_account_number > new_account_number)
            {
                new_account_number = cursor->stored_account_number;
            }
            cursor = cursor->next;
        }
    }
    new_account_number++;
    return new_account_number;
}

// Checks if password meets criteria
bool is_valide_password(const char* password)
{
    // Reset flags for password attempt
    bool password_accepted = false;
    bool contains_upper_char = false;
    bool contains_lower_char = false;
    bool contains_num = false;
    bool contains_symbol = false;
    
    if (strlen(password) < MIN_PASSWORD_LENGTH)
    {
        printf("Password must contain at least 6 characters.\n");
        return false;
    }
    if (strlen(password) > LENGTH)
    {
        printf("Password can only contain up to 30 characters.\n");
        return false;
    }

    for (int i = 0; password[i] != '\0'; i++)
    {
        if (islower(password[i]) && isalpha(password[i]))
        {
            contains_lower_char = true;
        }
        if (isupper(password[i]) && isalpha(password[i]))
        {
            contains_upper_char = true;
        }
        if (isdigit(password[i]))
        {
            contains_num = true;
        }
        if (!isdigit(password[i]) && !isalpha(password[i]))
        {
            contains_symbol = true;
        }
    }
    
    if (contains_lower_char && contains_upper_char && contains_num && contains_symbol)
    {
        password_accepted = true;
        return true;
    }
    else
    {
        printf("Password must contain 1 uppecase, 1 lowercase, 1 special character, and 1 number. Please try again.\n");
        return false;
    }
    return password_accepted;
}