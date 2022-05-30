#include <Arduino.h>

#include "W25Q64FV.hpp"

#define FLASH_CS PA3 
W25Q64FV flash; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  delay(3000); 
  Serial.println("Beginning!"); 
  if(flash.begin(FLASH_CS) == W25Q64FV_OK){
    Serial.println("FLASH INIT OK"); 
  } 

  byte manufacturer_id; 
  byte memory_type; 
  byte capacity; 
  flash.get_jedec(&manufacturer_id, &memory_type, &capacity); 
  Serial.print("Manufacturer ID: "); 
  Serial.print(manufacturer_id); 
  Serial.print("\nMemory Type: "); 
  Serial.print(memory_type);
  Serial.print("\nCapacity: "); 
  Serial.print(capacity); 
  Serial.print('\n'); 

  // erase a sector 
  unsigned long start = micros(); 
  flash.erase_sector(0); 
  unsigned long stop = micros(); 
  Serial.print("Erase time took: "); 
  Serial.print(stop - start); 
  Serial.print(" Micros\n"); 

  // write a single page 
  byte write_buffer[256]; 
  for(int i = 0; i < 256; i ++){ 
    write_buffer[i] = i; 
  } 

  for(int r = 0; r < 16; r ++){
    for(int c = 0; c < 16; c ++){
      int index = r*16 + c; 
      Serial.print(write_buffer[index]); 
      Serial.print('\t'); 
    }
    Serial.println(); 
  }

  flash.enable_writing(); 
  flash.write_page(0, write_buffer); 

  flash.wait_until_free();

  // read back the page 
  byte in_buffer[256]; 
  flash.read_page(0, in_buffer); 
  // print the results 
  for(int r = 0; r < 16; r ++){
    for(int c = 0; c < 16; c ++){
      int index = r*16 + c; 
      Serial.print(in_buffer[index]); 
      Serial.print('\t'); 
    }
    Serial.println(); 
  }
}



void loop() {
  
}