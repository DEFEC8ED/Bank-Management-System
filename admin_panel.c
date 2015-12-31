#include "admin_functions.c"

#define MAX 512

void admin_login_head(void);
char admin_login(void);
char check_login(char *us, char *p);
char check_accounts();

typedef struct {
	char a_username[MAX];
	char a_password[MAX];
} Logins;

// =====================================================


void admin_login_head()
{
	system("clear");

	printf(" _____   _       _        __            _     \n");
	printf("|  _  |_| |_____|_|___   |  |   ___ ___|_|___ \n");
	printf("|     | . |     | |   |  |  |__| . | . | |   |\n");
	printf("|__|__|___|_|_|_|_|_|_|  |_____|___|_  |_|_|_|\n");
	printf("                                   |___|      \n");
	printf("\n");

}

// =====================================================


char admin_login()
{
	check_accounts();
	admin_login_head();
	Logins login[MAX];

	while(getchar() != '\n');

	printf("Username: ");
	fgets(login->a_username, MAX, stdin);

	printf("Password: ");
	fgets(login->a_password, MAX, stdin);

	strtok(login->a_username, "\n");
	strtok(login->a_password, "\n");

	check_login(login->a_username, login->a_password);
}


// =====================================================


char check_login(char *us, char *p)
{
	unsigned int i;
	MYSQL *con = mysql_init(NULL);

	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}
	mysql_select_db(con, "B_M_S");

	const char *q1 = "SELECT * FROM Admins";
	if(mysql_real_query(con, q1, strlen(q1)))
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
			//printf("%s ", rows[i] ? rows[i] : "NULL");
			if(i == 1)
			{
				//printf("%s ", rows[i]);				
				if((strlen(us) == strlen(rows[i])) && (strlen(p) == strlen(rows[i + 1])))
				{
					if((strncmp(us, rows[i], strlen(rows[i])) == 0) && (strncmp(p, rows[i + 1], strlen(rows[i + 1])) == 0))
					{
						welcome_menu(us);						
						mysql_free_result(result);
						mysql_close(con);
						exit(0);
					}					
				}
	 		}
		}
	}				
    printf("\n[!] WRONG LOGIN CREDENTIALS!\n"); // no compromis, if wrong credentials, then over
	mysql_free_result(result);
	mysql_close(con);
	exit(0);				
}

// =====================================================

char check_accounts()
{
	MYSQL *con = mysql_init(NULL);
	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}

	mysql_select_db(con, "B_M_S");

	const char *q_1 =  "CREATE TABLE IF NOT EXISTS Admins(aid INT NOT NULL AUTO_INCREMENT PRIMARY KEY, username VARCHAR(255) NOT NULL, password VARCHAR(255) NOT NULL, email VARCHAR(128) NOT NULL)";
	if(mysql_real_query(con, q_1, strlen(q_1)))
	{
		error_msg(con);
	}

	const char *q_2 = "SELECT * FROM Admins";
	if(mysql_real_query(con, q_2, strlen(q_2)))
	{
		error_msg(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
	{
		error_msg(con);
	}

	int row_num = mysql_num_rows(result);
		
	if(row_num == 0)
	{
		system("clear");
		printf("\n[!] There are no admins accounts yet!\n\n");

		mysql_free_result(result);
		mysql_close(con);

		exit(1);
	}
}