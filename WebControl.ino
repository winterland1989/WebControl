/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

int ledRed = 3;
int ledBlue = 9;
int ledRedBrightness= 128;
int ledBlueBrightness= 128;
String HTTP_req;            // stores the HTTP request
String instruction;
void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  analogWrite(ledRed, ledRedBrightness);  
  analogWrite(ledBlue, ledBlueBrightness);
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        HTTP_req += c;  // save the HTTP request 1 char at a time
        // Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
                     if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: keep-alive");
                    client.println();
                    instruction = HTTP_req.substring(5,10);
                    if( instruction == "setRH"){
                      SetRedH();
                    }else if( instruction == "setRL"  ){
                      SetRedL();
                    }else if( instruction == "setBH"  ){
                      SetBlueH();
                    }else if( instruction == "setBL"  ){
                      SetBlueL();
                    }
                    else {  // HTTP request for web page
                        // send web page - contains JavaScript with AJAX calls
                        client.println("<!DOCTYPE html>");
                        client.println("<html>");
                        client.println("<head>");
                        client.println("<title>Arduino Web Page</title>");
                        client.println("<script>");
                        
                        client.println("function setLed(instr){");
                        client.println("nocache = \"&nocache=\" + Math.random() * 1000000;");
                        client.println("var request = new XMLHttpRequest();");
                        client.println("request.open(\"GET\",instr + nocache , true);");
                        client.println("request.send(null);");
                        client.println("}");

                        client.println("</script>");
                        client.println("</head>");
                        client.println("<body>");
                        client.println("<h1>LED web Control</h1>");
                        client.println(
                        "<p id=\"hint txt\">test</p>");
                        client.println("<p><button type=\"button\" onclick=\"setLed(\'setRH\')\">Set Red Brighter</button></p>");
                        client.println("<p><button type=\"button\" onclick=\"setLed(\'setRL\')\">Set Red dimmer</button></p>");
                        client.println("<p><button type=\"button\" onclick=\"setLed(\'setBH\')\">Set Blue Brighter</button></p>");
                        client.println("<p><button type=\"button\" onclick=\"setLed(\'setBL\')\">Set Blue dimmer</button></p>"); 
                        client.println("</body>");
                        client.println("</html>");
                    }
                    // display received HTTP request on serial port
                    Serial.println("HTTP REQ:");
                    Serial.print(HTTP_req);
                    HTTP_req = "";            // finished with request, empty string
                    break;
                }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }

      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


void SetRedOff()
{
  ledRedBrightness = 0;
    analogWrite(ledRed,ledRedBrightness);
}
void SetRedH()
{
  ledRedBrightness += 40;
  if(ledRedBrightness>255) ledRedBrightness=255;
    analogWrite(ledRed,ledRedBrightness);
}
void SetRedL()
{
  ledRedBrightness -= 40;
  if(ledRedBrightness<0) ledRedBrightness=0;
    analogWrite(ledRed,ledRedBrightness);
}

void SetBlueH()
{
  ledBlueBrightness += 40;
  if(ledBlueBrightness>255) ledBlueBrightness=255;
    analogWrite(ledBlue,ledBlueBrightness);
}
void SetBlueL()
{
  ledBlueBrightness -= 40;
  if(ledBlueBrightness<0) ledBlueBrightness=0;
    analogWrite(ledBlue,ledBlueBrightness);
}

