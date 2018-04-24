#include "Menu.h"




void Menu::start()
{
	using namespace std;
	cout << "Podaj k, do wygenerowanie k-regularnego grafu [1,30]//rozsadna ilosc\n";
	std::string input;
	cin >> input;

	while (true) {
		if (isdigit(input[0])) {
			if (stoi(input) > 0 && stoi(input) <= 30) {
				m_k = stoi(input);
				break;
			}
			else {
				cout << "Zly zakres, sprobuj jeszcze raz [1,30]\n";
				cin >> input;
			}
		}
		else {
			cout << "Sprobuj jeszcze raz (tym razem moze liczba)\n";
			cin >> input;
		}
	}
	cout << "Podaj liczbe wierzcholkow, wieksza od liczby k  oraz dla nieparzystej k\n parzysta liczba wierzcholkow\n";
	cin >> input;

	while (true) {
		if (isdigit(input[0])) {
			if (stoi(input) > m_k) {
				if ((m_k % 2 && !(stoi(input) % 2)) || !(m_k % 2)) {
					m_v = stoi(input);
					break;
				}
				else {
					cout << "Nie da sie stworzyc grafu k-regularnego z podanych parametrow\n";
					cin >> input;
				}
			}
			else {
				cout << "Zla liczba wierzcholkow...\n";
				cin >> input;
			}
		}
		else {
			cout << "Sprobuj jeszcze raz (tym razem moze liczba)\n";
			cin >> input;
		}
	}
	
	cout << "Forma generowania, (1) szybka slabo-losowy \n(2) superszybka i losowa metoda 100legit\n";
	cin >> input;
	while (true) {
		if (isdigit(input[0])) {
			if (!(stoi(input) == 1 || stoi(input) == 2))
				cin >> input;
			else {
				m_choice = stoi(input);
				break;
			}
		}
	}
}
void Menu::generateGraph()
{
	switch (m_choice) {
	case 1:
		generateKGraphConnectivity(m_k, m_v);
		break;
	case 2:
		generateGraphFullRandom(m_k, m_v);
		break;
	}
}
