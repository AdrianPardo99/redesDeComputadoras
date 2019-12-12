#!/usr/bin/env bash
echo "Compilando archivo crudo.c"
gcc -I/usr/include/mysql/ crudo.c -L/usr/lib/mysql -lmysqlclient -o crudo
echo "Compilación correcta"
