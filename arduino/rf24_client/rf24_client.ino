#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "printf.h"

#define RADIO_CE_PIN 9
#define RADIO_CS_PIN 10
#define RADIO_CHANNEL 0x4c

#define PAYLOAD_SIZE 10
#define LOOP_DELAY 1000
#define RETRIES_DELAY 15
#define RETRIES_COUNT 15

RF24 radio(RADIO_CE_PIN, RADIO_CS_PIN);

const uint64_t pipes[] = { 0x5250493031LL, 0x4152443031LL };

void setup(void){
  Serial.begin(9600);
  radio.begin();
  radio.setRetries(RETRIES_DELAY, RETRIES_COUNT);
  radio.setChannel(RADIO_CHANNEL);
  radio.setAutoAck(0);
  radio.setDataRate(RF24_1MBPS);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();  
  printf_begin();
  radio.printDetails();
}

void loop(void){
  bool status; 
  char serverResponse[PAYLOAD_SIZE];
  char command[PAYLOAD_SIZE];
  
  // Risposta dal server
  if (radio.available()) {
    radio.read(serverResponse, PAYLOAD_SIZE);
    Serial.print("Ricezione OK    - Risposta dal server          : ");
    Serial.println(serverResponse);
  }  

  // Invia comando al RaspberryPi
  radio.stopListening();  
  strcpy(command, "CMD000001");  
  status = radio.write(command, PAYLOAD_SIZE);
  if (status) {        
    Serial.print("Trasmissione OK - Comando inviato al server    : ");
    Serial.println(command);
  } else {
    Serial.println("*** Errore di trasmissione ***");
  }
  radio.startListening();

  Serial.println("---------------------------------------------------------------");  

  delay(LOOP_DELAY);  
}
