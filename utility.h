/*
 * utility.h
 *
 * Created: 22.04.2022 23:33:53
 *  Author: gritist
 */ 


#ifndef UTILITY_H_
#define UTILITY_H_
#define OCTAVE(_i) (uint8_t)(_i/12)+1
#define CALC_CENTS(_x,_i) 120*log2(_x/Notes_freq_set[_i])
#define FREQ_TO_NOTE(_i) Notes_letter_set[_i%12]

#include <avr/io.h>
#include <math.h>

const char Notes_letter_set[][3] {
	"C","C#","D","D#","E","F","F#","G","G#","A","A#","B",
};

const uint16_t Notes_freq_set[] {
	3270, 3465, 3671, 3889, 4120, 4365, 4625, 4900, 5191, 5500, 5827, 6174,
	6541, 6930, 7342, 7778, 8241, 8731, 9250, 9800, 10383, 11000, 11654, 12347,
	13081, 13859, 14683, 15556, 16481, 17461, 18500, 19600, 20765, 22000, 23308, 24694,
	26163, 27718, 29366, 31113, 32963, 34923,
};

float log2(float val);
uint8_t binary_search_by_freq(float x, float* cents);
uint8_t calc_arrow_pos(int8_t cents);

#endif /* UTILITY_H_ */