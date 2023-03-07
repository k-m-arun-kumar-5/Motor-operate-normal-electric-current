/**************************************************************************
   FILE          :    main.h
 
   PURPOSE       :    project header.  
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   PROJECT header- groups the key information about the PIC microcontroller you have used, along with other key
                     parameters – such as the oscillator frequency and commonly used information such as common data types in the project
	
  CHANGE LOGS     :
	   
 **************************************************************************/
 
 /* to avoid this header file to be included more than once, conditional directive is used  */
#ifndef _MAIN_H
#define _MAIN_H
/* In most case, microcontroller header is also a device header. device header will include the addresses of the special function registers (SFRs) 
 used for port access, plus similar details for other on-chip components such as analog-to-digital converters */
 /* preprocessor will search in microcontroller header in predetermined directory path to locate the device header file. */
// Must include the appropriate microcontroller header file here
//#define HI_TECH_COMPILER
#ifdef HI_TECH_COMPILER
  #include <pic.h>
  __CONFIG(0x2CE4);
#else // XC8 compiler
  #include <xc.h>
#endif


#define LED_OFF                                  (0)
#define LED_ON                                   (1) 
 
#define KEY_PRESSED                              (1) 
#define KEY_NOT_PRESSED                          (0)

#define STATE_YES_IN_CHAR                       ('y')
#define STATE_NO_IN_CHAR                        ('n')
#define STATE_INVALID_IN_CHAR                   ('X')
#define STATE_VALID_IN_CHAR                     ('V')

#define STATE_YES_IN_INT                          (0U) 
#define STATE_NO_IN_INT                           (1U)
#define STATE_UNKNOWN_IN_INT                      (2U)
#define STATE_INVALID_IN_INT                      (3U)
#define STATE_VALID_IN_INT                        (4U)

#define STATE_OFF                                (0)
#define STATE_ON                                 (1)

#define SUCCESS                                 (0)
#define FAILURE                                 (1)
 
#define DATA_STATE_UNKNOWN                      (0U)
#define DATA_STATE_INVALID                      (1U)
#define DATA_STATE_VALID                        (2U)


 /* Bit Operation macros */
     /* Set bit pos  in byte data   */
#define Set_Bit_in_Data(data , bit_pos)                         ((*data) |=   (1<<(bit_pos)))     
      /* Clear bit pos in byte data */ 
#define Clear_Bit_in_Data(data ,bit_pos)                         ((*data) &= (~(1<<(bit_pos))))      
    /* flip bit pos in byte data  */ 
#define Toggle_Bit_in_Data(data , bit_pos)                       ((*data) ^=   (1<<(bit_pos)))        
    /* Test if bit pos in byte data  is set   */
#define Test_Bit_Is_Set_in_Data(data ,bit_pos)                    ((*data) & (1<<bit_pos))       
   /* Test if bit pos in byte data is clear */  
#define Test_Bit_Is_Clear_in_Data(data ,bit_pos)                  (!((*data) & (1<<bit_pos)))     
#define BACKSPACE_CHAR                        ('\b')
#define ENTER_CHAR                            ('\r')
#define NULL_CHAR                             ('\0')   
#define ZERO_CHAR                              ('0')
#define NINE_CHAR                              ('9')

// Number of oscillations required to execute per instruction or increment a timer 
// 4 – Original PIC 16f887
#define OSC_PER_INST                            (4)

#define TIME_UNIT_1_SEC_IN_MILLI_SEC           (1000UL)

#define CHARS_RCV_TILL_TERMINATOR_CHAR_MODE        (0U)



 typedef union 
{
	union
	{
		unsigned long int value_long; 
	    struct 
	    {
		    unsigned int value_byte[2];
	    } val_in_bytes;
	    struct 
	    {
	      	unsigned int value_bit_0: 1;
	    	unsigned int value_bit_1: 1;
		    unsigned int value_bit_2: 1;
		    unsigned int value_bit_3: 1;
		    unsigned int value_bit_4: 1;
		    unsigned int value_bit_5: 1;
		    unsigned int value_bit_6: 1;
		    unsigned int value_bit_7: 1;
		    unsigned int value_bit_8: 1;
		    unsigned int value_bit_9: 1;
		    unsigned int value_bit_10: 1;
		    unsigned int value_bit_11: 1;
		    unsigned int value_bit_12: 1;
		    unsigned int value_bit_13: 1;
		    unsigned int value_bit_14: 1;
		    unsigned int value_bit_15: 1;
	   } val_in_bits;
	} unsigned_val;
  	union 
	{
		long int value_long; 
	    struct 
	    {
		     int value_byte[2];
	    } val_in_bytes;
	    struct 
	    {
	      	 unsigned  int value_bit_0: 1;
	    	 unsigned  int value_bit_1: 1;
		     unsigned  int value_bit_2: 1;
		     unsigned  int value_bit_3: 1;
		     unsigned  int value_bit_4: 1;
		     unsigned  int value_bit_5: 1;
		     unsigned  int value_bit_6: 1;
		     unsigned  int value_bit_7: 1;
		     unsigned  int value_bit_8: 1;
		     unsigned  int value_bit_9: 1;
		     unsigned  int value_bit_10: 1;
		     unsigned  int value_bit_11: 1;
		     unsigned  int value_bit_12: 1;
		     unsigned  int value_bit_13: 1;
		     unsigned  int value_bit_14: 1;
		     unsigned  int value_bit_15: 1;
	   } val_in_bits;
		
	} signed_val;
	
} value_types;

typedef enum 
{  
   DISP_UNSIGN_NUM_DIGIT1 = 0x01, DISP_UNSIGN_NUM_DIGIT2, DISP_UNSIGN_NUM_DIGIT3, DISP_UNSIGN_NUM_DIGIT4, DISP_UNSIGN_NUM_DIGIT5,\
   DISP_SIGN_NUM_DIGIT1 = 0x11, DISP_SIGN_NUM_DIGIT2, DISP_SIGN_NUM_DIGIT3, DISP_SIGN_NUM_DIGIT4, DISP_SIGN_NUM_DIGIT5,\
   DISP_HEX_DIGIT1 = 0x21, DISP_HEX_DIGIT2, DISP_HEX_DIGIT3, DISP_HEX_DIGIT4 
} disp_num_types;

typedef unsigned char tByte;
typedef unsigned int tWord;
typedef unsigned long tLong;


typedef unsigned char tByte;
typedef unsigned int tWord;
typedef unsigned long tLong;

typedef enum {
  INPUT_DEV_ID_INVALID, INPUT_DEV_ID_KEYBOARD, INPUT_DEV_ID_UART  	
} input_dev_id_types; 

typedef enum {
	OUTPUT_DEV_ID_INVALID, OUTPUT_DEV_ID_LCD, OUTPUT_DEV_ID_UART
} output_dev_id_types;

 typedef enum {
	DATA_IN_INVALID_FORMAT, DATA_IN_STR_FORMAT, DATA_IN_BINARY_FORMAT, DATA_IN_HEXA_FORMAT, DATA_IN_DECIMAL_FORMAT, DATA_IN_CHAR_FORMAT, DATA_IN_APPL_DEFINED_FORMAT
} data_format_types;


extern value_types to_disp;



#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
