/*********************************************************************/
/******** User Account program (integrated with mysql)****************/
/* *************** Done by [sunrix] and [zapchast] *******************/

// EXCLUSIVE USER ACCOUNT PROGRAM.

#include "mod_func.c"

/* ***************************************/

void user_acc(MYSQL *, char *, char *);		/* action happens */
void u_withdraw(char *, MYSQL *);			/* user withdraws */
void u_deposit(char *, MYSQL *);			/* user deposits */
void u_details(char *, MYSQL *);			/* user view details */
int u_modify(char *, MYSQL *);				/* user modify details */
void u_transfer(char *, char *, MYSQL *);	/* user transfer money */
int u_delete(char *, MYSQL *);				/* user delete account [WILL LOSE EVERYTHING] */

/* **************************************/

// =====================================================================================
// =====================================================================================

void user_acc(MYSQL *conn, char *username, char *acc_num)		/* our exclusive user main part */
{
	/* our main mysql connection is always intact during exclusive user account. */

	int c, n;

	while (1)		/* one more big loop for all the functions... :( */
	{
		user_menu(username, acc_num);

		c = user_choice();

		if (c == 1)			/* withdraw cash option : 1 */
		{	
			u_withdraw(username, conn);
		}

		else if (c == 2)	/* deposit cash option : 2 */
		{
			u_deposit(username, conn);
		}

		else if (c == 3)	/* view complete account details. */
		{
			u_details(username, conn);
		}

		else if (c == 4)
		{
			n = u_modify(username, conn);

			if (n == 1)
			{
				return;
			}
		}

		else if (c == 5)	/* transfer money to another user */
		{
			u_transfer(username, acc_num, conn);
		}

		else if (c == 6)	/* user delete account. */
		{	
			n = u_delete(username, conn);

			if (n == 1)
			{
				return;
			}
		}

		else if (c == 7)	/* log out of account. */
		{
			printf("Logging out...\n");			/* need to add color for this text. */

			sleep(2);

			return;
		}

	}

}

//============================================================================================================
//============================================================================================================

void u_withdraw(char *user, MYSQL *conn)
{
	/* need to get this table for calculations... */
	/* decided to do it seperately in each function rather than putting it in user_acc directly.*/\

	/* ***********************************************/

	const char *u_1 = "SELECT * FROM User_Acc";

	if (mysql_real_query(conn, u_1, strlen(u_1)))
	{
		error_msg(conn);
	}

	MYSQL_RES *r = mysql_store_result(conn);

	if (r == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(conn));

		exit(1);
	}

	/* **************************************************/

	system("clear");

	float temp, with, diff;
	int i;
	int max = 500;
	char t[max], c;

	MYSQL_ROW rows;

	int col = mysql_num_fields(r);		/* get columns */

	while ((rows = mysql_fetch_row(r)))		/* this can be improved in terms if execution time by adding a flag var. */
	{										/* to break out of loop faster when it finds (temp) */
		for (i = 0; i < col; i++)
		{
			if (i == 2)
			{
				if (strlen(user) == strlen(rows[i]))		/* prevention of bug. */
				{
					if (strncmp(user, rows[i], strlen(rows[i])) == 0)
					{
						temp = atof(rows[i + 1]);
					}
				}
			}
		}
	}

	if (temp == 0.0)		/* check if account empty */
	{
		system("clear");

		printf("\n || Balance : $0.00       ||\n");
		printf(" || Not able to Withdraw ||\n\n");

		sleep(3);

		system("clear");

		return;
	}

	printf("\t\t\tYOUR BALANCE: %.2f\n\n", temp);

	printf("Withdraw Amount $: ");
	scanf("%f", &with);

	if (with > temp)		/* if trying to withdraw out more than balance. */
	{
		system("clear");

		printf("\n\nInvalid Withdraw: Can't withdraw more than you have...\n\n");

		sleep(3);

		system("clear");

		return;
	}

	diff = temp - with;		/* just for this part must go through so much XD !!! */

	/* need to put the remaining into database again. */

	char temp_1[(2 * strlen(user)) + 1];

	mysql_real_escape_string(conn, temp_1, user, strlen(user));

	snprintf(t, max, "UPDATE User_Acc set balance='%f' where username='%s'", diff, temp_1);

	if (mysql_real_query(conn, t, strlen(t)))
	{
		error_msg(conn);
	}

	system("clear");	

	for (i = 0; i < 3; i++)
	{
		sleep(1);
		printf("Withdrawal in progress...\n");
	}

	printf("\nYOU WITHDREW: --> $%.2f\n", with);
	printf("BALANCE LEFT: %.2f\n\n", diff);	

	sleep(4);

	mysql_free_result(r);

	system("clear");		 

	return;
}

// =====================================================================================================
// =====================================================================================================
 
void u_deposit(char *user, MYSQL *conn)
{
	/* ****************************************************/

	const char *u_2 = "SELECT * FROM User_Acc";

	if (mysql_real_query(conn, u_2, strlen(u_2)))
	{
		error_msg(conn);
	}

	MYSQL_RES * r1 = mysql_store_result(conn);

	if (r1 == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(conn));

		exit(1);
	}	

	/* *****************************************************/

	system("clear");

	int i;
	int max = 500;
	float t, dep;
	char array[max];

	MYSQL_ROW row;

	int cols = mysql_num_fields(r1);

	while ((row = mysql_fetch_row(r1)))
	{
		for (i = 0; i < cols; i++)
		{
			if (i == 2)
			{
				if (strlen(user) == strlen(row[i]))
				{
					if (strncmp(user, row[i], strlen(row[i])) == 0)
					{
						t = atof(row[i + 1]);
					}
				}
			}
		}
	}

	printf("\t\t\tYOUR BALANCE: $%.2f\n", t);

	printf("Deposit Amount $: ");
	scanf("%f", &dep);

	t += dep;		/* increase in cash ! XD */

	/* need to update it. */

	char temp[(2 * strlen(user)) + 1];

	mysql_real_escape_string(conn, temp, user, strlen(user));

	snprintf(array, max, "UPDATE User_Acc set balance='%f' where username='%s'", t, temp);

	if (mysql_real_query(conn, array, strlen(array)))
	{
		error_msg(conn);
	}

	system("clear");

	for (i = 0; i < 3; i++)
	{
		sleep(1);
		printf("Deposit in progress...\n");
	}

	printf("\nYOU HAVE DEPOSITED --> $%.2f\n", dep);
	printf("BALANCE NOW: $%.2f\n\n", t);

	sleep(4);

	mysql_free_result(r1);

	system("clear");

	return;
}

// ====================================================================================
// ===================================================================================

void u_details(char *user, MYSQL *conn)			/* in progress --> 0.o */
{
	/* ***********************************************/

	const char *u_3 = "SELECT * FROM Bank_Users";		/* this is storing set of Bank_Users */

	if (mysql_real_query(conn, u_3, strlen(u_3)))
	{
		error_msg(conn);
	}

	MYSQL_RES *r2 = mysql_store_result(conn);

	if (r2 == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(conn));

		exit(1);
	}

	/*************************************************/

	const char *u_4 = "SELECT * FROM User_Acc";			/* this is storing set for User_Acc */

	if (mysql_real_query(conn, u_4, strlen(u_4)))
	{
		error_msg(conn);
	}

	MYSQL_RES *r3 = mysql_store_result(conn);

	if (r3 == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(conn));

		exit(1);
	}

	/***************************************************/

	system("clear");

	int j, i;

	for (j = 0; j < 3; j++)
	{
		printf("Gathering Details...\n");

		sleep(1);
	}

	system("clear");

	printf("\n\t\t\t\t|+| User Register Details |+|\n\n");

	printf("\t\t =====================================================================================\n\n");

	printf("\t\t LABELS: [Acc No.] [Username] [Firstname] [Lastname] [Password] [Phone] [Email]\n\n");

	int c_1 = mysql_num_fields(r2);		/* columns for Bank_Users set */

	MYSQL_ROW row1;

	while ((row1 = mysql_fetch_row(r2)))
	{
		for (i = 0; i < c_1; i++)
		{
			if (i == 2)
			{
				if (strlen(user) == strlen(row1[i]))
				{
					if (strncmp(user, row1[i], strlen(row1[i])) == 0)	/* print out details for user */
					{
						printf("\t\t INFO: [%s] [%s] [%s] [%s] [%s] [%s] [%s]\n\n", row1[i - 1], row1[i], row1[i + 1], row1[i + 2], row1[i + 3], row1[i + 4], row1[i + 5]);
					}
				}
			}
		}
	}

	printf("\t\t =====================================================================================\n\n\n");

	/* now show exclusive user account. */

	printf("\n\t\t\t\t|+| Exclusive User Details |+|\n\n");

	printf("\t\t =====================================================================================\n\n");

	printf("\t\t LABELS: [Acc No.] [Username] [Balance]\n\n");

	int c_2 = mysql_num_fields(r3);

	MYSQL_ROW row2;

	while ((row2 = mysql_fetch_row(r3)))
	{
		for (i = 0; i < c_2; i++)
		{
			if (i == 2)
			{
				if (strlen(user) == strlen(row2[i]))
				{
					if (strncmp(user, row2[i], strlen(row2[i])) == 0)
					{
						printf("\t\t INFO: [%s] [%s] [$%s]\n\n", row2[i - 1], row2[i], row2[i + 1]);
					}
				}
			}
		}
	}

	printf("\t\t =====================================================================================\n\n\n");

	mysql_free_result(r2);
	mysql_free_result(r3);

	sleep(5);

	system("clear");

	return;

}

// ===========================================================================================================
// ===========================================================================================================

int u_modify(char *user, MYSQL *conn)
{
	int ch, n;
		
	mini_menu();	/* another mini menu */

	ch = mini_ch();

	if (ch == 1)	/* modify username */
	{
		n = mod_username(user, conn);

		if (n == 1)
		{
			return 1;
		}

		else
		{
			return 0;
		}
	}

	else if (ch == 2)	/* modify firstname */
	{
		mod_firstname(user, conn);

		return 0;
	}

	else if (ch == 3)	/* modify lastname */
	{
		mod_lastname(user, conn);

		return 0;
	}

	else if (ch == 4)	/* modify password */
	{	
		mod_password(user, conn);

		return 0;
	}

	else if (ch == 5) 	/* modify phone No. */
	{
		mod_phone(user, conn);

		return 0;
	}

	else if (ch == 6)	/* modify email */
	{
		mod_email(user, conn);

		return 0;
	}
}

// ============================================================================================================
// ============================================================================================================

void u_transfer(char *user, char *acc_num, MYSQL *conn)			/* transfer monrey from one user to another. */
{
	/****************************************************/

	const char *p_1 = "SELECT * FROM User_Acc";

	if (mysql_real_query(conn, p_1, strlen(p_1)))
	{
		error_msg(conn);
	}

	MYSQL_RES *res = mysql_store_result(conn);

	if (res == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(conn));

		exit(1);
	}

	/******************************************************/

	system("clear");

	char a_n[15], temp_user[50], array[500], array_1[500];
	int i, flag;
	float t, t1, money, left;

	flag = 0;

	while (getchar() != '\n');

	printf("Enter In [Acc No.] of other user: ");
	fgets(a_n, 15, stdin);

	a_n[strcspn(a_n, "\n")] = '\0';

	MYSQL_ROW row;

	int col = mysql_num_fields(res);

	while ((row = mysql_fetch_row(res)))
	{
		for (i = 0; i < col; i++)
		{
			if (i == 1)
			{
				if (strlen(acc_num) == strlen(row[i]))		/* this technique also allows me to skip my own acc_number */
				{											
					if (strncmp(acc_num, row[i], strlen(row[i])) == 0)
					{
						t = atof(row[i + 2]);									/* grab balance current user*/

						break;
					}
				}

				if (strlen(a_n) == strlen(row[i]))
				{
					if (strncmp(a_n, row[i], strlen(row[i])) == 0)
					{
						flag = 1;

						t1 = atof(row[i + 2]);									/* grab balance other user */
						strcpy(temp_user, row[i + 1]);		/* grab username of other user */
					}
				}
			}	
		}
	}

	if (flag == 1)
	{
		system("clear");

		if (t == 0.0)
		{
			system("clear");

			printf("\n || Balance : $0.00       ||\n");
			printf(" || Not able to Withdraw ||\n\n");

			sleep(3);

			system("clear");

			return;
		}

		printf("\t\t\tYOUR BALANCE: %.2f\n\n", t);

		printf("Transfer Amount $: ");
		scanf("%f", &money);

		if (money > t)
		{
			system("clear");

			printf("\n\nInvalid Withdraw: Can't withdraw more than you have...\n\n");

			sleep(3);

			system("clear");

			return;
		}

		left = t - money;		/* left will go back in user account */

		/* update user account balance first */

		// ============================================================

		char user_esc[(2 * strlen(user)) + 1];

		mysql_real_escape_string(conn, user_esc, user, strlen(user));

		snprintf(array, 500, "UPDATE User_Acc set balance='%f' where username='%s'", left, user_esc);

		if (mysql_real_query(conn, array, strlen(array)))
		{
			error_msg(conn);
		}

		// ================================================================

		t1 = t1 + money;

		/* update other user account balance */

		char other_esc[(2 * strlen(temp_user)) + 1];

		mysql_real_escape_string(conn, other_esc, temp_user, strlen(temp_user));

		snprintf(array_1, 500, "UPDATE User_Acc set balance='%f' where username='%s'", t1, other_esc);

		if (mysql_real_query(conn, array_1, strlen(array_1)))
		{
			error_msg(conn);
		}

		mysql_free_result(res);

		system("clear");

		for (i = 0; i < 3; i++)
		{
			printf("Transfering...\n");

			sleep(1);
		}

		printf("\nProcess Done!...\n\n");

		sleep(2);

		system("clear");

		return;
	}

	else if (flag == 0)
	{
		system("clear");

		printf("\nNo matches of that [Acc No.]\n");
		printf("Please try again...\n\n");

		mysql_free_result(res);

		sleep(2);

		system("clear");

		return;
	}	
}

// ===============================================================================================================
// ===============================================================================================================

/* In Progress */

int u_delete(char *user, MYSQL *conn)	/* user deletes account */
{	
	int c, i;
	char array[500], arr[500];

	do
	{
		system("clear");

		printf("Doing this will [DELETE EVERYTHING]\n");
		printf("ARE YOU SURE?\n\n");

		printf("Hit [(1)-yes / (0)-no]: ");
		scanf("%d", &c);

		if (c == 0)		/* dont do this (T.T) XD */
		{
			system("clear");

			printf("\nTHAT WAS A CLOSE CALL...\n\n");

			sleep(2);

			return 0;
		}

	}while (c > 1 || c < 0);


	if (c == 1)				/* if you want to delete */
	{
		/* get result for checking later */
		/* get it from Bank_Users first */

		/**********************************************/

		const char *p1 = "SELECT * FROM Bank_Users";

		if (mysql_real_query(conn, p1, strlen(p1)))
		{
			error_msg(conn);
		}

		MYSQL_RES *r1 = mysql_store_result(conn);

		if (r1 == NULL)
		{
			fprintf(stderr, "%s\n", mysql_error(conn));

			exit(1);
		}

		/*********************************************/

		/**************************************************/
		/* Delete the user in --> Bank_Users */

		char user_esc[(2 * strlen(user)) + 1];	

		mysql_real_escape_string(conn, user_esc, user, strlen(user));

		snprintf(array, 500, "DELETE FROM Bank_Users WHERE username='%s'", user_esc);

		if (mysql_real_query(conn, array, strlen(array)))
		{
			error_msg(conn);
		}

		/************************************************************************/

		/* Delete the user in --> User_Acc */

		snprintf(arr, 500, "DELETE FROM User_Acc WHERE username='%s'", user_esc);

		if (mysql_real_query(conn, arr, strlen(arr)))
		{
			error_msg(conn);
		}

		/**************************************************************************/

		/* we need to check if we have deleted all users from table. */
		/* check Bank_Users only because User_Acc will be same as Bank_Users so... only do once */

		int rows = mysql_num_rows(r1);

		if (rows == 0)
		{
			/* set it back properly */

			const char *p3 = "ALTER TABLE Bank_Users AUTO_INCREMENT = 1";

			if (mysql_real_query(conn, p3, strlen(p3)))
			{
				error_msg(conn);
			}

			const char *p4 = "ALTER TABLE User_Acc AUTO_INCREMENT = 1";

			if (mysql_real_query(conn, p4, strlen(p4)))
			{
				error_msg(conn);
			}
		}

		else   		/* if the rows are not 0 yet */
		{
			char temp[500], temp1[500];

			snprintf(temp, 500, "ALTER TABLE Bank_Users AUTO_INCREMENT = %d", rows);
			snprintf(temp1, 500, "ALTER TABLE User_Acc AUTO_INCREMENT = %d", rows);

			if (mysql_real_query(conn, temp, strlen(temp)))
			{
				error_msg(conn);
			}

			if (mysql_real_query(conn, temp1, strlen(temp1)))
			{
				error_msg(conn);
			}

		}

		mysql_free_result(r1);

		system("clear");

		for (i = 0; i < 3; i++)
		{
			printf("Deleting...\n");
			sleep(1);
		}

		printf("\nACCOUNT DELETED!!...\n\n");

		sleep(2);

		system("clear");

		return 1;
	}

}

// ===============================================================================================================
// ===============================================================================================================











