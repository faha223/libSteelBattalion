#ifndef STEELBATTALION
#define STEELBATTALION

#include <libusb-1.0/libusb.h>
#include <pthread.h>
#include "Buttons.h"

namespace SteelBattalion
{
	void *pollThreaded(void *ptr);

	class SteelBattalionController
	{
		friend void* pollThreaded(void *ptr); // the pollThreaded function needs to be able to call poll() and see pollingInterval, but these dont need to be accessible to the user
		unsigned char *rawControlData;						// A pointer to the raw control data of the device. Accessible to the user through secure methods only
		unsigned char *rawLEDData;						// A pointer to the raw led data of the device. Accessible to the user through secure methods only
		libusb_device_handle *device;						// A pointer to a libusb_device_handle to hold the pointer to the device. Needed for all polling activities
		libusb_context *ctx;							// A pointer to a libusb_context that must be established before we can do anything on usb
		ButtonMasks masks;							// A set of masks for determining the values of the buttons
		int pollingInterval;							// The interval (in milliseconds) of time between polling
		int error;								// An int to hold the current error code (usually from libusb calls)
		bool shutdown;								// A boolean that the destructor uses to tell the polling method to stop
		bool refreshLED;							// A boolean the setLEDState method uses to tell the polling thread to update the LEDs
		void init();								// A function to initialize the device
		void poll();								// A method that polls the device continuously until we stop using the device
		pthread_t loop;								// A pthread to handle polling for input and writing to the buffer

	public:
		SteelBattalionController();						// Constructor

		void setPollingInterval(const int& i);					// set the polling interval to i (units in milliseconds)
		int getPollingInterval() const;						// Get the polling interval in milliseconds

		unsigned char *getRawLEDData() const;					// Get the raw data for the LEDs
		void setRawLEDData(unsigned char* data);				// Set the raw data for the LEDs (a quick access method for doing precalculated animations. Takes an array of 34. Any less will
											//	cause a segmentation fault, any more will be ignored
		unsigned char *getRawControlData() const;				// Get the raw control data

		int getLeftPedal() const;						// get the state of the left foot pedal (range 0 to 255)
		int getRightPedal() const;						// get the state of the right foot pedal (range 0 to 255)
		int getMiddlePedal() const;						// get the state of the middle foot pedal (range 0 to 255)

		int getRotationLever() const;						// get the state of the rotation lever (left joystick, range -127 to 127 )
		int getAimingX() const;							// get the state of the aiming stick X axis ( right joystick, range -127 to 127 )
		int getAimingY() const;							// get the state of the aiming stick Y axis ( right joystick, range -127 to 127 )
		int getSightChangeX() const;						// get the state of the sight change stick X axis ( thumbstick, range -127 to 127 )
		int getSightChangeY() const;						// get the state of the sight change stick Y axis ( thumbstick, range -127 to 127 )

		int getTunerDial() const;						// get the state of the tuner dial (range 0 to 15)
		int getGearLever() const;						// get the state of the gear lever (range 0 to 6)

		void setLEDState(int light, int intensity, bool refresh);		// Set the LED state for the given LED at a given intensity. light is a ControllerLEDEnum, intensity range 0 to 15.
		int getLEDState(ControllerLEDEnum light) const;				// Get the state of the given LED (brightness, range 0 to 15)
		void refreshLEDs();							// Push the current buffer of LED values to the device
		void resetLEDs();							// Reset the brightness of all the lights on the device to 0

		int getButtonState(ButtonEnum button) const;				// Get the state of the given button from the controller

		int getError() const;							// Get the error value from this device
		~SteelBattalionController();						// Destructor
	};
}
#endif
