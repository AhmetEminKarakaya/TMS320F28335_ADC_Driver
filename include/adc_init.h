/*
 * adc_init.h
 *
 *  Created on: 14 Eyl 2023
 *      Author: ahmet
 */

#ifndef INCLUDE_ADC_INIT_H_
#define INCLUDE_ADC_INIT_H_

#include <stdint.h>
#include "DSP2833x_Adc.h"
#include "DSP2833x_Examples.h"



typedef enum
{
    DUAL_SEQ_MODE = 0x00,
    CASCADED_MODE = 0x01
}ADC_Mode_Config_t;


typedef enum
{
    SEQUENTIAL = 0x00,
    SIMULTANEOUS = 0x01

}ADC_Sampling_Mode_Config_t;


typedef enum
{
    ADCINA0 = 0x0000,
    ADCINA1,
    ADCINA2,
    ADCINA3,
    ADCINA4,
    ADCINA5,
    ADCINA6,
    ADCINA7,
    ADCINB0,
    ADCINB1,
    ADCINB2,
    ADCINB3,
    ADCINB4,
    ADCINB5,
    ADCINB6,
    ADCINB7,
    ADINALL

}ADC_Input_Channel_Selected_t;

typedef enum
{
    CONV_1 = 0x00,
    CONV_2,
    CONV_3,
    CONV_4,
    CONV_5,
    CONV_6,
    CONV_7,
    CONV_8,
    CONV_9,
    CONV_10,
    CONV_11,
    CONV_12,
    CONV_13,
    CONV_14,
    CONV_15,
    CONV_16
}ADC_Number_of_Conversions_Config_t;

typedef enum
{
    AUTO_CONV_DISABLE = 0x0,
    AUTO_CONV_ENABLE

}ADC_Auto_Conv_Config_t;


typedef enum
{
    ADC_OFF = 0x00,
    ADC_POWER_DOWN = 0x06,
    ADC_POWER_UP = 0x07

}ADC_Power_Modes_Config_t;

typedef enum
{
    INTERNAL_REF = 0x00,
    EXT_REF_2_048_V,
    EXT_REF_1_5_V,
    EXT_REF_1_024_V

}ADC_Reference_Selection_Config_t;

typedef enum
{
    SYSCLK_150MHz,
    SYSCLK_100MHz

}ADC_SYSCLKOUT_Config_t;

typedef enum
{
    Mode_0 = 0x00,
    Mode_1,
    Mode_2,
    Mode_3
}Emulation_Suspend_Mode_Config_t;

typedef enum
{
    SEQ1,
    SEQ2,
    SEQ1_and_2
}Sequencer_Selection_t;

typedef struct
{
    uint8 Mode;                            // ADC_Mode_Config_t
    uint8 Sampling_Mode;                   // ADC_Sampling_Mode_Config_t
    uint8 Channel_Input[ADINALL];          // ADC_Input_Channel_Selected_t
    uint8 Number_of_SEQ1;                  // ADC_Number_of_Conversions_Config_t
    uint8 Number_of_SEQ2;                  // ADC_Number_of_Conversions_Config_t
    uint8 Auto_Conv;                       // ADC_Auto_Conv_Config_t
    uint8 Sysclkout_Selection;             // ADC_SYSCLKOUT_Config_t
    uint8 Reference_Voltage_Selection;     // ADC_Reference_Selection_Config_t

}ADC_InitTypedef_t;


            /*** Function Prototypes ***/

void ADC_Init(ADC_InitTypedef_t *Adc_InitTypedef);
void Read_ADC(ADC_InitTypedef_t *Adc_InitTypedef,Uint16 *rxbuf);
void Power_Mode_Config(uint8 Adc_Power_Mode);
void ADC_Clock_Chain(uint8 Adc_Sysclkout);
void Reset_ADC_Module();
void ADC_Start_IT(ADC_InitTypedef_t *Adc_InitTypedef);
void ADC_Clear_IT_Flag(Sequencer_Selection_t SEQ);



                                                    /*** KULLANICININ GÝRECEÐÝ VERÝLER ***/

/*      ADC_CONFIG { {Mode}, {Sampling_Mode}, \
        { {Channel_Input}, {Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input},{Channel_Input} },\
        {Number_of_SEQ1} , {Number_of_SEQ2}, {Auto_Conv}, {Sysclkout_Selection}, {Reference_Voltage_Selection} }
*/

#define ADC_CONFIG { {DUAL_SEQ_MODE}, {SEQUENTIAL}, \
        { {ADCINB0}, {ADCINA6},{0},{0},{0},{0},{0},{0},{ADCINA0},{ADCINB6},{0},{0},{0},{0},{0},{0} },\
        {CONV_2} , {CONV_2}, {AUTO_CONV_ENABLE}, {SYSCLK_150MHz}, {INTERNAL_REF} }


#endif /* INCLUDE_ADC_INIT_H_ */
