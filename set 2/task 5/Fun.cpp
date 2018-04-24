#include "Fun.h"

int const ATTEMPTS = 1000000;

void generateKGraphConnectivity(int k,int w)
{
	//https://math.stackexchange.com/questions/142112/how-to-construct-a-k-regular-graph?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
	using namespace std;

	vector<vector<int>> vertexes;
	vector <int> temp;

	for (int i = 0; i < w; ++i)
		vertexes.push_back(temp);
	//cout << "weszlo";
	int itemp;
	int dupa;
	//odd mode

	if (k % 2) {
		itemp = k - 1;
		itemp /= 2;
		for (int i = 0; i < w; ++i) {
			//z lewej
			for (int j = 1; j <= itemp; ++j) {
				dupa = i - j;
				if (dupa < 0) dupa = w + i - j;
				vertexes[i].push_back(dupa);
			}
			//z prawej
			for (int j = 1; j <= itemp; ++j) {
				vertexes[i].push_back((i + j) % w);
			}
			//na przeciwko
			vertexes[i].push_back((i + w / 2) % w);

		}
	}
	//even mode
	else {
		itemp = k / 2;
		for (int i = 0; i < w; ++i) {
			//z lewej
			for (int j = 1; j <= itemp; ++j) {
				dupa = i - j;
				if (dupa < 0) dupa = w + i- j;
				vertexes[i].push_back(dupa);
			}
			//z prawej
			for (int j = 1; j <= itemp; ++j) {
				vertexes[i].push_back((i + j) % w);
			}
		}
	}

//#define DEBUG

#ifdef DEBUG
	std::cout << "\nPrzed randomizacja\n";
	for (auto &vertex : vertexes) {
		for (auto &a : vertex)
			std::cout << a << " ";
		std::cout << std::endl;
	}
#endif 

	
	
	randomizeGraph(vertexes);

	for (auto &vertex : vertexes) {
		for (auto &a : vertex)
			std::cout << a << " ";
		std::cout << std::endl;
	}

	try {
		saveToFile(vertexes);
	}
	catch (std::string str) {
		cout << str;
	}


	



}


//copy paste z poprzedniego

void randomizeGraph(std::vector<std::vector<int>> &vertexes)
{
	using namespace std;
	const int vertex_num = vertexes.size();

	srand(time(NULL));
	int a, b, c, d, i;
	int attempts = ATTEMPTS;
	//ab i cd na parê ad i cb.
	while (attempts) {
		a = std::rand() % vertex_num;
		b = std::rand() % vertex_num;
		c = std::rand() % vertex_num;
		d = std::rand() % vertex_num;
		if (a != b && a != c && a != d && b != c && b != d && c != d) {
			if ((areConnected(vertexes,a, b) && areConnected(vertexes, c, d) && !areConnected(vertexes, a, d) && !areConnected(vertexes, c, b))) {
				//std::cout << "debug : randoming" <<a+1 << " "<<b+1<< " " << c+1 << " "<<d+1<< std::endl;
				disconnect(vertexes,a, b);
				disconnect(vertexes,c, d);
				connect(vertexes,a, d);
				connect(vertexes,c, b);
				break;
			}
		}
		--attempts;
	}
}

void setMins(std::map<int, int> & degrees, std::vector<int> &minsIndex)
{
	minsIndex.clear();
	int curr_min = INT_MAX;
	for (auto &el : degrees) {
		if (curr_min > el.second) {
			curr_min = el.second;
			minsIndex.clear();
			minsIndex.push_back(el.first);
		}
		else if (curr_min == el.second)
			minsIndex.push_back(el.first);
	}

}


void generateGraphFullRandom(int k, int w)
{
	using namespace std;
	srand(time(NULL));

	
	vector<vector<int>> vertexes;
	vector <int> temp;

	map<int,int> neigh;
	vector<int> min;
	vector<int> max;


	//boze zmiluj sie nad tym guwnianym kodem = D
	////
	int valid;
	bool generated = false;
	////

	int a, b;
	while(!generated){

		//bozia tego nie widzi
		vertexes.clear();

		for (int i = 0; i < w; ++i) {
			vertexes.push_back(temp);
			neigh[i] = 0;
		}
		valid = 100;
		//


		while (leftVertexes(neigh, k)) {
			setMins(neigh, min);
			a = rand() % min.size();
			b = rand() % min.size();
			while (a == b && min.size() > 1)
				b = rand() % min.size();

			if (min.size() == 1) {
				a = min[a];
				b = rand() % w;
				while (!(neigh[b] < k && neigh[b] == neigh[a] + 1))
					b = rand() % w;

				if (!areConnected(vertexes, a, b)) {
					connect(vertexes, a, b);
					++neigh[a];
					++neigh[b];
				}
			}
			else {
				if (a != b) {
					a = min[a];
					b = min[b];
					if (!areConnected(vertexes, a, b)) {
						connect(vertexes, a, b);
						++neigh[a];
						++neigh[b];
					}
					else {
						while (areConnected(vertexes, a, b)) {
							b = rand() % w;	
							--valid;

							if (!valid)
								break;
							

							while (!((neigh[b] < k) && a != b)) {
								b = rand() % w;
							}
							

						}
						if (!areConnected(vertexes, a, b)) {
							connect(vertexes, a, b);
							++neigh[a];
							++neigh[b];
						}
						
					}
				}

			}
			if (!valid)
				break;
		}
		
		if (valid)
			generated = true;

		if (!leftVertexes(neigh, k))
			generated = true;
		//////
//#define DEBUG
#ifdef DEBUG
		cout << endl;
		for (auto &vertex : vertexes) {
			for (auto &a : vertex)
				std::cout << a << " ";
			std::cout << std::endl;
		}
		cout << endl;

		///////
#endif 
	}
	




//#define DEBUG

	for (auto &vertex : vertexes) {
		for (auto &a : vertex)
			std::cout << a << " ";
		std::cout << std::endl;
	}
	//system("pause");


	try {
		saveToFile(vertexes);
	}
	catch (std::string str) {
		cout << str;
	}
	




	////////////////////////////////
	//second try
	



}

bool leftVertexes(std::map<int, int> &neigh,int k)
{
	for (auto &el : neigh) {
		if (el.second < k)
			return true;
	}
	return false;
}

bool areConnected(std::vector<std::vector<int>> &vertexes,int v1, int v2)
{
	for (int &neigh : vertexes[v1]) {
		if (neigh == v2)
			return true;
	}

	return false;
}

void disconnect(std::vector<std::vector<int>> &vertexes, int v1, int v2)
{
	auto it = std::find(vertexes[v1].begin(), vertexes[v1].end(), v2);
	vertexes[v1].erase(it);
	it = std::find(vertexes[v2].begin(), vertexes[v2].end(), v1);
	vertexes[v2].erase(it);
}

void connect(std::vector<std::vector<int>> &vertexes, int v1, int v2)
{
	vertexes[v1].push_back(v2);
	vertexes[v2].push_back(v1);
}
void saveToFile(std::vector<std::vector<int>> &vertexes)
{
	using namespace std;
	fstream file("data.txt", ios::out);
	if (!file.good())
		throw "Cannot open the file\n";

	file << "type: AdjacencyList" << endl;
	file << "size: " << vertexes.size() << endl;

	int iter = 1;
	for (auto &vertex : vertexes) {
		file << vertex.size() << " " << iter << ":";
		++iter;
		for (auto &n : vertex) {
			file << " " << n+1;
		}
		file << endl;
	}



	file.close();
}