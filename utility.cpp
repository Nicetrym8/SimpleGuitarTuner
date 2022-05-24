#include "utility.h"

float log2(float val) {
	union { float val; int32_t x; } u = { val };
	register float log_2 = (float)(((u.x >> 23) & 255) - 128);
	u.x   &= ~(255UL << 23UL);
	u.x   += 127UL << 23UL;
	log_2 += ((-0.3358287811f) * u.val + 2.0f) * u.val  -0.65871759316667f;
	return (log_2);
}
uint8_t calc_arrow_pos(int8_t cents){
	if(cents >5)cents =5;
	else if(cents <-5)cents=-5;
	return 40+cents*4;
	
}
uint8_t binary_search_by_freq(float x, float* cents)
{
	int l=0; int r = 41;
	x *= 100;
	while (l<=r) {
		int m = l + (r - l) / 2;
		*cents = CALC_CENTS(x,m);
		if (fabs(*cents)<=6)
		return m;
		if (Notes_freq_set[m] < x)
		l = m + 1;
		else
		r = m - 1;
	}
}


