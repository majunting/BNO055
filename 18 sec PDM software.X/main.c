/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15.1
        Device            :  PIC18F45K80
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "mcc_generated_files/mcc.h"

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    // edit 4 bits in CIOCON register (CAN IO control)
    CIOCONbits.CLKSEL = 1;  // CAN clk select
    CIOCONbits.ENDRHI = 1;  // enable drive high (CANTX drives VDD when recessive)
    CIOCONbits.TX2SRC = 0;  // CANTX2 pin data source (output CANTX)
    CIOCONbits.TX2EN = 1;   // CANTX pin enable (output CANRX or CAN clk based on TX2SRC)
    
    double VDD = 5000.0;    // input voltage 5V
    double x = VDD / 4096.0;
    
    uint8_t ADC_result_H, ADC_result_L;
    adc_result_t ADCResult;
    uint8_t linear_accel_x_LSB, linear_accel_x_MSB, linear_accel_y_LSB,
            linear_accel_y_MSB, linear_accel_z_LSB, linear_accel_z_MSB;
    uint8_t message;
    I2C_MESSAGE_STATUS status;

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowEnable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    while (1)
    {
        /** ADC */
        ADCResult = ADC_GetConversion(BT_FL) * x;
        ADC_result_H = ADCResult >> 8;
        ADC_result_L = ADCResult;
        uCAN_MSG test;
        test.frame.idType=dSTANDARD_CAN_MSG_ID_2_0B;
        test.frame.id=0x634;
        test.frame.dlc=8;
        test.frame.data0=ADC_result_H;
        test.frame.data1=ADC_result_L;
        test.frame.data2=ADC_result_H;
        test.frame.data3=ADC_result_L;
        test.frame.data4=ADC_result_H;
        test.frame.data5=ADC_result_L;
        test.frame.data6=ADC_result_H;
        test.frame.data7=ADC_result_L;
        
        CAN_transmit(&test);
        
        /** G sensor */
        //set mode to nine degree of freedom (fusion mode)
        message = OPERATION_MODE_NDOF;
        status = I2C_MESSAGE_PENDING;
        I2C_MasterWrite ( &message, 1, BNO055_OPR_MODE_ADDR, &status );
        while ( status != I2C_MESSAGE_COMPLETE );
        //read linear acceleration data for 3 axis
        status = I2C_MESSAGE_PENDING;
        I2C_MasterRead ( &linear_accel_x_LSB, 1, BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR, &status );
        while ( status != I2C_MESSAGE_COMPLETE );
        status = I2C_MESSAGE_PENDING;
        I2C_MasterRead ( &linear_accel_x_MSB, 1, BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR, &status );
        while ( status != I2C_MESSAGE_COMPLETE );
        status = I2C_MESSAGE_PENDING;
        I2C_MasterRead ( &linear_accel_y_LSB, 1, BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR, &status );
        while ( status != I2C_MESSAGE_COMPLETE );
        status = I2C_MESSAGE_PENDING;
        I2C_MasterRead ( &linear_accel_y_MSB, 1, BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR, &status );
        while ( status != I2C_MESSAGE_COMPLETE );
        status = I2C_MESSAGE_PENDING;
        I2C_MasterRead ( &linear_accel_z_LSB, 1, BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR, &status );
        while ( status != I2C_MESSAGE_COMPLETE );
        status = I2C_MESSAGE_PENDING;
        I2C_MasterRead ( &linear_accel_z_MSB, 1, BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR, &status );
        while ( status != I2C_MESSAGE_COMPLETE );
        
        uCAN_MSG G_sensor_data;
        
        G_sensor_data.frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
        G_sensor_data.frame.id = 0x635;
        G_sensor_data.frame.dlc = 6;
        G_sensor_data.frame.data0 = linear_accel_x_LSB;
        G_sensor_data.frame.data1 = linear_accel_x_MSB;
        G_sensor_data.frame.data2 = linear_accel_y_LSB;
        G_sensor_data.frame.data3 = linear_accel_y_MSB;
        G_sensor_data.frame.data4 = linear_accel_z_LSB;
        G_sensor_data.frame.data5 = linear_accel_z_MSB;
        
        CAN_transmit ( &G_sensor_data );
//        uint16_t result = ADC_GetConversion(pitot) * x;
//        ADC_result_H = result >> 8;
//        ADC_result_L = result;
//        uint16_t result = ADC_GetConversion(analog2) * x;
//        ADC_result_H = result >> 8;
//        ADC_result_L = result;
//        uint16_t result = ADC_GetConversion(analog2) * x;
//        ADC_result_H = result >> 8;
//        ADC_result_L = result;
//        test.frame.idType=dSTANDARD_CAN_MSG_ID_2_0B;
//        test.frame.id=0x175;
//        test.frame.dlc=8;
//        test.frame.data0=ADC_result_H;
//        test.frame.data1=ADC_result_L;
//        test.frame.data2=CCP_result_H;
//        test.frame.data3=CCP_result_L;
//        test.frame.data4=ADC_result_H;
//        test.frame.data5=ADC_result_L;
//        test.frame.data6=ADC_result_H;
//        test.frame.data7=ADC_result_L;
//        
//        CAN_transmit(&test);
        // Add your application code
        
    }
}
/**
 End of File
*/