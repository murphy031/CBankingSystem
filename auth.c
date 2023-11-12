#include "auth.h"

int main(void)
{
    hash_table *table = new_table();   // Creates a new hash table to store information
    while(!auth_user_menu(table));     // While auth user menu is false it keeps looping until user is logged in
    free_table(table);                 // Frees memory associated with hash table
    free(table);                       // Frees the hash table
}