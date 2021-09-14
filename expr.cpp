#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <ctime>
#include <chrono>

volatile int result;
int num;
int threads_num;
HANDLE mutex; // Мутекс для синхронизации доступа к infos
HANDLE* threads;

struct thread_info
{
	int upper_boarder;
	int lower_boarder;
	// Ключевое слово volatile запрещает компилятору оптимизации поля progress
}*INFO;

int main_proc(int n, int k)
{
	int choose = 1;

	if (n >= k && k > 0)
	{
		choose = 1;
	}	
	if (k > n)
	{
		choose = 2;
	}
	if (n == 0 && k == 0)
	{
		choose = 3;
	}
	if (n && k == 0)
	{
		choose = 4;
	}

	switch (choose)
	{
	case 1:
		return(main_proc(n, k - 1) + main_proc(n - k, k));
		break;
	case 2:
		return(main_proc(n, n));
		break;
	case 3:
		return 1;
		break;
	case 4:
		return 0;
		break;
	}
	return 0;
}

int brdrs_cr(int threads_num, int num)
{
	int fh = num / threads_num;   // целочисленная часть числа
	int sh = num % threads_num;  // дробная часть числа
	int previous_board = 1;
	int iterations;
	//Проверка на перебор потоков для числа 12 поток а число 10
	if (threads_num >= num)
	{
		iterations = num;
	}
	else
	{
		iterations = threads_num;
	}
	//Инициализация структур
	for (int i = 0; i < iterations; i++)
	{
		INFO[i].lower_boarder = previous_board;
		if (sh == 0)
		{
			INFO[i].upper_boarder = INFO[i].lower_boarder + fh - 1;
		}
		else
		{
			INFO[i].upper_boarder = INFO[i].lower_boarder + fh;
			sh--;
		}
		previous_board = INFO[i].upper_boarder + 1;
	}
	return iterations;
}

DWORD WINAPI thread_proc(void* param)
{
	WaitForSingleObject(mutex, INFINITE);
	int second = INFO[(int)param].upper_boarder;
	int first = INFO[(int)param].lower_boarder;
	ReleaseMutex(mutex);
	int tmp = 0;
	for (int i = first; i <= second; i++)
	{
		tmp += main_proc(num - i, i);
	}
	result += tmp;
	return 0;
}

int main()
{
	std::string filename = "input.txt";
	std::ifstream file(filename);
	file >> threads_num;
	file >> num;
	file.close();

	std::cout << threads_num << std::endl;
	std::cout << num << std::endl;

	memset(&threads, 0, sizeof(threads));
	mutex = CreateMutex(NULL, FALSE, NULL);

	INFO = (thread_info*)malloc(threads_num * sizeof(thread_info));
	int true_count_threads = brdrs_cr(threads_num, num);
	threads = (HANDLE*)malloc(true_count_threads * sizeof(HANDLE));

	volatile int i = 0; // "volatile" is to ask compiler not to optimize the loop away.
    auto start = std::chrono::steady_clock::now();
	
	for(int i =0; i< true_count_threads; ++i)
	{
		threads[i] = CreateThread(0, 0, thread_proc, (void*)i, 0, 0);
	}

	while (1)
{
		if (WAIT_TIMEOUT == WaitForMultipleObjects(threads_num, threads, TRUE, 1000))
		{
			// Потоки еще работают, вывести прогресс
			std::cout << "current result: " << result << std::endl;
		}
		else
		{
			// Потоки завершены
			break;
		}
	}

	auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	
	CloseHandle(mutex);
	for (i = 0; i < threads_num; i++)
		CloseHandle(threads[i]);
	free(INFO);
	free(threads);
	
	result--;
	filename = "output.txt";
	std::ofstream file1(filename);
	file1<< std::to_string(threads_num) << std::endl;
	file1 << num << std::endl;
	file1 << result << std::endl;
	file1.close();
	
	filename = "time.txt";
	std::ofstream file2(filename);
	file2 << elapsed.count();
	file2.close();
	
	return 0;
}