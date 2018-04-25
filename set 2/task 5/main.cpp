#include <iostream>
#include "Menu.h"
#include "Fun.h"

/* https://math.stackexchange.com/questions/142112/how-to-construct-a-k-regular-graph?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
*/

/*
A theorem by Nash-Williams says that every k‑regular graph on 2k + 1 vertices has a Hamiltonian cycle.

*/

//graf k - regularny
// dla k parzystego, liczba wierzcholkow > k
// dla k nieparzystego, liczba wierzcholkow  parzysta > k


int main()
{
	using namespace std;

	Menu menu;
	menu.start();
	generateGraphFullRandom(menu.getK(), menu.getV());

	system("pause");
	return 0;
}