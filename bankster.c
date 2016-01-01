#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>	/* very important file XD */
#include <my_global.h>
#include <signal.h>

int count = 0;
int lines = 0;

// ========================================================

#include "menus.c"			/* menu location */
#include "user_register.c"	/* user register program */
#include "user_login.c"		/*  user login program */
#include "admin_register.c"
#include "admin_panel.c"

#include "colors.h"

// ========================================================

void a_trap(int signum);

int main(int argc, int *argv[])
{
	// this is done to just create a database at the start of program if it does not exists.
	// this is the first time im doing it. so try to bare with the horrible code. XD
	/* *****************************************************************************************************/
	signal(SIGINT, a_trap);

	static char *host = "localhost";	/* you need to change this fields according */
	static char *user = "banker";
	static char *pass = "";

	MYSQL *con = mysql_init(NULL);

	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));

		exit(1);
	}

	if (mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)	/* connect successfully */
	{
		error_msg(con);
	}

	const char *q1 = "CREATE DATABASE IF NOT EXISTS B_M_S";

	if (mysql_real_query(con, q1, strlen(q1)))	/* C1 */
	{
		error_msg(con);
	}

	mysql_close(con);

	/* ********************************************************************************************************/
														/* lets try to keep it simple but efficient. */
	srand((unsigned int)time(0) + getpid());

	int ch;

	while (1)
	{
		bank_menu();

		ch = choice();

		if (ch == 1)	/* if user-login */
		{
			user_log();
		}

		else if (ch == 2)	/* if user register. */
		{
			user_reg();
		}

		else if(ch == 99)
		{
			admin_reg();
		}

		else if(ch == 255)
		{
			admin_login();
		}

		else
		{
			fprintf(stderr, "[%s!%s] Available are only options from the menu.\n", red, normal);
			exit(1);
		}

	}

	return 0;
}


void a_trap(int signum)
{
	printf("\n[%s!%s] CTRL + C detected. Shutting down...\n", red, normal);
	printf("[%s!%s] All active sessions was logged out.\n", red, normal);
	exit(0);
}
