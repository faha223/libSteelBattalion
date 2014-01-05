#ifndef STEELBATTALION
#define STEELBATTALION

#include <libusb-1.0/libusb.h>
#include <atomic>
#include <mutex>
#include <thread>
#include "Buttons.h"

namespace SteelBattalion
{
	class SteelBattalionController
	{
		uint8_t *rawControlData;						// A pointer to the raw control data of the device. Accessible to the user through secure methods only
		uint8_t *rawLEDData;						// A pointer to the raw led data of the device. Accessible to the user through secure methods only
		libusb_device_handle *device;						// A pointer to a libusb_device_handle to hold the pointer to the device. Needed for all polling activities
		libusb_context *ctx;							// A pointer to a libusb_context that must be established before we can do anything on usb
		ButtonMasks masks;							// A set of masks for determining the values of the buttons
		uint32_t pollingInterval;							// The interval (in milliseconds) of time between polling
		uint32_t error;								// An int to hold the current error code (usually from libusb calls)
		std::atomic_int shutdown;								// A boolean that the destructor uses to tell the polling method to stop
		std::atomic_int refreshLED;							// A boolean the setLEDState method uses to tell the polling thread to update the LEDs
		void init();								// A function to initialize the device
		void poll();								// A method that polls the device continuously until we stop using the device
		std::mutex mtx;
		std::thread *updateThread;								// A pthread to handle polling for input and writing to the buffer

		static void updateLoop(SteelBattalionController *sbc);

	public:
		SteelBattalionController();						// Constructor

		void setPollingInterval(const uint32_t& i);					// set the polling interval to i (units in milliseconds)
		uint32_t getPollingInterval();						// Get the polling interval in milliseconds

		uint8_t *getRawLEDData();					// Get the raw data for the LEDs
		void setRawLEDData(unsigned char* data);				// Set the raw data for the LEDs (a quick access method for doing precalculated animations. Takes an array of 34. Any less will
											//	cause a segmentation fault, any more will be ignored
		uint8_t *getRawControlData();				// Get the raw control data

		uint32_t getLeftPedal();						// get the state of the left foot pedal (range 0 to 255)
		uint32_t getRightPedal();						// get the state of the right foot pedal (range 0 to 255)
		uint32_t getMiddlePedal();						// get the state of the middle foot pedal (range 0 to 255)

		uint32_t getRotationLever();						// get the state of the rotation lever (left joystick, range -127 to 127 )
		uint32_t getAimingX();							// get the state of the aiming stick X axis ( right joystick, range -127 to 127 )
		uint32_t getAimingY();							// get the state of the aiming stick Y axis ( right joystick, range -127 to 127 )
		uint32_t getSightChangeX();						// get the state of the sight change stick X axis ( thumbstick, range -127 to 127 )
		uint32_t getSightChangeY();						// get the state of the sight change stick Y axis ( thumbstick, range -127 to 127 )

		uint32_t getTunerDial();						// get the state of the tuner dial (range 0 to 15)
		uint32_t getGearLever();						// get the state of the gear lever (range 0 to 6)

		void setLEDState(uint32_t light, uint32_t intensity, bool refresh);		// Set the LED state for the given LED at a given intensity. light is a ControllerLEDEnum, intensity range 0 to 15.
		uint32_t getLEDState(ControllerLEDEnum light);				// Get the state of the given LED (brightness, range 0 to 15)
		void refreshLEDs();							// Push the current buffer of LED values to the device
		void resetLEDs();							// Reset the brightness of all the lights on the device to 0

		uint32_t getButtonState(ButtonEnum button);				// Get the state of the given button from the controller

		uint32_t getError();							// Get the error value from this device
		~SteelBattalionController();						// Destructor
	};
}
#endif
