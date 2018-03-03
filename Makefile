

all: clean backdoor compile

compile:
	gcc -std=c99 -std=gnu99 src/poisønRunLevel.c -o poison -Ilib/ 

backdoor:
	gcc src/backd00r.c -o noip 

clean:
	rm -f opendoor poison unit.service
