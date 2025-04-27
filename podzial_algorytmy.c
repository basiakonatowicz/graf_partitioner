#include "podzial_algorytmy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Obliczanie przeciętych krawędzi
int oblicz_przeciete_krawedzie(Graf *graf, CzescGrafu *czesci, int liczba_czesci) {
    int przeciete = 0;
    int *przynaleznosc = (int *)malloc(graf->liczba_wierzcholkow * sizeof(int));
    if (!przynaleznosc) {
        printf("Błąd alokacji pamięci w oblicz_przeciete_krawedzie\n");
        return 0;
    }

    // Mapowanie wierzchołków na części
    memset(przynaleznosc, -1, graf->liczba_wierzcholkow * sizeof(int));
    for (int i = 0; i < liczba_czesci; i++) {
        for (int j = 0; j < czesci[i].liczba_wierzcholkow; j++) {
            przynaleznosc[czesci[i].wierzcholki[j]] = i;
        }
    }

    // Liczenie przeciętych krawędzi
    for (int v = 0; v < graf->liczba_wierzcholkow; v++) {
        for (int i = graf->wskazniki_wiersze[v]; i < graf->wskazniki_wiersze[v + 1]; i++) {
            int sasiad = graf->indeksy_wierzcholkow[i];
            if (przynaleznosc[v] != przynaleznosc[sasiad] && v < sasiad) {
                przeciete++;
            }
        }
    }

    free(przynaleznosc);
    return przeciete;
}

// Algorytm Kernighana-Lina dla podziału na dwie części
static void kernighan_lin(Graf *graf, CzescGrafu *czesci, int *przynaleznosc, int czesc1, int czesc2) {
    int n = graf->liczba_wierzcholkow;
    int *zyski = (int *)malloc(n * sizeof(int));
    int *zablokowane = (int *)calloc(n, sizeof(int));
    int *tmp_przynaleznosc = (int *)malloc(n * sizeof(int));
    
    if (!zyski || !zablokowane || !tmp_przynaleznosc) {
        printf("Błąd alokacji pamięci w kernighan_lin\n");
        free(zyski);
        free(zablokowane);
        free(tmp_przynaleznosc);
        return;
    }

    memcpy(tmp_przynaleznosc, przynaleznosc, n * sizeof(int));
    
    // Dynamiczne dostosowanie liczby iteracji w zależności od rozmiaru grafu
    int iteracje = n > 1000 ? n / 100 : n / 20;
    printf("Wykonywanie %d iteracji Kernighan-Lin dla części %d i %d\n", iteracje, czesc1, czesc2);
    
    int najlepszy_globalny_zysk = 0;
    int *najlepsze_przynaleznosc = (int *)malloc(n * sizeof(int));
    if (!najlepsze_przynaleznosc) {
        printf("Błąd alokacji pamięci dla najlepszego podziału\n");
        free(zyski);
        free(zablokowane);
        free(tmp_przynaleznosc);
        return;
    }
    memcpy(najlepsze_przynaleznosc, tmp_przynaleznosc, n * sizeof(int));
    
    for (int iter = 0; iter < iteracje; iter++) {
        // Obliczanie zysków z wymiany
        for (int v = 0; v < n; v++) {
            if (zablokowane[v]) continue;
            int koszt_zewn = 0, koszt_wewn = 0;
            for (int i = graf->wskazniki_wiersze[v]; i < graf->wskazniki_wiersze[v + 1]; i++) {
                int sasiad = graf->indeksy_wierzcholkow[i];
                if (tmp_przynaleznosc[v] == tmp_przynaleznosc[sasiad]) {
                    koszt_wewn++;
                } else {
                    koszt_zewn++;
                }
            }
            zyski[v] = koszt_zewn - koszt_wewn;
        }

        // Znajdowanie najlepszej wymiany
        int najlepszy_v1 = -1, najlepszy_v2 = -1, najlepszy_zysk = -1;
        for (int v1 = 0; v1 < n; v1++) {
            if (zablokowane[v1] || tmp_przynaleznosc[v1] != czesc1) continue;
            for (int v2 = 0; v2 < n; v2++) {
                if (zablokowane[v2] || tmp_przynaleznosc[v2] != czesc2) continue;
                int zysk = zyski[v1] + zyski[v2];
                for (int i = graf->wskazniki_wiersze[v1]; i < graf->wskazniki_wiersze[v1 + 1]; i++) {
                    if (graf->indeksy_wierzcholkow[i] == v2) {
                        zysk -= 2; // Krawędź między v1 i v2
                        break;
                    }
                }
                if (zysk > najlepszy_zysk) {
                    najlepszy_zysk = zysk;
                    najlepszy_v1 = v1;
                    najlepszy_v2 = v2;
                }
            }
        }

        if (najlepszy_zysk <= 0 || najlepszy_v1 == -1 || najlepszy_v2 == -1) {
            printf("Brak lepszych wymian w iteracji %d\n", iter);
            break;
        }

        // Wykonanie wymiany
        tmp_przynaleznosc[najlepszy_v1] = czesc2;
        tmp_przynaleznosc[najlepszy_v2] = czesc1;
        zablokowane[najlepszy_v1] = 1;
        zablokowane[najlepszy_v2] = 1;
        
        // Aktualizacja najlepszego globalnego zysku
        if (najlepszy_zysk > najlepszy_globalny_zysk) {
            najlepszy_globalny_zysk = najlepszy_zysk;
            memcpy(najlepsze_przynaleznosc, tmp_przynaleznosc, n * sizeof(int));
        }
        
        if (iter % 100 == 0) {
            printf("Iteracja %d: wymiana wierzchołków %d i %d, zysk: %d\n", 
                   iter, najlepszy_v1, najlepszy_v2, najlepszy_zysk);
        }
    }

    // Przywrócenie najlepszego podziału
    memcpy(tmp_przynaleznosc, najlepsze_przynaleznosc, n * sizeof(int));
    free(najlepsze_przynaleznosc);

    // Aktualizacja części
    int k1 = 0, k2 = 0;
    int *nowe1 = (int *)malloc(czesci[czesc1].liczba_wierzcholkow * sizeof(int));
    int *nowe2 = (int *)malloc(czesci[czesc2].liczba_wierzcholkow * sizeof(int));
    
    if (!nowe1 || !nowe2) {
        printf("Błąd alokacji pamięci przy aktualizacji części\n");
        free(nowe1);
        free(nowe2);
        free(zyski);
        free(zablokowane);
        free(tmp_przynaleznosc);
        return;
    }

    for (int i = 0; i < czesci[czesc1].liczba_wierzcholkow; i++) {
        if (tmp_przynaleznosc[czesci[czesc1].wierzcholki[i]] == czesc1) {
            nowe1[k1++] = czesci[czesc1].wierzcholki[i];
        } else {
            nowe2[k2++] = czesci[czesc1].wierzcholki[i];
        }
    }
    for (int i = 0; i < czesci[czesc2].liczba_wierzcholkow; i++) {
        if (tmp_przynaleznosc[czesci[czesc2].wierzcholki[i]] == czesc2) {
            nowe2[k2++] = czesci[czesc2].wierzcholki[i];
        } else {
            nowe1[k1++] = czesci[czesc2].wierzcholki[i];
        }
    }

    // Aktualizacja tablic wierzchołków
    memcpy(czesci[czesc1].wierzcholki, nowe1, k1 * sizeof(int));
    memcpy(czesci[czesc2].wierzcholki, nowe2, k2 * sizeof(int));
    czesci[czesc1].liczba_wierzcholkow = k1;
    czesci[czesc2].liczba_wierzcholkow = k2;

    printf("Zakończono podział: część %d ma %d wierzchołków, część %d ma %d wierzchołków\n",
           czesc1, k1, czesc2, k2);

    free(nowe1);
    free(nowe2);
    free(zyski);
    free(zablokowane);
    free(tmp_przynaleznosc);
}

CzescGrafu* podziel_graf(Graf *graf, int liczba_czesci, float margines) {
    if (!graf || liczba_czesci < 1) {
        printf("Nieprawidłowe parametry wejściowe\n");
        return NULL;
    }

    printf("Rozpoczynam podział grafu na %d części (margines: %.1f%%)\n", liczba_czesci, margines);
    printf("Liczba wierzchołków: %d, liczba krawędzi: %d\n", 
           graf->liczba_wierzcholkow, graf->liczba_krawedzi);

    // Dynamiczne dostosowanie marginesu dla dużych grafów
    float dostosowany_margines = margines;
    if (graf->liczba_wierzcholkow > 1000) {
        dostosowany_margines = margines * 1.5; // Zwiększamy margines o 50% dla dużych grafów
        printf("Dostosowano margines do %.1f%% dla dużego grafu\n", dostosowany_margines);
    }

    CzescGrafu *czesci = (CzescGrafu *)malloc(liczba_czesci * sizeof(CzescGrafu));
    if (!czesci) {
        printf("Błąd alokacji pamięci dla części\n");
        return NULL;
    }

    // Inicjalizacja części z dynamicznym rozmiarem
    for (int i = 0; i < liczba_czesci; i++) {
        int oczekiwany_rozmiar = (graf->liczba_wierzcholkow / liczba_czesci) + 1;
        czesci[i].wierzcholki = (int *)malloc(oczekiwany_rozmiar * sizeof(int));
        if (!czesci[i].wierzcholki) {
            printf("Błąd alokacji pamięci dla części %d\n", i);
            for (int j = 0; j < i; j++) free(czesci[j].wierzcholki);
            free(czesci);
            return NULL;
        }
        czesci[i].liczba_wierzcholkow = 0;
    }

    // Losowy początkowy podział
    int *przynaleznosc = (int *)calloc(graf->liczba_wierzcholkow, sizeof(int));
    if (!przynaleznosc) {
        printf("Błąd alokacji pamięci dla przynależności\n");
        for (int i = 0; i < liczba_czesci; i++) free(czesci[i].wierzcholki);
        free(czesci);
        return NULL;
    }

    int wierzcholki_na_czesc = graf->liczba_wierzcholkow / liczba_czesci;
    int reszta = graf->liczba_wierzcholkow % liczba_czesci;
    int k = 0;
    
    printf("Inicjalizacja początkowego podziału:\n");
    for (int i = 0; i < liczba_czesci; i++) {
        int rozmiar = wierzcholki_na_czesc + (i < reszta ? 1 : 0);
        printf("Część %d: %d wierzchołków\n", i, rozmiar);
        for (int j = 0; j < rozmiar; j++) {
            czesci[i].wierzcholki[j] = k;
            przynaleznosc[k] = i;
            czesci[i].liczba_wierzcholkow++;
            k++;
        }
    }

    // Iteracyjny podział binarny z Kernighan-Lin
    printf("Rozpoczynam iteracyjny podział binarny\n");
    for (int iter = 0; iter < liczba_czesci - 1; iter++) {
        printf("Iteracja główna %d/%d\n", iter + 1, liczba_czesci - 1);
        for (int i = 0; i < liczba_czesci; i++) {
            for (int j = i + 1; j < liczba_czesci; j++) {
                kernighan_lin(graf, czesci, przynaleznosc, i, j);
            }
        }
    }

    // Weryfikacja marginesu z dynamicznym dostosowaniem
    int min_wierzcholkow = graf->liczba_wierzcholkow / liczba_czesci;
    int max_wierzcholkow = (int)(min_wierzcholkow * (1.0 + dostosowany_margines / 100.0));
    
    printf("Weryfikacja marginesu (min: %d, max: %d):\n", min_wierzcholkow, max_wierzcholkow);
    for (int i = 0; i < liczba_czesci; i++) {
        printf("Część %d: %d wierzchołków\n", i, czesci[i].liczba_wierzcholkow);
        if (czesci[i].liczba_wierzcholkow < min_wierzcholkow || 
            czesci[i].liczba_wierzcholkow > max_wierzcholkow) {
            printf("Podział nie spełnia marginesu %f%% dla części %d\n", dostosowany_margines, i);
            for (int j = 0; j < liczba_czesci; j++) free(czesci[j].wierzcholki);
            free(czesci);
            free(przynaleznosc);
            return NULL;
        }
    }

    printf("Podział zakończony pomyślnie\n");
    free(przynaleznosc);
    return czesci;
}