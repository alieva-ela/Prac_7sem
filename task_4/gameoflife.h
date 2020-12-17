#include <iostream>
#include <chrono>
#include <thread>

typedef int8_t cell;	//тип элемента клеточного автомата, int8_t для сокращения расходов памяти, т.к. ячейка может принимать 2 значения: 0/1


struct Field	//поле клеточного автомата
{
	cell* Value = nullptr;	//указатель на динамический одномерный массив с значениями поля
	static int Size;	//размер одной стороны поля, не длина массива! Длина массива будет Size*Size. Общий для всех экземпляров

	Field()	//дефолтный конструктор, поле Size должно быть определено
	{
		Value = new cell[Size*Size];
	}


	Field(cell def_val)	//инициализация поля значением val, например 0
	{
		if (Value == nullptr)
			Value = new cell[Size*Size];
		for (int i = 0; i < Size*Size; i++)
		{
			Value[i] = def_val;
		}
	}

	void Generate()	//заполнение поля рандомно сгенерированными значениями
	{
		if (Value == nullptr)
			Value = new cell[Size*Size];
		for (int i = 0; i < Size*Size; i++)
		{
			Value[i] = rand() % 2;
		}
	}
	void Copy(const Field* field)	//поэлементное копирование значений поля
	{
		if (Value == nullptr)
			Value = new cell[Size*Size];
		for (int i = 0; i < Size*Size; i++)
		{
			Value[i] = field->Value[i];
		}
	}

	cell* operator[](const int index)	//перегрузка оператора для доступа к значениям массива по координатам в поле, а не по индексу
	{
		return &Value[index*Size];
	}

	~Field()
	{
		if (Value != nullptr)
			delete[] Value;
		Value = nullptr;
	}
};

class GameOfLife
{
public:
	Field ca;
	int maxSteps;


	bool isStatic = false;
	bool success = false;
	cell liveSteps = 0;
	GameOfLife() = default;

	GameOfLife(int num_steps)	//игра на рандомно сгенерированном поле с указанным размером и количеством шагов
	{
		ca.Generate();
		maxSteps = num_steps;
	}

	GameOfLife(const Field* base_conf, int num_steps)	//передаем уже существующую конфигурацию
	{
		ca.Copy(base_conf);
		maxSteps = num_steps;
	}

	int Start(int visualisation)	//запускаем работу клеточного автомата на заданное количество шагов, либо пока автомат не станет стационарным
	{	//значения visualisation: -1 без демонстрации, >=0 - задержка в секундах
		if (visualisation > -1)
			PrintState(visualisation);
		while (!isStatic&&liveSteps <= maxSteps)
		{
			liveSteps++;
			isStatic = true;
			if (ca[0][0] == -1)
				ca[0][0] = 0;
			for (int i = 0; i < Field::Size; i++)
			{
				for (int j = 0; j < Field::Size; j++)
				{
					int neighbors = 0;	//считаем соседей
					if (i>0)
					{
						neighbors += (j > 0 && ca[i - 1][j - 1] != 0 && ca[i - 1][j - 1] <= liveSteps) ? 1 : 0;
						neighbors += (ca[i - 1][j] != 0 && ca[i - 1][j] <= liveSteps) ? 1 : 0;
						neighbors += (j + 1<Field::Size && ca[i - 1][j + 1] != 0 && ca[i - 1][j + 1] <= liveSteps) ? 1 : 0;
					}
					neighbors += (j > 0 && ca[i][j - 1] != 0 && ca[i][j - 1] <= liveSteps) ? 1 : 0;
					if (j + 1 < Field::Size &&ca[i][j + 1] != 0)
					{
						if (ca[i][j + 1] == -1)
							ca[i][j + 1] = 0;
						else
							neighbors++;
					}

					if (i + 1 < Field::Size)
					{
						if (j > 0 && ca[i + 1][j - 1] != 0)
						{
							if (ca[i + 1][j - 1] == -1)
								ca[i + 1][j - 1] = 0;
							else
								neighbors++;
						}
						if (ca[i + 1][j] != 0)
						{
							if (ca[i + 1][j] == -1)
								ca[i + 1][j] = 0;
							else
								neighbors++;
						}
						if (j + 1<Field::Size && ca[i + 1][j + 1] != 0)
						{
							if (ca[i + 1][j + 1] == -1)
								ca[i + 1][j + 1] = 0;
							else
								neighbors++;
						}
					}

					if (ca[i][j] > 0 && (neighbors<2 || neighbors>3))	//клетка умирает
					{
						ca[i][j] = -1;
						isStatic = false;
					}
					else if (ca[i][j] == 0 && neighbors == 3)	//рождается новая клетка
					{
						ca[i][j] = liveSteps + 1;
						isStatic = false;
					}
				}
			}
			if (visualisation > -1)
				PrintState(visualisation);
		}
		if (!isStatic&&liveSteps >= maxSteps)	//автомат прожил нужное количество шагов и не стал вырожденным(стационарным)
			success = true;
		return liveSteps;
	}

	int StartNew(const Field* base_conf, int visualisation)	//запуск игры с новым начальным полем
	{
		liveSteps = 0;
		success = false;
		isStatic = false;
		ca.Copy(base_conf);
		return Start(visualisation);
	}

	void PrintState(int latency)	// визуализация работы алгоритма на конкретной конфигурации, latency - задержка времени в секундах
	{
		system("clear");
		std::cout << "\n\nstep " << (int)liveSteps << "\n\n";
		for (int i = 0; i < Field::Size; i++)
		{
			for (int j = 0; j < Field::Size; j++)
			{
				std::cout << ((ca[i][j]>0) ? "X" : "-");
			}
			std::cout << '\n';
		}
		std::this_thread::sleep_for(std::chrono::seconds(latency));
		if (isStatic)
			std::cout << "degenerated\n";
	}
};