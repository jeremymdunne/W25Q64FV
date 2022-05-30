/**
 * @file W25Q64FV.cpp
 * @author Jeremy Dunne 
 * @brief source file for the W25Q64 Flash Chip interface library 
 * @version 0.1
 * @date 2022-05-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <W25Q64FV.hpp>

W25Q64FV_status_t W25Q64FV::begin(int cs_pin){
    // initialize the cs pin 
    _cs = cs_pin; 
    pinMode(_cs,OUTPUT); 
    digitalWrite(_cs,HIGH); // deselect 
    // initialize SPI 
    // Serial.println("Setting SPI Settings!");
    SPI.begin(); 
    // Serial.println("Good!"); 
    _spi_settings = SPISettings(W25Q64FV_SPI_SPEED, MSBFIRST, SPI_MODE_0); 
    // // check the device id 
    // Serial.println("Checking Dev ID"); 
    uint8_t buffer[5]; 
    read_reg(W25Q64FV_INSTRUCTION_MANUFACTURER_DEVICE_ID, buffer, 5); 
    // for now, check that they're not all 0s 
    if(buffer[4] == 0){
        return W25Q64FV_COMMUNICATION_FAIL; 
    }
    // reset the device 
    return reset(); 
}

W25Q64FV_status_t W25Q64FV::enable_writing(){
    // enable writing on the device 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // write the enable command 
    W25Q64FV_status_t status = write_command(W25Q64FV_INSTRUCTION_WRITE_ENABLE); 
    if(status != W25Q64FV_OK) return status; 
    _write_enabled = true; 
    return W25Q64FV_OK; 
}

W25Q64FV_status_t W25Q64FV::disable_writing(){
    // disable writing to the device 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // write the enable command 
    W25Q64FV_status_t status = write_command(W25Q64FV_INSTRUCTION_WRITE_DISABLE); 
    if(status != W25Q64FV_OK) return status; 
    _write_enabled = false; 
    return W25Q64FV_OK; 
}

W25Q64FV_status_t W25Q64FV::write_page(uint32_t start_address, byte *buffer){
    // write a single page to the flash chip 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // check that writing is enables 
    if(!_write_enabled) enable_writing(); 
    // write the page 
    select_device(); 
    SPI.transfer(W25Q64FV_INSTRUCTION_PAGE_PROGRAM); 
    SPI.transfer(start_address>>16);
    SPI.transfer(start_address>>8); 
    SPI.transfer(start_address);
    for(int i = 0; i < 256; i ++){
        SPI.transfer(*buffer); 
        *buffer ++; 
    }
    release_device();
    return W25Q64FV_OK; 
}

W25Q64FV_status_t W25Q64FV::read_page(uint32_t start_address, byte *buffer){
    // read a single page from the flash chip 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // read the page 
    select_device(); 
    SPI.transfer(W25Q64FV_INSTRUCTION_READ_DATA); 
    SPI.transfer(start_address>>16);
    SPI.transfer(start_address>>8); 
    SPI.transfer(start_address);
    for(int i = 0; i < 256; i ++){
        *buffer = SPI.transfer(0x00); 
        *buffer ++; 
    }
    release_device();
    return W25Q64FV_OK; 
}

W25Q64FV_status_t W25Q64FV::erase_chip(bool hold){
    // erase the entire chip 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // check that writing is enables 
    if(!_write_enabled) enable_writing(); 
    // write the command to erase 
    W25Q64FV_status_t status = write_command(W25Q64FV_INSTRUCTION_CHIP_ERASE); 
    if(status != W25Q64FV_OK) return status; 
    // check for the hold 
    if(hold) return wait_until_free(W25Q64FV_CHIP_ERASE_TIMEOUT); 
    return W25Q64FV_OK; 
}

W25Q64FV_status_t W25Q64FV::erase_sector(uint32_t sector_address, bool hold){
    // erase the entire chip 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // check that writing is enables 
    if(!_write_enabled) enable_writing(); 
    // write the command to erase 
    byte buffer[3]; 
    buffer[0] = sector_address >> 16; 
    buffer[1] = sector_address >> 8;
    buffer[2] = sector_address;  
    W25Q64FV_status_t status = write_reg(W25Q64FV_INSTRUCTION_SECTOR_4K_ERASE, buffer, 3); 
    if(status != W25Q64FV_OK) return status; 
    //check for a hold 
    if(hold) return wait_until_free(); 
    return W25Q64FV_OK; 
}

W25Q64FV_status_t W25Q64FV::erase_block_32(uint32_t sector_address, bool hold){
    // erase the entire chip 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // check that writing is enables 
    if(!_write_enabled) enable_writing(); 
    // write the command to erase 
    byte buffer[3]; 
    buffer[0] = sector_address >> 16; 
    buffer[1] = sector_address >> 8;
    buffer[2] = sector_address;  
    W25Q64FV_status_t status = write_reg(W25Q64FV_INSTRUCTION_BLOCK_32K_ERASE, buffer, 3); 
    if(status != W25Q64FV_OK) return status; 
    //check for a hold 
    if(hold) return wait_until_free(); 
    return W25Q64FV_OK; 
}

W25Q64FV_status_t W25Q64FV::erase_block_64(uint32_t sector_address, bool hold){
    // erase the entire chip 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // check that writing is enables 
    if(!_write_enabled) enable_writing(); 
    // write the command to erase 
    byte buffer[3]; 
    buffer[0] = sector_address >> 16; 
    buffer[1] = sector_address >> 8;
    buffer[2] = sector_address;  
    W25Q64FV_status_t status = write_reg(W25Q64FV_INSTRUCTION_BLOCK_64K_ERASE, buffer, 3); 
    if(status != W25Q64FV_OK) return status; 
    //check for a hold 
    if(hold) return wait_until_free(); 
    return W25Q64FV_OK; 
}


W25Q64FV_status_t W25Q64FV::get_jedec(byte *manufacture_id, byte *memory_type, byte *capacity){
    // read the jedec id and information 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // get the jedec info 
    byte buffer[3]; 
    W25Q64FV_status_t status = read_reg(W25Q64FV_INSTRUCTION_JEDEC_ID, buffer, 3); 
    if(status != W25Q64FV_OK) return status; 
    *manufacture_id = buffer[0]; 
    *memory_type = buffer[1]; 
    *capacity = buffer[2];
    return W25Q64FV_OK; 
}


bool W25Q64FV::busy(){
    uint8_t busy; 
    select_device(); 
    SPI.transfer(W25Q64FV_INSTRUCTION_READ_STATUS_REGISTER_1); 
    busy = SPI.transfer(0); 
    // Serial.println("Busy: "); 
    // Serial.print(busy);
    release_device(); 
    if(busy & 0b00000001) return true; 
    return false; 
}

W25Q64FV_status_t W25Q64FV::wait_until_free(unsigned long max_timeout){
    // get the current time 
    unsigned long start_time = millis(); 
    while(busy() & ((millis() - start_time) < max_timeout)){
        delay(1); 
    }
    if(busy()) return W25Q64FV_TIMEOUT; 
    return W25Q64FV_OK; 
}
W25Q64FV_status_t W25Q64FV::reset(){
    // reset the device 
    // requires writing the reset enable followed by reset command to complete 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // write the reset enable command 
    W25Q64FV_status_t status = write_command(W25Q64FV_INSTRUCTION_ENABLE_RESET); 
    if(status != W25Q64FV_OK) return status; 
    status = write_command(W25Q64FV_INSTRUCTION_RESET); 
    // delay 
    delayMicroseconds(35); // typical reset time 30 us 
    return W25Q64FV_OK; 
}

W25Q64FV_status_t W25Q64FV::power_down(){
    // power down the device 
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // write the power down command 
    W25Q64FV_status_t status = write_command(W25Q64FV_INSTRUCTION_POWER_DOWN); 
    if(status != W25Q64FV_OK) return status; 
    delayMicroseconds(10); 
    return W25Q64FV_OK;
}

W25Q64FV_status_t W25Q64FV::release_power_down(){
    // release the device from the power down state 
    // cannot use the status register here 
    select_device(); 
    SPI.transfer(W25Q64FV_INSTRUCTION_RELEASE_POWERDOWN);
    release_device(); 
    delayMicroseconds(10); // delay for the device to turn on 
    return W25Q64FV_OK; 
}


W25Q64FV_status_t W25Q64FV::write_reg(uint8_t reg, uint8_t *buffer, unsigned int length){
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // select 
    select_device(); 
    // write instruction 
    SPI.transfer(reg); 
    // write accompyning data 
    while(length > 0){
        SPI.transfer(*buffer); 
        length --; 
        *buffer ++; 
    }
    release_device(); 
    return W25Q64FV_OK; 
}


W25Q64FV_status_t W25Q64FV::write_command(uint8_t command){
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // write the command 
    select_device(); 
    SPI.transfer(command);
    release_device(); 
    return W25Q64FV_OK;
}


W25Q64FV_status_t W25Q64FV::read_reg(uint8_t reg, uint8_t *buffer, unsigned int length){
    // check if busy 
    if(busy()) return W25Q64FV_BUSY; 
    // select 
    select_device(); 
    SPI.transfer(reg); 
    while(length > 0){
        *buffer = SPI.transfer(0); 
        length --; 
        *buffer ++; 
    }
    release_device(); 
    return W25Q64FV_OK; 
}

