#include "auth.h"

// Initizes a new hash table with N buckets and NULL terminates all buckets
hash_table *new_table()
{
    hash_table* table = malloc(sizeof(hash_table));
    if (table == NULL)
    {
        perror("Unable to allocate memory for hash table");
        return NULL;
    }
    for (int i = 0; i < N; i++)                   // Initialize the buckets array to NULL pointers
    {
        table->buckets[i] = NULL;
    }
    return table;
}

// Loads all the .txt data into a hash table for use.
bool load_table(hash_table *table, char *filename)
{
    FILE *load_ptr = fopen(filename, "r");
    if (load_ptr == NULL)
    {
        printf("could not open %s\n", filename);
        return false;
    }

    char line[100];                                // Buffer to store .txt file line in
    while(fgets(line, sizeof(line), load_ptr) != NULL)
    {
        // Remove newline character if present
        line[strcspn(line, "\n")] = '\0';

        char *username = strtok(line, ",");
        char *encrypted_password = strtok(NULL, ",");
        char *account_number = strtok(NULL, ",");
        int converted_account_number = atoi(account_number); // Convert to int from the char *


        node *new_node = malloc(sizeof(node));    // Creates a new node pointer to store a new word
        if (new_node == NULL)
        {
            perror("not enought memory\n");
            fclose(load_ptr);                     // Close the file before returning
            return false;
        }
        
        strncpy(new_node->stored_username, username, LENGTH);                 // Copies username into hashtable node
        strncpy(new_node->stored_password, encrypted_password, LENGTH);       // Copies password to hashtable node
        new_node->stored_account_number = converted_account_number;           // Store converted int account number in node
        new_node->next = NULL;                                                // Sets new node to point to NULL
    
        int index = hash(username);               // Uses hash function to store the new username in the hashtable under a calculated index

        if (table->buckets[index] == NULL)
        {
            table->buckets[index] = new_node;
        }
        else
        {
            new_node->next = table->buckets[index];
            table->buckets[index] = new_node;
        }
    }
    fclose(load_ptr);
    return true;
}

// Frees the hashtable after finished using
bool free_table(hash_table *table)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table->buckets[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        table->buckets[i] = NULL;
    }
    return true;
}

// Gives an index for the hash table
unsigned int hash(const char *word)
{
    int hash_num = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        hash_num += tolower(word[i]); 
    }
    return hash_num % N;
}

// Check the hash table for username after the hash table has been loaded from .txt file
bool check(hash_table *table, char *username)
{
    int index = hash(username);
    node *cursor = table->buckets[index];

    for (node *cursor = table->buckets[index]; cursor != NULL; cursor = cursor->next)
    {
        if (strncasecmp(cursor->stored_username, username, LENGTH) == 0)
        {
            return true; // Username already exists
        }
    }
    return false;        // Username does not exist
}

// Encrypts password stored in .txt file, so that it isn't just stored as the password itself
char *encrypt(const char *password)
{
    char *encrypted_password = malloc(sizeof(char) * (strlen(password) + 1));
    if (encrypted_password == NULL)
    {
        perror("Could not allocate memory for encrypt.\n");
        free(encrypted_password);
        return NULL;
    }
    int i;
    for (i = 0; i < strlen(password); i++)
    {
        encrypted_password[i] = password[i] - '!';
        if (encrypted_password[i] > '}')
        {
            encrypted_password[i] -= '!';
        }
        if (encrypted_password[i] < '!')
        {
            encrypted_password[i] += '#';
        }
        if (encrypted_password[i] == ',')
        {
            encrypted_password[i] = 'a';
        }
    }
    encrypted_password[i] = '\0';
    return encrypted_password;
}

// Clears input buffer
void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}