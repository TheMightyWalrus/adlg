adlg: adlstring.o fstatemachine.o
	gcc -Wall --std=c11 -o adlg adlg.c ./include/adlstring.o ./include/fstatemachine.o ./include/stringbuffer.o	
adlstring.o: fstatemachine.o stringbuffer.o
	gcc -Wall --std=c11 -c -o ./include/adlstring.o ./include/adlstring.c 
	
fstatemachine.o:
	gcc -Wall --std=c11 -c -o ./include/fstatemachine.o ./include/fstatemachine.c

stringbuffer.o:
	gcc -Wall --std=c11 -c -o ./include/stringbuffer.o ./include/stringbuffer.c
