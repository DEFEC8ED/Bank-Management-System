/* ******************************************/
/* SQL INTERGRATION PART for user-register */
/* *****************************************/
/*******Done by [Zapchast] and [Sunrix]*********/	

/* C1,C2,C3..etc. --> represent the changes users can make for their own database before running program */

// ================================================================================

void user_reg(void);
void error_msg(MYSQL *);
void generate_acc_num(char *, int);
void add_user(MYSQL *, char *, char *, char *, char *, char *, char *, char *);

// =================================================================================

void user_reg(void)
{
	int i;

	static char *host = "localhost";	/* you need to change this fields according */
	static char *user = "banker";
	static char *pass = "";

	// we need to open up our database again.(Bank)
	/* ************************************************************************************/

	MYSQL *con = mysql_init(NULL);

	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));

		exit(1);
	}

	if (mysql_real_connect(con, host, user, pass, "B_M_S", 0, NULL, 0) == NULL)	/* C2 */
	{
		error_msg(con);
	}

	const char *q2 = "USE B_M_S";

	if (mysql_real_query(con, q2, strlen(q2))) /* make this the active database to use now */	/* C3 */
	{
		error_msg(con);
	}

	const char *q3 = "CREATE TABLE IF NOT EXISTS Bank_Users(bank_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY, acc_num VARCHAR(20) NOT NULL, username VARCHAR(60) NOT NULL, firstname VARCHAR(60) NOT NULL, lastname VARCHAR(60) NOT NULL, password VARCHAR(60) NOT NULL, phone VARCHAR(30) NOT NULL, email VARCHAR(70) NOT NULL)";

	if (mysql_real_query(con, q3, strlen(q3)))	/* set labels in table */
	{
		error_msg(con);		/* C4 */
	}

	/* ************************************************************************************/

	char username[40], firstname[40], lastname[40], password[40], phone[20], email[50], acc[20];

	system("clear");

	printf("\t\t===== User Register Page =====\n\n");	/* put in details into our page. */

	while (getchar() != '\n');

	printf("\t Enter New Username: ");
	fgets(username, 40, stdin);

	printf("\t Enter New Firstname: ");
	fgets(firstname, 40, stdin);

	printf("\t Enter New Lastname: ");
	fgets(lastname, 40, stdin);

	printf("\t Enter New Password: ");
	fgets(password, 40, stdin);

	printf("\t Enter New phone No.: ");
	fgets(phone, 20, stdin);

	printf("\t Enter in new email: ");
	fgets(email, 50, stdin);

	/* **************************************************************************************************************************/

	username[strcspn(username, "\n")] = '\0';
	firstname[strcspn(firstname, "\n")] = '\0';
	lastname[strcspn(lastname, "\n")] = '\0';
	password[strcspn(password, "\n")] = '\0';
	phone[strcspn(phone, "\n")] = '\0';
	email[strcspn(email, "\n")] = '\0';

	generate_acc_num(acc, 10);					/* generate random account number 10 ch. */


	if ((strlen(username) == 1) || (strlen(firstname) == 1) || (strlen(lastname) == 1) || (strlen(password) == 1) || (strlen(phone) == 1) || (strlen(email) == 1))
	{
		system("clear");

		printf("\nDetails cannot be one character long....\n");
		printf("\nPlease register again...\n");

		sleep(2);

		mysql_close(con);

		return;
	}

	if (username[0] == '\0' || firstname[0] == '\0' || lastname[0] == '\0' || password[0] == '\0' || phone[0] == '\0' || email[0] == '\0')
	{
		system("clear");

		printf("\nDon't leave any field blank!...\n");
		printf("\nPlease register again...\n");

		sleep(2);

		mysql_close(con);

		return;
	}

	/* now we need to check if username's are clashing... */

	const char *q4 = "SELECT * FROM Bank_Users";

	if (mysql_real_query(con, q4, strlen(q4)))		/* showing all data inside our table */
	{
		error_msg(con);
	}

	MYSQL_RES *res = mysql_store_result(con);		/* store the result set (result query) which was returned by (SELECT) command... *res is pointer to structure which holds the result set. */

	if (res == NULL)
	{
		error_msg(con);
	}

	int fields = mysql_num_fields(res);		/* get the number of columns in table */

	MYSQL_ROW rows;

	while ((rows = mysql_fetch_row(res)))	/* fetch each row 1 by 1 */
	{
		for (i = 0; i < fields; i++)
		{
			if (i == 2)
			{
				if (strlen(username) == strlen(rows[i]))
				{
					if (strncmp(username, rows[i], strlen(rows[i])) == 0)
					{
						system("clear");

						printf("\nUsername Already Taken!!...\n");
						printf("\nPlease pick another username!!...\n");

						sleep(2);

						mysql_free_result(res);
						mysql_close(con);

						return;
					}
				} 
			}
		}
	}

	/* if the code manages to get this far XD... all checks have been done. */
	/* time to add user */

	add_user(con, acc, username, firstname, lastname, password, phone, email);

	system("clear");

	printf("\nUser has been Registered!...\n\n");

	sleep(2);

	mysql_free_result(res);
	mysql_close(con);

}

// =======================================================================================

void error_msg(MYSQL *c)
{
	fprintf(stderr, "%s\n", mysql_error(c));

	mysql_close(c);

	exit(1);
}

// ======================================================================================

void generate_acc_num(char *a, int size)
{
	char temp[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int i;

	for (i = 0; i < size; i++)
	{
		a[i] = temp[rand() % strlen(temp) + 1];
	}

	a[i] = '\0';

}

// =======================================================================================

void add_user(MYSQL *c, char *ac, char *un, char *fn, char *ln, char *pass, char *ph, char *em)
{
	/* User adding to database */

	int max = 2048;
	char array[max];

	char new[max];
	float def_bal = 500.00;			/* give users default of $500 */

	/* create the new table for the exclusive user account. */

	const char *q5 = "CREATE TABLE IF NOT EXISTS User_Acc(user_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY, acc_num VARCHAR(20) NOT NULL, username VARCHAR(60) NOT NULL, balance FLOAT(65,2) NOT NULL)";

	if (mysql_real_query(c, q5, strlen(q5)))
	{
		error_msg(c);
	}

	/* sends the formatted string to (array string)  */

	char ac_esc[(2 * strlen(ac)) + 1];
	char un_esc[(2 * strlen(un)) + 1];
	char fn_esc[(2 * strlen(fn)) + 1];
	char ln_esc[(2 * strlen(ln)) + 1];
	char pass_esc[(2 * strlen(pass)) + 1];
	char ph_esc[(2 * strlen(ph)) + 1];
	char em_esc[(2 * strlen(em)) + 1];

	mysql_real_escape_string(c, ac_esc, ac, strlen(ac));		/* self explain XD */
	mysql_real_escape_string(c, un_esc, un, strlen(un));
	mysql_real_escape_string(c, fn_esc, fn, strlen(fn));
	mysql_real_escape_string(c, ln_esc, ln, strlen(ln));
	mysql_real_escape_string(c, pass_esc, pass, strlen(pass));
	mysql_real_escape_string(c, ph_esc, ph, strlen(ph));
	mysql_real_escape_string(c, em_esc, em, strlen(em));


	snprintf(new, max, "INSERT INTO User_Acc VALUE('NULL', '%s', '%s', '%f')", ac_esc, un_esc, def_bal);

	if (mysql_real_query(c, new, strlen(new)))	/* added exclusive user. */
	{
		error_msg(c);
	}

	snprintf(array, max, "INSERT INTO Bank_Users VALUE('NULL', '%s', '%s', '%s', '%s', '%s', '%s', '%s')", ac_esc, un_esc, fn_esc, ln_esc, pass_esc, ph_esc, em_esc);

	if (mysql_real_query(c, array, strlen(array)))	/* added main "storage"! XD */
	{
		error_msg(c);
	}
}

// ===============================================================================================================================



