#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char s1=0;
unsigned char s2=0;
unsigned char m1=0;
unsigned char m2=0;
unsigned char h1=0;
unsigned char h2=0;
ISR(TIMER1_COMPA_vect)//interrupt service routine of timer1.
{ //It enters the ISR when it did 978 cycles (1sec).
		++s1;
	if(s1>9)
	{
		s1=0;
		++s2;
	}
	if(s2>5)
	{
		s2=0;
		++m1;
	}
	if(m1>9)
	{
		m1=0;
		++m2;
	}
	if(m2>5)
	{
		m2=0;
		++h1;
	}
	if(h1>3)
	{
		h1=0;
		++h2;
	}
	if(h2>2)
	{
		h2=0;
	}
}
ISR (INT0_vect)//interrupt service routine of interrupt0.
{
	s1=0;//set variables to its initial values.
	s2=0;
	m1=0;
	m2=0;
	h1=0;
	h2=0;
	TCNT1=0;
}
ISR (INT1_vect)//interrupt service routine of interrupt1.
{
	TIMSK&=~(1<<OCIE1A);//Putting the module interrupt enable 0 make the timer stop at its final value.
}
ISR (INT2_vect)//interrupt service routine of interrupt2.
{
	TIMSK=(1<<OCIE1A);//Activate the module interrupt enable make the timer continue from its last value.
}
void TIMER1_CTC_INit (void)
{
	TCCR1A=(1<<FOC1A);//enable the compare mode non PWM for A.
	TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);//set the clock/1024.
	TCNT1=0;//the initial value.
	OCR1A=978;//when it do 978 turn it will be 1 second.
	TIMSK=(1<<OCIE1A);//enable the module interrupt.
	SREG=(1<<7);//enable any interrupt.
}
void INT0_INit (void)//definition of interrupt0.
{
	DDRD&=~(1<<PD2);//pin pd2 i/p pin and connect push button to it to control the interrupt.
	PORTD|=(1<<PD2);//Activate the  internal pull-up resistor at PD2.
	MCUCR|=(1<<ISC01);//set falling edge.
	GICR|=(1<<INT0);//enable pin into .
	SREG|=(1<<7);//enable any interrupt.
}
void INT1_INit (void)//definition of interrupt1.
{
	DDRD&=~(1<<PD3);//pin pd3 i/p pin and connect push button to it to control the interrupt.
	MCUCR|=(1<<ISC11)|(1<<ISC10);//set raising edge.
	GICR|=(1<<INT1);//enable pin int1 .
	SREG|=(1<<7);//enable any interrupt.
}
void INT2_INit (void)//definition of interrupt2.
{
	DDRB&=~(1<<PB2);//pin pb2 i/p pin and connect push button to it to control the interrupt.
	PORTB|=(1<<PB2);//Activate the internal pull-up resistor at PB2.
	MCUCSR&=~(1<<ISC2);//set falling edge.
	GICR|=(1<<INT2);//enable pin int2 .
	SREG|=(1<<7);//enable any interrupt.
}
int main()
{
	TIMER1_CTC_INit();//enable and configure Timer1.
	INT0_INit();//enable and configure external int0.
	INT1_INit();//enable and configure external int1.
	INT2_INit();//enable and configure external int2.
	DDRC|=0X0F;//first 4 pins of C are o/p pins and connected to the decoder.
	PORTC=(0 & 0x0f)|(PORTC & 0xf0);//display number 0 in the begin.
	DDRA|=0X3F;//first 6 pins of A are o/p act as dis-en to the 6 7-segments.
	while(1)
	{
		PORTA|=0X3F;//enable all 7-segments.
		while((1<<0)&PORTA)
		{
			PORTA=0X01;
			PORTC=(s1 & 0x0f)|(PORTC & 0xf0);
			_delay_us(0.0001);//we need fast delay that your eyes can't detect .
			PORTA=0X00;
		}
		PORTA|=0X3F;
		while((1<<1)&PORTA)//we will loop fast over each pin with fast delay to appear it as constant pins.
		{
			PORTA=0X02;
			PORTC=(s2 & 0x0f)|(PORTC & 0xf0);
			_delay_us(0.0001);
			PORTA=0X00;
		}
		PORTA|=0X3F;
		while((1<<2)&PORTA)
		{
			PORTA=0X04;
			PORTC=(m1 & 0x0f)|(PORTC & 0xf0);
			_delay_us(0.0001);
			PORTA=0X00;
		}
		PORTA|=0X3F;
		while((1<<3)&PORTA)
		{
			PORTA=0X08;
			PORTC=(m2 & 0x0f)|(PORTC & 0xf0);
			_delay_us(0.0001);
			PORTA=0X00;
		}
		PORTA|=0X3F;
		while((1<<4)&PORTA)
		{
			PORTA=0X10;
			PORTC=(h1 & 0x0f)|(PORTC & 0xf0);
			_delay_us(0.0001);
			PORTA=0X00;
		}
		PORTA|=0X3F;
		while((1<<5)&PORTA)
		{
			PORTA=0X20;
			PORTC=(h2 & 0x0f)|(PORTC & 0xf0);
			_delay_us(0.0001);
			PORTA=0X00;
		}
	}
	return 0;
}
