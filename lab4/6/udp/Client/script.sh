#!/bin/bash
IP=127.0.0.1
PORT=8888

# loop through 10 times
for i in {1..300}
do
	(echo -e "hi $i" ) | ( ./Client $IP $PORT & ) 
  
done
