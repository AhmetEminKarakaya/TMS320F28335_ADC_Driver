#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "adc_init.h"

ADC_InitTypedef_t Adc_Init = ADC_CONFIG;

uint16_t rxbuf[16];


void main(void)
{

    ADC_Init(&Adc_Init);

    while(1)
    {
        Read_ADC(&Adc_Init,rxbuf);
    }


}
