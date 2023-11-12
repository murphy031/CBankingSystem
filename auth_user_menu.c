#include "auth.h"

bool auth_user_menu(hash_table *table)
{
    int choice;

    while (1)
    {
        printf("1: Login\n");
        printf("2: Create Account\n");
        printf("3: Exit Program\n\n");

        printf("Please enter your choice (1/2/3): ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Error: Invalid input. Please enter a valid option (1/2/3).\n\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();       // Clear input buffer after successful input

        switch (choice)
        {
            case 1:
                printf("Logging in...\n\n");
                node *authenticated_user = login(table);    // If user give correct login information this stores the node in the hashtable to where the authenticated user exists
                if (authenticated_user != NULL)
                {
                    printf("You have been logged in successfully.\n\n");
                    while(bank_user_menu(authenticated_user, table));
                    return false;
                }
                else
                {
                    printf("Login failed. Please try again.\n\n");
                }
                break;              // Exit the switch

            case 2:
                printf("Creating an account...\n\n");
                if (create_account(table))
                {
                    printf("You created an account and have been logged in successfully.\n\n");
                    return false;
                }
                else
                {
                    printf("Account creation failed. Please try again.\n\n");
                }
                break;              // Exit the switch

            case 3:
                printf("Exiting program.\n\n");
                return true;

            default:
                printf("Error: Invalid choice. Please select a valid option (1/2/3).\n\n");
                break;              // Exit the switch
        }
    }
    return false;
}