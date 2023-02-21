#include <wrapper_header.h>
#include <Servo.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

const uint16_t kIrLed = D2;

IRsend irsend(kIrLed); 

// blinker key
char auth[] = "564662e3dbb5";
// wifi 名称
char ssid[] = "9-1-10";
// wifi 密码
char pswd[] = "qw123123";

// 新建组件对象
BlinkerButton button_switch_light((char *)"switch-light");
BlinkerButton button_yellow_light((char *)"yellow_light");
BlinkerButton button_open_kongtiao((char *)"open_kongtiao");
BlinkerButton button_close_kongtiao((char *)"close_kongtiao");
BlinkerSlider slider_light((char *)"ran-servo");

// 舵机信号
int LIGHT = D3;
int OPEN_RANGE = 160;
int CLOSE_RANGE = 25;

void close_light()
{
  myservo.write(CLOSE_RANGE);
}

void open_light()
{
  myservo.write(OPEN_RANGE);
}

// 黄灯跳转
void button_yellow_light_handle(const String &state)
{
  int range = myservo.read();

  // 因为黄灯需要进行两次开关
  if (range == CLOSE_RANGE)
  {
    open_light();
    delay(1000);
    close_light();
    delay(1000);
    open_light();
    delay(1000);
  }
  else
  {
    close_light();
    delay(1000);
    open_light();
    delay(1000);
  }
}

// 按下按键即会执行该函数
void button_switch_light_handle(const String &state)
{
  int range = myservo.read();
  if (range == CLOSE_RANGE)
  {
    open_light();
  }
  else
  {
    close_light();
  }
}

// 小爱同学
void miotPowerState(const String &state)
{
  int range = myservo.read();

  if (state == BLINKER_CMD_ON)
  {
    // 打开黄灯
    if (range == CLOSE_RANGE)
    {
      open_light();
      delay(1000);
      close_light();
      delay(1000);
      open_light();
      delay(1000);
    }
    else
    {
      close_light();
      delay(1000);
      open_light();
      delay(1000);
    }
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
  }
  else if (state == BLINKER_CMD_OFF)
  {
    close_light();
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
  }
}

// 滑块控制舵机
void slider_handler(int32_t value)
{
  myservo.write(value);                     // 将value的值写入舵机 作者：很蓝的蛙 https://www.bilibili.com/read/cv16264450/ 出处：bilibili
}

// 空调数据
uint16_t KT_CLOSE[371] = {8994, 4416,  576, 522,  576, 520,  576, 520,  576, 522,  576, 522,  574, 1646,  576, 1646,  576, 522,  576, 522,  576, 522,  576, 522,  576, 1646,  576, 1646,  576, 1646,  576, 522,  576, 522,  574, 1648,  574, 1646,  576, 522,  576, 522,  576, 1646,  574, 522,  574, 522,  576, 522,  576, 522,  574, 522,  576, 522,  576, 522,  576, 1646,  574, 522,  576, 522,  576, 520,  576, 522,  576, 1646,  574, 522,  576, 1646,  574, 522,  576, 522,  576, 522,  576, 1646,  576, 522,  574, 522,  576, 522,  574, 524,  574, 522,  576, 522,  576, 522,  574, 522,  576, 522,  576, 522,  576, 1646,  576, 522,  576, 522,  574, 1648,  574, 1648,  574, 522,  574, 524,  574, 522,  574, 522,  574, 524,  574, 524,  574, 522,  574, 1648,  574, 522,  574, 1648,  574, 1648,  574, 522,  574, 1648,  574, 1648,  574, 524,  574, 522,  574, 1648,  574, 522,  574, 522,  574, 522,  574, 522,  574, 524,  574, 524,  574, 524,  574, 524,  574, 524,  574, 524,  574, 522,  574, 522,  574, 524,  574, 524,  574, 524,  574, 522,  574, 524,  572, 548,  550, 548,  548, 548,  548, 548,  548, 550,  548, 524,  574, 548,  550, 548,  550, 548,  550, 548,  550, 548,  548, 550,  548, 548,  548, 548,  550, 548,  550, 548,  548, 548,  548, 550,  548, 548,  548, 548,  548, 550,  548, 550,  548, 548,  548, 548,  548, 548,  548, 548,  548, 550,  548, 548,  548, 548,  548, 548,  548, 548,  548, 548,  548, 550,  548, 550,  548, 548,  548, 548,  548, 550,  548, 550,  548, 1676,  546, 548,  548, 1674,  548, 548,  548, 550,  546, 1674,  548, 550,  548, 550,  548, 1674,  548, 1674,  524, 1698,  524, 572,  548, 550,  524, 572,  524, 1698,  524, 1698,  548, 550,  524, 574,  524, 572,  524, 574,  522, 574,  524, 574,  522, 574,  524, 574,  524, 574,  524, 1698,  524, 574,  524, 1698,  522, 574,  524, 572,  524, 1698,  524, 574,  524, 574,  524, 572,  524, 574,  524, 574,  524, 574,  524, 574,  524, 574,  524, 574,  522, 574,  522, 574,  524, 574,  524, 574,  524, 574,  524, 574,  524, 574,  524, 574,  522, 574,  522, 1700,  522, 574,  522, 574,  522, 574,  522, 1700,  522, 1700,  522, 1700,  522, 576,  522};

uint16_t KT_OPEN[371] = {8994, 4416,  574, 522,  576, 522,  576, 522,  574, 522,  576, 522,  576, 1648,  574, 1648,  576, 522,  576, 522,  576, 522,  576, 522,  576, 1646,  576, 1646,  576, 1646,  576, 522,  576, 522,  574, 1648,  574, 1648,  576, 522,  576, 522,  576, 1646,  576, 522,  576, 522,  576, 522,  576, 1648,  574, 522,  576, 522,  574, 522,  574, 1648,  574, 522,  576, 522,  576, 522,  574, 522,  574, 1648,  576, 522,  574, 1648,  574, 522,  576, 522,  574, 522,  574, 1648,  574, 522,  574, 524,  574, 522,  574, 522,  576, 522,  574, 524,  574, 522,  574, 522,  576, 522,  574, 522,  574, 1648,  576, 522,  574, 522,  574, 1648,  574, 1648,  576, 522,  574, 522,  574, 524,  574, 522,  574, 522,  576, 522,  576, 522,  574, 1648,  574, 522,  574, 522,  574, 1648,  574, 524,  574, 1648,  574, 522,  574, 1648,  574, 524,  574, 1648,  574, 524,  574, 524,  574, 524,  574, 524,  574, 522,  574, 524,  574, 524,  574, 524,  574, 524,  574, 524,  574, 522,  574, 524,  572, 524,  574, 524,  572, 524,  574, 524,  574, 524,  572, 524,  572, 548,  548, 524,  574, 524,  574, 524,  572, 524,  574, 524,  572, 548,  548, 548,  548, 548,  550, 548,  550, 548,  548, 548,  550, 548,  550, 548,  548, 548,  548, 548,  548, 550,  548, 548,  550, 548,  548, 548,  548, 548,  548, 550,  548, 550,  548, 548,  548, 550,  548, 548,  548, 548,  548, 548,  548, 548,  548, 550,  548, 548,  548, 548,  548, 550,  548, 548,  548, 548,  548, 550,  548, 548,  548, 1674,  548, 550,  548, 1674,  548, 550,  548, 548,  548, 1674,  548, 550,  548, 550,  548, 1674,  548, 1674,  548, 1676,  548, 550,  548, 550,  548, 550,  546, 1676,  548, 1674,  546, 550,  546, 550,  546, 550,  548, 552,  546, 550,  546, 550,  546, 550,  546, 550,  522, 574,  548, 1674,  524, 572,  524, 1700,  522, 574,  522, 574,  524, 1698,  522, 574,  524, 574,  522, 574,  522, 574,  524, 574,  522, 574,  524, 574,  524, 574,  524, 574,  524, 574,  524, 574,  522, 574,  522, 574,  522, 574,  524, 574,  522, 574,  522, 574,  522, 576,  522, 1700,  522, 574,  522, 576,  522, 576,  522, 574,  524, 574,  522, 1700,  522, 576,  522};

void close_kongtiao() {
  irsend.sendRaw(KT_CLOSE, 371, 38); 
  Serial.println("kt_close");
}

void open_kongtiao() {
  irsend.sendRaw(KT_OPEN, 371, 38); 
  Serial.println("kt_open");
}

void close_kongtiao_handler(const String &state) {
  close_kongtiao();
}

void open_kongtiao_handler(const String &state) {
  open_kongtiao();
}

void setup()
{
  Serial.begin(115200);

  irsend.begin();

  // BLINKER_DEBUG.debugAll();

  myservo.attach(LIGHT); // attaches the servo on GIO2 to the servo object
  close_light();

  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  button_switch_light.attach(button_switch_light_handle);
  button_yellow_light.attach(button_yellow_light_handle);
  button_open_kongtiao.attach(close_kongtiao_handler);
  button_close_kongtiao.attach(open_kongtiao_handler);
  BlinkerMIOT.attachPowerState(miotPowerState);
  slider_light.attach(slider_handler);
}

void loop()
{
  Blinker.run();
  button_switch_light.print(myservo.read() == CLOSE_RANGE ? "off" : "on");
  
  // irsend.sendRaw(KT_CLOSE, 371, 38); 
  // button_yellow_light.print(myservo.read() == CLOSE_RANGE ? "off" : "on");
  // slider_light.print(myservo.read());
  delay(2);
}