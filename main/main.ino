#include <Gizwits.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include<EEPROM.h>
#include<MsTimer2.h>
#include <dht11.h>//引入DHT11库

Gizwits myGizwits;
#define   KEY1              6
#define   KEY2              7
#define   KEY1_SHORT_PRESS  1
#define   KEY1_LONG_PRESS   2
#define   KEY2_SHORT_PRESS  4
#define   KEY2_LONG_PRESS   8
#define   NO_KEY            0
#define   KEY_LONG_TIMER    3
dht11 DHT11;//定义传感器类型
#define humilityPIN A0
#define photosensitivePin A1
#define DHT11PIN 4
#define fans 5
#define water 6
#define Heater 7
#define Colder 8
#define LED 10
/////////////////////////////////////////////////////因为接入wifi模块，所以将ph的串口1给wifi模块使用，即SoftwareSerial PHSerial(3, 2);  // RX, TX
//#define PHSerial Serial1
#define PHSerial Serial1
#define TJCSerial Serial2
#define CO2Serial Serial3
SoftwareSerial NPKSerial(13, 12);  // RX, TX

#define FRAME_LENGTH 10

unsigned char item_PH[8]={0x02,0x03,0x00,0x00,0x00,0x01,0x84,0x39};//16进制测PH命令
unsigned char item_NPK[8]={0x01,0x03,0x00,0x1E,0x00,0x03,0x65,0xCD};//16进制测NPK命令
unsigned char item_CO2[8]={0x01,0x03,0x00,0x05,0x00,0x01,0x94,0x0B};//16进制测CO2命令
String data_NPK="";
String data_PH="";
String data_CO2="";
int m,n,o=0;
char command;
int object;
int data;
int threshold_heater,threshold_colder,threshold_water,threshold_fans,threshold_LED;
int auto_state_heater,auto_state_colder,auto_state_water,auto_state_fans,auto_state_LED;
long humility,temp;
int luminosity;
float PH,N,P,K,CO2,airhum;
char str[100];
char s[10];
int temp_threshold[5];
unsigned long previousMillis = 0; // 上次读取的时间
const long interval = 2000; // 每2秒读取一次
unsigned long Last_KeyTime = 0;

/*
unsigned long varW_Temp = 0;//Add Sensor Data Collection
unsigned long varW_soil_humi = 0;//Add Sensor Data Collection
unsigned long varW_light = 0;//Add Sensor Data Collection
float varW_PH = 0;//Add Sensor Data Collection
float varW_humi = 0;//Add Sensor Data Collection
float varW_N = 0;//Add Sensor Data Collection
float varW_CO2 = 0;//Add Sensor Data Collection
float varW_P = 0;//Add Sensor Data Collection
float varW_K = 0;//Add Sensor Data Collection
*/
bool varR_fans = 0;
bool varR_auto_fans = 0;
bool varR_heater = 0;
bool varR_colder = 0;
bool varR_auto_temp = 0;
bool varR_LED = 0;
bool varR_auto_LED = 0;
bool varR_water = 0;
bool varR_auto_water = 0;
unsigned long varR_temp_down = 0;
unsigned long varR_temp_up = 0;
unsigned long varR_water_Q = 0;
unsigned long varR_led_Q = 0;
float varR_CO2_up = 0;
void onTimer()//定时器中断
{
  update_threshold();
  if(auto_state_heater==1)//自动制热
  {
    if(temp<threshold_heater)
    {
      control_heater(1);

      Serial.print(temp);
      Serial.print("xxx");
      Serial.println(threshold_heater);
      Serial.println("heater open");
      ///////////////////
    }
    else
    {control_heater(0);
      //////////////////用于调试
      Serial.println("heater close");
      Serial.print(temp);
      Serial.print("xxx");
      Serial.println(threshold_heater);
      ///////////////////
    }
  }
  if(auto_state_colder==1)//自动制冷
  {
    if(temp>threshold_colder)
    {
      control_colder(1);
      Serial.print(temp);
      Serial.print("xxx");
      Serial.println(threshold_colder);
      Serial.println("colder open");
    }
    else
    {control_colder(0);
      Serial.println("colder close");
      Serial.print(temp);
      Serial.print("xxx");
      Serial.println(threshold_colder);
    }
  }

  if(auto_state_LED==1)//自动补光
  {
    luminosity=get_huminosity();//获取当前光照强度
    if(luminosity<threshold_LED)
    {
      control_led(1);
    }
    else control_led(0);
  }

  if(auto_state_water==1)//自动浇水
  {
    humility=get_humility();//获取当前土壤湿度
    if(humility<threshold_water)
    {
      control_water(1);
    }
    else control_water(0);
  }

  if(auto_state_fans==1)//自动通风
  {
    if(CO2>threshold_fans)
    {
      control_fans(1);
    }
    else control_fans(0);
  }
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  NPKSerial.begin(9600);
  PHSerial.begin(9600);
  TJCSerial.begin(115200);
  CO2Serial.begin(9600);

  MsTimer2::set(1000,onTimer);//定时器初始化，每隔1000ms进入一次定时器中断
  MsTimer2::start();//开启定时器中断

  pinMode(fans,OUTPUT);
  pinMode(water,OUTPUT);
  pinMode(Heater,OUTPUT);
  pinMode(Colder,OUTPUT);
  pinMode(LED,OUTPUT);
  
  while(TJCSerial.read()>=0);//初始化串口屏
  update_threshold();//更新阈值数据
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  myGizwits.begin();

  Serial.println("GoKit init  OK \n");

}


void loop() {  
  
  unsigned long currentMillis = millis(); // 获取当前时间
  
    if (currentMillis - previousMillis >= interval) //定时更新温度和co2数据，类似于定时器功能（由于不明原因定时器获取温度和co2数据出错，于是采取以下方式）
    {if(auto_state_colder==1)
    {
      previousMillis = currentMillis; // 更新上次读取时间
      temp=get_tempreture();
    }
    else if(auto_state_fans==1)
    {
      previousMillis = currentMillis; // 更新上次读取时间
      CO2=get_CO2();
    }
    }
 
 //与串口屏通信
  char str[100];
  while (TJCSerial.available()>=FRAME_LENGTH){//串口屏控制系统
    unsigned char ubuffer [FRAME_LENGTH];
    unsigned char frame_header=TJCSerial.peek();
    if(frame_header==0x55){
      TJCSerial.readBytes(ubuffer,FRAME_LENGTH);
      if(ubuffer[7]==0xff&&ubuffer[8]==0xff&&ubuffer[9]==0xff)
      {
        object=ubuffer[1];                //对象
        command=ubuffer[2];               //命令
        data=ubuffer[3]+16*16*ubuffer[4]; //数据
        switch (object){
        case 0: {//手动补光开关
          if (command==0) 
          {
            Serial.println("0close");
            control_led(0);
          }
          else {
            Serial.println("0open");
            control_led(1);
          }
          break;}
        case 1:{//自动补光
          if (command==0) 
          {
            Serial.println("1close");
            auto_led(0);
          }
          else 
          {
            Serial.println("1open");
            auto_led(1);
          }
          break;}
        case 2:{//手动浇水开关
          if (command==0) 
          {
            Serial.println("2close");
            control_water(0);
          }
          else 
          {
            Serial.println("2open");
            control_water(1);
          }
          break;}
        case 3:{//自动浇水
          if (command==0) 
          {
            Serial.println("3close");
            auto_water(0);
          }
          else 
          {
            Serial.println("3open");
            auto_water(1);
          }
          break;}
        case 4:{//刷新data1页面
          PH=get_PH();
          N=get_N();
          P=get_P();
          K=get_K();

          dtostrf(PH,1,2,s);
          sprintf(str,"tPH.txt=\"%s\"",s);
          TJCSerial.print(str);
          SendEnd();

          dtostrf(N,1,2,s);
          sprintf(str,"tN.txt=\"%s(mg/kg)\"",s);
          TJCSerial.print(str);
          SendEnd();

          dtostrf(P,1,2,s);
          sprintf(str,"tP.txt=\"%s(mg/kg)\"",s);
          TJCSerial.print(str);
          SendEnd();

          dtostrf(K,1,2,s);
          sprintf(str,"tK.txt=\"%s(mg/kg)\"",s);
          TJCSerial.print(str);
          SendEnd();
          break;
          }
        case 5:{//刷新data2页面
          humility=get_humility();
          CO2=get_CO2();
          temp=get_tempreture();
          airhum=get_airhumility();
          luminosity=get_huminosity();

          sprintf(str,"thumility.txt=\"%d\"",humility);
          TJCSerial.print(str);
          SendEnd();

          dtostrf(CO2,1,2,s);
          sprintf(str,"tco2.txt=\"%s(ppm)\"",s);
          TJCSerial.print(str);
          SendEnd();

          sprintf(str,"ttemp.txt=\"%d(°C)\"",temp);
          TJCSerial.print(str);
          SendEnd();

          dtostrf(airhum,1,2,s);
          sprintf(str,"tairhum.txt=\"%s\"",s);
          TJCSerial.print(str);
          SendEnd();

          sprintf(str,"tluminosity.txt=\"%d\"",luminosity);
          TJCSerial.print(str);
          SendEnd();

          break;
          }
        case 6:{//手动制热
          if (command==0) 
          {
            Serial.println("6close");
            control_heater(0);
          }
          else 
          {
            Serial.println("6open");
            control_heater(1);
          }
          break;}
        case 7:{//手动制冷
          if (command==0) 
          {
            Serial.println("7close");
            control_colder(0);
          }
          else 
          {
            Serial.println("7open");
            control_colder(1);
          }
          break;}
        case 8:{//自动温控
          if (command==0) 
          {
            Serial.println("8close");
            auto_heater(0);
            auto_colder(0);
          }
          else 
          {
            Serial.println("8open");
            auto_heater(1);
            auto_colder(1);
          }
          break;}
        case 9:{//手动通风
          if (command==0) 
          {
            Serial.println("9close");
            control_fans(0);
          }
          else 
          {
            Serial.println("9open");
            control_fans(1);
          }
          break;}
        case 10:{//自动通风
          if (command==0) 
          {
            Serial.println("10close");
            auto_fans(0);
          }
          else 
          {
            Serial.println("10open");
            auto_fans(1);
          }
          break;}
        case 11:{//获取温度上限
          EEPROM.write(0,data);
          Serial.println(EEPROM.read(0));
          break;}
        case 12:{//获取温度下限
          EEPROM.write(1,data);
          Serial.println(EEPROM.read(1));
          break;}
        case 13:{//获取浇水阈值
          EEPROM.write(2,data);
          Serial.println(EEPROM.read(2));
          break;}
        case 14:{//获取通风阈值（co2）
        int a;
          EEPROM.put(5,data);
          EEPROM.get(5,a);
          Serial.println(a);
          break;}
        case 15:{//获取补光阈值
          EEPROM.write(4,data);
          Serial.println(EEPROM.read(4));
          break;}
        }
      }
    }
    else break;
  }

  //Configure network
  //if(XXX) //Trigger Condition
  //myGizwits.setBindMode(0x02);  //0x01:Enter AP Mode;0x02:Enter Airlink Mode
    
  temp=get_tempreture();
  myGizwits.write(VALUE_Temp, temp);
  humility=get_humility();
  myGizwits.write(VALUE_soil_humi, humility);
  luminosity=get_huminosity();
  myGizwits.write(VALUE_light, (long)luminosity);
  PH=get_PH();
  myGizwits.write(VALUE_PH, PH);
  airhum=get_airhumility();
  myGizwits.write(VALUE_humi, airhum);
  N=get_N();
  myGizwits.write(VALUE_N, N);
  CO2=get_CO2();
  myGizwits.write(VALUE_CO2, CO2);
  P=get_P();
  myGizwits.write(VALUE_P, P);
  K=get_K();
  myGizwits.write(VALUE_K, K);


  
  if(myGizwits.hasBeenSet(EVENT_fans))
  {
    myGizwits.read(EVENT_fans,&varR_fans);//Address for storing data
    Serial.println(F("EVENT_fans"));
    Serial.println(varR_fans,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_auto_fans))
  {
    myGizwits.read(EVENT_auto_fans,&varR_auto_fans);//Address for storing data
    Serial.println(F("EVENT_auto_fans"));
    Serial.println(varR_auto_fans,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_heater))
  {
    myGizwits.read(EVENT_heater,&varR_heater);//Address for storing data
    Serial.println(F("EVENT_heater"));
    Serial.println(varR_heater,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_colder))
  {
    myGizwits.read(EVENT_colder,&varR_colder);//Address for storing data
    Serial.println(F("EVENT_colder"));
    Serial.println(varR_colder,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_auto_temp))
  {
    myGizwits.read(EVENT_auto_temp,&varR_auto_temp);//Address for storing data
    Serial.println(F("EVENT_auto_temp"));
    Serial.println(varR_auto_temp,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_LED))
  {
    myGizwits.read(EVENT_LED,&varR_LED);//Address for storing data
    Serial.println(F("EVENT_LED"));
    Serial.println(varR_LED,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_auto_LED))
  {
    myGizwits.read(EVENT_auto_LED,&varR_auto_LED);//Address for storing data
    Serial.println(F("EVENT_auto_LED"));
    Serial.println(varR_auto_LED,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_water))
  {
    myGizwits.read(EVENT_water,&varR_water);//Address for storing data
    Serial.println(F("EVENT_water"));
    Serial.println(varR_water,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_auto_water))
  {
    myGizwits.read(EVENT_auto_water,&varR_auto_water);//Address for storing data
    Serial.println(F("EVENT_auto_water"));
    Serial.println(varR_auto_water,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_temp_down))
  {
    myGizwits.read(EVENT_temp_down,&varR_temp_down);//Address for storing data
    Serial.println(F("EVENT_temp_down"));
    Serial.println(varR_temp_down,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_temp_up))
  {
    myGizwits.read(EVENT_temp_up,&varR_temp_up);//Address for storing data
    Serial.println(F("EVENT_temp_up"));
    Serial.println(varR_temp_up,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_water_Q))
  {
    myGizwits.read(EVENT_water_Q,&varR_water_Q);//Address for storing data
    Serial.println(F("EVENT_water_Q"));
    Serial.println(varR_water_Q,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_led_Q))
  {
    myGizwits.read(EVENT_led_Q,&varR_led_Q);//Address for storing data
    Serial.println(F("EVENT_led_Q"));
    Serial.println(varR_led_Q,DEC);
  }
  
  if(myGizwits.hasBeenSet(EVENT_CO2_up))
  {
    myGizwits.read(EVENT_CO2_up,&varR_CO2_up);//Address for storing data
    Serial.println(F("EVENT_CO2_up"));
    Serial.println(varR_CO2_up,DEC);
  }


  //binary datapoint handle
  
  KEY_Handle();//key handle , network configure
  wifiStatusHandle();//WIFI Status Handle
  myGizwits.process();
}