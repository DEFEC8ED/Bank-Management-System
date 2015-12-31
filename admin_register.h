#include "colors.h"

#define SIZE 64
#define MAX 512

void admin_reg(void);
void registration_header(void);
void cleantime();
char add_admin(MYSQL *c, char *un, char *p, char *e);
char same_username_check(MYSQL *c, char *un, char *pa, char *em);
char check_it();

typedef struct registration_data {

	char admin_username[SIZE];
	char admin_password[SIZE];
	char admin_email[SIZE];
	
} Reg;


MYSQL_ROW row_1;
MYSQL_ROW row_2;

char check_it()
{
	const char *host = "localhost";
	const char *user = "banker";
	const char *pass = "";

	MYSQL *con = mysql_init(NULL);
	mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0);
	mysql_select_db(con, "B_M_S");

	const char  *q_1 = "SELECT username FROM Admins WHERE aid=1";
	if(mysql_real_query(con, q_1, strlen(q_1)))
	{
		error_msg(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
	{
		error_msg(con);
	}
	row_1 = mysql_fetch_row(result);

	
	const char *q_2 = "SELECT username FROM Admins WHERE aid=2";
	if(mysql_real_query(con, q_2, strlen(q_2)))
	{
		error_msg(con);
	}

	result = mysql_store_result(con);
	if(result == NULL)
	{
		error_msg(con);
	}
	row_2 = mysql_fetch_row(result);

	printf("%s\n", row_1[0]);
	printf("%s\n", row_2[0]);
}