#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("Hello World!\n");

	int a = 10;
	double b = 3.14;
	char c = 'A';
	int d = 5;

	printf("int: %d, double: %.2f, char: %c \n", a, b, c);

	double suma1 = a + b;
	printf("Suma a i b = %.2f \n", suma1);

	int suma2 = dodawanie(a, d);
	printf("Wynik dodawania z funkcji = %d \n", suma2);

	czy_dodatnia(a);

	int tab[4] = { 1, 2, 3, 4 };
	int size = sizeof(tab) / sizeof(tab[0]);

	int suma_tablicy = sumuj_tab(tab, size);
	printf("Suma tablicy: %d \n", suma_tablicy);
	
	char decyzja = '+';
	kalkulator(a, d, decyzja);

	//czytaj_liczbe();

	// Zadania petla for
	printf("Silnia: %d \n", silnia(5));
	printf("Sumowanie w zakresie: %d \n", sumuj_w_zakresie(5, 10));
	generuj_tabliczke_mnozenia(5);

	// Zadania z pêtl¹ while
	printf("Suma cyfr: %d \n", suma_cyfr(123));
	printf("Odwrocona kolejnosc: %d \n", odwrotna_kolejnosc(123));

	// Zadania z pêtl¹ do while
	wprowadzenie_dodatniej();
	czy_palindrom(12);
	zgadnij_liczbe();

	// Zadania z instrukcj¹ switch-case
	ktory_dzien_tygodnia(2);
	wybor_figury_geometrycznej(1);

	return 0;
}