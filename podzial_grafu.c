#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graf.h"
#include "wejscie_wyjscie.h"
#include "podzial_algorytmy.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Za mało argumentów. Użycie: %s -i <plik_wejsciowy> -o <plik_wyjscowy> -p <liczba_czesci> -m <margines> [-b]\n", argv[0]);
        return 1;
    }

    const char *plik_wejsciowy = NULL;
    const char *plik_wyjscowy = NULL;
    int liczba_czesci = 2;
    float margines = 10.0; // Domyślnie margines 10%
    int zapis_binarny = 0; // Domyślnie zapis tekstowy

    // Parsowanie argumentów
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            plik_wejsciowy = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            plik_wyjscowy = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            liczba_czesci = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            margines = atof(argv[++i]);
        } else if (strcmp(argv[i], "-b") == 0) {
            zapis_binarny = 1;
        }
    }

    if (!plik_wejsciowy || !plik_wyjscowy) {
        printf("Brak pliku wejściowego lub wyjściowego.\n");
        return 1;
    }

    if (liczba_czesci < 1) {
        printf("Liczba części musi być dodatnia.\n");
        return 1;
    }

    printf("Parametry wejściowe:\n");
    printf("Plik wejściowy: %s\n", plik_wejsciowy);
    printf("Plik wyjściowy: %s\n", plik_wyjscowy);
    printf("Liczba części: %d\n", liczba_czesci);
    printf("Margines: %.1f%%\n", margines);
    printf("Zapis binarny: %s\n", zapis_binarny ? "tak" : "nie");

    // Wczytanie grafu
    printf("\nWczytywanie grafu z pliku %s...\n", plik_wejsciowy);
    Graf *graf = wczytaj_graf_z_pliku(plik_wejsciowy);
    if (!graf) {
        printf("Błąd wczytywania grafu.\n");
        return 1;
    }
    printf("Graf wczytany pomyślnie.\n");

    if (liczba_czesci > graf->liczba_wierzcholkow) {
        printf("Liczba części (%d) większa niż liczba wierzchołków (%d).\n", 
               liczba_czesci, graf->liczba_wierzcholkow);
        zwolnij_graf(graf);
        return 1;
    }

    // Podział grafu
    printf("\nRozpoczynam podział grafu...\n");
    CzescGrafu *czesci = podziel_graf(graf, liczba_czesci, margines);
    if (!czesci) {
        printf("Błąd podziału grafu.\n");
        zwolnij_graf(graf);
        return 1;
    }
    printf("Podział grafu zakończony pomyślnie.\n");

    // Obliczenie przeciętych krawędzi
    printf("\nObliczanie przeciętych krawędzi...\n");
    int przeciete_krawedzie = oblicz_przeciete_krawedzie(graf, czesci, liczba_czesci);
    printf("Liczba przeciętych krawędzi: %d\n", przeciete_krawedzie);

    // Zapis wyniku
    printf("\nZapisywanie wyniku do pliku %s...\n", plik_wyjscowy);
    zapisz_wynik_do_pliku(plik_wyjscowy, czesci, liczba_czesci, przeciete_krawedzie, zapis_binarny);
    printf("Wynik zapisany pomyślnie.\n");

    // Zwolnienie pamięci
    printf("\nZwolnienie pamięci...\n");
    for (int i = 0; i < liczba_czesci; i++) {
        free(czesci[i].wierzcholki);
    }
    free(czesci);
    zwolnij_graf(graf);
    printf("Pamięć zwolniona pomyślnie.\n");

    return 0;
}