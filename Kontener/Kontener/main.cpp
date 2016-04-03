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
	int location;
	int machine;	//the yard machine container is scheduled on
	int arrive;		//the arrival time of ship
	int depart;		//the departure time of ship
};



int main()
{
	int numer_pliku;
	int l_maszyn;
	int x, y;
	int l_kontenerow;
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

	//cout << numer_pliku << " " << l_maszyn << " " << x << " " << y;
	plik.close();
/*Wczytanie danych*/

	container* c;

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

	

	
	for (int i = 0; i < l_kontenerow; i++) {
		cout << c[i].number << endl;
	}
	delete[] c;
	cout << "Hello World! Dwa";
	cout << endl;
	system("pause");

	return 0;
}

