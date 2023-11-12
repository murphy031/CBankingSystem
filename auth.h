#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <stdlib.h>

#define LENGTH 30                   // Maximum length of username and password allowed
#define MIN_USERNAME_LENGTH 5       // Minimum username length allowed
#define MIN_PASSWORD_LENGTH 6       // Minimum password length allowed
#define N 10000                     // Hash table buckets
#define PASSWORD_ATTEMPTS 3         // Password attempt allowed

// Represents a node in a hash table bucket
typedef struct node
{
    char stored_username[LENGTH + 1];
    char stored_password[LENGTH + 1];
    int stored_account_number;
    int stored_balance;
    struct node *next;
} node;

// Represents a hash table with buckets
typedef struct
{
    node *buckets[N];
} hash_table;


bool auth_user_menu(hash_table *table);
bool bank_user_menu(node *authenticated_user, hash_table *table);
bool check(hash_table *table, char *username);
bool create_account(hash_table *table);
bool free_table(hash_table *table);
bool is_valide_password(const char* password);
bool load_table(hash_table *table, char *filename);
bool new_password(const char* filename);
bool statements(const char *filename);
bool store_new_user(const char *username, const char *encrypted_password, int account_number);
char *encrypt(const char *password);
char *hashed(const char *word);
hash_table *new_table();
int assign_account_number(hash_table *table);
int check_balance(node *authenticated_user, const char *filename);
int deposit(node *authenticated_user, const char *filename);
int transfer(node *authenticated_user, const char* filename, hash_table *table);
int withdraw(node *authenticated_user, const char *filename);
node *authenticate(hash_table *table, const char *username, const char *password, const int provided_account_number);
node *login(hash_table *table);
unsigned int hash(const char *word);
void clear_input_buffer();