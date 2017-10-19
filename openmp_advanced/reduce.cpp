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
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  int * arr = new int [atoi(argv[1])];

  generateReduceData (arr, atoi(argv[1]));
  
  //write code here
  long result=0;
  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  std::string scheduling = argv[3];
  int granularity = atoi(argv[4]);
  

   omp_set_num_threads(nbthreads);

  if(scheduling == "static")
        omp_set_schedule(omp_sched_static,granularity);
      else if(scheduling == "dynamic")
        omp_set_schedule(omp_sched_dynamic,granularity);
      else if(scheduling == "guided")
        omp_set_schedule(omp_sched_guided,granularity);
      else std::cerr << "Invalid scheduling options entered" << std::endl;

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  #pragma omp parallel
    {
      
    #pragma omp single
      {
        #pragma omp task
        for(int j=0; j<n ; j++)
          
          {
            result += arr[j];
          }
      }
    }
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  std::cout << result << std::endl;

  delete[] arr;

  return 0;
}
