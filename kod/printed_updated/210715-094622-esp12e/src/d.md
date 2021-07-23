<html>
<body>
<h1>NETIO button tutorial</h1>
<h2>Basic info</h2>
<p>
This is prototype of NETIO button that allow you to control the NETIO socket.
It works on sending HTTP request method.
On the other side, this communication is handled by the socket via JSON API. Button is made with small consumption. Button is only up when user sends HTTP request or turns on conf-mode.
</p>
<h3>Device structure</h3>
<p>
Device has 4 buttons.<br>
First button next to 5 programming pins is used for <b>programming.</b> <br>
Two buttons with S1 and S2 marks are used for <b>JSON configure</b>.
User can program those buttons via conf-mode. <br>
Last button is <b>RESET</b>. It is used for restart the device.<br>
</p>
<h3>LED and buzzer signaling</h3>
<p>
Device has LED with green and red color.
Buzzer is connected with red color of LED.<br>
If <b>green light</b> is on, device is trying to connect to wlan or communicate with the socket. <br>
If <b>red light</b> blinks once for longer time, device entered or quited conf-mode.<br> 
If <b>red light</b> blinks 2x with buzzer signalization, connection to wlan failed. <br>
If <b>red light</b> blinks 3x with buzzer signalization, HTTP communication failed.
Probably IP address of the socket is invalid. <br>
If <b>red light</b> blinks 4x with buzzer signalization, device wasn't able to recognize input. Please hold the button little bit longer.
</p>
<h3>Control</h3>
<p>
With each button user can execute command for selected socket. For entering conf-mode, hold both buttons until light turn on for 400ms.

</p>

<h6>
sviti zelene - komunikuje
3x cervena selhala komunikace kratce
3x dlouze 
</body>
</html>