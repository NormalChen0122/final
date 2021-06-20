#include "mbed.h"
#include "uLCD_4DGL.h"

using namespace std::chrono;
#define CENTER_BASE 1500

PwmOut servo_r(D6); // right
PwmOut servo_l(D5); // left

// BufferedSerial pc(USBTX, USBRX);
BufferedSerial uart(D1,D0); //tx,rx
DigitalInOut ping(D12);
uLCD_4DGL uLCD(A1, A0, D2);
BufferedSerial xbee(D10, D9);

Timer t;

void scan_the_QRcode(void);
void servo_control(int r_speed, int l_speed);
void follow_the_line(void);
void see_April_tag(void);


int main() {
//   char str[30];

   int i;
   char str[4];

    servo_r.period_ms(20); 
    servo_l.period_ms(20);
    servo_control(0, 0);
    uart.set_baud(9600);
    xbee.set_baud(9600);

   char buf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
      if (buf[0] == 'S' && buf[1] == 't') {
         // move forward
         if (buf[3] != '0') {
            str[0] = buf[3];
            str[1] = buf[4];
            str[2] = '\0';
         }
         else {
            str[0] = buf[4];
            str[1] = '\0';
         }
         i = atoi(str);
         servo_control(-32.2775, 27.9129); // 6cm
         wait_us(i * 1000000 / 6.0);
         servo_control(0, 0); // 6cm
         break;
      }
   }

    scan_the_QRcode();
    ThisThread::sleep_for(2s);
    follow_the_line();

    ThisThread::sleep_for(2s);
    see_April_tag();
   return 0;
}

void follow_the_line(void)
{
   // char str[30];

   servo_control(-32.2775, 27.9129); // 6cm
   while(1){
      if(uart.readable()){
         char recv[1];
         uart.read(recv, sizeof(recv));
         if (recv[0] == '1') { // turn right, turn left and trun right
            servo_control(0, 37.1777); // 6cm right, 8 cm left
            ThisThread::sleep_for(400ms);
            servo_control(-32.2775, 27.9129); // 6cm
         }
         else if (recv[0] == '2') { // Pan right, turn right and then trun left
            servo_control(-42.8398, 0); // 8cm right, 6 cm left
            ThisThread::sleep_for(400ms);
            servo_control(-32.2775, 27.9129); // 6cm
         }
         else if (recv[0] == '3') { // turn right
            // servo_control(-32.2775, 37.1777); // 6cm right, 8 cm left
            servo_control(0, 37.1777); // 6cm right, 8 cm left
            ThisThread::sleep_for(200ms);
            servo_control(-32.2775, 27.9129); // 6cm
         }
         else if (recv[0] == '4') { // turn left
            // servo_control(-42.8398, 27.9129); // 8cm right, 6 cm left
            servo_control(-42.8398, 0); // 8cm right, 6 cm left
            ThisThread::sleep_for(200ms);
            servo_control(-32.2775, 27.9129); // 6cm
         }
         else if (recv[0] == '5') { // stop
            servo_control(0, 0);
            return;
         }
      }
   }
   return;
}

void see_April_tag(void)
{
   int i; // looping index
   float val;
   char recv[1];
   char trans[1];
   char str_num[100];
   float num1 = 0;
   float num2 = 0;
   int move_us;
   int move_us2;
   float ang_turn;
   float dis_move;


   servo_r.period_ms(20); 
   servo_l.period_ms(20); 
   servo_control(0, 0);
   move_us = 19.5 * 1000000 / 6;
//   pc.set_baud(9600);
   uart.set_baud(9600);
   if (uart.readable()) {
      uart.read(recv, sizeof(recv));
   }
   if (uart.readable()) {
      uart.read(recv, sizeof(recv));
   }

   while(1) {
      if (uart.readable()) {
         uart.read(recv, sizeof(recv));
         uLCD.text_width(1); //4X size text
         uLCD.text_height(2);
         uLCD.color(RED);
         uLCD.cls();
         uLCD.locate(0,0);
         uLCD.printf("recv[0] = %c\n", recv[0]);
         if (recv[0] == '1') {
            servo_control(-33.4775, 0); // 6cm
            wait_us(290000); // turn left big
            servo_control(0, 0); // 6cm
            trans[0] = '1';
            uart.write(trans, 1);   // return the Ack means ok
            trans[0] = '\n';
            uart.write(trans, 1);
         }
         else if (recv[0] == '2') {
            servo_control(-33.4775, 0); // 6cm
            wait_us(150000); // turn left big
            servo_control(0, 0); // 6cm
            trans[0] = '1';
            uart.write(trans, 1);   // return the Ack means ok
            trans[0] = '\n';
            uart.write(trans, 1);
         }
         else if (recv[0] == '3') {
            servo_control(0, 27.9129); // 6cm
            wait_us(290000); // turn left big
            servo_control(0, 0); // 6cm
            trans[0] = '1';
            uart.write(trans, 1);   // return the Ack means ok
            trans[0] = '\n';
            uart.write(trans, 1);
         }
         else if (recv[0] == '4') {
            servo_control(0, 27.9129); // 6cm
            wait_us(150000); // turn left big
            servo_control(0, 0); // 6cm
            trans[0] = '1';
            uart.write(trans, 1);   // return the Ack means ok
            trans[0] = '\n';
            uart.write(trans, 1);
         }
         else if (recv[0] == '5') {
            i = 0;
            while (1) {
               if(uart.readable()){
                  uart.read(recv, sizeof(recv));
                  if (recv[0] == '\n') {
                     str_num[i] = '\0';
                     uLCD.locate(0,4);
                     uLCD.printf("I am break");
                     break;
                  }
                  str_num[i] = recv[0];
                  i++;
               }
            }
            num1 = atof(str_num);
            uLCD.text_width(1); //4X size text
            uLCD.text_height(2);
            uLCD.color(RED);
            uLCD.locate(0,6);
            uLCD.printf(str_num);
            uLCD.locate(0,2);
            if (num1 != 0) uLCD.printf("Tx = %8.6lf\r\n", num1);
            move_us = num1 / 6.0 * 1000000;
            servo_control(-33.4775, 27.9129); // 6cm
            wait_us(move_us);
            servo_control(0, 0);
            trans[0] = '1';
            uart.write(trans, 1);   // return the Ack means ok
            trans[0] = '\n';
            uart.write(trans, 1);
         }
         else if (recv[0] == '6') {
            i = 0;
            while (1) {
               if(uart.readable()){
                  uart.read(recv, sizeof(recv));
                  if (recv[0] == '\n') {
                     str_num[i] = '\0';
                     uLCD.locate(0,4);
                     uLCD.printf("I am break");
                     break;
                  }
                  str_num[i] = recv[0];
                  i++;
               }
            }
            num1 = atof(str_num);

            i = 0;
            while (1) {
               if(uart.readable()){
                  uart.read(recv, sizeof(recv));
                  if (recv[0] == '\n') {
                     str_num[i] = '\0';
                     uLCD.locate(0,4);
                     uLCD.printf("I am break");
                     break;
                  }
                  str_num[i] = recv[0];
                  i++;
               }
            }
            num2 = atof(str_num);

            uLCD.text_width(1); //4X size text
            uLCD.text_height(2);
            uLCD.color(RED);
            uLCD.cls();
            uLCD.locate(0,4);
            uLCD.printf("num1 = %f", num1);
            uLCD.locate(0,6);
            uLCD.printf("num2 = %f", num2);

            // num1 is total_dis
            // num2 is degree Ry
            if (num2 < 180) {
               // first turn right
               ang_turn = 90 - num2;
               move_us = 3000000.0 * ang_turn / 90;

               // dis_move = total_dis*sin(ang) - le_to_c - ri_to_c*cos(ang)
               num2 = num2 * 3.1415926 / 180;
               dis_move = num1 * sin(num2) - 6.5 - 4.3 * cos(num2);
               if (dis_move > 0) {
                  move_us2 = dis_move * 1000000 / 6;
               }
               else {
                  move_us2 = -dis_move * 1000000 / 6;
               }
               
               servo_control(0, 27.9129); // 6cm
               wait_us(move_us); // turn right 90 - num2 degree
               servo_control(0, 0);
               wait_us(500000);

               if (dis_move > 0) {
                  servo_control(-33.4775, 27.9129);
                  wait_us(move_us2); // move forward
               }
               else {
                  servo_control(28.7020, -34); // back 6cm
                  wait_us(move_us2); // move backward
               }
               servo_control(-35.4775, 0); // 6cm
               wait_us(2900000); // turn left 90 degree
               servo_control(0, 0);
               trans[0] = '1';
               uart.write(trans, 1);   // return the Ack means ok
               trans[0] = '\n';
               uart.write(trans, 1);
            }
            else { // num2 > 180
               num2 = (num2 - 360) * (-1);
               // first turn left
               ang_turn = 90 - num2;
               move_us = 2900000.0 * ang_turn / 90;

               num2 = num2 * 3.1415926 / 180;

               dis_move = num1 * sin(num2) - 4.3 - 6.5 * cos(num2);
               if (dis_move > 0) {
                  move_us2 = dis_move * 1000000 / 6;
               }
               else {
                  move_us2 = -dis_move * 1000000 / 6;
               }

               servo_control(-33.4775, 0); // 6cm
               wait_us(move_us); // turn left 90 - num2 degree
               servo_control(0, 0);
               wait_us(500000);

               if (dis_move > 0) {
                  servo_control(-33.4775, 27.9129);
                  wait_us(move_us2); // move forward
               }
               else {
                  servo_control(28.7020, -34); // back 6cm
                  wait_us(move_us2); // move backward
               }
               servo_control(0, 29.9129); // 6cm
               wait_us(3000000); // turn right 90 degree
               servo_control(0, 0);
               trans[0] = '1';
               uart.write(trans, 1);   // return the Ack means ok
               trans[0] = '\n';
               uart.write(trans, 1);
            }
         }
         else if (recv[0] == '7') {
            servo_control(28.7020, -34); // back 6cm
            wait_us(1000000); // move backward
            trans[0] = '1';
            uart.write(trans, 1);   // return the Ack means ok
            trans[0] = '\n';
            uart.write(trans, 1);
         }
         else if (recv[0] == '8') { // tag 0 success
            i = 0;
            while (1) {
               if(uart.readable()){
                  uart.read(recv, sizeof(recv));
                  if (recv[0] == '\n') {
                     str_num[i] = '\0';
                     uLCD.locate(0,4);
                     uLCD.printf("I am break");
                     break;
                  }
                  str_num[i] = recv[0];
                  i++;
               }
            }
            num1 = atof(str_num);

            i = 0;
            while (1) {
               if(uart.readable()){
                  uart.read(recv, sizeof(recv));
                  if (recv[0] == '\n') {
                     str_num[i] = '\0';
                     uLCD.locate(0,4);
                     uLCD.printf("I am break");
                     break;
                  }
                  str_num[i] = recv[0];
                  i++;
               }
            }
            num2 = atof(str_num);
            
            ping.output();
            ping = 0; wait_us(200);
            ping = 1; wait_us(5);
            ping = 0; wait_us(5);

            ping.input();
            while(ping.read() == 0);
            t.start();
            while(ping.read() == 1); // count how long the ping == 1
            val = t.read();
            uLCD.cls();
            uLCD.text_width(1); //4X size text
            uLCD.text_height(2);
            uLCD.color(GREEN);
            uLCD.locate(0,2);
            uLCD.printf("Ping = %8.6lf\r\n", val*17700.4f);
            uLCD.locate(0,4);
            uLCD.printf("ang = %8.6lf deg\r\n", num2);
            uLCD.locate(0,6);
            uLCD.color(GREEN);
            uLCD.printf("SUCCESS");

            t.stop();
            t.reset();
            // turn to the right angle
            if (num2 > 270 && num2 < 358) {
                // turn right
               ang_turn = 360 - num2;
               move_us = 3000000.0 * ang_turn / 90;
               servo_control(0, 27.9129); // 6cm
               wait_us(move_us); // turn right 90 - num2 degree
               servo_control(0, 0);
            }
            else if (num2 < 90 && num2 > 2) {
               // turn left
               ang_turn = 90 - num2;
               move_us = 2900000.0 * ang_turn / 90;
               servo_control(-33.4775, 0); // 6cm
               wait_us(move_us); // turn left 90 - num2 degree
               servo_control(0, 0);
            }
            // move back
            servo_control(28.7020, -34); // back 6cm
            move_us = 1000000.0 * (30 - val * 17700.4f) / 6;
            wait_us(move_us); // move backward
            servo_control(29.7020, 0); // back turn right 6cm
            ThisThread::sleep_for(2850ms);
            servo_control(0, 0);
            trans[0] = '1';
            uart.write(trans, 1);   // return the Ack means ok
            trans[0] = '\n';
            uart.write(trans, 1);
         }
         else if (recv[0] == '9') { // else tag success
            i = 0;
            while (1) {
               if(uart.readable()){
                  uart.read(recv, sizeof(recv));
                  if (recv[0] == '\n') {
                     str_num[i] = '\0';
                     uLCD.locate(0,4);
                     uLCD.printf("I am break");
                     break;
                  }
                  str_num[i] = recv[0];
                  i++;
               }
            }
            num1 = atof(str_num);

            i = 0;
            while (1) {
               if(uart.readable()){
                  uart.read(recv, sizeof(recv));
                  if (recv[0] == '\n') {
                     str_num[i] = '\0';
                     uLCD.locate(0,4);
                     uLCD.printf("I am break");
                     break;
                  }
                  str_num[i] = recv[0];
                  i++;
               }
            }
            num2 = atof(str_num);
            
            ping.output();
            ping = 0; wait_us(200);
            ping = 1; wait_us(5);
            ping = 0; wait_us(5);

            ping.input();
            while(ping.read() == 0);
            t.start();
            while(ping.read() == 1); // count how long the ping == 1
            val = t.read();
            uLCD.cls();
            uLCD.text_width(1); //4X size text
            uLCD.text_height(2);
            uLCD.color(GREEN);
            uLCD.locate(0,2);
            uLCD.printf("Ping = %8.6lf\r\n", val*17700.4f);
            uLCD.locate(0,4);
            uLCD.printf("ang = %8.6lf deg\r\n", num2);
            uLCD.locate(0,6);
            uLCD.color(GREEN);
            uLCD.printf("SUCCESS");

            t.stop();
            t.reset();

            trans[0] = '1';
            uart.write(trans, 1);   // return the Ack means ok
            trans[0] = '\n';
            uart.write(trans, 1);

            // turn to the right angle
            if (num2 > 270 && num2 < 358) {
                // turn right
               ang_turn = 360 - num2;
               move_us = 3000000.0 * ang_turn / 90;
               servo_control(0, 27.9129); // 6cm
               wait_us(move_us); // turn right 90 - num2 degree
               servo_control(0, 0);
            }
            else if (num2 < 90 && num2 > 2) {
               // turn left
               ang_turn = 90 - num2;
               move_us = 2900000.0 * ang_turn / 90;
               servo_control(-33.4775, 0); // 6cm
               wait_us(move_us); // turn left 90 - num2 degree
               servo_control(0, 0);
            }

            // go back
            move_us = (46 - val*17700.4f) * 1000000 / 6;
            servo_control(28.7020, -34); // back 6cm
            wait_us(move_us);
            servo_control(0, 0); // stop
            return;
         }
      }
      servo_control(0, 0); // 6cm
   }
   return;
}

void servo_control(int r_speed, int l_speed) {
   if (r_speed > 200)       r_speed = 200;
   else if (r_speed < -200) r_speed = -200;
   if (l_speed > 200)       l_speed = 200;
   else if (l_speed < -200) l_speed = -200;

   servo_r = (CENTER_BASE + r_speed)/20000.0f;
   servo_l = (CENTER_BASE + l_speed)/20000.0f;
}

void scan_the_QRcode(void)
{
    int i; // looping index
    char str[20];
    char recv[1];
    char trans[1];

    i = 0;
    while (1) {    
        if(uart.readable()){
            uart.read(recv, sizeof(recv));
            if (recv[0] == '\n') {
                str[i] = '\0';
                i = 0;

                uLCD.locate(0,4);
                uLCD.printf("I am break");
                uLCD.cls();
                uLCD.locate(0,0);
                uLCD.printf("%s", str);

            }
            else {
                str[i] = recv[0];
                i++;
            }
        }
        if (str[0] == 'y' && str[1] == 'e' && str[2] == 's') {
            servo_control(29.7020, 0); // back 6cm
            ThisThread::sleep_for(2850ms);
            servo_control(0, 0); // back 6cm
            break;
        }
    }
    trans[0] = '1';
    uart.write(trans, 1);   // return the Ack means ok
    trans[0] = '\n';
    uart.write(trans, 1);

    return;
}
