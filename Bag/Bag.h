/*
 * bag.h - Library for controlling airbags
 * Created by Magnus Borg, November 3, 2012.
 * Released into the public domain.
*/
#ifndef BAG_H
#define BAG_H

#include "Arduino.h"

#define BAG_INCREASE_TIME 400	
#define BAG_DECREASE_TIME 50
#define RELAY_ON LOW
#define RELAY_OFF HIGH

class Bag
{
  public:
    Bag(int increase_pin, int decrease_pin, int pressure_pin);
    
    void increase();
    void increase_start();
	void increase_stop();
	
    void decrease();
    void decrease_start();
    void decrease_stop();
  
  	void read_pressure();
  	
  	float pressure_bar;
  	int pressure_psi;
  	
  private:
    int _inc_pin;
    int _dec_pin;
    int _pre_pin;
};

#endif