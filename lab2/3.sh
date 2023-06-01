#! /bin/sh

filename=$1	#файл, на который необходимо найти символьные ссылки
ls -lRa / | grep $filename | grep ^l > symlinks.txt 	#поиск всех символьных ссылок
echo -n "total " >> symlinks.txt # вывод слова total 
wc -l symlinks.txt | cut -c 1 >> symlinks.txt	#посчёт количества найденных символьных ссылок

