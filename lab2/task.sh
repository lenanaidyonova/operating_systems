#!/bin/bash

full_dir=`readlink -f $@`

if [ -d "$@" ]; then
    t="это директория"
elif [ -f "$@" ]; then
    t="это регулярный файл"
elif [ -b "$@" ]; then
    t="это специальный файл блочного устройства"
elif [ -L "$@" ]; then
    t="это символьная ссылка"
elif [ -p "$@" ]; then
    t="это FIFO"
elif [ -S "$@" ]; then
    t="это сокет"
else
    t="это файл символьного устройства"
fi


echo "$full_dir" -- "$t"

