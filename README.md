# BOOTLOADER USB MASS STORAGE STM32G0 

In this repository : 

  * This software is an example of bootloader code to update the target microcontroller firmware into Flash using the USB peripheral as Device in Mass Storage Class. It allows the products to be updated without the necessity of any update software installation in the computer, providing a fast way for the costumers update his products by themselves. It was built over the NUCLEO-G0B1RE that embeds an STM32G0B1RE MCU, but it can be easily tailored to any other ST Microcontroller. 
  * This project uses the ST USB Middleware to enable and manage the MCU USB, a simple FATFS library code (to manage the MCU Mass Storage Buffer to be recognized and communicate with Personal Computer Operational Systems) and the bootloader. The bootloader was built based on the “How to Create a Super Simple Bootloader” video series, so, if you need more information regarding the bootloader creation, you may check this material by the following link: 
  https://www.youtube.com/watch?v=OkUQ3iMmiYQ&list=PLnMKNibPkDnEb1sphpdFJ3bR9dNy7S6mO


### <b>How it works ?</b>

  This bootloader starts checking the User Button state (this button is available on the NUCLEO-G0B1RE board, and it’s connected to the PC13 with a pull-up resistor), if the button is pressed, the code stills in the bootloader and prepare the microcontroller to be flashed. Else, the bootloader tries to jump to the application already programmed into the memory. We will cover each application section individually in this text.
	First, regarding the bootloader, after it checks that the User Button is pressed, it starts creating and computing the FAT File System sectors and settings into MCU RAM. After that, the USB is initialized as Device Mass Storage Class and finally all the operations are done into the USB Storage Read/Write FS functions.
	This demo allocates the first five File System sectors (Boot, Reserved, 2xFATs, and Root Directory Sector) into RAM, and the file sectors are copied to the Flash Memory just if a correct .bin file is identified through the USB transfer. To do it, the Write function (buffers coming from Computer to Microcontroller) manage the incoming packages looking for a file with the specified name and .bin extension (this name can be specified into the code, the instructions for it it’s on “HOW TO USE IT?” text section), and when the file is identified, it’s copied to the Flash Memory until the end of the file. Ending the Flash programming, the device de-initialize the USB and jump to the programmed application.
	The MCU Flash Memory was divided into two parts, being 10% (50KB) for the bootloader, and the other 90% (462KB) for the application. The STM32G0B1RE flash has 512KB of space, and it starts at the 0x08000000 address. So, the bootloader memory region starts at the 0x08000000 address, and goes until the 0x0800C7FF address. Also, the application memory region starts at the 0x0800C800 address and ends at the final memory address, 0x08080000. By this reason, the binary files construction should have an offset of the start code address and the NVIC position (whereas it should be placed into the application region). These changes are covered by the “CREATING YOUR OWN BINARY FILES” section of this text, but if you need a more detailed approach check the “How to Create a Super Simple Bootloader” material.
	Now, we will cover the second case of the bootloader, when the MCU is powered with the User Button released. In this case, the bootloader will first verify whether the application memory region has a valid code, by checking it first address (it’s a simple way to verify it, but if you need a more secure method, it can be easily changed to an improved code). If there is a valid code, the bootloader jumps to the application, in other case, it will blink the User LED at 0.5 Hz frequency. (NOTE: The jump code is also explained in the “How to Create a Super Simple Bootloader” video series).


### <b>Keywords</b>

  Bootloader; USB; USBD; Device; MSC; Mass Storage Class; Firmware Update. 

### <b>Directory contents</b>

  - USB_MSC_G0B1: STM32CubeIDE project with the Bootloader
  - USB_MSC_A_G0B1: STM32CubeIDE project showing how to create an application to be programmed by this bootloader
  - EXAMPLE BINARIES: Two binaries that can be used for a quick test of this package

### <b>Hardware and Software environment</b>

  - This example runs on STM32G0B1RET6 devices.

  - This example has been tested with NUCLEO-G0B1RE board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following steps. 

 - First, download, import and open the project "USB_MSC_G0B1" in STM32CubeIDE. After that, compile and flash the code into the target MCU. 

 - NOTE 1: It’s possible to change the standard file name by the “FAT_NAME_FILE” define into the “fat_app.h” file, it should be an ASCII in little endian mode, and should have exactly four bytes. The code verifies only the first four characters of the file name, so the other characters can be anyone. But the maximum number of characters in the name file should 8 characters, so make sure that your file name doesn’t exceeds this.

 - NOTE 2: It’s also possible to change the device volume name, changing the Volume Name attribute of the FAT_BaseEntry_t “hVolume” initialization at the FAT_Init function into fat_app.c file, but make sure to fill this field with exactly 11 bytes.

 - Disconnect the MCU power, and solder/connect an USB cable as follows:
	- PA11 at D-,
	- PA12 at D+,
	- GND to any GND pin,
	- 5 Vcc to the 5V Nucleo Board (change the J2 position to E5V).

	- Now, you can connect this soldered USB cable to a computer. But, before doing it, you may select whether you want to jump to application or stay in bootloader mode, by the User Button (as explained before).  Staying in bootloader mode, the computer should recognize it as a USB Mass storage, then you can drag and drop a .bin file with the correct file name (if you didn’t change the name, the standard four file name characters are “UPDF” an acronym for Update File, and the others four can be anyone). At the end of the file copy, the microcontroller will erase the remaining flash to avoid that a remaining code from another application interfere on the new code execution, and then the bootloader will jump to the application to starts to run it.
	  
  - Into this project files there is a folder named “EXAMPLE BINARIES” where there are some binary files already compiled to be programmed into the microcontroller with this bootloader, and it can be used to quick try this software.

  - If you want to create your own binary for your application, download the "USB_MSC_A_G0B1" project that contains an example project for STM32CubeIDE with the linker script modified to generate a binary to be uploaded by this firmware.

### <b>Available bin files </b>

	Inside the EXAMPLE BINARIES folders, there are two binary files made to be flash into a MCU using this bootloader. So, after flashing the bootloader into your microcontroller, you can try it uploading one of these binaries.

	- UPDF123.bin: is a simple demo that blinks the User LED (available on the Nucleo Board, connected to the PA5) at 5Hz frequency and opens a Virtual COM Port through the USB (the same USB used to the Mass Storage Bootloader). The demo also sends an “Application Stared” message at the beginning of the application and a “Button Pressed” message each time the User Button (also on the Nucleo Board, connected to the PC13) is pushed through the VCOM.
	- UPDF321.bin: is a demo that changes the User LED state every at each User 

### <b> CREATING YOUR OWN BINARY FILES: </b>

As the application must be placed in a specific flash address, some modifications in the linker script and config files should be made. Follow the next steps to create your own binary files:

- Create a new project using STM32CubeIDE.
- Open the “STM32G0B1RETX_FLASH.ld” linker file or equivalent. Change the Flash Origin Address to 0x0800C800, and the Length to 462K.
- Now open the ../Core/Src/system_stm32g0xx.c file, uncomment the “USER_VECT_TAB_ADDRESS” define, and change the “VECT_TAB_OFFSET” define value to 0x0000C800U.
- Close both files, give a right click on the current project in the Project Explorer, then click on “Properties”. In the opened window, go to: C/C++ Build -> Settings -> Tool Settings -> MCU Post build outputs, then enable the “Convert to binary file (-O binary)” to generate the binary file.
- Code your own application, compile it and use the binary file inside ../Debug folder.

### <b> KNOWN LIMITATIONS: </b>
	- This bootloader only accepts binary files with 8 characters file name length
	- The file name should be determined before the application implementation (it can be easily changed)
	- The user must rename the file before using it in the bootloader

