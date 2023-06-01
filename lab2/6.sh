#! /bin/bash

> test6.txt #открыт файл
echo "Создан новый регулярный файл:" > res6.txt
od -tc test6.txt >> res6.txt 

echo "Hello! I am Helena! I am from Russia" >> test6.txt
echo "Файл был изменён(добавлен текст):" >> res6.txt
od -tc test6.txt >> res6.txt

echo "Hello! I am Helena!" > test6.txt
echo "Файл был изменён(удалена часть текста):" >> res6.txt
od -tc test6.txt >> res6.txt
