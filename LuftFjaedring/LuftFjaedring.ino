

/*
 * Luftfjaedring v0.0.1
 * A sketch that controls 4 way air bags in a vehicle using adhoc wifi
 * Author Magnus Borg November 5 2012 
 */
 
#include <WiServer.h>
#include <Bag.h>

#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

#define DATA_USE_JSON
#define USE_SERIAL

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {192,168,1,1};	// IP address of WiShield
unsigned char gateway_ip[] = {192,168,1,1};	// router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = {"Max's Impala"};		// max 32 bytes

unsigned char security_type = 0;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"12345678"};	// max 64 characters

// WEP 128-bit keys
prog_uchar wep_keys[] PROGMEM = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,	// Key 0
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 1
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 2
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	// Key 3
				};

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_ADHOC;

unsigned char ssid_len;
unsigned char security_passphrase_len;
// End of wireless configuration parameters --------------------------------

// Airbags configuration parameters ----------------------------------------
Bag bag_FL(0, 2, A1); Bag bag_FR(1, 3, A2);
Bag bag_RL(4, 6, A3); Bag bag_RR(5, 7, A4);

//Tank tank(9);

// End of airbags configuration parameters ---------------------------------

// This is our page serving function that generates web pages
boolean sendMyPage(char* URL) {
  
    if (strcmp(URL, "/") == 0) {
        WiServer.print("<html>");
        WiServer.print("To add air go to /bag/fl/add for the bag of choice, FL, FR, RL, RR");
        WiServer.print("<br><br>");
        WiServer.print("To remove air go to /bag/fl/dec for the bag of choice, FL, FR, RL, RR");
        WiServer.print("</html>");
        
        return true;
    }
    else if (strcmp(URL, "/test") == 0) {
      bag_FL.increase();
      bag_FR.increase();
      bag_RL.increase();
      bag_RR.increase();
      return true;
    }
// FRONT LEFT
    else if (strcmp(URL, "/fl/add") == 0) { // Front Left +
        bag_FL.increase();
        // TODO: return HTML
        return true;
    }
    else if (strcmp(URL, "/fl/add_start") == 0) {
        bag_FL.increase_start();
        // TODO: return HTML
        return true;
    }
    else if (strcmp(URL, "/fl/add_stop") == 0) {
        bag_FL.increase_stop();
        // TODO: return HTML
        return true;
    }
    else if (strcmp(URL, "/fl/dec") == 0) { // Front Left -
        bag_FL.decrease();
        // TODO: return HTML    
        return true;
    }
    else if (strcmp(URL, "/fl/dec_start") == 0) {
        bag_FL.decrease_start();
        // TODO: return HTML    
        return true;
    }
    else if (strcmp(URL, "/fl/dec_stop") == 0) {
        bag_FL.decrease_stop();
        // TODO: return HTML    
        return true;
    }
// FRONT RIGHT
    else if (strcmp(URL, "/fr/add") == 0) { // Front Right +
        bag_FR.increase();
        // TODO: return HTML
        return true;
    }
    else if (strcmp(URL, "/fr/dec") == 0) { // Front Right -
        bag_FR.decrease();
        return true;
    }
// REAR LEFT
    else if (strcmp(URL, "/rl/add") == 0) { // Rear Left -
        bag_RL.increase();
        // TODO: return HTML
        return true;
    }
    else if (strcmp(URL, "/fr/dec") == 0) {  // Rear Left -
        bag_RL.decrease();
        // TODO: return HTML
        return true;
    }
// REAR RIGHT    
    else if (strcmp(URL, "/rr/add") == 0) {  // Rear Right +
        bag_RR.increase();
        // TODO: return HTML
        return true;
    }
    else if (strcmp(URL, "/rr/dec") == 0) { // Rear Right -
        bag_RR.decrease();
        // TODO: return HTML      
        return true;
    }
    else if (strcmp(URL, "/all/info") == 0) {
#ifdef DATA_USE_JSON   
        WiServer.print("[");
        WiServer.print("{ \"bag\":\"fr\", \"psi\":\" "); WiServer.print(bag_FR.pressure_psi); WiServer.print("\"},");
        WiServer.print("{ \"bag\":\"fl\", \"psi\":\" "); WiServer.print(bag_FL.pressure_psi); WiServer.print("\"},");
        WiServer.print("{ \"bag\":\"rr\", \"psi\":\" "); WiServer.print(bag_RR.pressure_psi); WiServer.print("\"},");
        WiServer.print("{ \"bag\":\"rl\", \"psi\":\" "); WiServer.print(bag_RL.pressure_psi); WiServer.print("\"}");
        WiServer.print("]");
#endif
        return true;
    }
    
    // URL not found
    return false;
}

int inByte = 0; // incoming serial byte

#ifdef USE_SERIAL
void checkSerial() {
  if (Serial.available() > 0) {
    inByte = Serial.read();
   
    Serial.write("Recieved serial inbyte=");
    Serial.write(inByte + "\n"); 
    
    if(inByte == '`') {
      bag_FL.increase();
      bag_FR.increase();
      bag_RL.increase();
      bag_RR.increase();
    } 
    else if(inByte == '1') { // Front Left +
       bag_FL.increase();
    } 
    else if(inByte == 'q') { // Front Left - 
       bag_FL.decrease();
    } 
    else if(inByte == '2') { // Front Right +
       bag_FR.increase();
    } 
    else if(inByte == 'w') { // Front Right -
       bag_FR.decrease();
    }
    else if(inByte == 'a') { // Rear Left +
       bag_RL.increase();
    } 
    else if(inByte == 'z') { // Rear Left -
       bag_RL.decrease();
    } 
    else if(inByte == 's') { // Rear Left +
      bag_RR.increase();
    } 
    else if(inByte == 'x') { // Rear Right -
       bag_RR.decrease();
    }  

  }
}
#endif

void read_pressure() {
   bag_FL.read_pressure();
   bag_FR.read_pressure();
   bag_RL.read_pressure();
   bag_RR.read_pressure();
}

#define READ_CNT_INTERVAL 10
int read_cnt = 0;

void process_bags() {
  if(read_cnt++ % READ_CNT_INTERVAL == 0) {
    read_pressure();
#ifdef DEBUG_BAGS    
    Serial.print("\nFL val = ");
    Serial.print(bag_FL.pressure_psi);
#endif
  }
  
}

void setup() {
   // Pul down for RX pin 0 , TX pin 1 
  //gitalWrite(0, LOW); digitalWrite(1, LOW); 
  
#ifdef USE_SERIAL
  Serial.begin(19200);
  Serial.println("Entering setup");
  Serial.println("Initilizing WiServer");
#endif

  WiServer.init(sendMyPage);
  WiServer.enableVerboseMode(true);

#ifdef USE_SERIAL  
  Serial.println("Done with setup");
#endif
}
  
void loop() {
  // Run server task
  WiServer.server_task();
 
#ifdef USE_SERIAL
  // Check serial commands  
  checkSerial();
#endif 

  // Run bags tasks
  process_bags();
  
  delay(10);
}






