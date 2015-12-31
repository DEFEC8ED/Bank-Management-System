#include "admin_register.h"

void cleantime()
{
	while(getchar() != '\n');
}


// ================================================


void registration_header(void)
{
	printf("   _   _   _   _   _     _   _   _   _   _   _   _   _   _   _   _   _ \n");
	printf("  / \\ / \\ / \\ / \\ / \\   / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ \n");
	printf(" ( A | d | m | i | n ) ( R | e | g | i | s | t | r | a | t | i | o | n )\n");
	printf("  \\_/ \\_/ \\_/ \\_/ \\_/   \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \n");
	printf("\n");
}


// ================================================


void admin_reg(void) // Max. 2 admins
{

	static char *host = "localhost";
	static char *user = "banker";
	static char *pass = "";

	int i, n_f;
	
	Reg add[MAX];
	MYSQL *con = mysql_init(NULL);

	if(con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}

	const char *q_1 = "USE B_M_S";
	if(mysql_real_query(con, q_1, strlen(q_1)))
	{
		error_msg(con);
	}

	const char *q_2 = "CREATE TABLE IF NOT EXISTS Admins(aid INT NOT NULL AUTO_INCREMENT PRIMARY KEY, username VARCHAR(255) NOT NULL, password VARCHAR(255) NOT NULL, email VARCHAR(128) NOT NULL)";
	if(mysql_real_query(con, q_2, strlen(q_2)))
	{
		error_msg(con);
	}

	system("clear");	
	registration_header();

	cleantime();	
	printf("Enter New Username: ");
	fgets(add->admin_username, SIZE, stdin);

	printf("Enter New Password: ");
	fgets(add->admin_password, SIZE, stdin);

	printf("Email: ");
	fgets(add->admin_email, SIZE, stdin);

	strtok(add->admin_username, "\n");
	strtok(add->admin_password, "\n");
	strtok(add->admin_email, "\n");

	if(strstr(add->admin_email, "@") == NULL)
	{
		fprintf(stderr, "Email is in a wrong format!\n");
		exit(1);
	}

	//const char *q_3 = "SELECT COUNT(*) AS aid FROM Admins;";
	const char *q_3 = "SELECT username FROM Admins;";
	if(mysql_real_query(con, q_3, strlen(q_3)))
	{
		error_msg(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL) 
	{
    	error_msg(con);
	}

	MYSQL_ROW row;

	while((row = mysql_fetch_row(result)))  
	{
		n_f = mysql_num_fields(result);
		for(i = 0; i < n_f; i++)
		{
			if(strcmp(row[i], add->admin_username) == 0)
			{
				printf("[!] Username is in use.\n");
				exit(1);
			}
			else
			{
				add_admin(con, add->admin_username, add->admin_password, add->admin_email);
			}			
		}
	}

	add_admin(con, add->admin_username, add->admin_password, add->admin_email);

	mysql_free_result(result);
	mysql_close(con);
	exit(0);
}


// ================================================

char add_admin(MYSQL *c, char *un, char *p, char *e)
{
	const char *host = "localhost";
	const char *user = "banker";
	const char *pass= "";

	int i, n_f;

	char send[512];
	snprintf(send, sizeof(send), "INSERT INTO Admins VALUES('', '%s', '%s', '%s')", un, p, e);

	MYSQL *con = mysql_init(NULL);
	if(con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if(mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
	{
		error_msg(con);
	}

	
	const char *q_1 = "USE B_M_S";
	if(mysql_real_query(con, q_1, strlen(q_1)))
	{
		error_msg(con);
	}

	const char *q_2 = "SELECT COUNT(*) AS aid FROM Admins;";
	if(mysql_real_query(con, q_2, strlen(q_2)))
	{
		error_msg(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL) 
	{
    	error_msg(con);
	}

	MYSQL_ROW row;

	while((row = mysql_fetch_row(result))) // retrieving rows 
	{
		n_f = mysql_num_fields(result); // returns number of fields in each row
		for(i = 0; i < n_f; i++)
		{			
			if(atoi(row[i]) < 2)
			{
				mysql_query(c, send);
			}

			else
			{
				fprintf(stderr, "[%s!%s] There can be max. 2 admin accounts!\n", red, normal);
				exit(1);
			}
		}
	}
	printf("[+] Account sucessfully added.\n");

	mysql_close(con);
	mysql_free_result(result);
	exit(0);
}



/*char same_username_check(MYSQL *c, char *un, char *pa, char *em)
{
//	MYSQL *c = mysql_init(NULL);

//	mysql_real_connect(c, host, user, pass, NULL, 0, NULL, 0);
//	mysql_select_db(c, "B_M_S");

	const char  *q_1 = "SELECT username FROM Admins WHERE aid=1";
	if(mysql_real_query(c, q_1, strlen(q_1)))
	{
		error_msg(c);
	}

	MYSQL_RES *result = mysql_store_result(c);
	if(result == NULL)
	{
		error_msg(c);
	}

	MYSQL_ROW row_1 = mysql_fetch_row(result);
	//printf("%s\n", row_1[0]);
	
	const char *q_2 = "SELECT username FROM Admins WHERE aid=2";
	if(mysql_real_query(c, q_2, strlen(q_2)))
	{
		error_msg(c);
	}

	result = mysql_store_result(c);
	if(result == NULL)
	{
		error_msg(c);
	}

	MYSQL_ROW row_2 = mysql_fetch_row(result);

	if(!strcmp(un, row_1[0]) || (!strcmp(un, row_2[0])))
	{
		fprintf(stderr, "[!] This username is in use. Try another one.\n");
		exit(0);
	}

	mysql_close(c);
	mysql_free_result(result);
	return 0;
}
*/

//add_admin(con, add->admin_username, add->admin_password, add->admin_email);