Following the Token value comes a sequence of zero or more CoAP
   Options in Type-Length-Value (TLV) format, optionally followed by a
   payload that takes up the rest of the datagram.

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Ver| T |  TKL  |      Code     |          Message ID           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |   Token (if any, TKL bytes) ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |   Options (if any) ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |1 1 1 1 1 1 1 1|    Payload (if any) ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                         Figure 7: Message Format



Test COAP Connection using netcat

Create COAP Get Request for the resrouce /test.hello
echo -n -e \\x40\\x01\\x8e\\x2c\\xb4\\x74\\x65\\x73\\x74\\x05\\x68\\x65\\x6c\\x6c\\x6f\\xc1\\x01 > data.bin

Use NETCAT to send udp binary data to 6lopan device
nc -u fd00::212:4b00:a27:e040 5683 <data.bin

Response from COAP Device
EA
   ���Hello World!


To Compile the Java Client, edit the JAVA file to set the correct ipv6 address of the Server. 

javac *.java
java UDPClient






