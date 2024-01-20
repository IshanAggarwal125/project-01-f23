MYps: MYps.c
	gcc -o MYps MYps.c -Wall -Werror
warm: warmup.c
	gcc -o warm warmup.c -Wall -Werror
clean:
	rm *.o MYps
