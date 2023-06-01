#! /bin/sh

filename=$1 #ввод файла, на который необходимо найти ссылки
inode=`ls -i $filename | cut -c 1-8` #получение inode у введённого файла
ls -lRi / | grep $inode > hardlinks.txt	#поиск всех жестких ссылок по полученному inode и вывод в файл
