#ifndef GRAF_H
#define GRAF_H

typedef struct {
    int liczba_wierzcholkow;
    int liczba_krawedzi;
    int *indeksy_wierzcholkow;  // Lista sąsiadów
    int *wskazniki_wiersze;     // Wskaźniki na początek wierszy
    int *grupy_wierzcholkow;    // Lista krawędzi/incydencji
    int *wskazniki_grupy;       // Wskaźniki na grupy wierzchołków
} Graf;

typedef struct {
    int *wierzcholki;
    int liczba_wierzcholkow;
} CzescGrafu;

Graf* wczytaj_graf_z_pliku(const char *nazwa_pliku);
void zwolnij_graf(Graf *graf);

#endif