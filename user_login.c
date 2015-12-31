/* Sql intergration part for user-login */
/****************************************/
/****** Done by sunrix & zapchast *******/


// ========================================

#include "user_account.c"	/* user will enter through here. */

void user_log(void);

// ========================================

void user_log(void)
{
	static char *host = "localhost";
	static char *user = "banker";
	static char *pass = "";

	MYSQL *connect = mysql_init(NULL);

	if (connect == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(connect));

		exit(1);
	}

	if (mysql_real_connect(connect, host, user, pass, "B_M_S", 0, NULL, 0) == NULL)
	{
		error_msg(connect);
	}

	const char *q_1 = "USE B_M_S";			/* go into DATABASE --> B_M_S */

	if (mysql_real_query(connect, q_1, strlen(q_1)))
	{
		error_msg(connect);
	}

	const char *q_2 = "CREATE TABLE IF NOT EXISTS Bank_Users(bank_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY, acc_num VARCHAR(20) NOT NULL, username VARCHAR(60) NOT NULL, firstname VARCHAR(60) NOT NULL, lastname VARCHAR(60) NOT NULL, password VARCHAR(60) NOT NULL, phone VARCHAR(30) NOT NULL, email VARCHAR(70) NOT NULL)";

	if (mysql_real_query(connect, q_2, strlen(q_2)))
	{
		error_msg(connect);
	}

	const char *q_3 = "SELECT * FROM Bank_Users";		/* display table to see contents */

	if (mysql_real_query(connect, q_3, strlen(q_3)))
	{
		error_msg(connect);
	}

	MYSQL_RES *result = mysql_store_result(connect);	/* point the SELECT result query set into (*result) */

	if (result == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(connect));

		exit(1);
	}

	int row_num = mysql_num_rows(result);

	if (row_num == 0)			/* if there is no rows in the database means its empty */
	{
		system("clear");

		printf("\nNo users have registered!...\n\n");

		sleep(2);

		mysql_free_result(result);
		mysql_close(connect);

		return;
	}

	/* ***************************************************************************/

	system("clear");

	char username[40], password[40];
	int i;

	printf("\t\t===== User Login =====\n\n");

	while (getchar() != '\n');

	printf("\t Username: ");
	fgets(username, 40, stdin);

	printf("\t Password: ");
	fgets(password, 40, stdin);

	username[strcspn(username, "\n")] = '\0';
	password[strcspn(password, "\n")] = '\0';


	if ((strlen(username) == 1) || (strlen(password) == 1))
	{
		system("clear");

		printf("\nDetails cannot be one character long....\n");
		printf("\nPlease register again...\n");

		sleep(2);

		mysql_close(connect);

		return;
	}

	/* check if any user login field empty */

	if (username[0] == '\0' || password[0] == '\0')
	{
		system("clear");

		printf("\nCannot leave any deatils blank.\n");
		printf("\nPlease re-enter it.\n\n");

		sleep(2);

		mysql_close(connect);

		return;
	}

	/* if everything ok we need to open up database to check */

	MYSQL_ROW rows;

	int cols = mysql_num_fields(result);		/* get number of columns from result set. */

	while ((rows = mysql_fetch_row(result)))	/* fetch row one at a time */
	{
		for (i = 0; i < cols; i++)
		{
			if (i == 2)
			{
				if ((strlen(username) == strlen(rows[i])) && (strlen(password) == strlen(rows[i + 3])))
				{
					if ((strncmp(username, rows[i], strlen(rows[i])) == 0) && 
						(strncmp(password, rows[i + 3], strlen(rows[i + 3])) == 0))
					{
						system("clear");

						user_acc(connect, rows[i], rows[i - 1]);		/* Exclusive account for users which currently doing. */

						mysql_free_result(result);
						mysql_close(connect);

						return;
					}
				}
			}
		}
	}

	system("clear");

	printf("\nUsername and Password not found...\n\n");

	sleep(2);

	mysql_free_result(result);
	mysql_close(connect);

}