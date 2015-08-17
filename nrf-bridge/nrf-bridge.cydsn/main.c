extern "C"
{
	#include <project.h>
	int main();
	volatile uint32 time;
}
#include <stdio.h>
#include <stdarg.h>
#include "RF24.h"

RF24 nrf(0,0);
uint8 rx_address[]={0x63,0x52,0xa3};

static void SysTick_isr()
{
	time++;
}

int main()
{	
    CyIntSetSysVector(SysTick_IRQn+16,SysTick_isr);
	SysTick_Config(BCLK__BUS_CLK__HZ/1000); //1ms SysTick
	
	CyGlobalIntEnable;
	USB_Start(0,USB_DWR_VDDD_OPERATION);
	while(!USB_GetConfiguration());
	
	CyDelay(100);
	nrf.begin();
	nrf.setAddressWidth(3);
	nrf.setRetries(1,15);
	nrf.setChannel(2);
	nrf.enableDynamicPayloads();
	nrf.setDataRate(RF24_2MBPS);
	nrf.setCRCLength(RF24_CRC_16);
	nrf.openReadingPipe(1,rx_address);
	
	nrf.startListening();
	for(;;) if(nrf.available())
	{
		uint8 buffer[32];
		uint8 length=nrf.getDynamicPayloadSize();
		if(length<1) continue;
		nrf.read(buffer,length);
		
		while(!USB_CDCIsReady());
		USB_PutData(buffer,length);
	}
}
