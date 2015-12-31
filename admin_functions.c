/*
All main functions are probably done. I only need to add a few 'error checks' like check for duplicate usernames
*/


#include "colors.h"

#define SZ 255
#define MAXS 1024

void welcome_menu(char *ac); // admin account
char choose_op(void);
char a_delete();
char a_change_username();
char a_list();
char og_username(int original); // let's see what username we are changing..
char change_my_pass();
char current_balance();
char add_sub_money();
char add_money();
char sub_money();

const char *host = "localhost";
const char *user = "banker";
const char *pass = "";

static char *cmp;


// =====================================================


void welcome_menu(char *ac)
{
	strtok(ac, "\n");
	system("clear");
	printf(" _____   _       _        _____             _ \n");
	printf("|  _  |_| |_____|_|___   |  _  |___ ___ ___| |\n");
	printf("|     | . |     | |   |  |   __| . |   | -_| |\n");
	printf("|__|__|___|_|_|_|_|_|_|  |__|  |__,|_|_|___|_|\n");
	printf("\t  Admin Logged: [ %s%s%s ]\n", red, ac, normal);
	printf("\n");
	cmp = ac;

	choose_op();
}


// =====================================================


char choose_op(void)
{	
	int ch;

	printf("\n=====================================\n");
	printf("(1) - List all accounts\n");
	printf("(2) - Change username\n");
	printf("(3) - Delete account\n");
	printf("(4) - Change my password\n");
	printf("(5) - Current balance\n");
	printf("(6) - Add/Sub money\n");	
	printf("(7) - Logout\n");
	printf("=====================================\n");

	do
	{
		printf("%sOptions%s: ", blue, normal);
		scanf("%d", &ch);

		if(ch == 1)
		{
			a_list();
			exit(0);
		}

		else if(ch == 2)
		{
			a_change_username();
			exit(0);
		}

		else if(ch == 3)
		{
			a_delete();
			exit(0);
		}

		else if(ch == 4)
		{
			change_my_pass();
			exit(0);
		}

		else if(ch == 5)
		{
			current_balance();
			exit(0);
		}

		else if(ch == 6)
		{
			add_sub_money();
			exit(0);
		}

		else if(ch == 7)
		{
			printf("[%s+%s] Logging off...\n", cyan, normal);
			printf("\nThanks for use!\n");
			exit(0);
		}

		else
		{
			fprintf(stderr, "[%s!%s] Use only available commands!\n", red, normal);
			//;
		}

	} while(ch > 7);
	return ch;
}


// =====================================================


char a_list()
{
	int i;
	int num_fields;

	MYSQL *con = mysql_init(NULL);

	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}

	mysql_select_db(con, "B_M_S");

	const char *q1 = "select * from Bank_Users";
	if(mysql_real_query(con, q1, strlen(q1)))
	{
		error_msg(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
	{
		error_msg(con);
	}

	MYSQL_ROW row;
	MYSQL_FIELD *field; 

	num_fields = mysql_num_fields(result);

	while((row = mysql_fetch_row(result)))
	{
		for(i = 0; i < num_fields; i++)
		{
			if(i == 0)
			{
				while(field = mysql_fetch_field(result))
				{
					printf(" %s ", field->name);

				}
				printf("\n");
				//printf("\n");
			}
			printf("%s ", row[i] ? row[i] : "NULL");
		}
	}
	printf("\n");

	mysql_free_result(result);
	mysql_close(con);

	choose_op();
	return 0;
}


// =====================================================

char a_delete()
{
	//a_list();
	int id;
	char del[MAXS], del2[MAXS], inc[MAXS], inc2[MAXS];

	printf("Enter ID of account you'd like to delete: ");
	scanf("%d", &id);

	snprintf(del, sizeof(del), "DELETE FROM Bank_Users WHERE bank_id=%d", id);
	snprintf(del2, sizeof(del2), "DELETE FROM User_Acc WHERE user_id=%d", id);

	MYSQL *con = mysql_init(NULL);
	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}

	mysql_select_db(con, "B_M_S");

	if(mysql_query(con, del))
	{
		error_msg(con);
	}

	if(mysql_query(con, del2))
	{
		error_msg(con);
	}

	if(mysql_query(con, "set @i := 0"))
	{
		error_msg(con);
	}

	if(mysql_query(con, "UPDATE Bank_Users SET bank_id = (@i := @i + 1) ORDER BY bank_id"))
	{
		error_msg(con);
	}

	if(mysql_query(con, "UPDATE User_Acc SET user_id = (@i := @i + 1) ORDER BY user_id"))
	{
		error_msg(con);
	}

	snprintf(inc, MAXS, "ALTER TABLE Bank_Users AUTO_INCREMENT = %d", id);
	snprintf(inc2, MAXS, "ALTER TABLE User_Acc AUTO_INCREMENT = %d", id);
	
	if(mysql_query(con, inc)) 
	{
		error(con);
	}

	if(mysql_query(con, inc2)) 
	{
		error(con);
	}

	mysql_close(con);

	choose_op();
	return 0;
}


// =====================================================


char a_change_username()
{
	int id;
	char new_username[255];
	char change_it[MAXS];
	char change_it2[MAXS];

	MYSQL *con = mysql_init(NULL);

	printf("Enter ID of user you want to change username: ");
	scanf("%d", &id);

	printf("Changing from username: ");
	og_username(id);

	while(getchar() != '\n');

	printf("Change username to: ");
	fgets(new_username, sizeof(new_username), stdin);
	strtok(new_username, "\n");

	char new_username_esc[(2 * strlen(new_username)) + 1];
	mysql_real_escape_string(con, new_username_esc, new_username, strlen(new_username));

	snprintf(change_it, MAXS, "UPDATE Bank_Users SET username='%s' WHERE bank_id=%d", new_username_esc, id);
	snprintf(change_it2, MAXS, "UPDATE User_Acc SET username='%s' WHERE user_id=%d", new_username_esc, id);

	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}

	mysql_select_db(con, "B_M_S");

	if(mysql_query(con, change_it))
	{
		error_msg(con);
	}

	if(mysql_query(con, change_it2))
	{
		error_msg(con);
	}

	printf("[%s+%s] Username successfully changed to: %s\n", green, normal, new_username_esc);

	mysql_close(con);
	choose_op();

	return 0;
}


// =====================================================


char og_username(int original)
{
	unsigned int i;
	char request[MAXS];

	MYSQL *con = mysql_init(NULL);

	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}
	mysql_select_db(con, "B_M_S");

	snprintf(request, MAXS, "SELECT * FROM Bank_Users WHERE bank_id=%d", original);

	if(mysql_real_query(con, request, strlen(request)))
	{
		error_msg(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
	{
		error_msg(con);
	}

	MYSQL_ROW rows;
	int col = mysql_num_fields(result);

	while((rows = mysql_fetch_row(result)))
	{
		for(i = 0; i < col; i++)
		{
			if(i == 1)
			{
				printf("%s\n", rows[i + 1]);
			}
		}
	}

}


// =====================================================


char change_my_pass()
{
	char username[128], new_pass_update[SZ];
	char old_pass[SZ], new_pass[SZ];

	printf("Changing password of '%s%s%s' admin account.\n", red, cmp, normal);

	MYSQL *con = mysql_init(NULL);

	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}
	mysql_select_db(con, "B_M_S");

	snprintf(username, sizeof(username), "SELECT password FROM Admins WHERE username='%s'", cmp);

	mysql_query(con, username);

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
	{
		error_msg(con);
	}
	MYSQL_ROW row = mysql_fetch_row(result);

	while(getchar() != '\n');

	printf("Verify old password: ");
	fgets(old_pass, SZ, stdin);

	strtok(old_pass, "\n");

	if(strcmp(old_pass, row[0]) != 0)
	{
		fprintf(stderr, "[!] Verification filed. Wrong password!\n");
		mysql_free_result(result);
		mysql_close(con);

		choose_op();
	}

	printf("Enter your new password: ");
	fgets(new_pass, SZ, stdin);
	strtok(new_pass, "\n");

	char new_pass_esc[(2 * strlen(new_pass)) + 1];
	mysql_real_escape_string(con, new_pass_esc, new_pass, strlen(new_pass));

	snprintf(new_pass_update, MAXS, "UPDATE Admins SET password='%s' WHERE username='%s'", new_pass_esc, cmp);
	
	mysql_real_query(con, new_pass_update, strlen(new_pass_update));
	printf("[+] Password sucessfully changed.\n");

	mysql_free_result(result);
	mysql_close(con);
	
	choose_op();
	return 0;
}


// =====================================================


char current_balance()
{
	int i;

	printf("\nList of a single users with its current balance: \n\n");

	MYSQL *con = mysql_init(NULL);
	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}

	mysql_select_db(con, "B_M_S");

	const char *q1 = "SELECT username, balance FROM User_Acc";
	if(mysql_real_query(con, q1, strlen(q1)))
	{
		error_msg(con);
	}

	MYSQL_RES *result = mysql_store_result(con);

	MYSQL_ROW rows;
	int col = mysql_num_fields(result);

	while((rows = mysql_fetch_row(result)))
	{
		for(i = 0; i < col; i++)
		{
			if(i == 1)
			{
				printf("%s : %s$%s%s \n", rows[i - 1], boldwhite, rows[i] ? rows[i] : "NULL", normal);
				//printf("%s ", rows[i - 1]);
			}
		}
	}

	mysql_free_result(result);
	mysql_close(con);
	choose_op();

	exit(0);
}


// =====================================================

char add_sub_money()
{
	unsigned int ch;
	do
	{		
		printf("\n(1) - Add money to a specific account.\n");
		printf("(2) - Subtract money to a specific account\n");
		printf("(3) - Back to menu.\n\n");
		printf("What would you like to do: ");

		scanf("%d", &ch);
		if(ch == 1)
		{
			add_money();
		}

		else if(ch == 2)
		{
			sub_money();
		}

		else if(ch == 3)
		{
			choose_op();
		}

		else
		{
			fprintf(stderr, "[%s!%s] Use only available commands!\n", red, normal);
		}
	} while(ch < 1 || ch > 3);
	return ch;
}


// =====================================================


char add_money()
{
	char s_user[SZ];
	int money;
	char u_request[MAXS], new_balance[MAXS];
	int i;

	MYSQL *con = mysql_init(NULL);

	printf("What user would you like to add money? ");

	while(getchar() != '\n');	
	fgets(s_user, sizeof(s_user), stdin);
	strtok(s_user, "\n");

	printf("How much money? ");
	scanf("%d", &money);

	char s_user_esc[(2 * strlen(s_user)) + 1];
	mysql_real_escape_string(con, s_user_esc, s_user, strlen(s_user));

	snprintf(u_request, SZ, "UPDATE User_Acc SET balance=balance+%d WHERE username='%s'", money, s_user_esc);

	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}

	mysql_select_db(con, "B_M_S");

	if(mysql_real_query(con, u_request, strlen(u_request)))
	{
		error_msg(con);
	}
	printf("\n[+] Money has been added.\n");
	
	snprintf(new_balance, MAXS, "SELECT balance FROM User_Acc WHERE username='%s'", s_user_esc);

	if(mysql_real_query(con, new_balance, strlen(new_balance)))
	{
		error_msg(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
	{
		error_msg(con);
	}

	int num_f = mysql_num_fields(result);

	MYSQL_ROW rows;

	while((rows = mysql_fetch_row(result)))
	{
		for(i = 0; i < num_f; i++)
		{
			printf("%s : $%s\n", s_user, rows[i] ? rows[i] : "NULL");
		}
	}

	mysql_free_result(result);
	mysql_close(con);
	choose_op();

	return 0;
}


// =====================================================


char sub_money()
{
	char s_user[SZ];
	int money;
	char u_request[MAXS], new_balance[MAXS];
	int i;

	MYSQL *con = mysql_init(NULL);

	printf("What user would you like to subtract money: ");

	while(getchar() != '\n');	
	fgets(s_user, sizeof(s_user), stdin);
	strtok(s_user, "\n");

	printf("How much money: ");
	scanf("%d", &money);

	char s_user_esc[(2 * strlen(s_user)) + 1];
	mysql_real_escape_string(con, s_user_esc, s_user, strlen(s_user));

	snprintf(u_request, SZ, "UPDATE User_Acc SET balance=balance-%d WHERE username='%s'", money, s_user_esc);

	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}

	mysql_select_db(con, "B_M_S");

	if(mysql_real_query(con, u_request, strlen(u_request)))
	{
		error_msg(con);
	}
	printf("\n[+] Money has been subtracted.\n");
	
	snprintf(new_balance, MAXS, "SELECT balance FROM User_Acc WHERE username='%s'", s_user_esc);

	if(mysql_real_query(con, new_balance, strlen(new_balance)))
	{
		error_msg(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
	{
		error_msg(con);
	}

	int num_f = mysql_num_fields(result);

	MYSQL_ROW rows;

	while((rows = mysql_fetch_row(result)))
	{
		for(i = 0; i < num_f; i++)
		{
			printf("%s : $%s\n", s_user, rows[i] ? rows[i] : "NULL");
		}
	}

	mysql_free_result(result);
	mysql_close(con);
	choose_op();

	return 0;
}