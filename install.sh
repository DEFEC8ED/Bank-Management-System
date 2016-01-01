#!/bin/bash

if [[ $EUID -ne 0 ]]; then
	echo "You must be root for run this script!" 2>&1 | grep "root" --color
	exit 1
fi

echo "[+] Installing mysql-server package..." | grep "+" --color
sudo apt-get install mysql-server

echo "[+] Installig mysql client lib..." | grep "+" --color
sudo apt-get install libmysqlclient-dev
