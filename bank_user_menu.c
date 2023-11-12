#include "auth.h"

bool bank_user_menu(node *authenticated_user, hash_table *table)
{
    int choice;
    while (1)
    {
        printf("1: Check balance\n");
        printf("2: Deposit\n");
        printf("3: Withdraw\n");
        printf("4: Transfer\n");
        printf("5: View statements\n");
        printf("6: Logout\n");

        printf("Please enter your choice (1/2/3/4/5/6): ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Error: Invalid input. Please enter a valid option (1/2/3/4/5/6).\n\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();       // Clear input buffer after successful input

        char filename[N * 2];
        snprintf(filename, sizeof(filename), "%s.txt", authenticated_user->stored_username);

        int new_balance;
        switch (choice)
        {
            case 1:
                printf("Checking balance... \n\n");
                int balance = check_balance(authenticated_user, filename);
                printf("Your current balance is $%d\n\n", balance);
                return true;
                break;              // Exit the switch
            
            case 2:
                new_balance = deposit(authenticated_user, filename);
                printf("New updated balance is $%d\n\n", new_balance);
                return true;
                break;              // Exit the switch

            case 3:
                new_balance = withdraw(authenticated_user, filename);
                printf("Balance is $%d\n\n", new_balance);
                return true;
                break;              // Exit the switch
            
            case 4:
                new_balance = transfer(authenticated_user, filename, table);
                printf("Money transferred: $%d\n\n", new_balance);
                return true;
                break;              // Exit the switch

            case 5:
                statements(filename);
                return true;
                break;              // Exit the switch
            
            case 6:
                printf("Logging out... \n\n");
                return false;

            default:
                printf("Error: Invalid choice. Please select a valid option (1/2/3/4/5/6).\n\n");
                break;              // Exit the switch
        }
    }
    return false;
}