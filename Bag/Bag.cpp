/*
 * bag.h - Library for controlling airbags
 * Created by Magnus Borg, November 3, 2012.
 * Released into the public domain.
*/
#include "Bag.h"

Bag::Bag(int increase_pin, int decrease_pin, int pressure_pin)
{
	_inc_pin = increase_pin;
	_dec_pin = decrease_pin;
	_pre_pin = pressure_pin;
	
	pressure_bar = -1;
	pressure_psi = -1;
	
	pinMode(_inc_pin, OUTPUT);
	digitalWrite(_inc_pin, RELAY_OFF); 
	pinMode(_dec_pin, OUTPUT);
	digitalWrite(_dec_pin, RELAY_OFF); 
}

void Bag::increase()
{
	digitalWrite(_inc_pin, RELAY_ON);	
	delay(BAG_INCREASE_TIME);
	digitalWrite(_inc_pin, RELAY_OFF); 
	pressure_psi++; //DEBUG
}

void Bag::increase_start()
{
	digitalWrite(_inc_pin, RELAY_ON); 
}

void Bag::increase_stop()
{
	digitalWrite(_inc_pin, RELAY_OFF); 
}

void Bag::decrease()
{
	digitalWrite(_dec_pin, RELAY_ON);	
	delay(BAG_DECREASE_TIME);
	digitalWrite(_dec_pin, RELAY_OFF); 
	pressure_psi--; //DEBUG
}

void Bag::decrease_start()
{
	digitalWrite(_dec_pin, RELAY_ON);
}

void Bag::decrease_stop()
{
	digitalWrite(_dec_pin, RELAY_OFF);
}

void Bag::read_pressure()
{
	// TODO: Lookup table
	// TODO: Average out over last 3 measurements
	int val = analogRead(_pre_pin);
	float voltage = val/1023.0*5.0;//map(val, 0, 1023, 0, 5);
	pressure_psi = (26.506*voltage*voltage) + (1.770*voltage) + 1.447;
	pressure_bar =  pressure_psi / 14.7;
}