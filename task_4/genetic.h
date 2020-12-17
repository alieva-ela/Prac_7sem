#include "gameoflife.h"
#include <omp.h>

int pop_size;	//размер популяции

class Mutation
{
public:
	virtual void mutate(Field* pop) = 0;
};

class Crossover
{
public:
	virtual Field* crossing(const Field* pop) = 0;
};

class Selection
{
public:
	virtual Field* select(const Field* pop, const int* func) = 0;
};



class SurvivalFunc
{
public:
	virtual int Survival(const Field* field) = 0;
};

//-----------------------------
//realization


class FuncCellAutomat : public SurvivalFunc	//реализация функции выживаемости
{
	int max_steps = 100;	//число шагов, на котором нужно проверять клеточный автомат
public:
	FuncCellAutomat(){}
	int Survival(const Field* field)
	{
		GameOfLife game(field, max_steps);	//запускаем новую игру
		game.Start(-1);
		int val = 0;
		for (int i = 0; i < Field::Size*Field::Size; i++)	//считаем число выживших
		{
			if (game.ca.Value[i]>0)
				val++;
		}
		//тут делать проверку на завершенность и невырожденность
		val = (val*game.liveSteps) / 100; //штраф для вырожденных конфигураций
		return val;
	}
};

class RouletteSelection : public Selection	//реализация функции рулеточной селекции
{
public:

	Field* select(const Field* pop, const int* func)
	{
		Field* new_pop = new Field[pop_size];
		int f_sum = 0;
		for (int i = 0; i < pop_size; i++)
			f_sum += func[i];
		srand(time(0));
		for (int i = 0; i < pop_size; i++)
		{
			int k = rand() % f_sum;	//крутим рулетку
			int old_index = 0;
			int t = 0;
			while (t <= k)
			{
				t += func[old_index++];	//находим номер победителя
			}
			if (old_index>0)
				old_index--;
			new_pop[i].Copy(&pop[old_index]);
		}
		return new_pop;
	}
};

class TwoPointCB : public Crossover	//реализация функции двухточечного кроссовера
{
	int point1, point2;
	double p_crossing = 0.8;

public:
	Field* crossing(const Field* pop)
	{
		Field* new_pop = new Field[pop_size];
		int length = Field::Size*Field::Size;
		srand(time(0));
		point1 = rand() % length;	//определение точек скрещивания
		point2 = point1 + rand() % (length - point1);
		for (int i = 0; i < pop_size; i += 2)
		{
			int par1_index = rand() % pop_size;
			int par2_index = rand() % pop_size;

			double p = (1.0*rand()) / RAND_MAX;
			if (p > p_crossing)	//проверка вероятности скрещивания
				point1 = length;	//не будут скрещиваться
			for (int j = 0; j < length; j++)	//копируем "родителей" со скрещиванием, если нужно
			{
				if (j <= point1 || j > point2)
				{
					new_pop[i].Value[j] = pop[par1_index].Value[j];
					if (i + 1<pop_size)
						new_pop[i + 1].Value[j] = pop[par2_index].Value[j];
				}
				else
				{
					new_pop[i].Value[j] = pop[par2_index].Value[j];	//обмениваемый участок
					if (i + 1<pop_size)
						new_pop[i + 1].Value[j] = pop[par1_index].Value[j];
				}
			}
		}
		return new_pop;
	}
};

class BitMutation : public Mutation	//реализация функции побитовой мутации
{
	double Pmut;
public:
	BitMutation(double p)
	{
		Pmut = p;
	}

	void mutate(Field* pop)	//функция меняет значения в исходном массиве
	{
		for (int i = 0; i < pop_size; i++)
		{
			for (int j = 0; j < Field::Size*Field::Size; j++)
			{
				double p = (1.0*rand()) / RAND_MAX;
				if (p < Pmut)
					pop[i].Value[j] = (pop[i].Value[j] == 0) ? 1 : 0;
			}
		}
	}
};

class GeneticGoL
{
	Mutation* mut;
	Selection* sel;
	Crossover* cov;
	SurvivalFunc* fs;

	Field* population;
	int* func;

	Field best;
	int best_value = 0;

	int msi;
	int steps_count;

public:

	GeneticGoL(Mutation* m, Selection* s, Crossover* c, SurvivalFunc* f, int max_stable_iterations)
	{
		mut = m;
		sel = s;
		cov = c;
		fs = f;
		msi = max_stable_iterations;
		population = new Field[pop_size];
		func = new int[pop_size];
		int av = 0;
#pragma omp parallel	//запускаем ресурсоемкие вычисления параллельно
		{
#pragma omp for
			for (int i = 0; i < pop_size; i++)
			{
				population[i].Generate();	//рандомная генерация новых клеточных автоматов
				func[i] = fs->Survival(&population[i]);	//первичное вычисление функции выживаемости
			}
		}
		for (int i = 0; i<pop_size; i++)	//находим лучший из начальных вариантов
		{
			av += func[i];
			if (func[i] > best_value)
			{
				best.Copy(&population[i]);
				best_value = func[i];
			}
		}
		std::cout << "Generation 0. Average f(i) = " << (1.0*av) / pop_size << ", best result: " << best_value << std::endl;
	}

	void Start()
	{
		int stable_count = 0;	//счетчик итераций без улучшения результата
		steps_count = 0;	//общий счетчик инераций
		Field* new_pop;
		while (stable_count < msi)
		{
			steps_count++;
			std::cout << "Generation " << steps_count << " started.";
			new_pop = sel->select(population, func);	//отбор новой популяции
			delete[] population;
			population = new_pop;
			new_pop = cov->crossing(population);	//скрещивание
			delete[] population;
			population = new_pop;
			mut->mutate(population);	//мутации
			int av = 0;

#pragma omp parallel	//запускаем ресурсоемкие вычисления параллельно
			{
#pragma omp for
				for (int i = 0; i < pop_size; i++)
					func[i] = fs->Survival(&(population[i]));	//подсчет значений выживания для новой популяции
			}
			for (int i = 0; i<pop_size; i++)	//обновляем лучшее значение
			{
				if (func[i] > best_value)
				{
					best.Copy(&population[i]);
					best_value = func[i];
					stable_count = -1;
				}
				av += func[i];
			}
			std::cout << "\tAverage f(i) = " << (1.0*av) / pop_size << ", best result: " << best_value << std::endl;
			stable_count++;
		}
	}

	Field* GetBestField()
	{
		return &best;
	}

	int GetBestValue()
	{
		return best_value;
	}

	int GetStepsCount()
	{
		return steps_count;
	}

	~GeneticGoL()
	{
		delete[] population;
		delete[] func;
		delete mut;
		delete sel;
		delete cov;
		delete fs;
	}

};