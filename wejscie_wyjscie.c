#include "wejscie_wyjscie.h"
#include <stdio.h>
#include <stdlib.h>

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
        for (int i = 0; i < liczba_czesci; i++) {
            fwrite(&czesci[i].liczba_wierzcholkow, sizeof(int), 1, plik);
            fwrite(czesci[i].wierzcholki, sizeof(int), czesci[i].liczba_wierzcholkow, plik);
        }
    } else {
        // Zapis tekstowy
        fprintf(plik, "%d\n", liczba_czesci);
        fprintf(plik, "%d\n", przeciete_krawedzie);
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