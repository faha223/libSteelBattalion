#include "SteelBattalion.h"
#include <cstdio>
#include <unistd.h>
#include <chrono>
#include <cstring>
/*
	VID:			0x0A7B
	PID:			0xD000

	Endpoint in:		0x82
	Endpoint out:		0x01
*/

#define ENDPOINT_OUT 0x01
#define ENDPOINT_IN 0x82

void SteelBattalion::SteelBattalionController::updateLoop(SteelBattalionController *sbc)
{
	sbc->poll();
}

SteelBattalion::SteelBattalionController::SteelBattalionController()
{
	ctx = nullptr;
	device = nullptr;
	updateThread = nullptr;
	pollingInterval = 0;
	masks.Init();
	rawLEDData = new unsigned char[34];
	for(size_t i = 0; i < 34; ++i)
		rawLEDData[i] = 0;
	rawControlData = new unsigned char[26];
	for(size_t i = 0; i < 26; ++i)
		rawControlData[i] = 0;
	init();
}

unsigned char*SteelBattalion::SteelBattalionController::getRawLEDData()
{
	std::unique_lock<std::mutex> lock(mtx);
	uint8_t *out = new uint8_t[34];
	memcpy(out, rawLEDData, 34);
	return out;
}

void SteelBattalion::SteelBattalionController::setRawLEDData(unsigned char *data)
{
	std::unique_lock<std::mutex> lock(mtx);
	memcpy(rawLEDData, data, 34);
}

unsigned char*SteelBattalion::SteelBattalionController::getRawControlData()
{
	std::unique_lock<std::mutex> lock(mtx);
	uint8_t *out = new uint8_t[26];
	memcpy(out, rawControlData, 26);
	return out;
}

void SteelBattalion::SteelBattalionController::init()
{
	refreshLED.store(0, std::memory_order_release);
	shutdown.store(0, std::memory_order_release);
	std::unique_lock<std::mutex> lock(mtx);
	if(!(error = libusb_init(&ctx)))
	{
		//If there was no error, continue
		device = libusb_open_device_with_vid_pid(ctx, 0x0A7B, 0xD000);
		if(device != NULL)
		{
			if(libusb_kernel_driver_active(device, 0))
				if((error = libusb_detach_kernel_driver(device, 0)))
					printf("Error detaching kernel driver: %d\n", error);
			if(!libusb_claim_interface(device, 0))
			{
				libusb_release_interface(device, 0);
				updateThread = new std::thread(updateLoop, this);
			}
			else
			{
				printf("Unable to claim device\n");
				error = 1;
			}
		}
		else
		{
			printf("Device Not Found\n");
			error = 1;
		}
	}
	else
		printf("Error initializing libusb: %d\n", error);
}

void SteelBattalion::SteelBattalionController::poll()
{
	uint8_t *temp;
	uint8_t *buffer = new unsigned char[26];
	int32_t done = 0;												// make an integer in which to store the amount of data we were able to transfer (bytes)
	while(((!shutdown.load(std::memory_order_acquire)) || refreshLED.load(std::memory_order_acquire)))
	{
		if(libusb_kernel_driver_active(device, 0))							// Check to see if the kernel is using the device
			if((error = libusb_detach_kernel_driver(device, 0)))					// if it is, detach the kernel from the device (it wasn't really using it anyway)
				printf("Unable to detach kernel driver: %d\n", error);				// If the kernel gives you trouble, give up.
		if((error = libusb_claim_interface(device, 0)))							// Assuming you got that sorted out, claim the interface
			printf("Error claiming interface: %d\n", error);					// If you don't succeed, then we're done here
		else
		{	
			{											// Assuming you were able to get the device claimed, we carry on
				std::unique_lock<std::mutex> lock(mtx);
				if((error = libusb_bulk_transfer(device, ENDPOINT_IN, buffer, 26, &done, 0)))	// bulk transfer the bytes over from the device into memory
					printf("Read Error: %d\n", error); 						// if it gives you trouble, then we've still managed to fail

				if(refreshLED.load(std::memory_order_acquire))
				{
					if((error = libusb_bulk_transfer(device, ENDPOINT_OUT, rawLEDData, 34, &done, 0)))	// Begin the bulk transfer of bytes from the device to the buffer in memory.
						printf("Write Error: %d\n", error);					// if it gives you trouble, then we've still managed to fail

					refreshLED.store(0, std::memory_order_release);
				}

				libusb_release_interface(device, 0);							// release the interface when we're done with it
				temp = rawControlData;									//
				rawControlData = buffer;								// Swap the buffers
				buffer = temp;										//
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000*pollingInterval));								// sleep for the number of milliseconds specified until the next update time
		}
	}
	resetLEDs();
	if(libusb_kernel_driver_active(device, 0))                                                      // Check to see if the kernel is using the device
		if((error = libusb_detach_kernel_driver(device, 0)))                                    // if it is, detach the kernel from the device (it wasn't really using it anyway)
			printf("Unable to detach kernel driver: %d\n", error);                          // If the kernel gives you trouble, give up.
	if((error = libusb_claim_interface(device, 0)))                                                 // Assuming you got that sorted out, claim the interface
		printf("Error claiming interface: %d\n", error);                                        // If you don't succeed, then we're done here
	else
	{                                                                                               // Assuming you were able to get the device claimed, we carry on
		if((error = libusb_bulk_transfer(device, ENDPOINT_OUT, rawLEDData, 34, &done, 0)))      // Begin the bulk transfer of bytes from the device to the buffer in memory.
			printf("Write Error: %d\n", error);						// if it gives you trouble, then we've still managed to fail
		refreshLED.store(0, std::memory_order_release);
	}
	libusb_release_interface(device, 0);

	delete [] buffer;
}

void SteelBattalion::SteelBattalionController::setPollingInterval(const uint32_t &i)						// This method changes the polling interval. It's not used yet
{
	std::unique_lock<std::mutex> lock(mtx);
	pollingInterval = i;
}

uint32_t SteelBattalion::SteelBattalionController::getPollingInterval()						// This method returns the polling interval. This works, but it isn't very accurate because it doesn't poll much
{
	std::unique_lock<std::mutex> lock(mtx);
	return pollingInterval;
}

void SteelBattalion::SteelBattalionController::setLEDState(uint32_t light, uint32_t intensity, bool refresh)
{
	// Write the value (intensity % 16) to rawLEDData[light] and if(refresh) update the LEDs
	int hexPos = light % 2;										// Figure out which half of the given byte applies to this LED
	int bytePos = light / 2;									// and which byte this LED corresponds to

	intensity %= 16;										// Clamp the intensity to a range of 0 to 15

	// Erase the byte position, and set the light intensity
	std::unique_lock<std::mutex> lock(mtx);
	rawLEDData[bytePos] &= (unsigned char)((hexPos == 1)?0x0F:0xF0);				// Erase the data only in the correct half of the byte
	rawLEDData[bytePos] += (unsigned char)(intensity * ((hexPos == 1)?0x10:0x01));			// and Assign it a new value equal to the intensity
	refreshLED.store((refresh)?1:0, std::memory_order_release);
}

uint32_t SteelBattalion::SteelBattalionController::getLEDState(ControllerLEDEnum light)				// returns the LED brightness value from the rawLEDData byte array
{
	int hexPos = ((int) light) % 2;
	int bytePos = (((int) light) - hexPos) / 2;
	std::unique_lock<std::mutex> lock(mtx);
	return (((uint32_t) rawLEDData[bytePos]) & ((hexPos == 1)?0xF0:0x0F)) / ((hexPos == 1)?0x10:0x01);
}

uint32_t SteelBattalion::SteelBattalionController::getButtonState(ButtonEnum button) 					// returns the button state from the rawControlData byte array. It must use the button mask list to find where in the
{													//	array to look for the info (the byte in the array, and the mask for the value in the mask)
	std::unique_lock<std::mutex> lock(mtx);
	return rawControlData[masks.MaskList[button]->bytePos] & masks.MaskList[button]->maskValue;
}

void SteelBattalion::SteelBattalionController::refreshLEDs()								// This method sets the flag to write the LED data to the device when the poll method gets called again
{													// Multithreading makes everything better
	refreshLED.store(1, std::memory_order_release);
}

void SteelBattalion::SteelBattalionController::resetLEDs()								// Resets the LED values to 0
{
	for(size_t i = 0; i < 34; ++i)									// Sets every value in the LED array to 0
		rawLEDData[i] = 0;									//
	refreshLED.store(1, std::memory_order_release);
}

uint32_t SteelBattalion::SteelBattalionController::getAimingX()							// This method returns the integer value of the x axis of the aiming stick (the right joystick on the device)
{
	std::unique_lock<std::mutex> lock(mtx);
	return uint32_t(rawControlData[9])-127;									// Stored in byte 9 of the buffer
}

uint32_t SteelBattalion::SteelBattalionController::getAimingY()							// This method returns the integer value of the y axis of the aiming stick (the right joystick on the device)
{
	std::unique_lock<std::mutex> lock(mtx);
	return uint32_t(rawControlData[11])-127;									// Stored in byte 11 of the buffer
}

uint32_t SteelBattalion::SteelBattalionController::getRotationLever()							// This method returns the integer value of the rotation lever (the left joystick on the device)
{
	std::unique_lock<std::mutex> lock(mtx);
	return ((uint32_t(rawControlData[13] < 128))?uint32_t(rawControlData[13]):uint32_t(rawControlData[13])-255);// Stored in byte 13 of the buffer
}

uint32_t SteelBattalion::SteelBattalionController::getSightChangeX()							// This method returns the integer value of the x axis of the sight change stick (the thumbstick on the device)
{
	std::unique_lock<std::mutex> lock(mtx);
	return ((uint32_t(rawControlData[15] < 128))?uint32_t(rawControlData[15]):uint32_t(rawControlData[15])-255);// Stored in byte 15 of the buffer
}

uint32_t SteelBattalion::SteelBattalionController::getSightChangeY()							// This method returns the integer value of the y axis of the sight change stick (the thumbstick on the device)
{
	std::unique_lock<std::mutex> lock(mtx);
	return ((uint32_t(rawControlData[17] < 128))?uint32_t(rawControlData[17]):uint32_t(rawControlData[17])-255);	// Stored in byte 17 of the buffer
}

uint32_t SteelBattalion::SteelBattalionController::getLeftPedal()							// This method returns the integer value of the Left pedal
{
	std::unique_lock<std::mutex> lock(mtx);
	return uint32_t(rawControlData[19]);									// Stored in byte buffer 19
}

uint32_t SteelBattalion::SteelBattalionController::getMiddlePedal()							// This method returns the integer value of the Middle pedal
{
	std::unique_lock<std::mutex> lock(mtx);
	return uint32_t(rawControlData[21]);									// Stored in byte buffer 21
}

uint32_t SteelBattalion::SteelBattalionController::getRightPedal()							// This method returns the integer value of the Right pedal
{
	std::unique_lock<std::mutex> lock(mtx);
	return uint32_t(rawControlData[23]);									// Stored in byte buffer 23
}

uint32_t SteelBattalion::SteelBattalionController::getTunerDial()							// This method returns the integer value of the tuner dial
{
	std::unique_lock<std::mutex> lock(mtx);
	return uint32_t(rawControlData[24]) & 0x0F;								// Stored in byte buffer 24
}

uint32_t SteelBattalion::SteelBattalionController::getGearLever()						// this method returns the integer value of the gear lever
{
	std::unique_lock<std::mutex> lock(mtx);
	switch(uint32_t(rawControlData[25]))									// Stored in byte buffer 25
	{
		case 254:
			return 0;
		case 255:
			return 1;
		case 1:
			return 2;
		case 2:
			return 3;
		case 3:
			return 4;
		case 4:
			return 5;
		default:
			return 6;
	}
}

uint32_t SteelBattalion::SteelBattalionController::getError()
{
	std::unique_lock<std::mutex> lock(mtx);
	return error;
}

SteelBattalion::SteelBattalionController::~SteelBattalionController()							// Destructor
{
	if(device != NULL)										// If the device was successfully initialized, now would be a good time to close it
	{
		if(updateThread)
		{
			shutdown.store(1, std::memory_order_release);									// Signal the thread to stop
			updateThread->join();
		}
//		libusb_reset_device(device);
		libusb_close(device);									// close the device
	}
	if(ctx != NULL)											// If the context was successfully initialized, now would be a good time to close it
	{
		libusb_exit(ctx);
	}
	delete [] rawLEDData;										// delete any variables we dont need anymore
	delete [] rawControlData;									//
	masks.deInit();											//
}

void SteelBattalion::ButtonMasks::Init()
{
	MaskList[RIGHT_JOY_MAIN_WEAPON]        	= new ButtonMask( 2, 0x01);
	MaskList[RIGHT_JOY_FIRE]              	= new ButtonMask( 2, 0x02);
	MaskList[RIGHT_JOY_LOCK_ON]            	= new ButtonMask( 2, 0x04);
	MaskList[EJECT]                     	= new ButtonMask( 2, 0x08);
	MaskList[COCKPIT_HATCH]              	= new ButtonMask( 2, 0x10);
	MaskList[IGNITION]                  	= new ButtonMask( 2, 0x20);
	MaskList[START]                     	= new ButtonMask( 2, 0x40);
	MaskList[MULTIMON_OPEN_CLOSE]         	= new ButtonMask( 2, 0x80);
	MaskList[MULTIMON_MAP_ZOOM_IN_OUT]      = new ButtonMask( 3, 0x01);
	MaskList[MULTIMON_MODE_SELECT]        	= new ButtonMask( 3, 0x02);
	MaskList[MULTIMON_SUB_MONITOR]        	= new ButtonMask( 3, 0x04);
	MaskList[MAINMON_ZOOM_IN]             	= new ButtonMask( 3, 0x08);
	MaskList[MAINMON_ZOOM_OUT]            	= new ButtonMask( 3, 0x10);
	MaskList[FSS]               			= new ButtonMask( 3, 0x20);
	MaskList[MANIP]       					= new ButtonMask( 3, 0x40);
	MaskList[LINE_COLOR_CHANGE]   			= new ButtonMask( 3, 0x80);
	MaskList[WASHING]                  		= new ButtonMask( 4, 0x01);
	MaskList[EXTINGUISHER]              	= new ButtonMask( 4, 0x02);
	MaskList[CHAFF]                    		= new ButtonMask( 4, 0x04);
	MaskList[TANK_DETACH]        			= new ButtonMask( 4, 0x08);
	MaskList[OVERRIDE]          			= new ButtonMask( 4, 0x10);
	MaskList[NIGHT_SCOPE]        			= new ButtonMask( 4, 0x20);
	MaskList[F1]                			= new ButtonMask( 4, 0x40);
	MaskList[F2]                			= new ButtonMask( 4, 0x80);
	MaskList[F3]                			= new ButtonMask( 5, 0x01);
	MaskList[WEAPON_CON_MAIN]             	= new ButtonMask( 5, 0x02);
	MaskList[WEAPON_CON_SUB]              	= new ButtonMask( 5, 0x04);
	MaskList[WEAPON_CON_MAG]        		= new ButtonMask( 5, 0x08);
	MaskList[COM1]                     		= new ButtonMask( 5, 0x10);
	MaskList[COM2]                     		= new ButtonMask( 5, 0x20);
	MaskList[COM3]                     		= new ButtonMask( 5, 0x40);
	MaskList[COM4]                     		= new ButtonMask( 5, 0x80);
	MaskList[COM5]                     		= new ButtonMask( 6, 0x01);
	MaskList[LEFT_JOY_SIGHT_CHANGE]        	= new ButtonMask( 6, 0x02);
	MaskList[TOGGLE_FILTER_CONTROL]       	= new ButtonMask( 6, 0x04);
	MaskList[TOGGLE_OXYGEN_SUPPLY]        	= new ButtonMask( 6, 0x08);
	MaskList[TOGGLE_FUEL_FLOW_RATE]        	= new ButtonMask( 6, 0x10);
	MaskList[TOGGLE_BUFFER_MAT]      		= new ButtonMask( 6, 0x20);
	MaskList[TOGGLE_VT_LOCATION]          	= new ButtonMask( 6, 0x40);
	MaskList[TUNER_DIAL_STATE_CHANGE]      	= new ButtonMask(24, 0x0F);
	MaskList[GEAR_LEVER_STATE_CHANGE]      	= new ButtonMask(25, 0xFF);
}

SteelBattalion::ButtonMask::ButtonMask(int bytePos, int maskValue)
{
	this->bytePos = bytePos;
	this->maskValue = maskValue;
}

void SteelBattalion::ButtonMasks::deInit()
{
	for(size_t i = 0; i < 41; ++i)
	{
		delete MaskList[i];
		MaskList[i] = NULL;
	}
}

SteelBattalion::ControllerLEDEnum SteelBattalion::ButtonToLED(ButtonEnum button)
{
	SteelBattalion::ControllerLEDEnum light;
	switch(button)
	{
		case COM1:
		{
			light = Comm1;
			break;
		}
		case COM2:
		{
			light = Comm2;
			break;
		}
		case COM3:
		{
			light = Comm3;
			break;
		}
		case COM4:
		{
			light = Comm4;
			break;
		}
		case COM5:
		{
			light = Comm5;
			break;
		}
		case F1:
		{
			light = F1LED;
			break;
		}
		case TANK_DETACH:
		{
			light = TankDetach;
			break;
		}
		case FSS:
		{
			light = ForecastShootingSystem;
			break;
		}
		case F2:
		{
			light = F2LED;
			break;
		}
		case OVERRIDE:
		{
			light = Override;
			break;
		}
		case MANIP:
		{
			light = Manipulator;
			break;
		}
		case F3:
		{
			light = F3LED;
			break;
		}
		case NIGHT_SCOPE:
		{
			light = NightScope;
			break;
		}
		case LINE_COLOR_CHANGE:
		{
			light = LineColorChange;
			break;
		}
		case WASHING:
		{
			light = Washing;
			break;
		}
		case EXTINGUISHER:
		{
			light = Extinguisher;
			break;
		}
		case CHAFF:
		{
			light = Chaff;
			break;
		}
		case WEAPON_CON_MAIN:
		{
			light = MainWeaponControl;
			break;
		}
		case WEAPON_CON_SUB:
		{
			light = SubWeaponControl;
			break;
		}
		case WEAPON_CON_MAG:
		{
			light = MagazineChange;
			break;
		}
		case MULTIMON_OPEN_CLOSE:
		{
			light = OpenClose;
			break;
		}
		case MULTIMON_MAP_ZOOM_IN_OUT:
		{
			light = MapZoomInOut;
			break;
		}
		case MULTIMON_MODE_SELECT:
		{
			light = ModeSelect;
			break;
		}
		case MULTIMON_SUB_MONITOR:
		{
			light = SubMonitorModeSelect;
			break;
		}
		case MAINMON_ZOOM_IN:
		{
			light = MainMonitorZoomIn;
			break;
		}
		case MAINMON_ZOOM_OUT:
		{
			light = MainMonitorZoomOut;
			break;
		}
		case COCKPIT_HATCH:
		{
			light = CockpitHatch;
			break;
		}
		case IGNITION:
		{
			light = Ignition;
			break;
		}
		case START:
		{
			light = Start;
			break;
		}
		default:
			light = EmergencyEject;
	}
	return light;
}

