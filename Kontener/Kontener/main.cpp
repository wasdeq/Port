#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;


char ustawienia[] = "Settings.txt";
char zadania[] = "Schedule1.txt";

struct storageLocation
{
	int number;
	int x;	//row coordinate(0, 1, 2, 3)
	int y;	//column coordinate(0, 1, 2, 3)
	int z;	//level(0, 1, 2)
};

struct container
{
	int number;
	storageLocation location;
	int machine;	//the yard machine container is scheduled on
	int arrive;		//the arrival time of ship
	int depart;		//the departure time of ship
};

struct maszyna
{
	int numer;  // numer dzwigu
	int start;	//poczatek pracy
	int koniec;		//koniec pracy
};


int main()
{
	int numer_pliku;
	int l_maszyn;
	int x, y;
	int l_kontenerow;
	container* c;
	maszyna *dzwig;

/*otwarcie pliku z ustawieniami*/
	ifstream plik;
	plik.open(ustawienia);

	if (!plik.good())
	{
		perror("Blad pliku");
		system("pause");
		return 0;
	}

	string line;
	//load number of schedule file
	getline(plik, line);
	numer_pliku = stoi(line);
	//load number of cranes
	getline(plik, line);
	l_maszyn = stoi(line);
	//load number of row in the yard
	getline(plik, line);
	x = stoi(line);
	//load number of column in the yard
	getline(plik, line);
	y = stoi(line);

	dzwig = new maszyna[l_maszyn];
	for (int i = 0; i < l_maszyn; i++) {
		dzwig[i].start = 0;
		dzwig[i].koniec = 0;
		dzwig[i].numer = i;
	}

	//cout << numer_pliku << " " << l_maszyn << " " << x << " " << y;
	plik.close();
/*Wczytanie danych*/

	ifstream plik2;
	plik2.open(zadania);
	if (!plik2.good())
	{
		perror("Blad pliku");
		system("pause");
		return 0;
	}

	string lineSche;
	getline(plik2, lineSche);
	string kon = lineSche;	//get the first line, which is the number of containers
	sscanf_s(kon.c_str(), "%d", &l_kontenerow);
	c = new container[l_kontenerow];
	for (int i = 0; i < l_kontenerow; i++) {
		getline(plik2, lineSche);
		c[i].number = stoi(lineSche); //store container number
		getline(plik2, lineSche);
		c[i].arrive = stoi(lineSche);	//store container arrival time
		getline(plik2, lineSche);
		c[i].depart = stoi(lineSche);	//store container departure time
	}
	plik2.close();


	int zmienna = 0;  // przydzielenie pozycji
	int i = 0, j = 0, k = 0;
	while (zmienna != l_kontenerow)
	{
		if (i == x) { 
			i = 0; 
			j++;
			if (j == y) {
				j = 0;
				k++;
			}
		}
		c[zmienna].location.x = i;
		c[zmienna].location.y = j;
		c[zmienna].location.z = k;
		i++;
		zmienna++;
	}





	const int lock = 30; //30 seconds to lock on a container
	const int vellocity = 1; //1 meter per second, speed of machine
	const int move = 30; //time required to move the container to ajacent position
	const int rw = 5;	//row width meter
	const int cw = 5;	//column width meter
	double totalTime = 0;
	double timeTaken; //time taken to handle one container



	for (int i = 0; i < l_kontenerow; i++) { //czas transportu

		if (c[i].location.z == 0) {
			timeTaken = lock + ((rw * c[i].location.x + cw * c[i].location.y) / vellocity) + lock;//travel time
			totalTime += timeTaken;
		}
		else {
			timeTaken = lock + ((rw * c[i].location.x + cw * c[i].location.y) / vellocity) + lock;//travel time
			timeTaken += (c[i].location.z * (4 * lock + move)) + (2 * lock) + move; //handle container time
			totalTime += timeTaken;
		}
	}

	cout << totalTime << endl;

	
	for (int i = 0; i < l_kontenerow; i++) {
		cout << "Container number: " << c[i].number << " is in location: " << c[i].machine <<
			" (row: " << c[i].location.x <<
			" column: " << c[i].location.y <<
			" level: " << c[i].location.z << ")" << endl;
	}
	delete[] c;
	delete[] dzwig;
	cout << "Hello World! Dwa";
	cout << endl;
	system("pause");

	return 0;
}

