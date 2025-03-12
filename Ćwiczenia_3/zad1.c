#include <stdio.h>
#include <stdlib.h>

int main() {
	// Zadania z petla for
	printf("Podwojna silnia %d\n", podwojnaSilnia(6));
	printf("Sumowanie %d\n", suma(2, 1, 5));

	// Zadania z petla while
	printf("Iloczyn cyfr %d\n", iloczynCyfr(123));
	intToBinary(5);
	printf("%d\n", cyfraKontrolna("11111111111"));

	// Zadania z petla do while
	wczytanieLiczby(5, 10);
	czyLiczbaArmstronga(370);
	odgadnijLiczbe();

	return 0;
}