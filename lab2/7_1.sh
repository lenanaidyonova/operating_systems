#! /bin/bash

size=`du -hs $1`
echo Текущий размер директории: $size

for i in {1..500}
do
    name=$1/$i.txt
    > $name
    echo "aaaaaaaaaaaaaaaaaaaaaaaaa" >> $name
done

size=`du -hs $1`
echo "Текущий размер директории(после добавления файлов):" $size

for i in {1..100}
do
    mkdir $1/$i
done

size=`du -hs $1`
echo "Текущий размер директории(после добавления подкатологов):" $size

for i in {1..250}
do
    name=$1/$i.txt
    rm $name
done

size=`du -hs $1`
echo "Текущий размер директории(после удаления файлов):" $size

for i in {1..50}
do
    rm -rf $1/$i
done

size=`du -hs $1`
echo "Текущий размер директории(после удаления подкаталогов):" $size
