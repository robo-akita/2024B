/* WiFi-Control-Car(softAP) */



//
//                       _oo0oo_
//                      o8888888o
//                      88" . "88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \\|     |// '.
//                 / \\|||  :  |||//  \
//                / _||||| -:- |||||-  \
//               |   | \\\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          ."" '<  `.___\_<|>_/___.' >' "".
//         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//         \  \ `_.   \_ __\ /__ _/   .-` /  /
//     =====`-.____`.___ \_____/___.-`___.-'=====
//                       `=---='
//
//
//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//               佛祖保佑         永无BUG
//
//
//



								
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ESP32Servo.h>

#include <string.h>


const char myssid[] = "asdrf1236";
const char mypass[] = "810114514";
const char Akssid[] = "ak-gakusei3";
const char Akpass[] = "WeWillMakeYouHappy";

const IPAddress ip(192,168,31,3);
const IPAddress subnet(255,255,255,0);
const char *CACHE_CONTROL="no-store,max-age=0";




String data ;
int int_payload ;
uint8_t * rawdata;
StaticJsonDocument<200> doc;
char jsondata[255];//{button:bo,axes0:a0,axes1:a1,axes2:a2,axes3:a3}
unsigned char lx,ly,rx,ry,lbit,rbit;
bool bo=false,circle=false,cross=false,triangle=false,square=false,left=false,right=false,up=false,down=false,r1=false,r2=false,l1=false,l2=false,r3=false,l3=false,start=false,ps_select=false;




////////////////サーボ設定///////////////////////

#define lxoff 0 //増やすと反時計回りにずれる
#define rxoff 0
#define lyoff 0
#define ryoff 0


Servo A_servo;
Servo B_servo;
Servo C_servo;
Servo D_servo;




float lxangle=90;
float lyangle=90;
float rxangle=90;
float ryangle=90;
int sole1=0;
int sole2=0;

int i=0,flag=0;


int pos =0;

////////////////サーボ設定///////////////////////




void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}



int button=doc["button"];
float axes0=doc["axes0"];
float axes1=doc["axes1"];
float axes2=doc["axes2"];
float axes3=doc["axes3"];


void ConvertFromJson(){
    DeserializationError error = deserializeJson(doc, jsondata);
    if(error) {Serial.println("Deserialization error.");}
    else {
        button=doc["button"];
        axes0=doc["axes0"];
        axes1=doc["axes1"];
        axes2=doc["axes2"];
        axes3=doc["axes3"];

        //Serial.printf("button:%d axes0:%.1f axes1:%.1f axes2:%.1f axes3:%.1f\n",button,axes0,axes1,axes2,axes3);
        Serial1.printf("%02d%03.0f%03.0f%03.0f%03.0f\n",button,axes0,axes1,axes2,axes3);
        //Serial.printf("%02d%+04.0f%+04.0f%+04.0f%+04.0f\n",button,axes0,axes1,axes2,axes3);
        //Serial2.printf("%02d%+04.0f%+04.0f%+04.0f%+04.0f\n",button,axes0,axes1,axes2,axes3);
        
    }
}



WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81); // 81番ポート


  // num: クライアント番号
  // type: イベント種別
  // payload, length: 受信データとそのサイズ

//StaticJsonBuffer<200> jsonBuffer;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{   

    bool sw = false;
    switch(type) {
        case WStype_DISCONNECTED:
           // 切断時の処理;
            Serial.println("disconnected");
            break;
        
        case WStype_CONNECTED:
        
          //IPAddress ip = webSocket.remoteIP(num); // クライアントのIPアドレスを取得
          // 接続時の処理
            Serial.println("connected");
            Serial1.println('n');
            break;

        case WStype_TEXT:
        
          // テキストデータ受信時の処理;
              //rawdata = payload;
 
            int_payload = atoi((const char *)payload);
            //Serial.printf("%s\n",payload);
            sprintf(jsondata,"%s",payload);

            switch (button){//////////////////////////////////ここから調整

				case 7:
					pos+=2;
				    Serial.printf("7\n");
					if (pos>180){
						pos = 180;
					}
					
					break;
				
				case 8:
					pos-=2;
					Serial.printf("8\n");
					if (pos<0){
						pos = 0;
					}
					
					break;

				default:
					
					break;
    
    		}

            break;

        case WStype_BIN:
          // バイナリデータ受信時の処理;
            break;

        case WStype_ERROR:
          // エラー時の処理;
            Serial.printf("ws:error");
            break;


    }
    ConvertFromJson();



   



}






const bool IS_STATION_MODE=false;



void setup()
{

    Serial.begin(115200);
    Serial1.begin(115200,SERIAL_8N1,22,23);//rx,tx
    //Serial2.begin(115200,SERIAL_8N1,27,14);//rx,tx //pre 12,14
    A_servo.attach(33, 500+lxoff, 2500+lxoff);
    B_servo.attach(25, 500+rxoff, 2500+rxoff);
    C_servo.attach(26, 500+rxoff, 2500+rxoff);
    D_servo.attach(27, 500+rxoff, 2500+rxoff);

    


    if (IS_STATION_MODE){
        //IPAddress myIP = WiFi.softAPIP();
        const IPAddress gatewayIPa(192,168,179,1);
        const IPAddress gatewayIPs(170,20,10,1);
        Serial.printf("Connecting to kousentimental\n");
        WiFi.config(ip,gatewayIPs,subnet);
        WiFi.begin("PassionClub", "asdrf1236");
        while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
        }
        Serial.println();
        Serial.print("Connected, IP address: ");
        Serial.println(WiFi.localIP());



    }
    else{
        WiFi.softAP(myssid,mypass);
        delay(100);
        WiFi.softAPConfig(ip,ip,subnet);

        Serial.print("SSID: ");
        Serial.println(myssid);
        Serial.print("AP IP address: ");
        Serial.println(ip);
        Serial.println("Server start!");
    }




    server.begin();

    



    if (!SPIFFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
 
  
    server.serveStatic("/",SPIFFS,"/",CACHE_CONTROL);
    listDir(SPIFFS,"/",0);


    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    A_servo.write(0);
    B_servo.write(0);
    C_servo.write(0);
    D_servo.write(0);

}



void loop(){
    
    
    
    //Serial.printf("%s\n",Serial2.readStringUntil('\n'));   <--これいれるとバグる！！
    
   
    server.handleClient();
    webSocket.loop();
    B_servo.write(pos);
    Serial.printf("%d\n",pos);
	
		

	//Serial.printf("%d\n",pos);

            
    //Serial.printf("aaaaaaaaaaaa");
   

}