//Each line becomes an octave up from our lowest (E1)
// +24 to get to the octave for chords
//Each next value in the array is a half step up :)

#include "freq.h"

int freq[64] = {
    220, 233, 247, 262, 277, 294, 311,
    330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622,
    659, 698, 740, 784, 831, 880, 932, 988, 1047
};
