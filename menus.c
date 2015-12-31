// ===================================================================================================

#include "colors.h"

void bank_menu(void);
void line(void);
int choice(void);
void user_menu(char *, char *);
int user_choice(void);
void mini_menu(void);
int mini_ch(void);

// ===================================================================================================

void bank_menu(void)
{
	system("clear");

	line();

	printf("\t\t\t  ____                          _                _                     \n");
    printf("\t\t\t | __ )      __ _     _ __     | | __    ___    | |_      ___     _ __ \n");
    printf("\t\t\t |  _ \\     / _` |   | '_ \\    | |/ /   / __|   | __|    / _ \\   | '__|\n");
    printf("\t\t\t | |_) |   | (_| |   | | | |   |   <    \\__ \\   | |_    |  __/   | |   \n");
    printf("\t\t\t |____/     \\__,_|   |_| |_|   |_|\\_\\   |___/    \\__|    \\___|   |_|   \n\n");
    printf("\t\t\t\t\t[ Bank Management System - beta ]\n\n");

	printf("\t\t\t 1. User Login\n\n");
	printf("\t\t\t 2. User Register\n\n");
	printf("\t\t\t 3. Exit \n\n");
	printf("\t\t\t Underline functions:\n");
	printf("\t\t\t (99) - Registration of new admin account\n");
	printf("\t\t\t (255) - Admin panel\n");

	line();
}

// ===================================================================================================

void line(void)
{
	printf("\n");
	printf("\t\t");

	int i;

	for (i = 1; i <= 85; i++)
	{
		printf("=");
	}

	printf("\n");

}

// ===================================================================================================

int choice(void)
{
	int ch;

	do
	{
		printf("Option: ");
		scanf("%d", &ch);

		if (ch == 3)
		{
			system("clear");

			printf("\t\t\t===== | PROGRAM TERMINATED | =====\n\n");

			exit(0);
		}

	}while (ch < 1);

	return ch;
}

// ===================================================================================================
// ===================================================================================================

void user_menu(char *username, char *acc_num)
{
	printf("\t\t===============================================\n");

	printf("\t\t__        __   _ \n");
    printf("\t\t\\ \\      / /__| | ___ ___  _ __ ___   ___\n");
    printf("\t\t \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\\n");
    printf("\t\t  \\ V  V /  __/ | (_| (_) | | | | | |  __/\n");
    printf("\t\t   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|\n\n");

    printf("\t\t\t|===[ User Logged: %s ]===|\n", username);
    printf("\t\t\t|===[Acc No.: %s%s%s  ]===|\n\n", boldwhite, acc_num, normal);

    printf("\t\t\t|1|. Withdraw\n\n");
    printf("\t\t\t|2|. Deposit\n\n");
    printf("\t\t\t|3|. View Account Details\n\n");
    printf("\t\t\t|4|. Modify Account Details\n\n");
    printf("\t\t\t|5|. Transfer Money\n\n");
    printf("\t\t\t|6|. Delete Account\n\n");
    printf("\t\t\t|7|. Log Out\n\n");

    printf("\t\t===============================================\n");
}

// ====================================================================================================
// ====================================================================================================

int user_choice(void)
{	
	int c;

	do
	{
		printf("User Option: ");
		scanf("%d", &c);

		if (c == 7)
		{
			return c;

			break;
		}

	}while (c > 7 || c < 1);

	return c;
}

// ==================================================================================================
// ==================================================================================================

void mini_menu(void)
{
	system("clear");

	printf("\t\t 1. Modify Username\n");
	printf("\t\t 2. Modify Firstname\n");
	printf("\t\t 3. Modify Lastname\n");
	printf("\t\t 4. Modify Password\n");
	printf("\t\t 5. Modify Phone\n");
	printf("\t\t 6. Modify Email\n\n");

}

// ===================================================================================================

int mini_ch(void)
{
	int c;

	do
	{
		printf("Option: ");
		scanf("%d", &c);

	}while (c < 1 || c > 6);

	return c;
}

// ========================================================================================================