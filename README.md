В данной программе необходимо найти уникальное количество разложений числа. Разложения, отличающиеся порядком слагаемых, считаются одинаковыми. Например, 3=2+1 и 3=1+2. Необходимо использовать механизм синхронизации «мьютекс».
Входные данные: файл «input.txt», содержащий в себе следующие данные:
	Первая строка – натуральное число T, число рабочих потоков, которые будут использоваться для решения задачи;
	Вторая строка – натуральное число 2≤N≤100, разложения которого и будут находиться.
Выходные данные: файл «output.txt», содержащий в себе следующие данные:
	Первая и вторая строки – повторение соответствующих строк входного файла;
	Третья строка – полученное число разложение.
Файл «time.txt», содержащий в себе одну строку со временем, за которое был найден ответ.
