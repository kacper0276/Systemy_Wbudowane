#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int dodawanie(int a, int b) {
	return a + b;
}

void czy_dodatnia(int liczba) {
	if (liczba > 0) {
		printf("Liczba jest dodatnia\n");
	} else {
		printf("Liczba nie jest dodatnia\n");
	}
}

int sumuj_tab(int tab[], int size) {
	int suma = 0;
	for (int i = 0; i < size; i++) {
		suma += tab[i];
	}

	return suma;
}

void kalkulator(int a, int b, char decyzja) {
	switch (decyzja) {
		case '+':
			printf("Wynik kalkulatora: %d\n", a + b);
			break;
		case '-':
			printf("Wynik kalkulatora: %d\n", a - b);
			break;
		default:
			printf("Nieznana operacja \n");
			break;
	}
}

void czytaj_liczbe() {
	int num, suma = 0;

	do {
		printf("Podaj liczbe ( liczba 0 konczy dzialanie ): ");
		scanf("%d", &num);
		suma += num;
	} while (num != 0);

	printf("Suma podanych liczb to: %d \n", suma);
}

// Zadania z pêtl¹ for
int silnia(int n) {
	if (n == 0 || n == 1) {
		return 1;
	}

	return n * silnia(n - 1);
}

int sumuj_w_zakresie(int start, int end) {
	int suma = 0;
	for (int i = start; i <= end; i++) {
		suma += i;
	}

	return suma;
}

void generuj_tabliczke_mnozenia(int n) {
	for (int i = 0; i <= 10; i++) {
		printf("%d * %d = %d \n", i, n, n * i);
	}
}

// Zadania z pêtl¹ while
int suma_cyfr(int liczba) {
	int suma = 0;

	while (liczba > 0) {
		suma += liczba % 10;
		liczba /= 10;
	}

	return suma;
}

int odwrotna_kolejnosc(int liczba) {
	int odwrocona = 0;

	while (liczba != 0) {
		odwrocona = odwrocona * 10 + liczba % 10;
		liczba /= 10;
	}

	return odwrocona;
}

// Zadania z pêtl¹ do while
void wprowadzenie_dodatniej() {
	int liczba;

	do {
		printf("Podaj liczbê dodatnia \n");
		scanf("%d", &liczba);
	} while (liczba <= 0);
}

void czy_palindrom(int n) {
	int pierwotna = n;
	int odwrocony = 0, pom;

	while (n != 0) {
		pom = n % 10;
		odwrocony = odwrocony * 10 + pom;
		n /= 10;
	}

	if (pierwotna == odwrocony) {
		printf("Sa takie same\n");
	} else {
		printf("Nie sa takie same\n");
	}
}

void zgadnij_liczbe() {
	int r = rand();
	int liczba;

	do {
		scanf("%d", &liczba);
	} while (liczba != r);

	printf("Brawo zgadles \n");
}

// Zadania z instrukcj¹ switch-case
void ktory_dzien_tygodnia(int n) {
	switch (n) {
		case 1:
			printf("Poniedzialek\n");
			break;
		case 2:
			printf("Wtorek\n");
			break;
		case 3:
			printf("Sroda\n");
			break;
		case 4:
			printf("Czwartek\n");
			break;
		case 5:
			printf("Piatek\n");
			break;
		case 6:
			printf("Sobota\n");
			break;
		case 7:
			printf("Niedziela\n");
			break;
		default:
			printf("Zly numer dnia \n");
			break;
	}
}

void wybor_figury_geometrycznej(int n) {
	switch (n) {
		case 1:
			printf("Kwadrat\n");
			break;
		case 2:
			printf("Trojkat\n");
			break;
		case 3:
			printf("Kolo\n");
			break;
		default:
			printf("Zly numer figury\n");
			break;
	}
}