#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <algorithm>

using namespace std;


char ustawienia[] = "Settings.txt";
char zadania[] = "Schedule1.txt";

struct storageLocation
{
	int number;
	int x;	//row coordinate(0, 1, 2, 3)
	int y;	//column coordinate(0, 1, 2, 3)
	int z;	//level(0, 1, 2)
	int czas; // czas transportu
	int status; // wolne (0) czy zajete(1)
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
	int x, y,z=3;
	int l_kontenerow;
	container* c;
	maszyna *dzwig;
	storageLocation *magazyn;

	const int lock = 30; //30 seconds to lock on a container
	const int vellocity = 1; //1 meter per second, speed of machine
	const int move = 30; //time required to move the container to ajacent position
	const int rw = 5;	//row width meter
	const int cw = 5;	//column width meter
	double totalTime = 0;
	double timeTaken; //time taken to handle one container


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

	if (x*y*z < l_kontenerow) {
		cout << "Za malo miejsca";
		system("pause");
		return 0;
	}

	magazyn = new storageLocation[x*y*z];

	int zmienna = 0;  // obliczenie czasu transportu do kazdej pozycji i przydzielenie numeru
	int i = 0, j = 0, k = 0;
	while (zmienna != x*y*z)
	{
		if (i == x) { 
			i = 0; 
			j++;
			if (j == y) {
				j = 0;
				k++;
			}
		}
		magazyn[zmienna].x = i;
		magazyn[zmienna].y = j;
		magazyn[zmienna].z = k;
		magazyn[zmienna].number = zmienna;
		magazyn[zmienna].status = 0;
		if (magazyn[zmienna].z == 0) {
			magazyn[zmienna].czas = lock + ((rw * magazyn[zmienna].x + cw * magazyn[zmienna].y) / vellocity) + lock;//travel time

		}
		else {
			magazyn[zmienna].czas = lock + ((rw * magazyn[zmienna].x + cw * magazyn[zmienna].y) / vellocity) + lock;//travel time
			magazyn[zmienna].czas += (magazyn[zmienna].z * (4 * lock + move)) + (2 * lock) + move; //handle container time
		}

		i++;
		zmienna++;
	}

	for (int i = 0; i < x*y*z; i++)
	{
		cout << "numer: " << magazyn[i].number << "  czas: " << magazyn[i].czas << "  x: " << magazyn[i].x << "  y: " << magazyn[i].y << "  z: " << magazyn[i].z << endl;
	}
	int low = 10000000;

	for (int i = 0; i < l_kontenerow; i++) { // ustawienie kontenerow od najblizszego miejsca do najdalszego
		low = 10000000;
		for (int j = 0; j < x*y*z; j++) { // znalezienie najblizszego miejsca
			if (magazyn[j].status == 0) { //jezeli wolne

				/*if (magazyn[j].z != 0) { // jezeli pod spodem juz jest kontener, to sprawdz czasy
					for (int k = 0; k < l_kontenerow; k++){
						if (c[k].location.x == magazyn[j].x && c[k].location.y == magazyn[j].y) { // znajdowanie tego na dole
							if (c[i].depart < c[k].depart) { // jezeli dolny odjezdza pozniej, to ustawiaj na gorze nowego
								low = min(low, magazyn[j].czas); // sprawdz czy najblizej
							}
						}
					}
				}
				else {
					low = min(low, magazyn[j].czas); // sprawdz czy najblizej
				}*/
				low = min(low, magazyn[j].czas); // sprawdz czy najblizej
			}
		}
		for (int j = 0; j < x*y*z; j++) { //przypisanie znalezionego miejsca
			if (low == magazyn[j].czas)
			{
				if (magazyn[j].status == 0) {
					c[i].location = magazyn[j];
					magazyn[j].status = 1;
					break;
				}
			}
		}
	}


	for (int i = 0; i < l_kontenerow; i++) { // ustawienie kontenerow od najblizszego miejsca do najdalszego
		totalTime += c[i].location.czas;
	}


	cout << totalTime << endl;

	
	for (int i = 0; i < l_kontenerow; i++) {
		cout << "Container number: " << c[i].number << " is in location: " << c[i].location.number << endl;
			/*" (row: " << c[i].location.x <<
			" column: " << c[i].location.y <<
			" level: " << c[i].location.z << ")" << endl;*/
	}
	delete[] c;
	delete[] dzwig;
	delete[] magazyn;
	cout << "Hello World! Dwa";
	cout << endl;
	system("pause");

	return 0;
}

