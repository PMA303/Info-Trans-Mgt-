Test COAP Connection using netcat

Create COAP Get Request for the resrouce /test.hello
echo -n -e \\x40\\x01\\x8e\\x2c\\xb4\\x74\\x65\\x73\\x74\\x05\\x68\\x65\\x6c\\x6c\\x6f\\xc1\\x01 > data.bin

Use NETCAT to send udp binary data to 6lopan device
nc -u fd00::212:4b00:a27:e040 5683 <data.bin

Response from COAP Device
EA
   ���Hello World!




