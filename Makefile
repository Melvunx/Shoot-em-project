all: main_jeu

Affichage.o: Affichage.c Affichage.h
	gcc -c -Wall Affichage.c

Tourelle.o: Tourelle.c Tourelle.h
	gcc -c -Wall Tourelle.c

Canon.o: Canon.c Canon.h
	gcc -c -Wall Canon.c

Canard.o: Canard.c Canard.h
	gcc -c -Wall Canard.c

Flechette.o: Flechette.c Flechette.h
	gcc -c -Wall Flechette.c

main_jeu.o: main_jeu.c Affichage.h
	gcc -c -Wall main_jeu.c

main_jeu_w: main_jeu.o Affichage.o Canon.o Canard.o Flechette.o
	gcc -o main_jeu.exe main_jeu.o Affichage.o Canon.o Canard.o Flechette.o

main_jeu: main_jeu.o Affichage.o Canon.o Canard.o Flechette.o Tourelle.o
	gcc -o main_jeu main_jeu.o Affichage.o Canon.o Canard.o Flechette.o Tourelle.o -lncurses

clean:
	rm -fr *~ *.o main_jeu

cleanW:
	del /f /q *.o main_jeu.exe 2>nul

run: main_jeu
	./main_jeu

run_easy: main_jeu
	./main_jeu 50 50 7

run_mid: main_jeu
	./main_jeu 50 50 10

run_hard: main_jeu
	./main_jeu 50 50 12
