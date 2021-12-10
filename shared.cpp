#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <stat.h>
#include <errorno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <string>
#include <semaphore.h>
#include <random>
using namespace std;

#define dimens 20

sem_t mutex;
int oglindit(int nr)
{
	int og = 0;
	int c;
	while (nr != 0)
	{
		c = nr % 10;
		nr /= 10;
		og = og * 10 + c;
	}
	return og;
}
char* converteste_la_char(int nr)
{
	char x[dimens];
	int aux = oglindit(nr);
	int k = 0;
	if (aux == 0)
	{
		x[0] = '0';
		x[1] = 0;
	}
	else
	{
		while (aux != 0)
		{
			int c = aux % 10;
			aux /= 10;
			x[k] = c + '0';
			k++;
		}
		x[k] = 0;
	}
	return x;
}
int converteste_la_int(char x[dimens])
{
	int nr = 0;
	int i, cf;
	for (i = 0; x[i] != 0; i++)
	{
		cf = x[i] - '0';
		nr = nr * 10 + cf;
	}
	return nr;
}
int main()
{
	void* shared_mem;
	int shmid;
	shmid = shmget((key_t)1122, 1024, 0666|IPC_CREAT);
	cout << "Cheia pentru memoria partajata: " << shmid << endl;
	shared_mem = shmat(shmid, NULL, 0);
	printf("Procesul atasat la adresa %p", shared_mem);
	cout << endl;

	int continut_crt_mem = 0;
	char s[dimens];
	strcpy(s, converteste_la_char(continut_crt_mem));
	memcpy(shared_mem, s, sizeof(s));
	srand(time(0));
	int nr_rand;
	int pid = fork();

	while (continut_crt_mem <= 1000)
	{
		nr_rand = rand() % 2 + 1;
		if (nr_rand == 2)
		{
			if (pid == 0)
			{
				cout << "Procesul copil: " << endl;
			}
			else
			{
				cout << "Procesul parinte: " << endl;
			}
			printf("	El citeste din memorie: %s", shared_mem);
			cout << endl;
			continut_crt_mem = converteste_la_int((char*)shared_mem);
			continut_crt_mem++;
			strcpy(s, converteste_la_char(continut_crt_mem));
			memcpy(shared_mem, s, sizeof(s));
			printf("	Si scrie in memorie: %s", shared_mem);
		}
	}

	return 0;
}