all: serveur
clean: 
	rm -rf *.o serveur_app.exe

serveur:
	gcc -o serveur_app.exe *.c
