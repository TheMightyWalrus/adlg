adlg: adlstring.o fstatemachine.o
	gcc -Wall --std=c11 -o adlg adlg.c ./include/adlstring.o ./include/fstatemachine.o 	
adlstring.o: fstatemachine.o
	gcc -Wall --std=c11 -c -o ./include/adlstring.o ./include/adlstring.c 
	
fstatemachine.o:
	gcc -Wall --std=c11 -c -o ./include/fstatemachine.o ./include/fstatemachine.c
