#include <wrapper_header.h>
#include <Servo.h>

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

// blinker key
char auth[] = "564662e3dbb5";
// wifi 名称
char ssid[] = "9-1-10";
// wifi 密码
char pswd[] = "qw123123";

// 新建组件对象
BlinkerButton button_switch_light((char *)"switch-light");
BlinkerButton button_yellow_light((char *)"yellow_light");
BlinkerSlider slider_light((char *)"ran-servo");

// 舵机信号
int LIGHT = D2;
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
  BLINKER_LOG("get yellow light state:", state);
  int range = myservo.read();
  button_yellow_light.print(range == CLOSE_RANGE ? "off" : "on");

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
  BLINKER_LOG("get switch state: ", state);
  int range = myservo.read();
  button_switch_light.print(range == CLOSE_RANGE ? "off" : "on");

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
  BLINKER_LOG("need set power state: ", state);
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
  BLINKER_LOG("get slider value: ", value); // 获取滑动条数值(0~180)
  myservo.write(value);                     // 将value的值写入舵机 作者：很蓝的蛙 https://www.bilibili.com/read/cv16264450/ 出处：bilibili
}

void setup()
{
  Serial.begin(115200);

  // BLINKER_DEBUG.debugAll();

  myservo.attach(LIGHT); // attaches the servo on GIO2 to the servo object
  close_light();

  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  button_switch_light.attach(button_switch_light_handle);
  button_yellow_light.attach(button_yellow_light_handle);
  BlinkerMIOT.attachPowerState(miotPowerState);
  slider_light.attach(slider_handler);
}

void loop()
{
  Blinker.run();
  button_switch_light.print(myservo.read() == CLOSE_RANGE ? "off" : "on");
  // button_yellow_light.print(myservo.read() == CLOSE_RANGE ? "off" : "on");
  // slider_light.print(myservo.read());
  delay(2);
}