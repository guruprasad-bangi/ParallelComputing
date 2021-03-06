#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  void generateReduceData (int* arr, size_t n);
  
#ifdef __cplusplus
}
#endif

//void generateReduceData (int* arr, size_t n);
long reduce_sum(int *a, size_t n);

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

  generateReduceData (arr, atoi(argv[1]));
  
  //write code here
  long sum =0;
  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);  

  omp_set_num_threads(nbthreads);  

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  long result =0;

  // #pragma omp parallel 
  //   {
      
  //   // #pragma omp single         
  //     for(int j=0; j<n ; j++)  
  //       #pragma omp task   
  //       {
  //         result += arr[j];
  //       }      
  //   sum = result;
  //   }

  #pragma omp parallel
  {
    #pragma single
    #pragma omp task
      result = reduce_sum(arr, n);
  }
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  std::cout << result << std::endl;

  delete[] arr;

  return 0;
}

// void generateReduceData (int* arr, size_t n){
//   int* array = arr;
//   long arraySize = n;
//   srand(time(NULL));
//   for (int i=0 ; i<arraySize ; i++){
//     array[i] = 1;
//   }
// }

long reduce_sum(int *a, size_t n){

  if(n == 1)
    return a[0];

  long x,y;
  size_t half = n/2;

  #pragma omp task shared(x)
  x = reduce_sum(a,half);

  #pragma omp task shared(y)
  y= reduce_sum(a + half, n - half);

  #pragma omp taskwait
  x+=y;

  return x;
}

