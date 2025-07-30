#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#define TIMES	100000
#define SIZE	100

typedef struct s_arr
{
	int	*arr;
	int	idx;
}	t_arr;

/* void	*funcc(void *arrs)
{
	t_arr	*a = (t_arr*) arrs;
	a->arr[a->idx] = a->idx;
	return NULL;
}

int	main()
{
	pthread_t	thread1;
	int 		times;
	int			idx;
	int			arr[SIZE];
	t_arr		arrs = {arr, 0};

	times = TIMES;
	while (times--)
	{
		if (idx >= 100)
			idx = 0;
		// pthread_create(&thread1, NULL, funcc, (void*) &arrs);
		// pthread_join(thread1, NULL);
		// idx++;



		arr[idx++] = times;

		// for(int i = 0; i < SIZE; i++) {
		// 	arr[i] = times;
		// }


		// char	*mall = malloc(SIZE);
		// if (!mall)
		// 	return perror("malloc failed: "), 1;
		// free (mall);

	}
} */


void	*funcc(void *arrs)
{
	t_arr	*a = (t_arr*) arrs;
	for (int i = 0; i < 100000; i++) {
		if (a->idx >= 100)
			a->idx = 0;
		a->arr[a->idx++] = i;
	}
	return NULL;
}

int	main()
{
	pthread_t thread;
	int arr[100];
	t_arr arrs = {arr, 0};

	pthread_create(&thread, NULL, funcc, (void*)&arrs);
	pthread_join(thread, NULL);
}
