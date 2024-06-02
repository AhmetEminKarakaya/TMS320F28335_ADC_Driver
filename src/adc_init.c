#include "adc_init.h"

/*
 *     @brief: ADC_Init initializes the ADC according to the configurations entered by the user
 *
 *     @param: *Adc_InitTypedef : Struct that holds the data entered by the user
 */


void ADC_Init(ADC_InitTypedef_t *Adc_InitTypedef)
{

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    ADC_Clock_Chain(Adc_InitTypedef->Sysclkout_Selection);     // SysCtrlRegs.HISPCP.all = G_HISPCP;
    ADC_cal();
    EDIS;

    AdcRegs.ADCTRL3.all = 0x00E0; // Power up
    DSP28x_usDelay(5);           // DELAY_US(5);


    AdcRegs.ADCTRL1.bit.SEQ_CASC = Adc_InitTypedef->Mode;
    AdcRegs.ADCTRL3.bit.SMODE_SEL = Adc_InitTypedef->Sampling_Mode;
    AdcRegs.ADCTRL1.bit.CONT_RUN = Adc_InitTypedef->Auto_Conv;
    AdcRegs.ADCREFSEL.bit.REF_SEL = Adc_InitTypedef->Reference_Voltage_Selection;


    if(Adc_InitTypedef->Mode == DUAL_SEQ_MODE)
    {
        AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = Adc_InitTypedef->Number_of_SEQ1;
        AdcRegs.ADCMAXCONV.bit.MAX_CONV2 = Adc_InitTypedef->Number_of_SEQ2;
    }

    else
    {
       AdcRegs.ADCMAXCONV.all = (Adc_InitTypedef->Number_of_SEQ1 + Adc_InitTypedef->Number_of_SEQ2);
    }


    if(Adc_InitTypedef->Sampling_Mode == SEQUENTIAL)
    {
        AdcRegs.ADCCHSELSEQ1.bit.CONV00 = Adc_InitTypedef->Channel_Input[0];
        AdcRegs.ADCCHSELSEQ1.bit.CONV01 = Adc_InitTypedef->Channel_Input[1];
        AdcRegs.ADCCHSELSEQ1.bit.CONV02 = Adc_InitTypedef->Channel_Input[2];
        AdcRegs.ADCCHSELSEQ1.bit.CONV03 = Adc_InitTypedef->Channel_Input[3];
        AdcRegs.ADCCHSELSEQ2.bit.CONV04 = Adc_InitTypedef->Channel_Input[4];
        AdcRegs.ADCCHSELSEQ2.bit.CONV05 = Adc_InitTypedef->Channel_Input[5];
        AdcRegs.ADCCHSELSEQ2.bit.CONV06 = Adc_InitTypedef->Channel_Input[6];
        AdcRegs.ADCCHSELSEQ2.bit.CONV07 = Adc_InitTypedef->Channel_Input[7];
        AdcRegs.ADCCHSELSEQ3.bit.CONV08 = Adc_InitTypedef->Channel_Input[8];
        AdcRegs.ADCCHSELSEQ3.bit.CONV09 = Adc_InitTypedef->Channel_Input[9];
        AdcRegs.ADCCHSELSEQ3.bit.CONV10 = Adc_InitTypedef->Channel_Input[10];
        AdcRegs.ADCCHSELSEQ3.bit.CONV11 = Adc_InitTypedef->Channel_Input[11];
        AdcRegs.ADCCHSELSEQ4.bit.CONV12 = Adc_InitTypedef->Channel_Input[12];
        AdcRegs.ADCCHSELSEQ4.bit.CONV13 = Adc_InitTypedef->Channel_Input[13];
        AdcRegs.ADCCHSELSEQ4.bit.CONV14 = Adc_InitTypedef->Channel_Input[14];
        AdcRegs.ADCCHSELSEQ4.bit.CONV15 = Adc_InitTypedef->Channel_Input[15];
    }

    else if(Adc_InitTypedef->Sampling_Mode == SIMULTANEOUS)
    {
        AdcRegs.ADCCHSELSEQ1.bit.CONV00 = Adc_InitTypedef->Channel_Input[0];
        AdcRegs.ADCCHSELSEQ1.bit.CONV01 = Adc_InitTypedef->Channel_Input[1];
        AdcRegs.ADCCHSELSEQ1.bit.CONV02 = Adc_InitTypedef->Channel_Input[2];
        AdcRegs.ADCCHSELSEQ1.bit.CONV03 = Adc_InitTypedef->Channel_Input[3];
        AdcRegs.ADCCHSELSEQ2.bit.CONV04 = Adc_InitTypedef->Channel_Input[4];
        AdcRegs.ADCCHSELSEQ2.bit.CONV05 = Adc_InitTypedef->Channel_Input[5];
        AdcRegs.ADCCHSELSEQ2.bit.CONV06 = Adc_InitTypedef->Channel_Input[6];
        AdcRegs.ADCCHSELSEQ2.bit.CONV07 = Adc_InitTypedef->Channel_Input[7];
    }
}

/*
 *     @brief: Read_ADC is the function where ADC values are read.
 *
 *     @param: *rxbuf : Buffer where data will be written
 *     @param: *Adc_InitTypedef : Struct that holds the data entered by the user
 */


void Read_ADC(ADC_InitTypedef_t *Adc_InitTypedef,Uint16 *rxbuf)
{

    if(Adc_InitTypedef->Mode == DUAL_SEQ_MODE && Adc_InitTypedef->Sampling_Mode == SEQUENTIAL)
    {

        AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 0x1;

        while(AdcRegs.ADCST.bit.SEQ2_BSY)
        {
            AdcRegs.ADCTRL2.bit.RST_SEQ2 = 0x1;

            rxbuf[8] = AdcRegs.ADCRESULT8 >> 4;
            rxbuf[9] = AdcRegs.ADCRESULT9 >> 4;
            rxbuf[10] = AdcRegs.ADCRESULT10 >> 4;
            rxbuf[11] = AdcRegs.ADCRESULT11 >> 4;
            rxbuf[12] = AdcRegs.ADCRESULT12 >> 4;
            rxbuf[13] = AdcRegs.ADCRESULT13 >> 4;
            rxbuf[14] = AdcRegs.ADCRESULT14 >> 4;
            rxbuf[15] = AdcRegs.ADCRESULT15 >> 4;
        }

        AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 0x1;

        while(AdcRegs.ADCST.bit.SEQ1_BSY)
        {
            AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0x1;

            rxbuf[0] = AdcRegs.ADCRESULT0 >> 4;
            rxbuf[1] = AdcRegs.ADCRESULT1 >> 4;
            rxbuf[2] = AdcRegs.ADCRESULT2 >> 4;
            rxbuf[3] = AdcRegs.ADCRESULT3 >> 4;
            rxbuf[4] = AdcRegs.ADCRESULT4 >> 4;
            rxbuf[5] = AdcRegs.ADCRESULT5 >> 4;
            rxbuf[6] = AdcRegs.ADCRESULT6 >> 4;
            rxbuf[7] = AdcRegs.ADCRESULT7 >> 4;
        }
    }

    else
    {
        AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 0x1;

        rxbuf[0] = AdcRegs.ADCRESULT0 >> 4;
        rxbuf[1] = AdcRegs.ADCRESULT1 >> 4;
        rxbuf[2] = AdcRegs.ADCRESULT2 >> 4;
        rxbuf[3] = AdcRegs.ADCRESULT3 >> 4;
        rxbuf[4] = AdcRegs.ADCRESULT4 >> 4;
        rxbuf[5] = AdcRegs.ADCRESULT5 >> 4;
        rxbuf[6] = AdcRegs.ADCRESULT6 >> 4;
        rxbuf[7] = AdcRegs.ADCRESULT7 >> 4;
        rxbuf[8] = AdcRegs.ADCRESULT8 >> 4;
        rxbuf[9] = AdcRegs.ADCRESULT9 >> 4;
        rxbuf[10] = AdcRegs.ADCRESULT10 >> 4;
        rxbuf[11] = AdcRegs.ADCRESULT11 >> 4;
        rxbuf[12] = AdcRegs.ADCRESULT12 >> 4;
        rxbuf[13] = AdcRegs.ADCRESULT13 >> 4;
        rxbuf[14] = AdcRegs.ADCRESULT14 >> 4;
        rxbuf[15] = AdcRegs.ADCRESULT15 >> 4;
    }
}

/*
 *     @brief: ADC_Clock_Chain function sets ADC Clocking and Sample rate Calculations
 *
 *     @param: Adc_Sysclkout : The SYSCLKOUT value indicates how much it will be and is entered by the user. @def group ADC_SYSCLKOUT_Config_t
 */

void ADC_Clock_Chain(uint8 Adc_Sysclkout)
{

    switch(Adc_Sysclkout)
    {

    case SYSCLK_150MHz:
        SysCtrlRegs.HISPCP.all = 0x3;
        AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x00;
        AdcRegs.ADCTRL1.bit.CPS = 0x00;
        AdcRegs.ADCTRL1.bit.ACQ_PS = 0x00;
        break;

    case SYSCLK_100MHz:
        SysCtrlRegs.HISPCP.all = 0x2;
        AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x02;
        AdcRegs.ADCTRL1.bit.CPS = 0x01;
        AdcRegs.ADCTRL1.bit.ACQ_PS = 0x15;
        break;

    default:
        //SysCtrlRegs.HISPCP.all = G_HISPCP;
        AdcRegs.ADCTRL3.bit.ADCCLKPS = 0xF;
        AdcRegs.ADCTRL1.bit.ACQ_PS = 0x1;
    }

}

/*
 *     @brief: Adjusts the power mode setting
 *
 *     @param: Adc_Power_Mode : @def group ADC_Power_Modes_Config_t
 */

void Power_Mode_Config(uint8 Adc_Power_Mode)
{
    if(Adc_Power_Mode == ADC_OFF)
    {
        AdcRegs.ADCTRL3.bit.ADCPWDN = 0x00;
        AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x00;
    }
    else if(Adc_Power_Mode == ADC_POWER_DOWN)
    {
        AdcRegs.ADCTRL3.bit.ADCPWDN = 0x00;
        AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x03;
    }
    else
    {
        AdcRegs.ADCTRL3.bit.ADCPWDN = 0x01;
        AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x03;
    }
}

/*
 *     @brief: Reset_ADC_Module Resets the ADC module
 *
 */

void Reset_ADC_Module()
{
    AdcRegs.ADCTRL1.bit.RESET = 0x1;
}


/*
 *     @brief: ADC_Start_IT function activates the interrupt bits and activates the interrupt
 *
 *     @param: *Adc_InitTypedef : Struct that holds the data entered by the user
 */


void ADC_Start_IT(ADC_InitTypedef_t *Adc_InitTypedef)
{
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;
    AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0x1;

    if(Adc_InitTypedef->Mode == DUAL_SEQ_MODE && Adc_InitTypedef->Sampling_Mode == SEQUENTIAL)
    {
        AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 0x1;
        AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 = 0x1;
    }
}

/*
 *     @brief: ADC_Clear_IT_Flag function clears interrupt flags
 *
 *     @param: SEQ : SEQ1 or SEQ2
 */


void ADC_Clear_IT_Flag(Sequencer_Selection_t SEQ)
{

    if(SEQ == SEQ1)
        AdcRegs.ADCST.bit.INT_SEQ1_CLR = 0x1;

    else if(SEQ == SEQ2)
        AdcRegs.ADCST.bit.INT_SEQ2_CLR = 0x1;

    else
    {
        AdcRegs.ADCST.bit.INT_SEQ1_CLR = 0x1;
        AdcRegs.ADCST.bit.INT_SEQ2_CLR = 0x1;
    }
}
