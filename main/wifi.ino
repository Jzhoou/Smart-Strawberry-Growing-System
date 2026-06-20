

unsigned long gokit_time_s(void)
{
  return millis() / 1000;
}

char gokit_key1down(void)
{
  unsigned long keep_time = 0;
  if (digitalRead(KEY1) == LOW)
  {
    delay(100);
    if (digitalRead(KEY1) == LOW)
    {
      keep_time = gokit_time_s();
      while (digitalRead(KEY1) == LOW)
      {
        if ((gokit_time_s() - keep_time) > KEY_LONG_TIMER)
        {
          Last_KeyTime = gokit_time_s();
          return KEY1_LONG_PRESS;
        }
      } //until open the key

      if ((gokit_time_s() - Last_KeyTime) > KEY_LONG_TIMER)
      {
        return KEY1_SHORT_PRESS;
      }
      return 0;
    }
    return 0;
  }
  return 0;
}

char gokit_key2down(void)
{
  unsigned long keep_time = 0;
  if (digitalRead(KEY2) == LOW)
  {
    delay(100);
    if (digitalRead(KEY2) == LOW)
    {
      keep_time = gokit_time_s();
      while (digitalRead(KEY2) == LOW) //until open the key
      {

        if ((gokit_time_s() - keep_time) > KEY_LONG_TIMER)
        {
          Last_KeyTime = gokit_time_s();
          return KEY2_LONG_PRESS;
        }
      }

      if ((gokit_time_s() - Last_KeyTime) > KEY_LONG_TIMER)
      {
        return KEY2_SHORT_PRESS;
      }
      return 0;
    }
    return 0;
  }
  return 0;
}

char gokit_keydown(void)
{
  char ret = 0;
  ret |= gokit_key2down();
  ret |= gokit_key1down();
  return ret;

}

/**
* KEY_Handle 
* @param none
* @return none
*/
void KEY_Handle(void)
{
  /*  Press for over than 3 second is Long Press  */
  switch (gokit_keydown())
  {
    case KEY1_SHORT_PRESS:
      Serial.println(F("KEY1_SHORT_PRESS , Production Test Mode "));
      myGizwits.setBindMode(WIFI_PRODUCTION_TEST);
      break;
    case KEY1_LONG_PRESS:
      Serial.println(F("KEY1_LONG_PRESS ,Wifi Reset"));
      myGizwits.setBindMode(WIFI_RESET_MODE);
      break;
    case KEY2_SHORT_PRESS:
      Serial.println(F("KEY2_SHORT_PRESS Soft AP mode"));
      myGizwits.setBindMode(WIFI_SOFTAP_MODE);
      //Soft AP mode
      break;
    case KEY2_LONG_PRESS:
      Serial.println(F("KEY2_LONG_PRESS ,AirLink mode"));
      myGizwits.setBindMode(WIFI_AIRLINK_MODE);
      //AirLink mode
      break;
    default:
      break;
  }
}



/**
* Wifi status printf  
* @param none
* @return none
*/
void wifiStatusHandle()
{
  if(myGizwits.wifiHasBeenSet(WIFI_SOFTAP))
  {
    Serial.println(F("WIFI_SOFTAP!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_AIRLINK))
  {
    Serial.println(F("WIFI_AIRLINK!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_STATION))
  {
    Serial.println(F("WIFI_STATION!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_CON_ROUTER))
  {
    Serial.println(F("WIFI_CON_ROUTER!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_DISCON_ROUTER))
  {
    Serial.println(F("WIFI_DISCON_ROUTER!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_CON_M2M))
  {
    Serial.println(F("WIFI_CON_M2M!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_DISCON_M2M))
  {
    Serial.println(F("WIFI_DISCON_M2M!"));
  }
}


