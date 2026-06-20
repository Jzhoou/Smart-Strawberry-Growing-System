long get_humility()//获取土壤湿度
{
  long sensorValue=analogRead(humilityPIN);
  sensorValue=((1023-sensorValue)*100)/1023;
  return sensorValue;
}
/*
—————————————————————————————分割线——————————————————————————————————————
*/
long get_tempreture()//获取温度
{
  DHT11.read(DHT11PIN);
  return DHT11.temperature;
}
/*
—————————————————————————————分割线——————————————————————————————————————
*/
float get_airhumility()//获取空气湿度
{
  DHT11.read(DHT11PIN);
  return (float)DHT11.humidity;
}
/*
—————————————————————————————分割线——————————————————————————————————————
*/
int get_huminosity() { //获取光照强度       
    int photosenVal = 0;                                    //光照度数值
    int photoContent = 0;
    photosenVal = analogRead(photosensitivePin);        //获取原始值
    photosenVal = constrain(photosenVal, 10, 1024);     //原始值限制在一定范围
    photoContent = map(photosenVal, 10, 1024, 100, 0);  //映射到对应的区间
    return photoContent;
}
/*
—————————————————————————————分割线——————————————————————————————————————
*/
///////////////////////////获取CO2
float getCO2(String CO2);  // 函数声明
float get_CO2() {
  //delay(500);  // 放慢输出频率
  for (int i = 0 ; i < 8; i++) {  // 发送测量CO2浓度命令
    CO2Serial.write(item_CO2[i]);   // write输出
  }
  delay(100);  // 等待数据返回
  data_CO2 = "";
  while (CO2Serial.available()) {//从串口中读取数据
    unsigned char in = (unsigned char)CO2Serial.read();  // read读取
    data_CO2 += in;
    data_CO2 += ',';
  }
  return ((float)getCO2(data_CO2));
}
float getCO2(String CO2) {
  int commaPosition = -1;
  String info[7];  // 用字符串数组存储
  for (int i = 0; i < 7; i++) {
    commaPosition = CO2.indexOf(',');
    if (commaPosition != -1)
    {
      info[i] = CO2.substring(0, commaPosition);
      CO2 = CO2.substring(commaPosition + 1, CO2.length());
    }
    else {
      if (CO2.length() > 0) {  // 最后一个会执行这个
        info[i] = CO2.substring(0, commaPosition);
      }
    }
  }
  return (info[3].toInt() * 256 + info[4].toInt()) ;
}
//////////////////////////
/*
—————————————————————————————分割线——————————————————————————————————————
*/
//////////////////////////获取NPK含量
void pre_getdata();
float get_data(String NPK);  // 函数声明 获取NPK数据
float get_N() {
  pre_getdata();
  m=3;n=4;
  return get_data(data_NPK);
}
float get_P() {
  pre_getdata();
  m=5;n=6;
  return get_data(data_NPK);
}
float get_K() {
  pre_getdata();
  m=7;n=8;
  return get_data(data_NPK);
}
void pre_getdata()
{
  //delay(500);  // 放慢输出频率
  for (int i = 0 ; i < 8; i++) {  // 发送测温命令
    NPKSerial.write(item_NPK[i]);   // write输出
  }
  delay(100);  // 等待测温数据返回
  data_NPK = "";
  while (NPKSerial.available()) {//从串口中读取数据
    unsigned char in = (unsigned char)NPKSerial.read();  // read读取
    data_NPK += in;
    data_NPK += ',';
  }
}
float get_data(String NPK) 
{
  int commaPosition = -1;
  String info[11];  // 用字符串数组存储
  for (int i = 0; i < 11; i++) {
    commaPosition = NPK.indexOf(',');
    if (commaPosition != -1)
    {
      info[i] = NPK.substring(0, commaPosition);
      NPK = NPK.substring(commaPosition + 1, NPK.length());
    }
    else {
      if (NPK.length() > 0) {  // 最后一个会执行这个
        info[i] = NPK.substring(0, commaPosition);
      }
    }
  }
  return (info[m].toInt() * 256 + info[n].toInt()) ;
}
//////////////////////////
/*
—————————————————————————————分割线——————————————————————————————————————
*/
//////////////////////////获取pH含量
float getPH_temp(String PH);  // 函数声明
float get_PH() {
  //delay(500);  // 放慢输出频率
  for (int i = 0 ; i < 8; i++) {  // 发送测温命令
    PHSerial.write(item_PH[i]);   // write输出
  }
  delay(100);  // 等待测温数据返回
  data_PH = "";
  while (PHSerial.available()) {//从串口中读取数据
    unsigned char in = (unsigned char)PHSerial.read();  // read读取
    data_PH += in;
    data_PH += ',';
  }
   return ((float)getPH_temp(data_PH)/10);
}
float getPH_temp(String PH) {
  int commaPosition = -1;
  String info[7];  // 用字符串数组存储
  for (int i = 0; i < 7; i++) {
    commaPosition = PH.indexOf(',');
    if (commaPosition != -1)
    {
      info[i] = PH.substring(0, commaPosition);
      PH = PH.substring(commaPosition + 1, PH.length());
    }
    else {
      if (PH.length() > 0) {  // 最后一个会执行这个
        info[i] = PH.substring(0, commaPosition);
      }
    }
  }
  return (info[3].toInt() * 256 + info[4].toInt()) ;
}
//////////////////////////