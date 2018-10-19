#include "font.h"

#define DATA			(2)
#define CLK				(3)
#define NUM_OF_DISPLAYS	(2)

uint8_t column[] = {4,5,6,7,8};

int brightness = 1500; // keep high for max visibility. To high will cause flikkering 
int buffer[NUM_OF_DISPLAYS*4*5]; // this is the array that store's what on the displays. 4 digits per display and 5 columns per digit.

unsigned long tijd; // used to trigger events in one of the examples below

void setup(){
	Serial.begin(9600);
	for(uint8_t i = 0; i < 5; i++){
		pinMode(column[i], OUTPUT);
		digitalWrite(column[i], LOW);
	}
	pinMode(DATA, OUTPUT);
	pinMode(CLK, OUTPUT);
	digitalWrite(DATA, LOW);
	digitalWrite(CLK, HIGH);
	tijd = millis();
}

void loop(){
	char clr[] = "    ";
	char ten[] = "....ten...";
	char nine[] = "....nine....";
	char eight[] = "....eight...";    
	char seven[] = "....seven...";     
	char six[] = "....six...";  
	char five[] = "....five...";
	char four[] = "....four...";
	char three[] = "....three...";
	char two[] = "....two...";
	char one[] = "....one...";
	char zero[] = "....zero....";
	char hello[] = "Hello";
	char world[] = "World!!";

	scrollText(nine, 8);
	delay(300);
	scrollText(eight, 8);
	delay(300);
	scrollText(seven, 8); 
	delay(300);
	scrollText(six, 8);
	delay(300);
	scrollText(five, 8);
	delay(300);
	scrollText(four, 8);
	delay(300);
	scrollText(three, 8);  
	delay(300);
	scrollText(two, 8);
	delay(300);
	scrollText(one, 8);
	delay(300);
	scrollText(zero, 8);
	delay(100);

	//helloworld
	scrollText(hello, 16);
	scrollText(world, 16);
	delay(1000);  

	writeText("they") ;
	for (int i = 0; i< 100; i++) draw();
	delay(5);
	writeText(" see") ; 
	for (int i = 0; i< 70; i++) draw();
	delay(5);
	writeText(" me") ;
	for (int i = 0; i< 100; i++) draw();
	delay(50);
	writeText(" rollin'") ;
	for (int i = 0; i< 50; i++) draw();
	delay(50);
}

void writeDigit(int positie, int ascii){
	for(uint8_t i =0; i< 5;i++){
		buffer[(NUM_OF_DISPLAYS * 4 - positie) * 5 + i] = font[ascii*5+i];
	}
}

void scrollText(String tekkst, long howSlow){
	int leng = tekkst.length();
	for (int u =-NUM_OF_DISPLAYS*4; u<leng; u++){ // first N digits will be spaces. N = all digits of display. So one screen blank before first character apears.
		for (int v=0; v< NUM_OF_DISPLAYS*4; v++){
			if (( (u+v) >= 0 ) && ((u+v) < leng)){ // separete leading and tailing spaces from actual string
				char oneChar= tekkst.charAt(u+v);
				writeDigit(v+1, oneChar); // write the character of the string
			}
			else{
				writeDigit(v+1, 32); // write ascii-value 32 = a space.
			}
		}
		for (long teller = 0; teller < howSlow; teller++){
			draw();   
		}
	}
}

void writeText(String tekst){
	int tl = tekst.length();
	int spacesNeeded = 4 * NUM_OF_DISPLAYS - tl;
	if(spacesNeeded <0){
		tl = 4 * NUM_OF_DISPLAYS;    
	}
	else{
		for(uint8_t u = 0; u < spacesNeeded; u++){
			tekst.concat(" ");
		}
		tl = 4 * NUM_OF_DISPLAYS;    
	}
	char tekst_[tl];
	for (uint8_t vv = 0; vv < tl; vv++){
		char tt = tekst.charAt(tl-vv-1);
		for(uint8_t i =0; i< 5;i++){
			buffer[vv*5+i] = font[tt*5+i];
		}
	}
}

void dot(byte x, byte y, boolean oo){
	if((x < (NUM_OF_DISPLAYS *4 * 5)) && (y < 7)){
		byte oldByte = buffer[x] & (0xFF - (2<<y));
		buffer[x] = oldByte | (2<<y);
	}
}

void draw(){
	for(uint8_t col = 0; col < 5; col++){	// 5 columns to be strobed
		// pumping in 3 x 4 x 7 bits per one strobe of the column
		for(uint8_t d = 0; d < NUM_OF_DISPLAYS; d++){	// number of displays
			for(uint8_t i = 0; i < 4; i++){	// 4 characters per display
				for(uint8_t r = 0; r < 7; r++){ // 7 row-elements
					digitalWrite(CLK, HIGH);
					digitalWrite(DATA, buffer[col + 5*(d*4 + i) ] &(2<<r)); // magic
					digitalWrite(CLK, LOW);
				}
			}
		}
		digitalWrite(column[col], HIGH);
		delayMicroseconds(brightness);
		digitalWrite(column[col], LOW);
	}
}
