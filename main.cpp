#include <iostream>
#include <omp.h>

long long num = 100000000;
double step;

double par_nothing(void)
{
	int num_of_threads = 3;
	omp_set_num_threads(num_of_threads);
	long long inc = 0;
	long long i = 0;
	double x = 0.0;
	double pi;
	double S = 0.0;
	step = 1.0 / (double)num;
	double t = omp_get_wtime();
	#pragma omp parallel private(i, x) reduction ( + : S )
	for (i = 0; i < num; i++)
	{
		x = (i + 0.5)*step;
		S = S + 4.0 / (1.0 + x*x);
	}
	t = omp_get_wtime() - t;
	pi = step * (S / num_of_threads);
	printf("Par: pi = %.14f\n", pi);
	return t;
}

double par_atomic(void)
{
	int num_of_threads = 3;
	omp_set_num_threads(num_of_threads);
	long long inc = 0;
	long long i = 0;
	double x = 0.0;
	double pi;
	double S = 0.0;
	step = 1.0 / (double)num;
	double t = omp_get_wtime();
#pragma omp parallel private(i, x) reduction ( + : S )
	for (i = 0; i < num; i++)
	{
		x = (i + 0.5)*step;
		S = S + 4.0 / (1.0 + x*x);
		#pragma omp atomic
		inc++;
	}
	t = omp_get_wtime() - t;
	pi = step * (S / num_of_threads);
	printf("Par: pi = %.14f\n", pi);
	return t;
}

double par_critical(void)
{
	int num_of_threads = 3;
	omp_set_num_threads(num_of_threads);
	long long inc = 0;
	long long i = 0;
	double x = 0.0;
	double pi;
	double S = 0.0;
	step = 1.0 / (double)num;
	double t = omp_get_wtime();
#pragma omp parallel private(i, x) reduction ( + : S )
	for (i = 0; i < num; i++)
	{
		x = (i + 0.5)*step;
		S = S + 4.0 / (1.0 + x*x);
		#pragma omp critical
		inc++;
	}
	t = omp_get_wtime() - t;
	pi = step * (S / num_of_threads);
	printf("Par: pi = %.14f\n", pi);
	return t;
}

double par_lock(void)
{
	int num_of_threads = 3;
	omp_set_num_threads(num_of_threads);
	long long inc = 0;
	long long i = 0;
	double x = 0.0;
	double pi;
	double S = 0.0;
	step = 1.0 / (double)num;
	omp_lock_t writelock;
	omp_init_lock(&writelock);
	double t = omp_get_wtime();
#pragma omp parallel private(i, x) reduction ( + : S )
	for (i = 0; i < num; i++)
	{
		x = (i + 0.5)*step;
		S = S + 4.0 / (1.0 + x*x);
		omp_set_lock(&writelock);
		inc++;
		omp_unset_lock(&writelock);
	}
	t = omp_get_wtime() - t;
	pi = step * (S / num_of_threads);
	printf("Par: pi = %.14f\n", pi);
	return t;
}

int main()
{
	printf("Nothing: time: %f sec.\n\n", par_nothing());
	printf("Atomic: time: %f sec.\n\n", par_atomic());
	printf("Critical: time: %f sec.\n\n", par_critical());
	printf("Lock: time: %f sec.\n\n", par_lock());
	//char c;
	//scanf_s("%c", &c);
	return 0;
}

