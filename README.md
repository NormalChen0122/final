# final project
## (1) how to setup and run my program <br>
1. 一開始先將車車組裝起來(就是像課程網站的那樣步驟，在下面的連結)<br>
<https://www.ee.nthu.edu.tw/ee240500/mbed-lab-13-boe-bot-car.html><br>
(注意) 要記得這裡的馬達左輪的訊號線是接到D5，而右輪是接到D6<br>
2. 接下來將xbee給裝在車子上面的白色小板板，然後依照下面的指示來接pin角<br>
xbee的5V接到mbed板子的5V<br>
xbee的GND接到mbed板子的GND<br>
xbee的rx接到mbed板子的D9<br>
xbee的tx接到mbed板子的D10<br>

3. 再來依照lab14的課程網站上面一樣，將openMV的板子裝在車車上(連結如下)<br>
<https://www.ee.nthu.edu.tw/ee240500/mbed-lab-14-machine-vision-with-openmv-h7-plus.html><br>
4. 接下來將依照下面的指示來接pin角<br>
openMV的p5接到D1<br>
openMV的p4接到D0<br>
openMV的reset接到mbed的reset<br>

5. 再來將uLCD display和B_L4S5I_IOT01A相接，角位如下<br>
serial rx接到A0<br>
serial tx接到A1<br>
reset pin接到D2<br>
5V接到5V<br>
GND接到GND<br>
(注意) 要記得這裡的rx, tx, reset不是看uLCD板子上的，而是線上面的<br>

6. 接下來將ping插在車子上的小板板上，然後依照課程網站上面的方式連接(連結如下)<br>
<https://www.ee.nthu.edu.tw/ee240500/mbed-lab-12-servos-encoder-and-ping.html>
(注意) 要記得這裡的sig我是設定接到D12<br>

7. 再來要組建好場地(如下列的圖)
(全場)<br>
![image](https://github.com/NormalChen0122/final/blob/master/pictures/map.jpg)<br><br>
(細節1: 包含yes訊息的QRcode)<br>
![image](https://github.com/NormalChen0122/final/blob/master/pictures/map_QRcode.jpg)<br><br>
(細節2: follow the line的黃線)<br>
![image](https://github.com/NormalChen0122/final/blob/master/pictures/map_yellow_line.jpg)<br><br>
(細節3: Aprial tag tag.id() 0和1的)<br>
![image](https://github.com/NormalChen0122/final/blob/master/pictures/map_A_T.jpg)<br><br>
(細節4: 倒車入庫的車庫))<br>
![image](https://github.com/NormalChen0122/final/blob/master/pictures/map_garage.jpg)<br>

(注意) 要記得這裡停車格的的寬度不能超過車車的寬度4公分<br>

8. 接下來就可以將B_L4S5I_IOT01A和notebook給接起來<br>
9. 然後輸入此command，將code給燒入B_L4S5I_IOT01A(要注意command裡面的那個ee2405v3那個要看自己的mbed-os-build是在哪個資料夾底下，來去做改變)<br>
```sudo mbed compile --source . --source ~/ee2405v3/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM --profile tflite.json -f```

10. 接下來把openMV和notebook連接，將main.py給改成我github放在final/For_openMV裡面的final.py
11. 接下來就可以把USB插頭給接到行動電源上，並且因為老師發的金色的線有兩個頭，所以可以分別插在openMV和mbed上(如下圖)<br>
![image](https://github.com/NormalChen0122/hw4/blob/master/pictures/hw4_2_connect.jpg)<br>

12. 接下來按下B_L4S5I_IOT01A的reset button(也就是在藍色按鈕旁邊的黑色按鈕)，放在面對QRcode的前面(隔著大於23公分的距離)<br>
13. 再來將另一個xbee給連到電腦上
(注意) 在這之前兩個xbee的ATMY和ATDL都要先設定好可以互相溝通了喔<br>

14. 接下來在host的terminal上輸入下列command<br>
```sudo python3 xbee.py```
15. 接下來會需要我們輸入要從PC傳給車車的開始訊號，以及要前進多少的訊號，可以量要前進多少，車車才會距離QRcode 23公分左右<br>
![image](https://github.com/NormalChen0122/final/blob/master/pictures/python_code_execute.png)<br>

## (2) what are the results<br>
因為是會動的，所以看影片會比較快，影片所在的連結如下<br>
<https://drive.google.com/drive/folders/1ZrSTWW2W5u7T4fFd9W4SygHyerIS69CI?usp=sharing>
而因為影片上傳長度有限制沒辦法太長，所以分為兩段影片<br>
