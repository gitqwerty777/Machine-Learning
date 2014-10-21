#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
  int size = atoi(argv[1]);
  srand(time(NULL));
  FILE* f = fopen("17data", "w");
  for(int i = 0; i < size; i++){
    double n = double(rand()) / 2147483647;
    int r = rand();
    if(r % 2)
      n = -n;
    int output = (n > 0)?1:-1;
    if(r % 10 == 1 || r % 10 == 2)
      output = -output;
    fprintf(f, "%lf %d\n", n, output);
  }
  fclose(f);
  return 0;
}
