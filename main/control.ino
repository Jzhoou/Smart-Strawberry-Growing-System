void control_heater(int heater_state)//手动控制制热
{
  if (heater_state==1)
  {
      digitalWrite(Heater,1);
  }
  else digitalWrite(Heater,0);
}
void auto_heater(int state)//自动控制制热
{
  auto_state_heater=state;
}
/*
—————————————————————————————分割线——————————————————————————————————————
*/
void control_colder(int colder_state)//手动控制制冷
{
  if (colder_state==1)
  {
      digitalWrite(Colder,1);
  }
  else digitalWrite(Colder,0);
}
void auto_colder(int state)//自动控制制冷
{
  auto_state_colder=state;
}
/*
—————————————————————————————分割线——————————————————————————————————————
*/
void control_led(int led_state)//手动控制补光
{
  if (led_state==1)
  {
      digitalWrite(LED,1);
  }
  else digitalWrite(LED,0);
}

void auto_led(int state)//自动控制补光
{
  auto_state_LED=state;
}
/*
—————————————————————————————分割线——————————————————————————————————————
*/
void control_fans(int fans_state)//手动控制通风
{
  if (fans_state==1)
  {
      digitalWrite(fans,1);
  }
  else digitalWrite(fans,0);
}

void auto_fans(int state)//自动控制通风
{
  auto_state_fans=state;
}
/*
—————————————————————————————分割线——————————————————————————————————————
*/
void control_water(int water_state)//手动控制浇水
{
  if (water_state==1)
  {
      digitalWrite(water,1);
  }
  else digitalWrite(water,0);
}
void auto_water(int state)//自动控制浇水
{
  auto_state_water=state;
}
