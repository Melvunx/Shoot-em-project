all: main_jeu

Affichage.o: Affichage.c Affichage.h 
	gcc -c -Wall Affichage.c

Canon.o: Canon.c Canon.h
	gcc -c -Wall Canon.c

Canard.o: Canard.c Canard.h Flechette.h
	gcc -c -Wall Canard.c

main_jeu.o: main_jeu.c Affichage.h 
	gcc -c -Wall main_jeu.c

main_jeu: main_jeu.o Affichage.o Canon.o 
	gcc -o main_jeu.exe main_jeu.o Affichage.o Canon.o

clean:
	rm -fr *~ *.o main_jeu

cleanW:
	del /f /q *.o main_jeu.exe 2>nul

run: main_jeu
	./main_jeu.exe

run_easy: main_jeu
	./main_jeu.exe 50 50 1