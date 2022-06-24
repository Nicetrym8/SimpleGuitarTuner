#define F_CPU 4000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "arduinoFFT.h"
#include "utility.h"
extern "C"{
	#include "nokia5110.h"
};
#include <avr/io.h>




volatile uint8_t mode = 0;
volatile uint8_t res=0;

const uint8_t arrow_Width = 7;
const uint8_t arrow_Height = 8;
const uint8_t PROGMEM arrow_Bitmap[] = {
	0xc0, 0xe0, 0xf8, 0xfe, 0xf8, 0xe0, 0xc0
};

const uint8_t PROGMEM logo_Bitmap[] = {
	0x00, 0x80, 0x80, 0xe0, 0xe0, 0xf8, 0xf8, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xf8, 0xf8, 0xe0, 0xe0, 0x80, 0x80, 0x00,
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x03, 0x01, 0xfc, 0xfc, 0x8c, 0x8c, 0x0c, 0x0c, 0x1c, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xf8, 0xf0, 0xe0, 0xe3, 0xe7, 0xe7, 0xe0, 0xe0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xe0, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0xe7, 0xe7, 0x1f, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x00, 0x03, 0x03, 0x0f, 0x0f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xff, 0xff, 0xf9, 0xf9, 0xff, 0xff, 0xfe, 0xfe, 0xf9, 0xf9, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x3f, 0x0f, 0x0f, 0x03, 0x03, 0x00
};



arduinoFFT FFT = arduinoFFT();

const uint16_t samples = 128; 
const uint32_t samplingFrequency = 3000;
const uint32_t sampling_period_us = round(F_CPU*(1./samplingFrequency));

double vReal[samples]; 
double vImag[samples];


char buf[10] = "";

ISR(INT0_vect){
	if(mode){
		if(++res>41)res =0;
	}
	_delay_ms(50);
}
ISR(INT1_vect){
	mode = !mode;
	_delay_ms(50);
}
void capture_wave (double* vReal, double* vImag, uint16_t count)
{

	ADMUX|=(1<<REFS0)|(1<<REFS1)|(1<<MUX0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1);
	do {
		ADCSRA |= (1<<ADSC);
		_delay_us(sampling_period_us);
		*vReal++ =ADC;
		*vImag++= 0;
	} while(--count);
	
	ADMUX = 0;
	ADCSRA = 0;
}

int main(void)
{
	
	float cents =0;
	EIMSK|=(1<<INT1)|(1<<INT0);
	DDRC &= ~(1 << PINC1);
	PORTD|=(1<<PORTD2)|(1<<PORTD3);
	nokia_lcd_init();
	nokia_lcd_draw_bitmap(logo_Bitmap,20,5,40,35);
	nokia_lcd_render();
	_delay_ms(2000);
    while (1) 
    {
		sei();
		nokia_lcd_clear();
		capture_wave(vReal,vImag,samples);
		FFT.DCRemoval();
		FFT.Windowing(vReal, samples, FFT_WIN_TYP_NUTTALL, FFT_FORWARD);
		FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
		FFT.ComplexToMagnitude(vReal, vImag, samples);
		float freq = FFT.MajorPeak(vReal, samples, samplingFrequency)/4.0826226551; 
		if(!mode)res = binary_search_by_freq(freq,&cents);
		else cents = CALC_CENTS(freq*100,res);
		if(cents>20)cents=20;
		else if(cents<-20)cents =-20;
		nokia_lcd_inverse_area(0,43,83,45);
		nokia_lcd_draw_bitmap(arrow_Bitmap,calc_arrow_pos((uint8_t)roundf(cents)),35,arrow_Width,arrow_Height);
		sprintf(buf," oct:%d mode:%d ",OCTAVE(res),mode);
		nokia_lcd_set_cursor(0,1);
		nokia_lcd_write_string(buf,1);
		nokia_lcd_inverse_area(0,0,83,8);
		sprintf(buf,"%s:%d",FREQ_TO_NOTE(res),(int)roundf(cents));
		nokia_lcd_set_cursor(0,15);
		nokia_lcd_write_string(buf,2);
		nokia_lcd_render();
    }
}



