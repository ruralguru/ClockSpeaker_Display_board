/**
 * @file CS_display_board.h
 * @author Earl R. Watkins II
 * @date 26 June 2019
 * @brief Header file for the Clock Speaker Display Board
 */

 #ifndef CS_display_board_h
 #define CS_display_board_h

 #include <Arduino.h>

#include <ERW_PCA9685.h>
#include <ERW_LED_Display.h>

#define logo_length 24      //!< Length of the LOGO string
#define printable_length 95 //!< Length of the printable characters string.

#define LED_driver_address    0x40  //!< I2C address in Binary for the device with all address pins grounded.
#define LED_driver1_address   0x41  //!< I2C address in Binary for the first device .
#define LED_driver2_address   0x42  //!< I2C address in Binary for the second device.
#define LED_driver3_address   0x43  //!< I2C address in Binary for the third device.
#define LED_driver4_address   0x44  //!< I2C address in Binary for the fourth device.
#define LED_driver5_address   0x45  //!< I2C address in Binary for the fifth device.
#define LED_driver_dp_address 0x46  //!< I2C address in Binary for the dp device.


#define display_state_time        0x01  //!< Comparison value for displaying time
#define display_state_timer       0x02  //!< Comparison value for displaying timer
#define display_state_text        0x04  //!< Comparison value for displaying text.
#define display_state_test_time   0x11  //!< Comparison value for running time test.
#define display_state_test_timer  0x22  //!< Comparison value for running time test.
#define display_state_test_text   0x44  //!< Comparison value for running text test.
#define display_state_test_serial 0x84  //!< Comparison value for running serial test.
#define display_state_test_gen    0xF0  //!< Comparison value for checkinf if a test.

/** @brief LED_driver_Enable is the pin that controls OE_n functionallity.
  * NOTE: If multiple devices are ran with one OE_n control the enable can be
  *       omitted from the other constructors. Using the OE_n in all devices
  *       using the same OE_n line may cause odd functionallity.
  */
#define LED_driver_Enable 10

#define LED_driver_LED_count 16       //!< This is the number of LED's driven by the display. This is used to offset the on off times so that the current draw is cleaned up some. */
#define LED_display_count 6
#define LED_driver_Max 4095           //!< This is used to set LEDs to Max Brightness in the for loop */

 class CS_display_board
 {
  public:
  //Public Functions

  /**
   * @brief CS_display_board is the constructor for the class
   * @param[in] user_max_brightness Sets the Maximum Brightness to use for the display. Less than 4095.
   * @param[in] user_driveMode      Sets the drive mode of the displays.
   * @param[in] user_pwm_frequency  Sets the PWM frequency to use.
   */
  CS_display_board(uint16_t user_max_brightness, uint8_t user_driveMode, float user_pwm_frequency, unsigned long user_time_step);

  /**
   * begin starts the displays as desired.
   */
  void begin();

  /**
   * display_host_process is called in the main loop to provide functionallity
   */
  void display_host_process(void);

  //Public Variables

  uint32_t LED_driver_state[LED_display_count]; //!< Current state of the 16Segment Displays.

  char test_printable[printable_length];      //!< Array for all printable Characters
  char logo_string[logo_length];    //!< Array for holding my logo

  uint8_t display_state;          //!< Current overall Display State.

  private:
  //Private Functions

  /**
   * set_DPs is called to correctly assign the DPs depending on special states
   */
  void set_DPs(void);

  /**
   * test_all_chars is called to itterate through all printable characters.
   */
  void test_all_chars(void);

  /**
   * scroll_logo iterates through the desired characters for the logo.
   */
  void scroll_logo(void);

  /**
   * display_shift is used to shift the characters for scroll effects.
   */
  void display_shift(void);

  /**
   * set_displays pushes the current states out to the ICs.
   */
  void set_displays(void);

  /**
   * Run test based on mask.
   */
  void run_test(void);

  //Private Variables

  uint8_t LED_driver_DriveMode;   //!< Variable for the drive mode of the device.
  float LED_pwm_frequency;        //!< Used to set the Frequency of the PWM.

  uint16_t LED_driver_brightness[LED_driver_LED_count];   //!< Holder for the current brightness of each bit.
  uint32_t LED_driver_dp_state;   //!< State of the 2 DPs of each Display.
  uint16_t desired_max_brightness;//!< User's desired max brightness.
  unsigned long display_time_step;//!< User's desired time step in millis for scrolling.
  unsigned long display_last_step;//!< User's desired time step in millis for scrolling.

  uint8_t test_all_char_index;    //!< Index for displaying all characters
  uint8_t display_logo_index;     //!< Index for displaying logo

  unsigned long timer_start;
  unsigned long timer_current;
  uint8_t timer_start_flag;
  char timer_string[7];
};
#endif
