#include <Servo.h>
#include <HCSR04.h>
HCSR04 hc(5, 4);
Servo pick;
Servo Hight;
Servo Length;
Servo Angle;
int p = 0;
int h = 60;
int l = 20;
int a = 180;
#define REMOTEXY_MODE__ESP8266WIFI_LIB_CLOUD
#include <ESP8266WiFi.h>
#include <RemoteXY.h>
// RemoteXY connection settings
#define REMOTEXY_WIFI_SSID "resh" 
#define REMOTEXY_WIFI_PASSWORD "*******" 
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com" 
#define REMOTEXY_CLOUD_PORT 6376
#define REMOTEXY_CLOUD_TOKEN
"33d7540b2b63d03294d5e83c9e63989b" 
#define REMOTEXY_ACCESS_PASSWORD "resh1234"
// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = // 70 bytes
{ 255,5,0,4,0,63,0,16,27,2,3,3,89,3,8,22,48,10,8,22, 205,26,5,0,11,31,30,30,4,76,21,21,24,26,31,5,0,64,33,30, 30,37,75,22,22,24,26,31,66,131,41,8,16,7,24,25,16,7,2,24, 65,112,8,5,9,9,6,9,9,9 };
// this structure defines all the variables and events of your control interface
struct {
uint8_t select_1; // =0 if select position A, =1 if position B, =2 if position C, ... 
int8_t joystick_1_x; // from -100 to 100
int8_t joystick_1_y; // from -100 to 100
int8_t joystick_2_x; // from -100 to 100
int8_t joystick_2_y; // from -100 to 100
// output variables
int8_t level_1; // =0..100 level position
uint8_t led_2_r; // =0..255 LED Red brightness
uint8_t led_2_g; // =0..255 LED Green brightness
uint8_t led_2_b; // =0..255 LED Blue brightness
// other variable
uint8_t connect_flag; // =1 if wire connected, else =0
} RemoteXY;
#pragma pack(pop)
void setup() {
RemoteXY_Init ();
pick.attach(14);
Hight.attach(12);
Length.attach(13);
Angle.attach(2);
/*
pick.attach(2);
Hight.attach(3);
Length.attach(4);
Angle.attach(5);
*/
pick.write(180);
delay(100);
Length.write(20);
delay(100);
Hight.write(180);
delay(100);
Angle.write(180);
delay(100);
}
void loop() {

RemoteXY_Handler ();
if(RemoteXY.select_1==1){
Default();
}
if(RemoteXY.select_1==2){
detect();
}
if(RemoteXY.select_1==0){
manual();
}
}
void manual(){
if(RemoteXY.joystick_1_x>80){
if(a<=180);{
a=a+1;
Angle.write(a);
}}
if(RemoteXY.joystick_1_x<-80){
if(a>=0){
a=a-1;
Angle.write(a);
}}
if(RemoteXY.joystick_1_y>80){
if(h<=180){
h=h+1;
Hight.write(h);
}}
if(RemoteXY.joystick_1_y<-80){
if(h>=0){
h=h-1;
Hight.write(h);
}}
if(RemoteXY.joystick_2_x>80){
if(p<=180);{
p=p+1;
pick.write(p);
}}
if(RemoteXY.joystick_2_x<-80){
if(p>=0){
p=p-1;
pick.write(p);
}}
if(RemoteXY.joystick_2_y>80){
if(l<=180){
l=l+1;
Length.write(l);
}}
if(RemoteXY.joystick_2_y<-80){
if(l>=20){
l=l-1;
Length.write(l);
}}
}
void detect(){
delay(500);
if(RemoteXY.select_1==1){
Default();
}
if(RemoteXY.select_1==0){
manual();
}
int dis = hc.dist();
while (a >= 20) { // goes from 180 degrees to 0 degrees
a -= 1;
dis = hc.dist();
RemoteXY.level_1=dis;
if((dis>10)&&(dis<13)) {
goto end;
}
Angle.write(a); // tell servo to go to position in variable 'pos' 
delay(25); // waits 15 ms for the servo to reach the position
}
while (a <= 180) { // goes from 180 degrees to 0 degrees
a += 1;
dis = hc.dist();
RemoteXY.level_1=dis;
if((dis>10)&&(dis<13)) {
goto end;
}
Angle.write(a); // tell servo to go to position in variable 'pos' 
delay(25); // waits 15 ms for the servo to reach the position
}
end:
if((dis>10)&&(dis<13))
{
Angle.write(a-15); // tell servo to go to position in variable 'pos' 
delay(25);
take();
dispose();
}
}
void take(){
for (h = 180; h >= 40; h -= 1) { // goes from 180 degrees to 0 degrees
RemoteXY.led_2_r=255;
Hight.write(h); // tell servo to go to position in variable 'pos' 
delay(25); // waits 15 ms for the servo to reach the position
}
for (l = 20; l <= 120; l += 1) { // goes from 0 degrees to 180 degrees
// in steps of 1 degree
RemoteXY.led_2_g=255;
Length.write(l); // tell servo to go to position in variable 'pos'

delay(15); // waits 15 ms for the servo to reach the position
}
for (p = 180; p >= 0; p -= 1) { // goes from 180 degrees to 0 degrees
RemoteXY.led_2_b=255;
pick.write(p); // tell servo to go to position in variable 'pos' 
delay(25);}
for (l = 120; l >= 20; l -= 1) { // goes from 180 degrees to 0 degrees
RemoteXY.led_2_g=255;
Length.write(l); // tell servo to go to position in variable 'pos' 
delay(25); // waits 15 ms for the servo to reach the position
}
for (h = 40; h <= 180; h += 1) { // goes from 0 degrees to 180 degrees
// in steps of 1 degree
RemoteXY.led_2_r=255;
Hight.write(h); // tell servo to go to position in variable 'pos' 
delay(15); // waits 15 ms for the servo to reach the position
}
}
void dispose(){
while ( a >= 0 ) { // goes from 180 degrees to 0 degrees
a -= 1;
RemoteXY.led_2_r=255;
RemoteXY.led_2_g=255;
RemoteXY.led_2_b=255;
Angle.write(a); // tell servo to go to position in variable 'pos'
delay(25); // waits 15 ms for the servo to reach the position
}
for (h = 180; h >= 40; h -= 1) { // goes from 180 degrees to 0 degrees
RemoteXY.led_2_r=255;
Hight.write(h); // tell servo to go to position in variable 'pos' 
delay(25); // waits 15 ms for the servo to reach the position
}
for (l = 20; l <= 120; l += 1) { // goes from 0 degrees to 180 degrees
// in steps of 1 degree
RemoteXY.led_2_g=255;
Length.write(l); // tell servo to go to position in variable 'pos' 
delay(15); // waits 15 ms for the servo to reach the position
}
for (p = 0; p <= 180; p += 1) { // goes from 0 degrees to 180 degrees
// in steps of 1 degree
RemoteXY.led_2_b=255;
pick.write(p); // tell servo to go to position in variable 'pos' 
delay(15);}
for (l = 120; l >= 20; l -= 1) { // goes from 180 degrees to 0 degrees
RemoteXY.led_2_g=255;
Length.write(l); // tell servo to go to position in variable 'pos' 
delay(25); // waits 15 ms for the servo to reach the position
}
for (h = 40; h <= 180; h += 1) { // goes from 0 degrees to 180 degrees
Hight.write(h); // tell servo to go to position in variable 'pos' 
delay(15); // waits 15 ms for the servo to reach the position
}
while ( a <= 180 ) { // goes from 180 degrees to 0 degrees
a += 1;
Angle.write(a); // tell servo to go to position in variable 'pos' 
delay(25);}
}
void Default(){
if(RemoteXY.select_1==2){
detect();
}
if(RemoteXY.select_1==0){
manual();
}
RemoteXY_Handler ();
for (p = 0; p <= 180; p += 1) { // goes from 0 degrees to 180 degrees
// in steps of 1 degree
RemoteXY.led_2_b=255;
pick.write(p); // tell servo to go to position in variable 'pos' 
delay(15);}
for (l = 120; l >= 20; l -= 1) { // goes from 180 degrees to 0 degrees
RemoteXY.led_2_g=255;
Length.write(l); // tell servo to go to position in variable 'pos'

delay(25); // waits 15 ms for the servo to reach the position
}
for (h = 40; h <= 180; h += 1) { // goes from 0 degrees to 180 degrees
// in steps of 1 degree
Hight.write(h); // tell servo to go to position in variable 'pos' 
delay(15); // waits 15 ms for the servo to reach the position
}
for (a = 0; a <= 180; a += 1) { // goes from 0 degrees to 180 degrees
RemoteXY.led_2_r=255;
RemoteXY.led_2_g=255;
RemoteXY.led_2_b=255;
// in steps of 1 degree
Angle.write(a); // tell servo to go to position in variable 'pos' 
delay(15); }
for (h = 180; h >= 40; h -= 1) { // goes from 180 degrees to 0 degrees
Hight.write(h); // tell servo to go to position in variable 'pos' 
delay(25); // waits 15 ms for the servo to reach the position
}
for (l = 20; l <= 120; l += 1) { // goes from 0 degrees to 180 degrees
// in steps of 1 degree
RemoteXY.led_2_g=255;
Length.write(l); // tell servo to go to position in variable 'pos' 
delay(15); // waits 15 ms for the servo to reach the position
}
for (p = 180; p >= 0; p -= 1) { // goes from 180 degrees to 0 degrees
RemoteXY.led_2_b=255;

pick.write(p); // tell servo to go to position in variable 'pos' 
delay(25);}
for (l = 120; l >= 20; l -= 1) { // goes from 180 degrees to 0 degrees
RemoteXY.led_2_g=255;
Length.write(l); // tell servo to go to position in variable 'pos' 
delay(25); // waits 15 ms for the servo to reach the position
}
for (h = 40; h <= 180; h += 1) { // goes from 0 degrees to 180 degrees
// in steps of 1 degree
RemoteXY.led_2_r=255;
Hight.write(h); // tell servo to go to position in variable 'pos' 
delay(15); // waits 15 ms for the servo to reach the position
}
for (a = 180; a >= 0; a -= 1) { // goes from 180 degrees to 0 degrees
RemoteXY.led_2_r=255;
RemoteXY.led_2_g=255;
RemoteXY.led_2_b=255;
Angle.write(a); // tell servo to go to position in variable 'pos' 
delay(25); // waits 15 ms for the servo to reach the position
}
for (h = 180; h >= 40; h -= 1) { // goes from 180 degrees to 0 degrees
RemoteXY.led_2_r=255;
Hight.write(h); // tell servo to go to position in variable 'pos' 
delay(25); // waits 15 ms for the servo to reach the position
}
for (l = 20; l <= 120; l += 1) { // goes from 0 degrees to 180 degrees
RemoteXY.led_2_r=255;
// in steps of 1 degree
Length.write(l); // tell servo to go to position in variable 'pos' 
delay(15); // waits 15 ms for the servo to reach the position
}
}
