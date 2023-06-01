#!/bin/bash

types='- c d l s b p'
for i in $types
do     
    echo $i
    file_search=`ls -lR $1 | grep ^$i | head -1`
	if [[ -z $file_search ]]
	then
		echo "notfound"
	else		
		echo "$file_search"
	fi
done
