/* ********************************************************************
FILE                   : main.c

PROGRAM DESCRIPTION    : single phase motor indicated as lamp and R supply(ie as single phase supply).
                          VAR_POWER_POT is adjusted to vary current across VAR_POWER_POT to simulate R supply current.
						  When current across VAR_POWER_POT is within range of >= MIN_R_RMS_CURRENT and <= MAX_R_RMS_CURRENT,
						  then trigger TRIAC gate ON to start or continue run motor and MOTOR_ON LED is used as an indicator.
						  When current across VAR_POWER_POT is either < MIN_R_RMS_CURRENT, indicated by 
						  R_CURRENT_MIN LED or > MAX_R_RMS_CURRENT, indicated by R_CURRENT_MAX LED, then stop the motor by trigger TRIAC gate OFF,
						  as R supply current is abnormal, which will damage the motor, others machines and circuits connected to R supply. 
	 
AUTHOR                :  K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS            :    

NOTE                  :  AC current measurement by using shunt resistor and Current transformer. We use shunt resistor for AC current measurement and 
                         used AC voltage measurement by using differential amplifier.  Peak R voltage is greater than 5V max DC volt that PIC16F887 support
                         and min DC volt that PIC16F887 support is 0V. Other method for ac voltage measurement by using potential transformer.
                       										
                                    
CHANGE LOGS           : 

*****************************************************************************/
// PIC16F887 Configuration Bit Settings
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
//#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include "main.h"
#include "port.h"
#include "io_conf.h"
#include "appl_conf.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"

 //__CONFIG(0x2ce2);
value_types to_disp;
/*------------------------------------------------------------*-
FUNCTION NAME  : main

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 
-*------------------------------------------------------------*/

void main()
{
    unsigned long r_at_ch_instant_current_encoded_val, r_at_ch_peak_current_int, r_at_ch_peak_current_frac, r_at_ch_peak_current_encoded_value = 0, temp_int, \
	   temp_frac, i, temp, calc_r_source_rms_current_in_milli_amp;
	char fault_occured_flag = STATE_NO_IN_CHAR;
	unsigned int cur_disp_block = 0;
	
	R_RMS_MAX_LIMIT_CURRENT_LED_TRIS = 0;
	R_RMS_MIN_LIMIT_CURRENT_LED_TRIS = 0;
	MOTOR_CTRL_TRIS = 0;
	
	ANSEL = 0x02;
    ANSELH = 0x00;
	UART_Init();
  //  LCD_Init();
	
	//MOTOR_CTRL_PIN = STATE_ON;	   
    while(1)
	{ 
      //if(fault_occured_flag == STATE_NO_IN_CHAR)
	  {
		  r_at_ch_peak_current_encoded_value = 0;
	      for(i = 0; i < 400ul; ++i )
	      {
	    	ADC_Conf_Channel(R_RMS_CURRENT_CH);		
	        ADC_Start_Conv(&adc_cur_parameters[R_RMS_CURRENT_CH]);
		
		    r_at_ch_instant_current_encoded_val = adc_cur_parameters[adc_cur_channel].adc_value_channel;
	        if(r_at_ch_instant_current_encoded_val > r_at_ch_peak_current_encoded_value )
	        {
		       r_at_ch_peak_current_encoded_value = r_at_ch_instant_current_encoded_val; 
	        }	    
	    }
		Encoded_To_Actual_Analog_Val_Calc(r_at_ch_peak_current_encoded_value, FULL_SCALE_ANALOG_VAL_CH1, MIN_ANALOG_VALUE_CH1, &r_at_ch_peak_current_int, &r_at_ch_peak_current_frac );
		
		#ifdef TRACE
		   to_disp.unsigned_val.value_long = r_at_ch_peak_current_encoded_value;
		   UART_Transmit_Num(DISP_UNSIGN_NUM_DIGIT4, to_disp );
		   UART_Transmit_Str("  ");
		
		   to_disp.signed_val.val_in_bytes.value_byte[0] = r_at_ch_peak_current_int;
		   UART_Transmit_Num(DISP_SIGN_NUM_DIGIT3, to_disp );
		   UART_Transmit_Str("  ");
		
		   to_disp.signed_val.val_in_bytes.value_byte[0] = r_at_ch_peak_current_frac ;
	       UART_Transmit_Num(DISP_SIGN_NUM_DIGIT3, to_disp );
		   UART_Transmit_Str("  ");
		#endif
		
        temp_int = r_at_ch_peak_current_int * 2; // (10k + 10k) / 10k =   voltage divider
		temp_frac = (r_at_ch_peak_current_frac /10) * 2;
        temp = temp_int +  (temp_frac/100);		
		if(temp >= 1)
		{
	      temp  -= 1;  // 0.7V + 0.7V = voltage drop across 2 forward diodes
		  
		}
		/*  1/35 = gain of differential amplifier as Vout = (V2 - V1)*(R3/R1), if R1 = R2 and R3 = R4 and in sinosidual waveform, rms = peak/ sqrt(2)*/
		temp_int *= (35 * 707/1000);  
		
		temp_frac *= (35 * 707/1000); 
		// 1000/50 = 20 as 50 = shunt resistor and amp to milli amp conv 1000, rms_current in milli amp = rms_volt/shunt resistor
		calc_r_source_rms_current_in_milli_amp = (temp_int + (temp_frac/100)) * 20;  
		
		#ifdef TRACE
		     to_disp.signed_val.val_in_bytes.value_byte[0] = calc_r_source_rms_current_in_milli_amp;
	    	UART_Transmit_Num(DISP_SIGN_NUM_DIGIT5, to_disp );
	    	UART_Transmit_Char(ENTER_CHAR);	
		#endif
		
	   /*	//actual r phase source current < MIN_R_RMS_CURRENT in milli Amp (2790)  
		    //calc_r_source_rms_current_in_milli_amp approx 2660 for MIN_R_RMS_CURRENT in milli Amp(2790) found by analysis, make sure SHUNT RESISTOR = 50 ie 100% 
		    // and calc_r_source_rms_current_in_milli_amp for known actual r phase source current */
		if(calc_r_source_rms_current_in_milli_amp < 2660)
		{
			//disp block = 1 : begin
			R_RMS_MIN_LIMIT_CURRENT_LED_PIN = LED_ON;
			MOTOR_CTRL_PIN = STATE_OFF;
			fault_occured_flag = STATE_YES_IN_CHAR;
			if(cur_disp_block != 1)
			{
			     UART_Transmit_Str("ERR: R RMS CURRENT < MIN LIMIT \r");
                 cur_disp_block = 1;				 
			}
            //disp block = 1 : end			
		}
		else 
		{ 
		    /*// actual r phase source volt > MAX_R_RMS_VOLTAGE in milli Amp(4180)
            // calc_r_source_rms_current_in_milli_amp approx 4020 for MAX_R_RMS_VOLTAGE in milli Amp (4180) found by analysis, make sure SHUNT RESISTOR = 50 ie 100% 
			//for calc_r_source_rms_current_in_milli_amp for known actual r phase source current */	
		    if(calc_r_source_rms_current_in_milli_amp > 4020)
		    {
				//disp block = 2 : begin
				
		    	R_RMS_MAX_LIMIT_CURRENT_LED_PIN = LED_ON;
				MOTOR_CTRL_PIN = STATE_OFF;
				fault_occured_flag = STATE_YES_IN_CHAR;
				if(cur_disp_block != 2)
			    {
				      UART_Transmit_Str("ERR: R RMS CURRENT > MAX LIMIT \r"); 
                      cur_disp_block = 2;  					  
			    }
                //disp block = 2 : end				
		    }
			else
			{
				//disp block = 3 : begin
				R_RMS_MIN_LIMIT_CURRENT_LED_PIN = LED_OFF;
				R_RMS_MAX_LIMIT_CURRENT_LED_PIN = LED_OFF;
				MOTOR_CTRL_PIN = STATE_ON;
				if(cur_disp_block != 3)
				{
				   UART_Transmit_Str("R RMS CURRENT within LIMIT \r");
				   cur_disp_block = 3; 
			    }	
                //disp block = 3 : end				
			}
		} 
	  }		
    }
}
/*------------------------------------------------------------*-
FUNCTION NAME  : Write_Bit

DESCRIPTION     : write bit valve to data's bit, without affecting other bits of data .
                . 0 is the least bit pos and 7 is the most bit pos 

INPUT          : 

OUTPUT         : none

NOTE           : 
-*------------------------------------------------------------*/
void Write_Bit_in_Data(unsigned int *data, const unsigned int bit_pos, const unsigned int set_bit_val )
{
       if (set_bit_val == 1)
       {
          Set_Bit_in_Data(data, bit_pos);
          return;
       }
       Clear_Bit_in_Data(data, bit_pos ); 
}
/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/	 
