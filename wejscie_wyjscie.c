#include "wejscie_wyjscie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void zapisz_wynik_do_pliku(const char *nazwa_pliku, CzescGrafu *czesci, int liczba_czesci, int przeciete_krawedzie, int zapis_binarny) {
    FILE *plik = fopen(nazwa_pliku, zapis_binarny ? "wb" : "w");
    if (!plik) {
        printf("Nie można otworzyć pliku %s do zapisu.\n", nazwa_pliku);
        return;
    }

    if (zapis_binarny) {
        // Zapis binarny
        fwrite(&liczba_czesci, sizeof(int), 1, plik);
        fwrite(&przeciete_krawedzie, sizeof(int), 1, plik);
        
        // Zapis podziału
        for (int i = 0; i < liczba_czesci; i++) {
            fwrite(&czesci[i].liczba_wierzcholkow, sizeof(int), 1, plik);
            fwrite(czesci[i].wierzcholki, sizeof(int), czesci[i].liczba_wierzcholkow, plik);
        }
    } else {
        // Zapis tekstowy
        fprintf(plik, "%d\n", liczba_czesci);
        fprintf(plik, "%d\n", przeciete_krawedzie);
        
        // Zapis podziału
        for (int i = 0; i < liczba_czesci; i++) {
            fprintf(plik, "%d", czesci[i].liczba_wierzcholkow);
            for (int j = 0; j < czesci[i].liczba_wierzcholkow; j++) {
                fprintf(plik, " %d", czesci[i].wierzcholki[j]);
            }
            fprintf(plik, "\n");
        }
    }

    fclose(plik);
}

void zapisz_wynik_do_pliku_nowy(const char *nazwa_pliku, Graf *graf, CzescGrafu *czesci, int liczba_czesci, int przeciete_krawedzie, float margines, int zapis_binarny) {
    FILE *plik = fopen(nazwa_pliku, zapis_binarny ? "wb" : "w");
    if (!plik) {
        printf("Nie można otworzyć pliku %s do zapisu.\n", nazwa_pliku);
        return;
    }

    if (zapis_binarny) {
        // Zapis binarny
        fwrite(&graf->liczba_wierzcholkow, sizeof(int), 1, plik);
        fwrite(&graf->liczba_krawedzi, sizeof(int), 1, plik);
        fwrite(&liczba_czesci, sizeof(int), 1, plik);
        fwrite(&przeciete_krawedzie, sizeof(int), 1, plik);
        fwrite(&margines, sizeof(float), 1, plik);
        
        // Zapis oryginalnego grafu
        fwrite(graf->indeksy_wierzcholkow, sizeof(int), graf->liczba_krawedzi, plik);
        fwrite(graf->wskazniki_wiersze, sizeof(int), graf->liczba_wierzcholkow + 1, plik);
        
        // Zapis podziału
        for (int i = 0; i < liczba_czesci; i++) {
            fwrite(&czesci[i].liczba_wierzcholkow, sizeof(int), 1, plik);
            fwrite(czesci[i].wierzcholki, sizeof(int), czesci[i].liczba_wierzcholkow, plik);
        }
    } else {
        // Zapis tekstowy
        fprintf(plik, "%d\n", graf->liczba_wierzcholkow);
        fprintf(plik, "%d\n", graf->liczba_krawedzi);
        fprintf(plik, "%d\n", liczba_czesci);
        fprintf(plik, "%d\n", przeciete_krawedzie);
        fprintf(plik, "%.1f\n", margines);
        
        // Zapis oryginalnego grafu
        for (int i = 0; i < graf->liczba_krawedzi; i++) {
            fprintf(plik, "%d", graf->indeksy_wierzcholkow[i]);
            if (i < graf->liczba_krawedzi - 1) fprintf(plik, ";");
        }
        fprintf(plik, "\n");
        
        for (int i = 0; i <= graf->liczba_wierzcholkow; i++) {
            fprintf(plik, "%d", graf->wskazniki_wiersze[i]);
            if (i < graf->liczba_wierzcholkow) fprintf(plik, ";");
        }
        fprintf(plik, "\n");
        
        // Zapis podziału
        for (int i = 0; i < liczba_czesci; i++) {
            fprintf(plik, "%d", czesci[i].liczba_wierzcholkow);
            for (int j = 0; j < czesci[i].liczba_wierzcholkow; j++) {
                fprintf(plik, " %d", czesci[i].wierzcholki[j]);
            }
            fprintf(plik, "\n");
        }
    }

    fclose(plik);
}