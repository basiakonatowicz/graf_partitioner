#include "graf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funkcja pomocnicza do wczytywania linii z danymi oddzielonymi średnikami
int* wczytaj_linie(FILE *plik, int *rozmiar) {
    char linia[4096]; // Większy bufor na długie linie
    if (!fgets(linia, sizeof(linia), plik)) {
        return NULL;
    }

    // Zliczanie elementów
    int count = 1;
    for (char *p = linia; *p; p++) {
        if (*p == ';') count++;
    }

    int *tablica = (int *)malloc(count * sizeof(int));
    if (!tablica) return NULL;

    // Parsowanie elementów
    char *token = strtok(linia, ";");
    int i = 0;
    while (token && i < count) {
        tablica[i++] = atoi(token);
        token = strtok(NULL, ";");
    }

    *rozmiar = count;
    return tablica;
}

Graf* wczytaj_graf_z_pliku(const char *nazwa_pliku) {
    FILE *plik = fopen(nazwa_pliku, "r");
    if (!plik) {
        printf("Nie można otworzyć pliku %s.\n", nazwa_pliku);
        return NULL;
    }

    Graf *graf = (Graf *)malloc(sizeof(Graf));
    if (!graf) {
        printf("Błąd alokacji pamięci dla grafu.\n");
        fclose(plik);
        return NULL;
    }

    // Wczytanie liczby wierzchołków
    if (fscanf(plik, "%d\n", &graf->liczba_wierzcholkow) != 1) {
        printf("Błąd wczytywania liczby wierzchołków.\n");
        free(graf);
        fclose(plik);
        return NULL;
    }

    // Wczytanie indeksów wierzchołków (lista sąsiadów)
    int rozmiar_indeksow;
    graf->indeksy_wierzcholkow = wczytaj_linie(plik, &rozmiar_indeksow);
    if (!graf->indeksy_wierzcholkow) {
        printf("Błąd wczytywania indeksów wierzchołków.\n");
        free(graf);
        fclose(plik);
        return NULL;
    }
    graf->liczba_krawedzi = rozmiar_indeksow; // Liczba krawędzi = długość listy sąsiadów

    // Wczytanie wskaźników wierszy
    int rozmiar_wskaznikow;
    graf->wskazniki_wiersze = wczytaj_linie(plik, &rozmiar_wskaznikow);
    if (!graf->wskazniki_wiersze || rozmiar_wskaznikow != graf->liczba_wierzcholkow + 1) {
        printf("Błąd wczytywania wskaźników wierszy.\n");
        free(graf->indeksy_wierzcholkow);
        free(graf);
        fclose(plik);
        return NULL;
    }

    // Wczytanie grup wierzchołków
    int rozmiar_grup;
    graf->grupy_wierzcholkow = wczytaj_linie(plik, &rozmiar_grup);
    if (!graf->grupy_wierzcholkow) {
        printf("Błąd wczytywania grup wierzchołków.\n");
        free(graf->wskazniki_wiersze);
        free(graf->indeksy_wierzcholkow);
        free(graf);
        fclose(plik);
        return NULL;
    }

    // Wczytanie wskaźników grup
    int rozmiar_wskaznikow_grup;
    graf->wskazniki_grupy = wczytaj_linie(plik, &rozmiar_wskaznikow_grup);
    if (!graf->wskazniki_grupy) {
        printf("Błąd wczytywania wskaźników grup.\n");
        free(graf->grupy_wierzcholkow);
        free(graf->wskazniki_wiersze);
        free(graf->indeksy_wierzcholkow);
        free(graf);
        fclose(plik);
        return NULL;
    }

    fclose(plik);
    return graf;
}

void zwolnij_graf(Graf *graf) {
    if (graf) {
        free(graf->indeksy_wierzcholkow);
        free(graf->wskazniki_wiersze);
        free(graf->grupy_wierzcholkow);
        free(graf->wskazniki_grupy);
        free(graf);
    }
}