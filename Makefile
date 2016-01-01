#
# Makefile for Bankster
#

CC = gcc
CFLAGS = -g `mysql_config --cflags --libs`

bms-main: bankster.c
	$(CC) bankster.c -o bankster $(CFLAGS)
