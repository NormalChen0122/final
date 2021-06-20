# Untitled - By: user - 週日 六月 6 2021

import pyb, sensor, image, time, math

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must turn this off to prevent image washout...
clock = time.clock()

uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)
i = 0
while(True):
   clock.tick()
   img = sensor.snapshot()
   img.lens_corr(1.8) # strength of 1.8 is good for the 2.8mm lens.
   for code in img.find_qrcodes():
      i = 1
      img.draw_rectangle(code.rect())
      uart.write(("%s\n"%(code.payload())).encode())
   if (i == 1):
      time.sleep(2)
      Ack1 = uart.read(3)
      print("Ack1 = ", Ack1)
      if (Ack1 == b'1\n'):
         print("success")
         break

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA) # QVGA ==> 320 * 240
sensor.skip_frames(time = 2000)
clock = time.clock()
ROI=(0,0,320,70)
#(x, y, w, h)


count = 0
while(True):
    clock.tick()
    img = sensor.snapshot()
    line = img.get_regression([(71, 100, -128, 127, 30, 127)], roi=ROI)
    if (line):
        img.draw_line(line.line(), color = 127)
        print(line.magnitude())

        if (line.magnitude() > 0):
            print("line", line.line())
            print("img.width() * 4 / 10 = ", img.width() * 45 / 100.0)
            if ((line.line()[0] + line.line()[2]) / 2 < img.width() * 40 / 100.0):
                uart.write(('1').encode()) # turn right
                print("uart", 1)
            elif ((line.line()[0] + line.line()[2]) / 2 > img.width() * 60 / 100.0):
                uart.write(('2').encode()) # turn left
                print("uart", 2)
            elif ((line.line()[0] + line.line()[2]) / 2 < img.width() * 45 / 100.0):
                uart.write(('3').encode()) # turn right a little
                print("uart", 3)
            elif ((line.line()[0] + line.line()[2]) / 2 > img.width() * 55 / 100.0):
                uart.write(('4').encode()) # turn left a little
                print("uart", 4)
        else:
            count = count + 1
            if (count > 1):
                uart.write(('5').encode())
                print("uart", 5)
                count = 0
                break
    else:
        count = count + 1
        if (count > 1):
            uart.write(('5').encode())
            print("I am useful")
            count = 0
            break
    time.sleep_ms(500)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # run out of memory if the resolution is much bigger...
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...
clock = time.clock()

f_x = (2.8 / 3.984) * 160 # find_apriltags defaults to this if not set
f_y = (2.8 / 2.952) * 120 # find_apriltags defaults to this if not set
c_x = 160 * 0.5 # find_apriltags defaults to this if not set (the image.w * 0.5)
c_y = 120 * 0.5 # find_apriltags defaults to this if not set (the image.h * 0.5)
kx = 4 / 2.2
kz = 20 / -8.4
def degrees(radians):
    return (180 * radians) / math.pi

#uart = pyb.UART(3,9600,timeout_char=1000)
#uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)
while(True):
    clock.tick()
    img = sensor.snapshot()
    i = 0 # used to chech if there is tag in the screen
    for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y): # defaults to TAG36H11
        i = 1 # there is tag
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        # The conversion is nearly 6.2cm to 1 -> translation
        print_args = (tag.x_translation(), tag.y_translation(), tag.z_translation(), \
             degrees(tag.x_rotation()), degrees(tag.y_rotation()), degrees(tag.z_rotation()))
        print("Tx: %f, Ty %f, Tz %f, Rx %f, Ry %f, Rz %f" % print_args)
        dx = kx * tag.x_translation()
        dz = kz * tag.z_translation()
        total_dis = (dx**2 + dz**2)**0.5
    if (i == 1): # there is tag in the screen
        if (abs(tag.x_translation()) > 1.5):
            if (tag.x_translation() > 0):
                uart.write(("1").encode())
                print(1)
            else:
                uart.write(("3").encode())
                print(3)
        elif (abs(tag.x_translation()) > 0.8):
            if (tag.x_translation() > 0):
                uart.write(("2").encode())
                print(2)
            else:
                uart.write(("4").encode())
                print(4)
        elif (total_dis > 25):
            uart.write(("5").encode())
            print(5)
            time.sleep(0.3)
            total_dis = total_dis - 20 + 2
            print("total_dis = ", total_dis)
            uart.write(("%5.4f\n"%(total_dis)).encode())
            time.sleep(5)
        elif ((degrees(tag.y_rotation()) > 5 and degrees(tag.y_rotation()) < 90) or (degrees(tag.y_rotation()) < 355 and degrees(tag.y_rotation()) > 270)):
            uart.write(("6").encode())
            print(6)
            time.sleep(0.3)

            uart.write(("%5.4f\n%5.4f\n"%(total_dis, degrees(tag.y_rotation()))).encode())
            print("total_dis = ", total_dis)
            print("Ry = ", degrees(tag.y_rotation()))
            time.sleep(10)
        elif (total_dis < 13):
            uart.write(("7").encode()) # go backwrad
            time.sleep(2)
            print("NO tag")
            Ack1 = uart.read(3)
            print("Ack1 = ", Ack1)
            if (Ack1 == b'1\n'): print("success")
        else:
            if (tag.id() == 0):
                uart.write(("8").encode())
                print(8)
            else:
                uart.write(("9").encode())
                print(9)
            time.sleep(0.3)
            uart.write(("%5.4f\n%5.4f\n"%(total_dis, degrees(tag.y_rotation()))).encode())
            print("total_dis = ", total_dis)
            print("Ry = ", degrees(tag.y_rotation()))
            time.sleep(10)
            if (tag.id() != 0):
                break

        time.sleep(2)
        Ack1 = uart.read(3)
        print("Ack1 = ", Ack1)
        if (Ack1 == b'1\n'): print("success")
    else:
        uart.write(("7").encode()) # go backwrad
        time.sleep(2)
        print("NO tag")
        Ack1 = uart.read(3)
        print("Ack1 = ", Ack1)
        if (Ack1 == b'1\n'): print("success")

