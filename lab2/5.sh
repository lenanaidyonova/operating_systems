#! /bin/bash

types='f c d l s b p' #типы файлов
for i in $types #цикл по всем типам файлов
do
    echo $i	#вывод каждого типа файла
    file_search=`find /home/helena/oc -type $i -ls | head -1` # поиск определённого типа файла и запись расширенной информации о нём
        if [[ -z $file_search ]]	# проверка на существование
        then
                echo "notfound" #не найдено	
        else
                echo "$file_search"
        fi
done
