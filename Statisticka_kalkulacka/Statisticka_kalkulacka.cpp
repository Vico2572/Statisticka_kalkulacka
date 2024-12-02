// Statisticka_kalkulacka.cpp : Defines the entry point for the application.
//

#include "Statisticka_kalkulacka.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

using namespace std;

void Hlavicka() {
	printf("STATISTICKA KALKULACKA\n\n---------------------------------\n\nOperacie:\nAritmeticky priemer\nVazeny priemer\nRozptyl\nSmerodajna odchylka\nMedian\nHistogram\n\nTextove statistiky:\nPocet znakov\nPocet cisel\nPocet cifier v jednotlivych cislach\n\n---------------------------------\n\n");
}

float ArPriemer(const char* vstup, const char* vahy, int jeVazeny) {
    float sum = 0.0, sum_weights = 0.0, cislo, vaha = 1.0;
    char temp[1000], temp2[1000];
    strcpy(temp, vstup);
    if (jeVazeny && vahy != NULL) {
        strcpy(temp2, vahy);
    }

    char* token_cislo = strtok(temp, " ");
    char* token_vaha = jeVazeny && vahy != NULL ? strtok(temp2, " ") : NULL;

    while (token_cislo != NULL) {
        cislo = atof(token_cislo);
        if (jeVazeny && token_vaha != NULL) {
            vaha = atof(token_vaha);
            token_vaha = strtok(NULL, " ");
        }
        else {
            vaha = 1.0;
        }
        sum += cislo * vaha;
        sum_weights += vaha;
        token_cislo = strtok(NULL, " ");
    }

    return sum_weights > 0 ? sum / sum_weights : 0.0;
}

float Rozptyl(const char* vstup) {
    float sum = 0.0, sum_squares = 0.0, cislo;
    int count = 0;

    char temp[1000];
    strcpy(temp, vstup);
    char* token = strtok(temp, " ");

    while (token != NULL) {
        cislo = atof(token);
        sum += cislo;
        sum_squares += cislo * cislo;
        count++;
        token = strtok(NULL, " ");
    }

    if (count > 1) {
        float mean = sum / count;
        return (sum_squares / count) - (mean * mean);
    }
    return 0.0;
}

float Median(const char* vstup) {
    float cisla[1000];
    int count = 0;

    char temp[1000];
    strcpy(temp, vstup);
    char* token = strtok(temp, " ");

    while (token != NULL) {
        cisla[count++] = atof(token);
        token = strtok(NULL, " ");
    }

    if (count == 0) return 0.0;

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (cisla[i] > cisla[j]) {
                float tmp = cisla[i];
                cisla[i] = cisla[j];
                cisla[j] = tmp;
            }
        }
    }

    return count % 2 == 0 ? (cisla[count / 2 - 1] + cisla[count / 2]) / 2 : cisla[count / 2];
}

int PocetZnakov(const char* vstup) {
    int count = 0;
    for (int i = 0; vstup[i] != '\0'; i++) {
        if (!isspace(vstup[i])) count++;
    }
    return count;
}

void Histogram(const char* vstup, int* dlzky, int* pocetCisel) {
    int intervaly[10] = { 0 }, count = 0;
    float cisla[1000];

    char temp[1000];
    strcpy(temp, vstup);
    char* token = strtok(temp, " ");

    while (token != NULL) {
        cisla[count] = atof(token);
        dlzky[count] = strlen(token);
        count++;
        token = strtok(NULL, " ");
    }

    *pocetCisel = count;

    printf("\nHistogram:\n");
    for (int i = 0; i < count; i++) {
        int index = (int)cisla[i] / 10;
        if (index >= 0 && index < 10) {
            intervaly[index]++;
        }
    }

    for (int i = 0; i < 10; i++) {
        printf("%2d - %2d | ", i * 10, i * 10 + 9);
        for (int j = 0; j < intervaly[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
}

void UlozitVysledky(const char* cesta, const char* obsah) {
    FILE* subor = fopen(cesta, "w");
    if (subor != NULL) {
        fprintf(subor, "%s", obsah);
        fclose(subor);
        printf("\nVysledky boli ulozene do suboru: %s\n", cesta);
    }
    else {
        printf("\nNepodarilo sa otvorit subor pre zapis\n");
    }
}

int main() {
    Hlavicka();

    char vstup1[1000], vstup2[1000];
    printf("Zadajte cisla oddelene medzerou od 0 po 99 (pre koniec zadavania stlacte enter):\n");
    fgets(vstup1, sizeof(vstup1), stdin);
    vstup1[strcspn(vstup1, "\n")] = '\0';
    printf("\nZadajte vahy cisel oddelene medzerou (pre koniec zadavania stlacte enter):\n");
    fgets(vstup2, sizeof(vstup2), stdin);
    vstup2[strcspn(vstup2, "\n")] = '\0';

    float arpriemer = ArPriemer(vstup1, NULL, 0);
    printf("\nAritmeticky priemer: %.4lf\n", arpriemer);

    float vapriemer = ArPriemer(vstup1, vstup2, 1);
    printf("\nVazeny priemer: %.4lf\n", vapriemer);

    float rozptyl = Rozptyl(vstup1);
    printf("\nRozptyl: %.4lf\n", rozptyl);

    float s_odchylka = sqrt(rozptyl);
    printf("\nSmerodajna odchylka: %.4lf\n", s_odchylka);

    float median = Median(vstup1);
    printf("\nMedian: %.4lf\n", median);

    int pocetZnakov = PocetZnakov(vstup1);
    printf("\nPocet znakov (bez medzier): %d\n", pocetZnakov);

    int dlzky[1000] = { 0 }, pocetCisel = 0;
    Histogram(vstup1, dlzky, &pocetCisel);
    printf("\nPocet zadanych cisel: %d\n", pocetCisel);

    char vysledky[2000];
    snprintf(vysledky, sizeof(vysledky),
        "Pocet znakov: %d\nPocet zadaných čísel: %d\n", pocetZnakov, pocetCisel);
    strcat(vysledky, "Dlzky jednotlivych cisel:\n");
    for (int i = 0; i < pocetCisel; i++) {
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "Cislo %d ma dlzku: %d znakov\n", i + 1, dlzky[i]);
        strcat(vysledky, buffer);
    }

    const char* cesta = "C:\\Users\\simiv\\source\\repos\\Statisticka_kalkulacka\\Statisticka_kalkulacka\\Vystup.txt";
    UlozitVysledky(cesta, vysledky);

    return 0;
}
