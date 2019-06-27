/**
 * @file CS_display_board.c
 * @author Earl R. Watkins II
 * @date 26 June 2019
 * @brief Header file for the Clock Speaker Display Board
 */

 #include "CS_display_board.h"

 ERW_LED_Display BL_230B(Display_18); //!< Constructor for the Alphanumeric Translator.
 ERW_PCA9685 LED_driver(LED_driver_address, LED_driver_LED_count, LED_driver_Enable); //!< This alows command of the PCA9685 associated with display 0.
 ERW_PCA9685 LED_driver1(LED_driver1_address, LED_driver_LED_count); //!< This alows command of the PCA9685 associated with display 1.
 ERW_PCA9685 LED_driver2(LED_driver2_address, LED_driver_LED_count); //!< This alows command of the PCA9685 associated with display 2.
 ERW_PCA9685 LED_driver3(LED_driver3_address, LED_driver_LED_count); //!< This alows command of the PCA9685 associated with display 3.
 ERW_PCA9685 LED_driver4(LED_driver4_address, LED_driver_LED_count); //!< This alows command of the PCA9685 associated with display 4.
 ERW_PCA9685 LED_driver5(LED_driver5_address, LED_driver_LED_count); //!< This alows command of the PCA9685 associated with display 5.
 ERW_PCA9685 LED_driver_dp(LED_driver_dp_address, LED_driver_LED_count); //!< This alows command of the PCA9685 associated with DPs.

/**
 * @brief CS_display_board is the constructor for the class
 * @param[in] user_max_brightness Sets the Maximum Brightness to use for the display. Less than 4095.
 * @param[in] user_driveMode      Sets the drive mode of the displays.
 * @param[in] user_pwm_frequency  Sets the PWM frequency to use.
 */
CS_display_board::CS_display_board(uint16_t user_max_brightness, uint8_t user_driveMode, float user_pwm_frequency, unsigned long user_time_step)
{
  strcpy( test_printable, " !\"\#$%&'()*+,-./0123456789:;<.>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz");
  strcpy(logo_string, "RURALGURU PROJECTS     ");

  timer_start_flag = 0;
  //  LED_driver_Enable = user_OE_n;
  display_time_step = user_time_step;
  LED_driver_DriveMode = user_driveMode;
  LED_pwm_frequency = user_pwm_frequency;
  if( user_max_brightness >= LED_driver_Max)
  {
    desired_max_brightness = LED_driver_Max;
  }
  else
  {
    desired_max_brightness = user_max_brightness;
  }

  for(int index = 0; index < LED_driver_LED_count; index++)
  {
    LED_driver_brightness[index] = desired_max_brightness;
  }
  display_state = display_state_text;
}

/**
 * begin starts the displays as desired.
 */
void CS_display_board::begin()
{
  LED_driver.begin(LED_driver_DriveMode);
  LED_driver.PMW_freq(LED_pwm_frequency);
  LED_driver.set_brightness(LED_driver_brightness); /* For ease of transition from off to on the brightness is stored. */
  LED_driver.toggleIC(); /* turn IC on and off with the Pin */

  LED_driver1.begin(LED_driver_DriveMode);
  LED_driver1.PMW_freq(LED_pwm_frequency);
  LED_driver1.set_brightness(LED_driver_brightness); /* For ease of transition from off to on the brightness is stored. */

  LED_driver2.begin(LED_driver_DriveMode);
  LED_driver2.PMW_freq(LED_pwm_frequency);
  LED_driver2.set_brightness(LED_driver_brightness); /* For ease of transition from off to on the brightness is stored. */

  LED_driver3.begin(LED_driver_DriveMode);
  LED_driver3.PMW_freq(LED_pwm_frequency);
  LED_driver3.set_brightness(LED_driver_brightness); /* For ease of transition from off to on the brightness is stored. */

  LED_driver4.begin(LED_driver_DriveMode);
  LED_driver4.PMW_freq(LED_pwm_frequency);
  LED_driver4.set_brightness(LED_driver_brightness); /* For ease of transition from off to on the brightness is stored. */

  LED_driver5.begin(LED_driver_DriveMode);
  LED_driver5.PMW_freq(LED_pwm_frequency);
  LED_driver5.set_brightness(LED_driver_brightness); /* For ease of transition from off to on the brightness is stored. */

  LED_driver_dp.begin(LED_driver_DriveMode);
  LED_driver_dp.PMW_freq(LED_pwm_frequency);
  LED_driver_dp.set_brightness(LED_driver_brightness); /* For ease of transition from off to on the brightness is stored. */

  for( uint8_t loopIndex = 0; loopIndex < LED_driver_LED_count; loopIndex++ ) /* this  loop turns the LEDs on in a wave */
  {
    bitSet(LED_driver_state[0], loopIndex);
    LED_driver.LED_state(LED_driver_state[0]);
    LED_driver1.LED_state(LED_driver_state[0]);
    LED_driver2.LED_state(LED_driver_state[0]);
    LED_driver3.LED_state(LED_driver_state[0]);
    LED_driver4.LED_state(LED_driver_state[0]);
    LED_driver5.LED_state(LED_driver_state[0]);
    LED_driver_dp.LED_state(LED_driver_state[0]);
    delay(100);
  }
  for( uint8_t loopIndex = 0; loopIndex < LED_display_count; loopIndex++)
  {
    LED_driver_state[loopIndex]=0;
  }
  LED_driver_dp_state=0;

  LED_driver.LED_state(LED_driver_state[0]);
  LED_driver1.LED_state(LED_driver_state[1]);
  LED_driver2.LED_state(LED_driver_state[2]);
  LED_driver3.LED_state(LED_driver_state[3]);
  LED_driver4.LED_state(LED_driver_state[4]);
  LED_driver5.LED_state(LED_driver_state[5]);
  LED_driver_dp.LED_state(LED_driver_dp_state);

  BL_230B.begin_alphanumeric();

  display_last_step = millis();
}

/**
 * display_host_process is called in the main loop to provide functionallity
 */
void CS_display_board::display_host_process(void)
{
  if( millis() >= ( display_last_step + display_time_step))
  {
    if(display_state & display_state_test_gen)
    {
      run_test();
    }

    set_displays();
    display_last_step = millis();
  }

}

/**
 * set_DPs is called to correctly assign the DPs depending on special states
 */
void CS_display_board::set_DPs(void)
{
  if( display_state & display_state_text )
  {
    LED_driver_dp_state = 0;
    LED_driver_dp_state = (( LED_driver_state[0] & 0x10000 ) >> 14 ) |
                          (( LED_driver_state[1] & 0x10000 ) >> 15 ) |
                          (( LED_driver_state[2] & 0x10000 ) >> 16 ) |
                          (( LED_driver_state[3] & 0x10000 ) >> 11 ) |
                          (( LED_driver_state[4] & 0x10000 ) >> 12 ) |
                          (( LED_driver_state[5] & 0x10000 ) >> 13 );
  }
  else if(display_state & display_state_time)
  {
    LED_driver_dp_state = 0B10000010000;
  }
  else if(display_state & display_state_timer)
  {
    LED_driver_dp_state = 0B100010000010001;
  }
  else
  {
    LED_driver_dp_state = 0;
  }

}

/**
 * test_all_chars is called to itterate through all printable characters.
 */
void CS_display_board::test_all_chars(void)
{
  LED_driver_state[0] = BL_230B.ASCII2AN(test_printable[test_all_char_index]);

  if (test_all_char_index >=( printable_length -1 ) )
  {
    test_all_char_index = 0;
  }
  else
  {
    test_all_char_index++;
  }
}

/**
 * scroll_logo iterates through the desired characters for the logo.
 */
void CS_display_board::scroll_logo(void)
{
  LED_driver_state[0] = BL_230B.ASCII2AN(logo_string[display_logo_index]);

  if (display_logo_index >=( logo_length -1 ) )
  {
    display_logo_index = 0;
  }
  else
  {
    display_logo_index++;
  }
}

/**
 * display_shift is used to shift the characters for scroll effects.
 */
void CS_display_board::display_shift(void)
{
  for(int index = (LED_display_count - 2); index >= 0; index--)
  {
    LED_driver_state[index + 1] = LED_driver_state[index];
  }
}

/**
 * set_displays pushes the current states out to the ICs.
 */
void CS_display_board::set_displays(void)
{
  set_DPs();
  LED_driver.LED_state(LED_driver_state[0]);
  LED_driver1.LED_state(LED_driver_state[1]);
  LED_driver2.LED_state(LED_driver_state[2]);
  LED_driver3.LED_state(LED_driver_state[3]);
  LED_driver4.LED_state(LED_driver_state[4]);
  LED_driver5.LED_state(LED_driver_state[5]);
  LED_driver_dp.LED_state(LED_driver_dp_state);
}

/**
 * Run test based on mask.
 */
void CS_display_board::run_test(void)
{
  uint8_t temp_state  = display_state & 0xF0;
  if( temp_state & display_state_test_timer )
  {
    if(timer_start_flag == 0)
    {
      timer_start = millis();
      timer_start_flag = 1;
    }
    timer_current = millis() - timer_start;
    sprintf( timer_string, "%02lu%04lu", timer_current / 60000, timer_current/10);
    Serial.println(timer_current);
    for(int index = 0; index < LED_display_count; index++)
    {
      LED_driver_state[index] = BL_230B.ASCII2AN(timer_string[LED_display_count - index - 1]);
    }
  }
  else
  {
    timer_start_flag = 0;
    if ( temp_state & display_state_test_text )
    {
      display_shift();
      test_all_chars();
    }
    else if( temp_state & display_state_test_time )
    {
      char time_string[7] = "0314AM";

      for(int index = 0; index < LED_display_count; index++)
      {
        LED_driver_state[index] = BL_230B.ASCII2AN(time_string[LED_display_count - index - 1]);
      }
      display_state = display_state_time;
    }
    else if ( temp_state & display_state_test_serial)
    {
      if(Serial)   /* Delays serial until connection made */
      {
        int charHolder = Serial.read();
        if( ( charHolder >= 32 ) && ( charHolder <= 126 ) )
        {
          display_shift();
          LED_driver_state[0] = BL_230B.ASCII2AN(charHolder);
          Serial.print("Desired Character: ");
          Serial.write(charHolder);
          Serial.print("\tASCII: ");
          Serial.print(charHolder);
          Serial.print("\tLED Character: ");
          Serial.print(LED_driver_state[0]);
          Serial.println();
        }
      }
      else
      {
        display_shift();
        scroll_logo();
      }
    }
  }
}
