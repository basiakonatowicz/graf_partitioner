CC = gcc
CFLAGS = -Wall -g -O2
OBJ = podzial_grafu.o graf.o wejscie_wyjscie.o podzial_algorytmy.o
EXEC = podzial_grafu

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

podzial_grafu.o: podzial_grafu.c graf.h wejscie_wyjscie.h podzial_algorytmy.h
	$(CC) $(CFLAGS) -c podzial_grafu.c

graf.o: graf.c graf.h
	$(CC) $(CFLAGS) -c graf.c

wejscie_wyjscie.o: wejscie_wyjscie.c wejscie_wyjscie.h graf.h
	$(CC) $(CFLAGS) -c wejscie_wyjscie.c

podzial_algorytmy.o: podzial_algorytmy.c podzial_algorytmy.h graf.h
	$(CC) $(CFLAGS) -c podzial_algorytmy.c

clean:
	rm -f $(OBJ) $(EXEC)