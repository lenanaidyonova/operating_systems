#!/bin/bash

for i in {1..300}
do
	(echo -e "hi $i \n")  |  (./client & ) 
  
done
