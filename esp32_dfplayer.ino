

#include <WiFi.h>

// введите ssid и пароль вашей точки доступа
const char* ssid     = "ssid";   // SSID
const char* password = "pass";   //пароль
//Укажите параметры для создания точки доступа на ESP32 если подключение к внешней точке доступа невозможно
const char* apssid = "ESP32_DFPlayer";//SSID
const char* appassword = "12345678";         //AP пароль. Не меньше 8 символов
WiFiServer server(80);//укажем порт для подключения к веб серверу

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include <HardwareSerial.h>
HardwareSerial mySerial(1);  // RX:gpio16 TX:gpio17

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

String Feedback="", Command="",cmd="",P1="",P2="",P3="",P4="",P5="",P6="",P7="",P8="",P9="";
byte ReceiveState=0,cmdState=1,strState=1,questionstate=0,equalstate=0,semicolonstate=0;

void ExecuteCommand()
{
  Serial.println("");
  //Serial.println("Command: "+Command);
  Serial.println("cmd= "+cmd+" ,P1= "+P1+" ,P2= "+P2+" ,P3= "+P3+" ,P4= "+P4+" ,P5= "+P5+" ,P6= "+P6+" ,P7= "+P7+" ,P8= "+P8+" ,P9= "+P9);
  Serial.println("");
  
  if (cmd=="your cmd") {
    // для собственных команд
    // Укажите что должно быть в Feedback="<font color=\"red\">Hello World</font>";
  }
  else if (cmd=="ip") {
    Feedback="AP IP: "+WiFi.softAPIP().toString();    
    Feedback+=", ";
    Feedback+="STA IP: "+WiFi.localIP().toString();
  }  
  else if (cmd=="mac") {
    Feedback="STA MAC: "+WiFi.macAddress();
  }  
  else if (cmd=="restart") {
    setup();
  }    
  else if (cmd=="resetwifi") {
    WiFi.begin(P1.c_str(), P2.c_str());
    Serial.print("Connecting to ");
    Serial.println(P1);
    long int StartTime=millis();
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        if ((StartTime+5000) < millis()) break;
    } 
    Serial.println("");
    Serial.println("STAIP: "+WiFi.localIP().toString());
    Feedback="STAIP: "+WiFi.localIP().toString();
  }   
  else if (cmd=="volume") {   // value = 0 to 30
    myDFPlayer.pause();
    delay(300);
    if (P1.toInt()>30)
      P1="30";
    else if (P1.toInt()<0)
      P1="0";
    myDFPlayer.volume(P1.toInt());
    delay(300);
    myDFPlayer.start();
  }     
  else if (cmd=="volumeUp") {
    myDFPlayer.pause();
    delay(300);
    myDFPlayer.volumeUp();
    delay(300);
    myDFPlayer.start();
  }   
  else if (cmd=="volumeDown") {
    myDFPlayer.pause();
    delay(300);
    myDFPlayer.volumeDown();
    delay(300);
    myDFPlayer.start();
  }    
  else if (cmd=="EQ") {  //P1 -> NORMAL|POP|ROCK|JAZZ|CLASSIC|BASS
    if (P1=="NORMAL")
      myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
    else if  (P1=="POP")
      myDFPlayer.EQ(DFPLAYER_EQ_POP);
    else if  (P1=="ROCK")
      myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
    else if  (P1=="JAZZ")
      myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
    else if  (P1=="CLASSIC")
      myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
    else if  (P1=="BASS")
      myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  }      
  else if (cmd=="DEVICE") {  //P1 -> U_DISK|SD|AUX|SLEEP|FLASH
    if (P1=="U_DISK")
      myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
    else if  (P1=="SD")
      myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
    else if  (P1=="AUX")
      myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
    else if  (P1=="SLEEP")
      myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
    else if  (P1=="FLASH")
      myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);
  }   
  else if (cmd=="sleep") {
    myDFPlayer.sleep();
  }  
  else if (cmd=="reset") {
    myDFPlayer.reset();
  }
  else if (cmd=="enableDAC") {
    myDFPlayer.enableDAC();
  }  
  else if (cmd=="disableDAC") {
    myDFPlayer.disableDAC();
  }
  else if (cmd=="outputSetting") {  //myDFPlayer.outputSetting(true, 15); //enable the output and set the gain to 15
    myDFPlayer.outputSetting(P1.toInt(), P2.toInt());
  }
  else if (cmd=="next") {
    myDFPlayer.next();
  }
  else if (cmd=="previous") {
    myDFPlayer.previous();
  }
  else if (cmd=="play") {
    myDFPlayer.play(P1.toInt());
  }
  else if (cmd=="loop") {
    myDFPlayer.loop(P1.toInt());
  }
  else if (cmd=="pause") {
    myDFPlayer.pause();
  }  
  else if (cmd=="start") {
    myDFPlayer.start();
  }  
  else if (cmd=="playFolder") {  //myDFPlayer.playFolder(15, 4);  //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)
    myDFPlayer.playFolder(P1.toInt(), P2.toInt());
  }   
  else if (cmd=="enableLoopAll") {
    myDFPlayer.enableLoopAll();
  }    
  else if (cmd=="disableLoopAll") {
    myDFPlayer.disableLoopAll();
  }   
  else if (cmd=="playMp3Folder") {  //myDFPlayer.playMp3Folder(4); //play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)
    myDFPlayer.playMp3Folder(P1.toInt());
  }     
  else if (cmd=="advertise") {  //myDFPlayer.advertise(3); //advertise specific mp3 in SD:/ADVERT/0003.mp3; File Name(0~65535)
    myDFPlayer.advertise(P1.toInt());
  }   
  else if (cmd=="stopAdvertise") {
    myDFPlayer.stopAdvertise();
  }  
  else if (cmd=="playLargeFolder") {  //myDFPlayer.playLargeFolder(2, 999); //play specific mp3 in SD:/02/004.mp3; Folder Name(1~10); File Name(1~1000)
    myDFPlayer.playLargeFolder(P1.toInt(), P2.toInt());
  }  
  else if (cmd=="loopFolder") {  //myDFPlayer.loopFolder(5); //loop all mp3 files in folder SD:/05.
    myDFPlayer.loopFolder(P1.toInt());
  }  
  else if (cmd=="randomAll") {
    myDFPlayer.randomAll();
  }  
  else if (cmd=="enableLoop") {
    myDFPlayer.enableLoop();
  }  
  else if (cmd=="disableLoop") {
    myDFPlayer.disableLoop();
  }   
  else {
    Feedback="Command is not defined";
  }
  
  if (Feedback=="") Feedback=Command;
}

void setup()
{
    mySerial.begin(9600, SERIAL_8N1, 16, 17);
      
    Serial.begin(115200); //запускаем серийный порт для вывода отладочных сообщений
    delay(10);
    
    WiFi.mode(WIFI_AP_STA); //Режим работы wifi. Одновременно и точка доступа и клиент

    WiFi.begin(ssid, password); //Подключение к wiFi

    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    long int StartTime=millis();
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        if ((StartTime+10000) < millis()) break;
    } 

    Serial.println("");
    Serial.println("STAIP address: ");
    Serial.println(WiFi.localIP());
    
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFi.softAP((WiFi.localIP().toString()+"_"+(String)apssid).c_str(), appassword);
    }
    else
      WiFi.softAP(apssid, appassword);
      
    Serial.println("");
    Serial.println("APIP address: ");
    Serial.println(WiFi.softAPIP());    
    server.begin(); 
    
    Serial.println();
    Serial.println(F("DFRobotDemo"));
    Serial.println(F("Initializing DFPlayer ..."));
    
    if (!myDFPlayer.begin(mySerial)) {  //Использовать серийный порт для взаимодействия
      Serial.println(F("Unable to begin:"));
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
    }
    else {
      Serial.println(F("DFPlayer Mini online."));
      myDFPlayer.volume(10);  //Установка громкости. От 0 до 30
      
      pinMode(2, OUTPUT);
      for (int i=0;i<10;i++)
      {
        digitalWrite(2,LOW);
        delay(100);
        digitalWrite(2,HIGH);
        delay(100);
      }
    }
}

void loop()
{
  //Объявим несколько переменных для хранения ключей команд и полученных ответов
  Command="";cmd="";P1="";P2="";P3="";P4="";P5="";P6="";P7="";P8="";P9="";
  ReceiveState=0,cmdState=1,strState=1,questionstate=0,equalstate=0,semicolonstate=0;

  WiFiClient client = server.available();

  if (client) 
  { 
    String currentLine = "";

    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();             
        
        getCommand(c);
                
        if (c == '\n') 
        {
          if (currentLine.length() == 0) 
          { 
            Feedback+="<h2>Управление плеером</h2>";
            Feedback+="<form method=\"get\" action=\"\">";
            Feedback+="cmd:";
            Feedback+="<select name=\"cmd\" id=\"cmd\">";
            Feedback+="<option value=\"\"></option>";          
            Feedback+="<option value=\"play\">Играть(P1)</option>";
            Feedback+="<option value=\"next\">Следующий</option>";
            Feedback+="<option value=\"previous\">Предыдущий</option>";  
            Feedback+="<option value=\"pause\">Пауза</option>";
            Feedback+="<option value=\"start\">Запуск</option>";  
            Feedback+="<option value=\"loop\">Повтор(P1)</option>";  
            Feedback+="<option value=\"volume\">Громкость(P1)</option>";
            Feedback+="<option value=\"volumeUp\">Громкость вверх</option>";
            Feedback+="<option value=\"volumeDown\">Громкость вниз</option>";
            Feedback+="<option value=\"EQ\">EQ(P1=NORMAL,POP,ROCK,JAZZ,CLASSIC,BASS)</option>";
            Feedback+="<option value=\"DEVICE\">Источник(P1=U_DISK,SD,AUX,SLEEP,FLASH)</option>";
            Feedback+="<option value=\"sleep\">Сон</option>";
            Feedback+="<option value=\"reset\">Сброс DFPlayer</option>";
            Feedback+="<option value=\"enableDAC\">Активация ЦАП</option>";
            Feedback+="<option value=\"disableDAC\">Деактивация ЦАП</option>";
            Feedback+="<option value=\"outputSetting\">Усиление сигнала(P1;P2)</option>";
            Feedback+="<option value=\"randomAll\">Случайное воспроизведение</option>";
            Feedback+="<option value=\"playFolder\">Играть папку(P1;P2)</option>";
            Feedback+="<option value=\"enableLoopAll\">Автоповтор всего</option>";
            Feedback+="<option value=\"disableLoopAll\">Отключить автоповтор всего</option>";
            Feedback+="<option value=\"playMp3Folder\">Играть из папки MP3(P1)</option>";
            Feedback+="<option value=\"advertise\">Рекламный блок(P1)</option>";
            Feedback+="<option value=\"stopAdvertise\">Остановка рекламного блока</option>";
            Feedback+="<option value=\"playLargeFolder\">Играть большую папку(P1;P2)</option>";
            Feedback+="<option value=\"loopFolder\">Повтор папки(P1)</option>";
            Feedback+="<option value=\"enableLoop\">Включить автоповтор</option>";
            Feedback+="<option value=\"disableLoop\">Выключить автоповтор</option>";
            Feedback+="<option value=\"ip\">IP</option>";
            Feedback+="<option value=\"mac\">MAC</option>";
            Feedback+="<option value=\"restart\">Перезагрузка ESP</option>";
            Feedback+="<option value=\"resetwifi\">Перезапуск Wifi</option>";              
            Feedback+="</select>";
            Feedback+="<br><br>P1:"; 
            Feedback+="<input type=\"text\" name=\"P1\" id=\"P1\" size=\"20\">";      
            Feedback+="<br><br>P2:";
            Feedback+="<input type=\"text\" name=\"P2\" id=\"P2\" size=\"20\">";   
            Feedback+="<br><br>";           
            Feedback+="<input type=\"button\" value=\"Отправить\" onclick=\"location.href='?'+cmd.value+'='+P1.value+';'+P2.value\">"; 
            Feedback+="</form>";

            client.println("HTTP/1.1 200 OK");
            client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
            client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
            client.println("Content-Type: text/html; charset=utf-8");
            client.println("Access-Control-Allow-Origin: *");
            //client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html><head>");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<meta http-equiv=\"Access-Control-Allow-Origin\" content=\"*\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("</head><body>");
            client.println("<h1>ESP32 DFPlayer Mini MP3 панель управления</h1>");
            client.println("<h2>Краткий справочник по командам</h2><ul>");
            client.println("<li>Играть (P1) - параметр P1 номер трека</li>");
            client.println("<li>Повтор (P1 - параметр P1 номер трека для повторного воспроизведения</li>");
            client.println("<li>Громкость (P1) - установить громкость (0-30)</li>");
            client.println("<li>EQ(P1) - установка пресета эквалайзера. 0-NORMAL,1-POP,2-ROCK,3-JAZZ,4-CLASSIC,5-BASS</li>");
            client.println("<li>Источник (P1) - установка источника воспроизведения. 0-U_DISK, 1- TF, 3 - AUX, 4- SLEEP, 5 - FLASH</li>");
            client.println("<li>Усиление (P1,P2) - P1 = true/false, P2 = 0-31</li>");
            client.println("<li>Играть папку(P1;P2) - P1 = Имя папки(1-99); P2 = Имя файла(1-255)</li>");
            client.println("<li>Играть из папки MP3(P1) - P1 = имя файла из папки MP3 на SD карте (0-65535)</li>");
            client.println("<li>Рекламный блок (P1) - P1 = имя файла рекламного блока из папки ADVERT на SD карте (0-65535)</li>");
            client.println("<li>Играть большую папку(P1;P2) - P1 = Имя папки(1-10); P2 = Имя файла(1-1000)</li>");
            client.println("<li>Повтор папки(P1) -  P1=номер папки для автоповтора всех треков из неё</li></ul>");
            client.println("<h2>Ответ модуля</h2>");
            client.println(Feedback);
            client.println("</body></html>");
            client.println();
                        
            Feedback="";
            break;
          } else {
            currentLine = "";
          }
        } 
        else if (c != '\r') 
        {
          currentLine += c;
        }

        if ((currentLine.indexOf("/?")!=-1)&&(currentLine.indexOf(" HTTP")!=-1))
        {
          if (Command.indexOf("stop")!=-1) {
            client.println();
            client.println();
            client.stop();
          }
          currentLine="";
          Feedback="";
          ExecuteCommand();
        }
      }
    }
    delay(1);
    client.stop();
  }
}

void getCommand(char c)
{
  if (c=='?') ReceiveState=1;
  if ((c==' ')||(c=='\r')||(c=='\n')) ReceiveState=0;
  
  if (ReceiveState==1)
  {
    Command=Command+String(c);
    
    if (c=='=') cmdState=0;
    if (c==';') strState++;
  
    if ((cmdState==1)&&((c!='?')||(questionstate==1))) cmd=cmd+String(c);
    if ((cmdState==0)&&(strState==1)&&((c!='=')||(equalstate==1))) P1=P1+String(c);
    if ((cmdState==0)&&(strState==2)&&(c!=';')) P2=P2+String(c);
    if ((cmdState==0)&&(strState==3)&&(c!=';')) P3=P3+String(c);
    if ((cmdState==0)&&(strState==4)&&(c!=';')) P4=P4+String(c);
    if ((cmdState==0)&&(strState==5)&&(c!=';')) P5=P5+String(c);
    if ((cmdState==0)&&(strState==6)&&(c!=';')) P6=P6+String(c);
    if ((cmdState==0)&&(strState==7)&&(c!=';')) P7=P7+String(c);
    if ((cmdState==0)&&(strState==8)&&(c!=';')) P8=P8+String(c);
    if ((cmdState==0)&&(strState>=9)&&((c!=';')||(semicolonstate==1))) P9=P9+String(c);
    
    if (c=='?') questionstate=1;
    if (c=='=') equalstate=1;
    if ((strState>=9)&&(c==';')) semicolonstate=1;
  }
}


void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}