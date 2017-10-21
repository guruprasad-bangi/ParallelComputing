#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  //void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

  
#ifdef __cplusplus
}
#endif

void merge(int *a[], int startIndex, int endIndex);
void mergeSort(int* array, int start, int end);
void generateMergeSortData (int* arr, size_t n);

int main (int argc, char* argv[]) {

  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }
  
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int * arr = new int [atoi(argv[1])];

  generateMergeSortData (arr, atoi(argv[1]));

  //write code here

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);  

  for(int a=0 ; a<n ; a++)
    printf("%d  ",arr[a]);

  printf("\n");

  

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  mergeSort(arr, 0, n-1);
     
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  //checkMergeSortResult (arr, atoi(argv[1]));
  
  for(int a=0 ; a<n ; a++)
    printf("%d  ",arr[a]);

  delete[] arr;

  return 0;
}

void merge(int *a, int l, int m, int r)
{
  int n1 = m - l + 1, n2 = r - m;

  // Creating left and right arrays
  //int L[n1], R[n2];

  int * L = new int [n1];
  int * R = new int [n2];

  for (int i = 0; i < n1; i++)
  {
    L[i] = a[l + i]; // from l to l+n1-1
  }
  for (int j = 0; j < n2; j++)
  {
    R[j] = a[m + 1+ j]; // from m+1 to m+n2
  }

  
  int i = 0, j = 0, k = l;

  // Merging 2 arrays of length n1 & n2
  while (i < n1 && j < n2)
  {
    if (L[i] <= R[j])
    {
      a[k++] = L[i++];
    }
    else
    {
      a[k++] = R[j++];
    }
  }

  if(j == n2) // in case R finishes first, leftover elements of L will get inserted as it is
  {
    while (i < n1)
    {
      a[k++] = L[i++];
    }
  }
  else if(i == n1) // in case L finishes first, leftover elements of R will get inserted as it is
  {
    while (j < n2)
    {
      a[k++] = R[j++];
    }
  }

  delete[] R;
  delete[] L;
  
  return;
}


void mergeSort(int* array, int start, int end) {
    if(start < end) {

        //omp_set_num_threads(nbthreads);
        //printf("Thread %d is sorting %d through %d\n", omp_get_thread_num(), start, end);
        int middle = (start + end) / 2;

      /*  mergeSort(array, start, middle);

        mergeSort(array, middle + 1, end);

        merge(array, start,middle, end);*/
        /* sort both halves in parallel */
        #pragma omp parallel 
        {
            #pragma omp single
            {
                #pragma omp task
                mergeSort(array, start, middle);

                #pragma omp task
                mergeSort(array, middle + 1, end);
            
        
        #pragma omp taskwait
        /* merge the two halves */
        merge(array, start, middle, end);
      }
      }
    }
}

void generateMergeSortData (int* arr, size_t n){
  int* array = arr;
  long arraySize = n;
  srand(time(NULL));
  //int[arraySize] array= {0};
  for (int i=0 ; i<arraySize ; i++){
    array[i] = arraySize-i;
    //array[i] = rand() % arraySize;
    
  }
}