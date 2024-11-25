#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint32_t *stringTo32Bit(const char *);
uint32_t *sigmaFunction(uint32_t *);
uint32_t rotFunctions(uint32_t, int);
uint32_t generalFunctions(uint32_t, uint32_t, uint32_t,int);


int main(int argc, char* argv[]) {
  if(argc != 2){
    perror("Usage: program <word>");
    return EXIT_FAILURE;

  }
  
  static const unsigned int k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
  };

  unsigned int p[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372,
    0xa54ff53a, 0x510e527f, 0x9b05688c,
    0x1f83d9ab, 0x5be0cd19
  };

  unsigned int aux[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372,
    0xa54ff53a, 0x510e527f, 0x9b05688c,
    0x1f83d9ab, 0x5be0cd19
  };
  
  char *word = argv[1];
  
  uint32_t *w = stringTo32Bit(word); 
  w = sigmaFunction(w); 
  
  uint32_t auxiliar_var = 0;
  uint32_t auxiliar_var2 = 0;

  for(int j = 0; j < 64; j++){
    
    uint32_t T1 = p[7] +  generalFunctions(p[4],p[4],p[4],3) + generalFunctions(p[4],p[5],p[6],0) + k[j] + w[j];
    uint32_t T2 = generalFunctions(p[0],p[0],p[0],2) +  generalFunctions(p[0],p[1],p[2],1);

    auxiliar_var = p[0];
    p[0] = T1 + T2;
    
    for(int i = 1; i < 8; i++){

      auxiliar_var2 = p[i];

      if(i == 4){
	p[4] = auxiliar_var + T1;
      } else {
	p[i] = auxiliar_var;
      }
      
      auxiliar_var = auxiliar_var2; 

    }
    
  }
  
  for(int m = 0; m < 8; m++){
    printf("%.8x\n",p[m]);

  }
  printf("\n");

  return 0;
}

uint32_t generalFunctions(uint32_t x, uint32_t y, uint32_t z, int tp){

  switch(tp){
  case 0:
    return ((x & y) ^ ( ~x & z)); //Ch
  case 1:
    return ((x & y) ^ ( x & z) ^ (y ^ z)); // Maj
  case 2:
    return ( ( (x >> 2)   | (x << (32 - 2)))) ^ ( ( (x >> 13)   | (x << (32 - 13)))) ^ ( ( (x >> 22)   | (x << (32 - 22)))); //SIGMA 0
  case 3:
    return ( ( (x >> 6)   | (x << (32 - 6)))) ^ ( ( (x >> 11)   | (x << (32 - 11)))) ^ ( ( (x >> 25)   | (x << (32 - 25)))); //SIGMA 1
  case 4:
    return  ((x >> 3) & (uint32_t)-1 ) ^ ( (x >> 7)   | (x << (32 - 7)))  ^ ( (x >> 18)  | (x << (32 - 18))); //sigma 0
  case 5:
    return  ((x >> 10)& (uint32_t)-1 ) ^ ( (x >> 17)  | (x << (32 - 17))) ^ ( (x >> 19)  | (x << (32 - 19)));  // sigma 1
  }

}

uint32_t *sigmaFunction(uint32_t *array){
  for(int j = 16; j < 64; j++){
      
    *(array + j) = *(array + (j-7)) + *(array + (j-16)) + generalFunctions(*(array + (j - 15)),*(array + (j - 15)),*(array + (j - 15)),4) + generalFunctions(*(array + (j - 2)),*(array + (j - 15)),*(array + (j - 15)),5);

  }
  return array;
}



//Wt variable 
uint32_t *stringTo32Bit(const char *str) {
  uint32_t *array = (uint32_t *)calloc(64,sizeof(uint32_t));

  if(array == NULL){
    perror("Allocation memory error");
    exit(1);
  }

  uint32_t result = 0;
  uint32_t *aux = array;

  int large = strlen(str);

  int j = 0;

  int r = 0;

  for (int i = 0; i < large; i++) { 
    
    uint32_t element = str[i] << (8 * (3 - j));
    result |= element; 

    if((i > 0 && (i + 1) % 4 == 0)){
      *(aux + r) = result;
      r++;
      j = 0;
      result = 0;
      continue;

    }
    
    j++;

  }

  uint32_t z = 0x80000000;

  result |= (z >> 8*(large % 4));
  
  *(aux + r) = result;

  *(aux + 15) |= (8*large);
  
  return array; 
}
