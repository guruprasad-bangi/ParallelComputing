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

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

  
#ifdef __cplusplus
}
#endif

void merge(int *a, unsigned long l, unsigned long m, unsigned long r);
unsigned long min2(unsigned long a1, unsigned long a2);

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

  unsigned long n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);  

  omp_set_num_threads(nbthreads);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  for (unsigned long len=1; len<=n-1; len = 2*len)
  {
    #pragma omp parallel for schedule(static)
    for (unsigned long ls=0; ls<n-1; ls += 2*len)
    {
      unsigned long mid = min2(ls+len-1, n-1);
      unsigned long right = min2(ls+(2*len)-1, n-1);
      merge(arr, ls, mid, right);
    }
  }
     
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  checkMergeSortResult (arr, atoi(argv[1]));
  
  delete[] arr;

  return 0;
}

unsigned long min2(unsigned long a1, unsigned long a2)
{
  return a1 < a2 ? a1 : a2;
}

void merge(int *a, unsigned long l, unsigned long m, unsigned long r)
{
  unsigned long n1 = m - l + 1, n2 = r - m;

  // Creating left and right arrays
  int L[n1], R[n2];
  for (unsigned long i = 0; i < n1; i++)
  {
    L[i] = a[l + i]; // from l to l+n1-1
  }
  for (unsigned long j = 0; j < n2; j++)
  {
    R[j] = a[m + 1+ j]; // from m+1 to m+n2
  }

  
  unsigned long i = 0, j = 0, k = l;

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
  return;
}


