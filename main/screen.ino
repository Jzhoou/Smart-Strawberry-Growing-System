void SendEnd()//与串口屏通信的发送结尾fffffff
{
  TJCSerial.write(0xff);
  TJCSerial.write(0xff);
  TJCSerial.write(0xff);
}
void update_threshold()//更新阈值
{
  threshold_colder=EEPROM.read(0);
  threshold_heater=EEPROM.read(1);
  threshold_water=EEPROM.read(2);
  EEPROM.get(5,threshold_fans);
  threshold_LED=EEPROM.read(4);
  if(o==0){
  update_firsttime();
  o=1;
  }
}
void update_firsttime()//开机时更新第一次阈值
{
  sprintf(str,"threshold.n0.val=\%d",threshold_colder);
  TJCSerial.print(str);
  SendEnd();

  sprintf(str,"threshold.n1.val=\%d",threshold_heater);
  TJCSerial.print(str);
  SendEnd();

  sprintf(str,"threshold.n2.val=\%d",threshold_water);
  TJCSerial.print(str);
  SendEnd();

  sprintf(str,"threshold.n3.val=\%d",threshold_fans);
  TJCSerial.print(str);
  SendEnd();

  sprintf(str,"threshold.n4.val=\%d",threshold_LED);
  TJCSerial.print(str);
  SendEnd();
  
}