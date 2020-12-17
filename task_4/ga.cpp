// GeneticAlg.cpp : Defines the entry point for the console application.
//
#include<iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <cmath>

#include "genetic.h"

using namespace std;


int Field::Size;

void research()
{
	Field::Size = 50;	//размер стороны клеточного автомата
	pop_size = 100;	//размер популяции
	int max_stable = 20;	//задаем максимальное количество поколений без улучшения результата
	double pmut_init = 1.0 / (Field::Size*Field::Size);
	GameOfLife best_game(100);

	ofstream logfile;
	logfile.open("./GAresearch.log");
	ofstream resfile;
	string fname;
	for (int i = 0; i <= 9; i++)
	{
		double pmut = pmut_init*pow(1.5, i);
		for (int j = 0; j <= 9; j++)
		{
			cout << "\n\n----------\nSeries " << i << " run " << j << " with Pmut = " << pmut << " starting.\n";

			auto t_start = chrono::system_clock::now();
			GeneticGoL ga(new BitMutation(pmut), new RouletteSelection(), new TwoPointCB(), new FuncCellAutomat(), max_stable);
			ga.Start();
			auto t_end = chrono::system_clock::now();
			chrono::duration<double> elapsed = t_end - t_start;
			cout << "\nRun " << i << ":" << j << " successfully finished.\n";
			cout << "Was elapsed " << elapsed.count() << " seconds\n";
			logfile << "Series " << i << " run " << j << ":\n\tPmut = " << pmut << "; time = " << elapsed.count() << " seconds; ";
			logfile << "Generations total " << ga.GetStepsCount() << "; Best value = " << ga.GetBestValue() << endl;
			Field* bf = ga.GetBestField();
			best_game.StartNew(bf, -1);
			resfile.open("./series_" + to_string(i) + "_run_" + to_string(j) + "_sol.txt");
			for (int m = 0; m < 50; m++)
			{
				for (int n = 0; n < 50; n++)
					resfile << (((*bf)[m][n]>0) ? "X" : "-");
				resfile << endl;
			}
			resfile.close();
			resfile.open("./series_" + to_string(i) + "_run_" + to_string(j) + "_sol_after100.txt");
			for (int m = 0; m < 50; m++)
			{
				for (int n = 0; n < 50; n++)
					resfile << ((best_game.ca[m][n]>0) ? "X" : "-");
				resfile << endl;
			}
			resfile.close();
		}
		cout << "\nSeries " << i << " finished\n";
	}
	


}


int main()
{		
	int choice = 0;
	cout << "Enter number of action you want:\n" <<
		"1: Start planer\n2: Start Flasher\n3: Start Navigation lights\n4: Open from file\n5: Start single genetic algorithm\n0: Start series of GA\n: ";
	cin >> choice;
	if (choice == 1)
	{
		Field::Size = 20;
		Field Planer(0);	//"планер"
		Planer[5][3] = Planer[5][4] = Planer[5][5] = Planer[4][5] = Planer[3][4] = 1;
		GameOfLife gameplaner(&Planer, 30);
		gameplaner.Start(1);
		return 0;
	}
	else if (choice == 2)
	{
		Field::Size = 10;
		Field Flasher(0);
		Flasher[3][5] = Flasher[4][5] = Flasher[5][5] = 1;	//"мигалка"
		GameOfLife gameflasher(&Flasher, 30);
		gameflasher.Start(1);
		return 0;
	}
	else if (choice == 3)
	{
		Field::Size = 15;
		Field Lights(0);
		Lights[7][5] = Lights[7][6] = Lights[7][7] = Lights[6][6] = 1;	//"навигационные огни"
		GameOfLife gamelights(&Lights, 15);
		gamelights.Start(1);
		return 0;
	}
	else if (choice==4)
	{
		Field::Size = 50;
		Field ff;
		string fname = "";
		cout << "Enter filename: ";
		cin >> fname;
		ifstream f;
		f.open(fname);
		if (f.bad())
		{
			cout << "Cannot open this file.\n";
			return 0;
		}
		char c = '1';
		int i = 0, j = 0;
		while (!f.bad() && c != EOF)
		{
			c = f.get();
			if (c == '\n')
			{
				i++; j = 0;
			}
			else if (c == 'X')
				ff[i][j++] = 1;
			else if (c == '-')
				ff[i][j++] = 0;
		}
		GameOfLife filegame(&ff, 100);
		filegame.Start(1);
		cout << "result: \n\n\n";
		int v1 = 0, v2 = 0;
		for (i = 0; i < 50; i++)
		{
			for (j = 0; j < 50; j++)
				cout << ((filegame.ca[i][j]>0) ? "X" : "-");
			cout << endl;
		}
		FuncCellAutomat fc;
		cout << "\nValue = " << fc.Survival(&ff) << endl;
		return 0;
	}
	else if (choice == 0)
	{
		research();
		return 0;
	}


	srand(time(0));
	//int w = 0;

	Field::Size = 50;
	pop_size = 200;
	double Pmut = (1. / (Field::Size*Field::Size));
	int max_stable_steps = 20;

	//запуск генетического алгоритма
	auto t_start = chrono::system_clock::now();
	GeneticGoL ga(new BitMutation(Pmut), new RouletteSelection(), new TwoPointCB(), new FuncCellAutomat(), max_stable_steps);
	ga.Start();
	auto t_end = chrono::system_clock::now();
	chrono::duration<double> elapsed = t_end - t_start;
	cout << "\nSuccessfully finished.\n";
	cout << " Was elapsed " << elapsed.count() << " seconds\n";

	//получение результатов
	int bv = ga.GetBestValue();
	Field* bf = ga.GetBestField();
	GameOfLife best_game(bf, 100);
	cout << "Best value = "<<bv<<"\nshow visualisation? (y|n) ";
	char ans;
	cin >> ans;	
	cout << "\n\n";
	if (ans == 'y' || ans == 'Y')
	{
		cout << "Enter latency in seconds: ";
		int l;
		cin >> l;
		best_game.Start(l);
	}
	else
	{
		best_game.Start(-1);
		best_game.PrintState(0);
	}

	cout << "Save to file? (y|n) ";
	cin >> ans;
	if (ans == 'y' || ans == 'Y')
	{
		string newfile;
		cout << "Enter filename: ";
		cin >> newfile;
		ofstream fout;
		fout.open("./"+newfile+".txt");
		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 50; j++)
			{
				fout << (((*bf)[i][j]>0) ? "X" : "-");
			}
			fout << endl;
		}
		fout.close();
		ofstream fout2;
		fout2.open(newfile + "_100.txt");
		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 50; j++)
			{
				fout2 << ((best_game.ca[i][j]>0) ? "X" : "-");
			}
			fout2 << endl;
		}
		fout2.close();
	}
	return 0;
}