#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Zadania z petla for
long long podwojnaSilnia(int n) {
	long long wynik = 1;
	if (n % 2 == 0) {
		for (long long i = n; i >= 2; i-=2) {
			wynik *= i;
		}
	} else {
		for (long long i = n; i >= 1; i -= 2) {
			wynik *= i;
		}
	}

	return wynik;
}

int suma(int k, int start, int end) {
	int wynik = 0;

	for (int i = start; i <= end; i++) {
		if (i % k == 0)
			wynik += i;
	}

	return wynik;
}

// Zadania z petla while
int iloczynCyfr(int n) {
	int wynik = 1;

	while (n > 0) {
		wynik *= n % 10;
		n /= 10;
	}

	return wynik;
}

void intToBinary(int n) {
	int binarna[32];
	int iterator = 0;

	while (n > 0) {
		binarna[iterator] = n % 2;
		n /= 2;
		iterator++;
	}

	printf("Reprezentacja binarna: \n");
	for (int i = iterator - 1; i >= 0; i--) {
		printf("%d", binarna[i]);
	}
	printf("\n");
}

int cyfraKontrolna(char pesel[11]) {
	int wynik = 0;
	int wagi[10] = { 1, 3, 7, 9, 1, 3, 7, 9, 1, 3 };

	int i = 0;
	while (i < 10) {
		int temp = (pesel[i] - '0') * wagi[i];
		wynik += temp % 10;
		i++;
	}

	return 10 - wynik % 10;
}

// Zadania z petla do while

void wczytanieLiczby(int a, int b) {
	int num;
	do {
		printf("Prosze podac liczbe z zakresu <%d, %d>\n", a, b);
		scanf("%d", &num);
	} while (num > b || num < a);
}

int ileCyfr(int n) {
	int wynik = 0;

	while (n > 0) {
		wynik++;
		n /= 10;
	}

	return wynik;
}

int pow(int n, int t) {
	int wynik = 1;

	for (int i = 1; i <= t; i++) {
		wynik *= n;
	}

	return wynik;
}

void czyLiczbaArmstronga(int n) {
	int wynik = 0;
	int cyfry = ileCyfr(n);
	int basic = n;

	do {
		wynik += pow(n % 10, cyfry);
		n /= 10;
	} while (n > 0);

	if (wynik == basic) {
		printf("Podana liczba jest liczba Armstronga\n");
	}
	else {
		printf("Podana liczba nie jest liczba Armostronga\n");
	}
}

void odgadnijLiczbe() {
	int num;
	int r = rand();

	do {
		scanf("%d", &num);

		if (num > r) {
			printf("Za duzo\n");
		} else if (num < r) {
			printf("Za malo\n");
		}
	} while (num != r);

	printf("Brawo! Udalo ci sie zgadnac liczbe\n");
}