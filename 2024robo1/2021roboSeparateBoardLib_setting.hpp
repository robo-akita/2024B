#ifndef M2021ROBOSEPARATEBOARDLIB_INCLUDE_GUARD
#define M2021ROBOSEPARATEBOARDLIB_INCLUDE_GUARD

//���̃t�@�C������shift-jis�ȓ_�ɂ��Ē���

#include "2021roboSeparateBoardLib/lib.hpp"

//////////////////////////
// �����Ŋe��l��ݒ肷��
//////////////////////////

// �ݒ肱������
///////////////////////////

//ENABLE_���Ƃ�
//�g�p���Ă��Ȃ����̂̓R�����g�A�E�g����Ƃ悢

// #define ENABLE_motor1_AS_KMD
//#define ENABLE_motor2_AS_KMD
//#define ENABLE_motor3_AS_KMD
//#define ENABLE_motor4_AS_KMD
//#define ENABLE_motor5_AS_KMD
#define ENABLE_motor1_AS_ROHMMD
#define ENABLE_motor2_AS_ROHMMD
#define ENABLE_motor3_AS_ROHMMD
#define ENABLE_motor4_AS_ROHMMD
#define ENABLE_motor5_AS_ROHMMD
#define ENABLE_motor6_AS_ROHMMD
#define ENABLE_rotaryEncoder1
#define ENABLE_rotaryEncoder2
// #define ENABLE_imu03a
//#define ENABLE_xbeeCore
// #define ENABLE_switchJ8
// #define ENABLE_switchJ7


//VAL_���Ƃ�
//�K�؂Ȓl��ݒ肷�邱��

//�V���A���̒ʐM���x��ݒ�
//xbee���̐ݒ�ƍ��킹�Ă�������
//��ԑ���115200�𐄏����܂�
#define VAL_xbeeCore_serialSpeed 115200

//xbeeCore�ɂԂ牺����node�̐����w�肵�܂�
//������node�Ƃ�xbeeComm�̂��Ƃł�
//���������Ă��炦�΂킩��܂������̃m�[�h�����삷�邱�Ƃ��ł��܂�
#define VAL_xbeeCore_nodeNum 3


///////////////////////////
// �ݒ肱���܂�


//�ȉ��͊�{�I�ɂ͂�����K�v�͂���܂���


namespace rob{
#ifdef ENABLE_motor1_AS_KMD
	#ifdef ENABLE_motor1_AS_ROHMMD
		#error ("������KMD��ROHMMD�𓯈�̃|�[�g1�ɑ΂��Ďg�p���邱�Ƃ͂ł��܂��� 2021roboSeparateBoardLib_setting.hpp���m�F���Ă�������")
	#endif
	extern aKMD motor1;
#elif defined(ENABLE_motor1_AS_ROHMMD)
	extern aRohmMD motor1;
#endif

#ifdef ENABLE_motor2_AS_KMD
	#ifdef ENABLE_motor2_AS_ROHMMD
		#error ("������KMD��ROHMMD�𓯈�̃|�[�g2�ɑ΂��Ďg�p���邱�Ƃ͂ł��܂��� 2021roboSeparateBoardLib_setting.hpp���m�F���Ă�������")
	#endif
	extern aKMD motor2;
#elif defined(ENABLE_motor2_AS_ROHMMD)
	extern aRohmMD motor2;
#endif

#ifdef ENABLE_motor3_AS_KMD
	#ifdef ENABLE_motor3_AS_ROHMMD
		#error ("������KMD��ROHMMD�𓯈�̃|�[�g3�ɑ΂��Ďg�p���邱�Ƃ͂ł��܂��� 2021roboSeparateBoardLib_setting.hpp���m�F���Ă�������")
	#endif
	extern aKMD motor3;
#elif defined(ENABLE_motor3_AS_ROHMMD)
	extern aRohmMD motor3;
#endif

#ifdef ENABLE_motor4_AS_KMD
	#ifdef ENABLE_motor4_AS_ROHMMD
		#error ("������KMD��ROHMMD�𓯈�̃|�[�g4�ɑ΂��Ďg�p���邱�Ƃ͂ł��܂��� 2021roboSeparateBoardLib_setting.hpp���m�F���Ă�������")
	#endif
	extern aKMD motor4;
#elif defined(ENABLE_motor4_AS_ROHMMD)
	extern aRohmMD motor4;
#endif

#ifdef ENABLE_motor5_AS_KMD
	#ifdef ENABLE_motor5_AS_ROHMMD
		#error ("������KMD��ROHMMD�𓯈�̃|�[�g5�ɑ΂��Ďg�p���邱�Ƃ͂ł��܂��� 2021roboSeparateBoardLib_setting.hpp���m�F���Ă�������")
	#endif
	extern aKMD motor5;
#elif defined(ENABLE_motor5_AS_ROHMMD)
	extern aRohmMD motor5;
#endif

#ifdef ENABLE_motor6_AS_ROHMMD
	extern aRohmMD motor6;
#endif

#ifdef ENABLE_rotaryEncoder1
extern aRotaryEncoder rotaryEncoder1;
#endif

#ifdef ENABLE_rotaryEncoder2
extern aRotaryEncoder rotaryEncoder2;
#endif

#ifdef ENABLE_imu03a
//mosi miso slck csPin
extern a_imu03a imu03a;
#endif

#ifdef ENABLE_xbeeCore
//tx rx 
extern aXbeeCoreCallback<VAL_xbeeCore_nodeNum> xbeeCore;
#endif

#ifdef ENABLE_switchJ7
extern DigitalIn switchJ7;
#endif

#ifdef ENABLE_switchJ8
extern DigitalIn switchJ8;
#endif

}

#endif
