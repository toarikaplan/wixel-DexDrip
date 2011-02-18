#include <cc2511_map.h>
#include <wixel.h>
#include <usb_com.h>
#include <stdio.h>

int32 CODE param_blink_period = 500;

int32 CODE param_baud_rate = 115200;

// A feature we could easily add later:
__at(1) paramo_blink_period_min;
__at(5000) paramo_blink_period_max;


void updateLeds()
{
	static uint32 lastToggle = 0;

	usbShowStatusWithGreenLed();

    LED_YELLOW(0);

    if ((uint16)(timeMs - lastToggle) >= param_blink_period)
    {
    	LED_RED(!LED_RED_STATE);
    	lastToggle = timeMs;
    }
}

void receiveCommands()
{
	uint8 XDATA response[64];

	if (usbComRxAvailable() && usbComTxAvailable() >= 64)
	{
		uint8 byte;
		byte = usbComRxReceiveByte();
		if (byte == (uint8)'?')
		{
			uint8 length = sprintf(response, "? blink period = %d, baud = %d\r\n", (uint16)param_blink_period, (uint16)param_baud_rate);
			usbComTxSend(response, length);
		}
	}
}

void main()
{
	wixelInit();
	usbInit();

	while(1)
	{
		wixelService();
		updateLeds();
		usbComService();
		receiveCommands();
	}
}
