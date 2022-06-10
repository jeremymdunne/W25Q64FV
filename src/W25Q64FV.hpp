/**
 * @file W25Q64FV.hpp
 * @author Jeremy Dunne
 * @brief header file for the W25Q64 Flash Chip interface library 
 * @version 0.1
 * @date 2022-05-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _W25Q64FV_HPP_
#define _W25Q64FV_HPP_

/********** INSTRUCTION SETS **********/
#define W25Q64FV_INSTRUCTION_WRITE_ENABLE                   0x06 
#define W25Q64FV_INSTRUCTION_VOLATILE_SR_WRITE_ENABLE       0x50
#define W25Q64FV_INSTRUCTION_WRITE_DISABLE                  0x04 
#define W25Q64FV_INSTRUCTION_READ_STATUS_REGISTER_1         0x05 
#define W25Q64FV_INSTRUCTION_READ_STATUS_REGISTER_2         0x35 
#define W25Q64FV_INSTRUCTION_WRITE_STATUS_REGISTER          0x01
#define W25Q64FV_INSTRUCTION_PAGE_PROGRAM                   0x02 
#define W25Q64FV_INSTRUCTION_SECTOR_4K_ERASE                0x20 
#define W25Q64FV_INSTRUCTION_BLOCK_32K_ERASE                0x52
#define W25Q64FV_INSTRUCTION_BLOCK_64K_ERASE                0xD8
#define W25Q64FV_INSTRUCTION_CHIP_ERASE                     0xC7 // might also be 0x60
#define W25Q64FV_INSTRUCTION_ERASE_PROGRAM_SUSPEND          0x75
#define W25Q64FV_INSTRUCTION_ERASE_PROGRAM_RESUME           0x7A
#define W25Q64FV_INSTRUCTION_POWER_DOWN                     0xB9
#define W25Q64FV_INSTRUCTION_READ_DATA                      0x03 
#define W25Q64FV_INSTRUCTION_FAST_READ                      0x0B
#define W25Q64FV_INSTRUCTION_RELEASE_POWERDOWN              0xAB
#define W25Q64FV_INSTRUCTION_MANUFACTURER_DEVICE_ID         0x90 
#define W25Q64FV_INSTRUCTION_JEDEC_ID                       0x9F
#define W25Q64FV_INSTRUCTION_READ_UNIQUE_ID                 0x4B 
#define W25Q64FV_INSTRUCTION_READ_SFDP_REGISTER             0x5A 
#define W25Q64FV_INSTRUCTION_ERASE_SECURITY_REGISTERS       0x44
#define W25Q64FV_INSTRUCTION_PROGRAM_SECURITY_REGISTERS     0x42 
#define W25Q64FV_INSTRUCTION_READ_SECURITY_REGISTERS        0x48
#define W25Q64FV_INSTRUCTION_ENABLE_QPI                     0x38
#define W25Q64FV_INSTRUCTION_ENABLE_RESET                   0x66 
#define W25Q64FV_INSTRUCTION_RESET                          0x99
// DUAL IO TABLE 
#define W25Q64FV_INSTRUCTION_FAST_READ_DUAL_OUTPUT          0x3B 
#define W25Q64FV_INSTRUCTION_FAST_READ_DUAL_IO              0xBB
#define W25Q64FV_INSTRUCTION_MANUFACTURER_DEVICE_ID_DUAL_IO 0x92 
// QUAD IO TABLE 
#define W25Q64FV_INSTRUCTION_QUAD_PAGE_PROGRAM              0x32 
#define W25Q64FV_INSTRUCTION_FAST_READ_QUAD_OUTPUT          0x6B 
#define W25Q64FV_INSTRUCTION_FAST_READ_QUAD_IO              0xEB
#define W25Q64FV_INSTRUCTION_WORD_READ_QUADO_IO             0xE7
#define W25Q64FV_INSTRUCTION_OCTAL_WORD_READ_QUAD_IO        0xE3 
#define W25Q64FV_INSTRUCTION_SET_BURST_WRAP                 0x77
#define W25Q64FV_INSTRUCTION_MANUFACTURER_DEVICE_ID_QUAD_IO 0x94     

/********** SETTINGS **********/ 
#define W25Q64FV_SPI_SPEED          20000000 // 104 MHz (104 max)

#define W25Q64FV_DEFAULT_TIMEOUT    5000 // Default timeout for most operations 
#define W25Q64FV_CHIP_ERASE_TIMEOUT 100000 // Chip erase timeout. Per spec, this is typically 20 seconds, at most 100 seconds. 



/// Default Status Return Enum
typedef enum{ 
    W25Q64FV_OK = 0, ///<Chip OK 
    W25Q64FV_COMMUNICATION_FAIL, ///<Communication fail 
    W25Q64FV_BUSY, ///<Chip report busy 
    W25Q64FV_TIMEOUT, ///<Chip timeout on wait operation 
    W25Q64FV_NOT_VALID ///<Not a valid operation
} W25Q64FV_status_t; 


#include <Arduino.h>
#include <SPI.h> 

/**
 * @brief Interface Class for W25Q64FV Flash Chip 
 * 
 */
class W25Q64FV{
public: 
    /**
     * @brief Initialize communication with the flash chip
     * 
     * Begins SPI communication and attempts to check the ID of the device.
     *  
     * @param cs_pin                Chip select pin  
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t begin(int cs_pin); 

    /**
     * @brief Enable writing to the flash chip 
     * 
     * Sets the enable register to allow writing to the flash chip 
     * 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t enable_writing(); 

    /**
     * @brief Disable writing to the device 
     * 
     * Sets the enable register to dissallow writing to the flash chip
     * 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t disable_writing(); 

    /**
     * @brief Write a page to the flash chip 
     * 
     * Writes a single page (256 bytes) to the flash chip 
     * 
     * @param start_address         Start address to write to. Must be the start of a page 
     * @param buffer                Buffer of data to write
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t write_page(uint32_t start_address, byte *buffer); 

    /**
     * @brief Read a page from the flash chip 
     * 
     * Reads a single page (256 bytes) from the flash chip 
     * 
     * @param start_address         Start address to read from
     * @param buffer                Buffer of data to read into
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t read_page(uint32_t start_address, byte *buffer); 
    
    /**
     * @brief Erase a 4kB sector from the flash chip 
     * 
     * Erases a single sector (4kB) from the device. Address is truncated. 
     * 
     * @param sector_address        Sector start address to erase 
     * @param hold                  Hold for the device to finish the erase 
     * @return W25Q64FV_status_t    Status return (does not report busy)
     */
    W25Q64FV_status_t erase_sector(uint32_t sector_address, bool hold = true); 

    /**
     * @brief Erase a 32kB block from the flash chip 
     * 
     * Erases a single block of 32kB from the device. Address is truncated 
     * 
     * @param block_address         Block start address to erase 
     * @param hold                  Hold for the device to finish the erase 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t erase_block_32(uint32_t block_address, bool hold = true); 

    /**
     * @brief Erase a 64kB block from the flash chip 
     * 
     * Erases a single block of 64kB from the device. Address is truncated 
     * 
     * @param block_address         Block start address to erase 
     * @param hold                  Hold for the device to finish the erase 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t erase_block_64(uint32_t block_address, bool hold = true); 

    /**
     * @brief Erase entire flash chip contents 
     * 
     * Erases the entire contents of the flash chip. 
     * 
     * @param hold                  Hold for the device to finish the erase 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t erase_chip(bool hold = true);

    /**
     * @brief Get the jedec object id 
     * 
     * Returns the Jedec ID of the device 
     * 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t get_jedec(byte *manufacture_id, byte *memory_type, byte *capacity); 

    /**
     * @brief Checks if the flash chip is busy 
     * 
     * Checks the status register and returns the state of the flash chip 
     * 
     * @return true     Device is busy (cannot execute new tasks)
     * @return false    Device is free (can execute new tasks)
     */
    bool busy(); 
    
    W25Q64FV_status_t wait_until_free(unsigned long max_timeout = W25Q64FV_DEFAULT_TIMEOUT); 

    /**
     * @brief Reset the flash chip 
     * 
     * Sends the reset enable and the reset command to reset the flash chip 
     * 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t reset(); 

    /**
     * @brief Power down the flash chip 
     * 
     * Puts the device in a low-power mode
     * 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t power_down();

    /**
     * @brief Release the device from power down 
     * 
     * Releases the device from the low-power mode. This must be called if the device 
     * is in a low-power state 
     * 
     * @return W25Q64FV_status_t    Status return  
     */
    W25Q64FV_status_t release_power_down();




private: 
    SPISettings _spi_settings;      ///< Flash chip specific SPI settings 
    int _cs;                        ///< Chip select pin 

    /**
     * @brief Write information to a register 
     * 
     * Writes a buffer of information to a desired register. 
     * 
     * @param reg                   Register to write to 
     * @param buffer                Buffer of data to write 
     * @param length                Length of the buffer 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t write_reg(uint8_t reg, uint8_t *buffer, unsigned int length); 

    /**
     * @brief Write a single command to the flash chip 
     * 
     * Writes a single byte to the flash chip  
     * 
     * @param command               Byte to write 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t write_command(uint8_t command); 

    /**
     * @brief Read multiple bytes from a register 
     * 
     * Reads from a register the desired number of bytes 
     * 
     * @param reg                   Register to read from 
     * @param buffer                Buffer to read data into 
     * @param length                Number of bytes to read 
     * @return W25Q64FV_status_t    Status return 
     */
    W25Q64FV_status_t read_reg(uint8_t reg, uint8_t *buffer, unsigned int length); 

    /**
     * @brief Select the device for communication
     * 
     * Selects the device by pulling the chip select low and setting the SPI transaction info
     * 
     * @return (void)
     */
    void select_device() {
        SPI.beginTransaction(_spi_settings);
        digitalWrite(_cs,LOW);
    } 

    /**
     * @brief Release the device for communication
     * 
     * De-selects the device by pulling the chip select high and ending the SPI transaction
     * 
     * @return (void)
     */
    void release_device() {
        SPI.endTransaction(); 
        digitalWrite(_cs,HIGH);
    } 

}; 



#endif 