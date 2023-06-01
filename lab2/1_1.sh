#!/bin/bash

types='- c d l s b p' #возможные типы файлов
for i in $types #цикл по всем типам файлов
do
    echo $i     #вывод типа файла
    file_search=`ls -lR $1 | grep ^$i | head -1` #рекурсивный проход по директориям, выбор только определённого типа файла и сохранение первого попавшегося
    if [[ -z $file_search ]] #если пусто
    then
        echo "not found"        #не найдено
    else
        cmd=`ls -lR $1 | grep ^$i | head -1 | cut -b 51-1000`
        echo "$file_search -- `pwd`/$cmd"  #вывод полного пути
    fi
done
