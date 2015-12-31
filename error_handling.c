/*
Not only real error handlings, but all very often functions
for checking duplicate usernames, NOT null etc..
*/
#include "colors.h"

void error_msg(MYSQL *c);
void a_trap(int signum);


void error_msg(MYSQL *c)
{
	fprintf(stderr, "%u - %s\n", mysql_errno(c), mysql_error(c));
	mysql_close(c);
	exit(1);
}


void a_trap(int signum)
{
	printf("\n[%s!%s] CTRL + C detected. Shutting down...\n", red, normal);
	printf("[%s!%s] All active sessions was logged out.\n", red, normal);
	exit(0);
}