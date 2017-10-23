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

// void merge(int *a[], int startIndex, int endIndex);
// void mergeSort(int* array, int start, int end);
void generateBubbleSortData (int* arr, size_t n);

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
  
  if (argc < 5) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <schedule> granularity"<<std::endl;
    return -1;
  }

  int * arr = new int [atoi(argv[1])];

  generateMergeSortData (arr, atoi(argv[1]));

  //write code here

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);  
  std::string scheduling = argv[3];
  int granularity = atoi(argv[4]);

  // for(int a=0 ; a<n ; a++)
  //   printf("%d  ",arr[a]);

  // printf("\n");

  

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();


  if(scheduling == "static")
        omp_set_schedule(omp_sched_static,granularity);
      else if(scheduling == "dynamic")
        omp_set_schedule(omp_sched_dynamic,granularity);
      else if(scheduling == "guided")
        omp_set_schedule(omp_sched_guided,granularity);
      else std::cerr << "Invalid scheduling options entered" << std::endl;

  //bubblesort(arr, 0, n-1);
  omp_set_num_threads(nbthreads);
  #pragma omp parallel 
    {
      // long result =0;
    //#pragma omp single         
      for(int j=0; j<n ; j++)  
        #pragma omp for schedule(runtime)   
          for(int i= j%2 ; i <n-1 ; i=i+2){           
            if(arr[i] > arr[i+1]){
                int temp = arr[i+1];
                arr[i+1] = arr[i];
                arr[i] = temp;
            }            
          } 
          // for(int a=0 ; a<n ; a++)
          //     printf("%d  ",arr[a]);

          //   printf("\n");         
    }
     
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  checkMergeSortResult (arr, atoi(argv[1]));
  
// for(int a=0 ; a<n ; a++)
//   printf("%d  ",arr[a]);

// printf("\n");

  delete[] arr;

  return 0;
}


// void generateBubbleSortData (int* arr, size_t n){
//   int* array = arr;
//   long arraySize = n;
//   srand(time(NULL));
//   for (int i=0 ; i<arraySize ; i++){
//     array[i] = arraySize-i;
//   }
// }