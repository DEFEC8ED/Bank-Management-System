// ================================================
// ============== DOne by: sunrix and zapchast ====
// ================================================

// MODIFY USER FUNCTIONS PROGRAM

// =================================================

int mod_username(char *, MYSQL *);
void mod_firstname(char *, MYSQL *);
void mod_lastname(char *, MYSQL *);
void mod_password(char *, MYSQL *);
void mod_phone(char *, MYSQL *);
void mod_email(char *, MYSQL *);

// ==================================================


int mod_username(char *user, MYSQL *conn)
{
	/* **********************************************************************************/

	const char *s_1 = "SELECT * FROM Bank_Users";		/* we need to keep calling it for mods in database */

	if (mysql_real_query(conn, s_1, strlen(s_1)))
	{
		error_msg(conn);
	}

	MYSQL_RES *res_1 = mysql_store_result(conn);

	if (res_1 == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(conn));

		exit(1);
	}

	/* *****************************************************************************/

	system("clear");

	char new_user[40], arr[500], arr_1[500];;
	int i;

	while (getchar() != '\n');

	printf("Enter In New Username: ");
	fgets(new_user, 40, stdin);	

	new_user[strcspn(new_user, "\n")] = '\0';

	if (strlen(new_user) == 1)				/* can't be 1 character long. */
	{
		system("clear");

		printf("\nDetails cannot be one character long....\n");

		sleep(2);

		system("clear");

		mysql_free_result(res_1);

		return 0;
	}

	if (new_user[0] == '\0')				/* can't be empty */
	{
		system("clear");

		printf("\nDon't leave any field blank!...\n");

		sleep(2);

		system("clear");

		mysql_free_result(res_1);

		return 0;
	}

	/* need to check if the new name conflicts with other users...*/

	MYSQL_ROW row_1;

	int col_1 = mysql_num_fields(res_1);

	while ((row_1 = mysql_fetch_row(res_1)))
	{
		for (i = 0; i < col_1; i++)
		{
			if (i == 2)
			{
				if (strlen(user) == strlen(row_1[i]))
				{
					if (strncmp(user, row_1[i], strlen(row_1[i])) == 0)
					{
						break;		/* so basically ignore current username and move to next line */
					}
				}

				if (strlen(new_user) == strlen(row_1[i]))
				{
					if (strncmp(new_user, row_1[i], strlen(row_1[i])) == 0)
					{
						system("clear");

						printf("\nUsername Already Taken!!...\n");
						printf("\nPlease pick another username!!...\n");

						sleep(2);

						system("clear");

						mysql_free_result(res_1);

						return 0;
					}
				}
			}
		}
	}

	/* if everything checks out ok modify it. */

	system("clear");

	char new_esc[(2 * strlen(new_user)) + 1];		/* for edscaping string */
	char temp[(2 * strlen(user)) + 1];

	mysql_real_escape_string(conn, new_esc, new_user, strlen(new_user));
	mysql_real_escape_string(conn, temp, user, strlen(user));

	snprintf(arr, 500, "UPDATE Bank_Users set username='%s' where username='%s'", new_esc, temp);	/* Format the string for Bank_Users */

	if (mysql_real_query(conn, arr, strlen(arr)))
	{
		error_msg(conn);
	}

	snprintf(arr_1, 500, "UPDATE User_Acc set username='%s' where username='%s'", new_esc, temp);	/* Format the string for User_Acc */

	if (mysql_real_query(conn, arr_1, strlen(arr_1)))
	{
		error_msg(conn);
	}

	for (i = 0; i < 3; i++)
	{
		printf("Doing necessary changes...\n");

		sleep(1);
	}

	system("clear");

	printf("\t\t=== [CHANGES DONE] Login Again ===\n\n");

	sleep(2);

	mysql_free_result(res_1);

	return 1;
}

// =====================================================================================================================
// =====================================================================================================================

void mod_firstname(char *user, MYSQL *conn)
{
	system("clear");

	char new_first[40], array[500];;
	int i;

	while (getchar() != '\n');

	printf("Enter In New Firstname: ");
	fgets(new_first, 40, stdin);

	new_first[strcspn(new_first, "\n")] = '\0';

	/* as usual...little checks */

	if (strlen(new_first) == 1)
	{
		system("clear");

		printf("\nDetails cannot be one character long....\n");

		sleep(2);

		system("clear");

		return;
	}

	if (new_first[0] == '\0')
	{
		system("clear");

		printf("\nDon't leave any field blank!...\n");

		sleep(2);

		system("clear");

		return;
	}

	system("clear");

	char first_esc[(2 * strlen(new_first)) + 1];
	char temp[(2 * strlen(user)) + 1];

	mysql_real_escape_string(conn, first_esc, new_first, strlen(new_first));	/* to avoid injection */
	mysql_real_escape_string(conn, temp, user, strlen(user));

	snprintf(array, 500, "UPDATE Bank_Users set firstname='%s' where username='%s'", first_esc, temp);

	if (mysql_real_query(conn, array, strlen(array)))
	{
		error_msg(conn);
	}

	for (i = 0; i < 3; i++)
	{
		printf("Doing necessary changes...\n");

		sleep(1);
	}

	printf("\nChanges Complete!\n\n");

	sleep(2);

	system("clear");

	return;
}

// =====================================================================================================================
// =====================================================================================================================

void mod_lastname(char *user, MYSQL *conn)
{
	system("clear");

	char new_last[40], arr[500];
	int i;

	while (getchar() != '\n');

	printf("Enter In New Lastname: ");
	fgets(new_last, 40, stdin);

	new_last[strcspn(new_last, "\n")] = '\0';

	if (strlen(new_last) == 1)
	{
		system("clear");

		printf("\nDetails cannot be one character long....\n");

		sleep(2);

		system("clear");

		return;
	}

	if (new_last[0] == '\0')
	{
		system("clear");

		printf("\nDon't leave any field blank!...\n");

		sleep(2);

		system("clear");

		return;
	}

	system("clear");

	char last_esc[(2 * strlen(new_last)) + 1];
	char temp[(2 * strlen(user)) + 1];

	mysql_real_escape_string(conn, last_esc, new_last, strlen(new_last));	/* secure */
	mysql_real_escape_string(conn, temp, user, strlen(user));

	snprintf(arr, 500, "UPDATE Bank_Users set lastname='%s' where username='%s'", last_esc, temp);

	if (mysql_real_query(conn, arr, strlen(arr)))			/* load the sucker! */
	{
		error_msg(conn);
	}

	for (i = 0; i < 3; i++)
	{
		printf("Doing necessary changes...\n");

		sleep(1);
	}

	printf("\nChanges Complete!\n\n");

	sleep(2);

	system("clear");

	return;
}

// ==============================================================================================================
// ==============================================================================================================

void mod_password(char *user, MYSQL *conn)
{
	system("clear");

	char new_pass[40], array[500];
	int i;

	while (getchar() != '\n');

	printf("Enter In New Password: ");
	fgets(new_pass, 40, stdin);

	new_pass[strcspn(new_pass, "\n")] = '\0';

	if (strlen(new_pass) == 1)
	{
		system("clear");

		printf("\nDetails cannot be one character long....\n");

		sleep(2);

		system("clear");

		return;
	}

	if (new_pass[0] == '\0')
	{
		system("clear");

		printf("\nDon't leave any field blank!...\n");

		sleep(2);

		system("clear");

		return;
	}

	system("clear");

	char pass_esc[(2 * strlen(new_pass)) + 1];
	char temp[(2 * strlen(user)) + 1];

	mysql_real_escape_string(conn, pass_esc, new_pass, strlen(new_pass));
	mysql_real_escape_string(conn, temp, user, strlen(user));

	snprintf(array, 500, "UPDATE Bank_Users set password='%s' where username='%s'", pass_esc, temp);

	if (mysql_real_query(conn, array, strlen(array)))
	{
		error_msg(conn);
	}

	for (i = 0; i < 3; i++)
	{
		printf("Doing necessary changes...\n");

		sleep(1);
	}

	printf("\nChanges Complete!\n\n");

	sleep(2);

	system("clear");

	return;
}

// ==========================================================================================================================
// ==========================================================================================================================

void mod_phone(char *user, MYSQL *conn)
{
	system("clear");

	char new_ph[20], arr[500];
	int i;

	while (getchar() != '\n');

	printf("Enter In New Phone No.: ");
	fgets(new_ph, 20, stdin);

	new_ph[strcspn(new_ph, "\n")] = '\0';

	if (strlen(new_ph) == 1)
	{
		system("clear");

		printf("\nDetails cannot be one character long....\n");

		sleep(2);

		system("clear");

		return;
	}

	if (new_ph[0] == '\0')
	{
		system("clear");

		printf("\nDon't leave any field blank!...\n");

		sleep(2);

		system("clear");

		return;
	}

	char phone_esc[(2 * strlen(new_ph)) + 1];
	char temp[(2 * strlen(user)) + 1];

	mysql_real_escape_string(conn, phone_esc, new_ph, strlen(new_ph));
	mysql_real_escape_string(conn, temp, user, strlen(user));

	snprintf(arr, 500, "UPDATE Bank_Users set phone='%s' where username='%s'", phone_esc, temp);

	if (mysql_real_query(conn, arr, strlen(arr)))
	{
		error_msg(conn);
	}

	system("clear");

	for (i = 0; i < 3; i++)
	{
		printf("Doing necessary changes...\n");

		sleep(1);
	}

	printf("\nChanges Complete!\n\n");

	sleep(2);

	system("clear");

	return;
}

// ===============================================================================================================
// ===============================================================================================================

void mod_email(char *user, MYSQL * conn)
{
	system("clear");

	char new_e[50], array[500];
	int i;

	while (getchar() != '\n');

	printf("Enter In New Email: ");
	fgets(new_e, 50, stdin);

	new_e[strcspn(new_e, "\n")] = '\0';

	if (strlen(new_e) == 1)
	{
		system("clear");

		printf("\nDetails cannot be one character long....\n");

		sleep(2);

		system("clear");

		return;
	}

	if (new_e[0] == '\0')
	{
		system("clear");

		printf("\nDon't leave any field blank!...\n");

		sleep(2);

		system("clear");

		return;
	}

	char e_esc[(2 * strlen(new_e)) + 1];
	char temp[(2 * strlen(user)) + 1];

	mysql_real_escape_string(conn, e_esc, new_e, strlen(new_e));
	mysql_real_escape_string(conn, temp, user, strlen(user));

	snprintf(array, 500, "UPDATE Bank_Users set email='%s' where username='%s'", e_esc, temp);

	if (mysql_real_query(conn, array, strlen(array)))
	{
		error_msg(conn);
	}

	system("clear");

	for (i = 0; i < 3; i++)
	{
		printf("Doing necessary changes...\n");

		sleep(1);
	}

	printf("\nChanges Complete!\n\n");

	sleep(2);

	system("clear");

	return;
}