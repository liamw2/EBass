
   
//Takes the bass note, whether it's a chord or not, and the variation
//sequence to determine an array of frequencies to play

//Bass = binary value from buttons (0-7) + string value (E=0, A=5, D=10, G=15)
//m = strings pressed - 1 (single note is 0, major chord is 1, minor is 2)
//alt = button1 + button2 * 2

#include <stdbool.h>
#include <stdio.h>

#include "chord_finder.h"
#include "freq.h"


void chord_builder(int bass, int m, int alt, int *chord) {
  //chord
  //printf("Looks like we made it\n");
  
  if (m){
    int bass_temp = bass+24;
    //printf("chord time!\n");
    //Major
    if (m = 1){
      //printf("ooh major, %d\n", alt);
      //Straight
      if (!alt){
	//printf("straight up\n");
        chord[0] = freq[bass_temp];
        chord[1] = freq[bass_temp+4];
        chord[2] = freq[bass_temp+7];
        chord[3] = freq[bass_temp+12];
	printf("Here we are, %d, %d, %d, %d\n", chord[0], chord[1], chord[2], chord[3]);
      }
      //7
      else if (alt = 1){
        chord[0] = freq[bass_temp];
        chord[1] = freq[bass_temp+4];
        chord[2] = freq[bass_temp+7];
        chord[3] = freq[bass_temp+10];
	printf("Here we are but special, %d, %d, %d, %d\n", chord[0], chord[1], chord[2], chord[3]); 
      }
      //2
      else if (alt = 2){
        chord[0] = freq[bass_temp];
        chord[1] = freq[bass_temp+2];
        chord[2] = freq[bass_temp+4];
        chord[3] = freq[bass_temp+7];
      }
      //4
      else if (alt = 3){
        chord[0] = freq[bass_temp];
        chord[1] = freq[bass_temp+4];
        chord[2] = freq[bass_temp+5];
        chord[3] = freq[bass_temp+7];
	printf("huh\n");
      }
    }
    
    //Minor
    else if (m = 2){
      //Straight
      if (!alt){
        chord[0] = freq[bass_temp];
        chord[1] = freq[bass_temp+3];
        chord[2] = freq[bass_temp+7];
        chord[3] = freq[bass_temp+12];
      }
      //m7
      else if (alt = 1){
        chord[0] = freq[bass_temp];
        chord[1] = freq[bass_temp+3];
        chord[2] = freq[bass_temp+7];
        chord[3] = freq[bass_temp+10];
      }
      //dim
      else if (alt = 2){
        chord[0] = freq[bass_temp];
        chord[1] = freq[bass_temp+3];
        chord[2] = freq[bass_temp+6];
        chord[3] = freq[bass_temp+12];
      }
    }
    printf("%d, %d, %d, %d\n", chord[0], chord[1], chord[2], chord[3]);
    
  }
  //single note
  else {
    int note[1];  
    note[0] = freq[bass];
    
  }
  
}
