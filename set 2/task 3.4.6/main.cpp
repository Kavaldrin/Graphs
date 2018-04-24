#include "Graph.h"
#include <iostream>
#include <fstream>
#include "InputParser.h"

using namespace std;

int main()
{
	shared_ptr<Graph> graph;
	try{
		graph  = InputParser("data.txt").getGraph();
	}
	catch(myException e){
		cout << e.what() << endl;
		system("pause");
		return -1;
	}

	//chamski if bo juz nie chce mi sie klasy na parsowanie pisac
	char choice;
	cout << "Choose fuction->:\n";
	cout << "1) Finding  largest group\n";
	cout << "2) Finding eulers cycle\n";
	cout << "3) Finding hamiltons cycle\n";
	cin >> choice;

	try {
		switch (choice) {
		case '1':
			graph->onlyComponents();
			break;
		case '2':
			graph->EulerMode();
			break;
		case '3':
			graph->HamiltonMode();
			break;
		default:
			cout << "chesz to se napisz\n";
		}
	}
	catch (const myException &e) {
		cout<< e.what() <<endl;
	}

	system("pause");


}