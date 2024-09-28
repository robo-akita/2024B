#include "mbed.h"
#include "Serial.h"
#include "aTB6643KQ.hpp"
#include "2021roboSeparateBoardLib_setting.hpp"
#include <string>
Serial receive(PC_10,PC_11);//uart3 tx,rx
Serial pc(USBTX, USBRX); 
Serial thru(PA_2,PA_3);//uart4 tx,rx

//use "rohm" motor

bool espmode=true;
char direction  = 'n';

bool is_solenoid = false;
const float PPR = 2048;

class Motor{
private:
	PwmOut motor_r, motor_l;
public:
	Motor(PinName pin_r, PinName pin_l):
	motor_r(pin_r), motor_l(pin_l){
		motor_r.period_us(1000);
		motor_l.period_us(1000);
	}
	void set_power(float power){
		if(power > 0.95){
			power = 0.95;
		}
		else if(power < -0.95){
			power = -0.95;
		}
		if(power < 0.0){
			power *= -1.0;
			motor_r = 0.0;
			motor_l = power;
		}else{
			motor_l = 0.0;
			motor_r = power;
		}
	}
	float operator=(const float power){
		set_power(power);
		//pc.printf("%f\n\r",power);
		return power;
	}
};



class Solenoid{
private:
	DigitalOut solenoid;
public:
	Solenoid(PinName pin):
	solenoid(pin){}
	void set_output(bool output){
		if(output){
			solenoid = 1;
		}else{
			solenoid = 0;
		}
	}
	bool operator=(const bool output){
		set_output(output);
		return output;
	}
};

class regularC_ms{
private:
	unsigned long interval;
	unsigned long nextTime;
	Timer t;
public:
	regularC_ms(unsigned long intervalArg,unsigned long start=0):
	interval(intervalArg)
	{
		t.start();
		nextTime=start;
	}
	bool ist(){
		if(nextTime<(unsigned long)t.read_ms()){
			nextTime=interval+t.read_ms();
			return true;
		}else{
			return false;
		}
	}
	void set(unsigned long val){interval=val;}
	unsigned long read(){return interval;}
	operator bool(){return ist();}
};

class oneshotC_ms{
private:
	bool isActive;
	long nextTime;
	Timer t;
public:
	oneshotC_ms():isActive(false),nextTime(0){
		t.start();
	}
	long fromNowTo(const long wait){
		if(isActive){
			return wait;
		}
		if(wait>=0){
			nextTime=wait+t.read_ms();
			isActive=true;
		}else{
			isActive=false;
		}
		return wait;
	}
	bool ist(){
		//pc.printf("isActive%s\n",isActive?"true":"false");
		if(isActive){
			if(t.read_ms()>=nextTime){
				isActive = false;
				return true;
			}
		}
		return false;
	}
	operator bool(){return ist();}
	long operator=(const long wait){
		return fromNowTo(wait);
	}
};

class Omuni{
private:

public:
	Omuni();
};

void getC()
{	
	const char data = receive.getc();
	pc.printf("%c",data);
	if(data == 'w' || data == 'a' || data == 's' || data == 'd' || data == 'r' || data == 'l'|| data == 'n'){
		direction = data;
	}


}

void getPc()
{
	const char data = pc.getc();
	pc.printf("%c",data);
	if(data == 'w' || data == 'a' || data == 's' || data == 'd' || data == 'r' || data == 'l' || data == 'n'){
		direction = data;
	}
}

/*
Motor motor1(PC_8,PB_4);
Motor motor2(PC_9,PB_8);
Motor motor3(PA_10,PB_3);
Motor motor4(PB_10,PB_5);
Motor motor5(PA_11,PB_2);
*/

rob::aRohmMD &motor1=rob::motor1;//PB4,PC8
rob::aRohmMD &motor2=rob::motor2;//PC9,PB8
rob::aRohmMD &motor3=rob::motor3;//PB3,PA10
rob::aRohmMD &motor4=rob::motor4;//PB10,PB5
rob::aRohmMD &motor5=rob::motor5;//PA11,PB2


/*
rob::aTB6643KQ motor1(PB_4, PC_8);
rob::aTB6643KQ motor2(PC_9, PB_8);
rob::aTB6643KQ motor3(PB_3, PA_10);
rob::aTB6643KQ motor4(PB_10, PB_5);
*/

float m2,m3,m5;


rob::aRotaryEncoder &Encoder_carry_R = rob::rotaryEncoder1;//なんか0と1しか出ない
									 //PA_8,PA_9,PullDown
rob::aRotaryEncoder &Encoder_carry_L = rob::rotaryEncoder2;
rob::aRotaryEncoder Encoder_reload(PA_6, PA_7, PullDown);
int value;


const int BUF_LEN=255;
int bufIndex=0;
char buf[BUF_LEN]={};
int button;
double axes0,axes1,axes2,axes3;

void serialReceiver() {
	const char receiveByte = (char)receive.getc();//getcでたまっているのを1文字受信
	if (receiveByte == '\n') {//もし\n（1つの文字列の終わり）なら
		buf[bufIndex] = '\0';//bufの最後に\nを書き込む
		//val=atof((const char*)buf);//bufの文字列をatofで戻し(doubleに変換される)valに書き込む
		bufIndex = 0;//\nを読んだってことは配列の場所をリセットする
		//pc.printf(" %s\n ",buf);
		string str = string(buf);
				
		if(espmode==false){
			thru.printf("%s\n",str);
		}
		
		string str1 = str.substr(0, 2);
		string str2 = str.substr(2, 3);
		string str3 = str.substr(5, 3);
		string str4 = str.substr(8, 3);
		string str5 = str.substr(11,3);

		
	
		button = atoi(str1.c_str());
		axes0  = atof(str2.c_str());
		axes1  = atof(str3.c_str());
		axes2  = atof(str4.c_str());
		axes3  = atof(str5.c_str());
		//pc.printf("%d  ",button);
		//pc.printf("%3.0f  ",axes0);
		//pc.printf("%04.0lf\n",axes1);

		/*
		if (str.size() == 20&&checksum==subd_linkrevel+subropemotor+subup+subdown+subsank+subbatu+subencreset) {//怪しい
			
		d_linklevel= subd_linkrevel;
		ropemotor  = subropemotor;
		up         = subup;
		down 	   = subdown;
		sank       = subsank;
		batu       = subbatu;
		encreset   = subencreset;
		
			
			
		ifreceive();		
			
		} else {
			pc.printf("error!(size%d)",str.size());//フリーズ危険ユニオン
		}
		*/
		
		
		
	}
	else {//\nじゃないなら
		if(!(bufIndex<BUF_LEN))return;
		buf[bufIndex] = receiveByte;//bufの指定した場所(bufindex)に1文字書き込む
		bufIndex++;
	}
	return;
}


regularC_ms one_time(1000);



int main(){
	//motor2.setIsBrakeWhenZero(false);
	int count=0;
	
	pc.baud(115200);
	pc.attach(getPc,Serial::RxIrq);
	pc.printf("hello\n");
	//toServo.baud(115200);
	receive.baud(115200);
	receive.attach(serialReceiver, Serial::RxIrq);
	int shot_encoderVal_R;


	
	while(true){
		//receive.printf("motor 2 :%.1f motor 3 :%.1f motor 5 :%.1f\n",m2,m3,m5);
/*
				進行方向
		
				 ^
				 |
		-----------------
		|③				④|
		|				  |
		|				  |
		|				  |
		|			      |
		|				  |
		|②				①|
		-----------------
		
		
		
		
*/
		switch(button){
			case 13: //w 前
				motor1 = -0.5;
				motor2 = 0.5;
				motor3 = -0.5;
				motor4 = 0.5;
				receive.printf("forward");
				pc.printf("forward");

				break;
			case 15://a　左
				motor1 = 0.5;
				motor2 = 0.5;
				motor3 = 0.5;
				motor4 = 0.5;
			

				break;
			case 16:// d　右
				motor1 = -0.5;
				motor2 = -0.5;
				motor3 = -0.5;
				
				motor4 = -0.5;
				break;
			case 14://s 後ろ
				motor1 = 0.5;
				motor2 = -0.5;
				motor3 = 0.5;
				motor4 = -0.5;
				
				break;
			
				
			case 1:

				break;
				
				
				
			case 5:
						
				break;
				
				
			
			case 6:
				m2=0.0;
				m3=0.0;
				m5=0.0;
			
			

				break;
			case 0:
				motor1 = 0.0;
				motor2 = 0.0;
				motor3 = 0.0;
				motor4 = 0.0;
				
				
				
				break;
					
		}
		
	}

    return 0;
}



