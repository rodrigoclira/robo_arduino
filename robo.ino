/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "robo";
const char* password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;


void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            Serial.println(header.indexOf("\n"));

            String line = pegarMovimentos(header); 
            int fim;  
            int index_prox_virgula = line.indexOf(",");
            String comando; 
            if (index_prox_virgula != -1){

              String currentString = line;
              do{
                  comando = currentString.substring(0, index_prox_virgula);
                  Serial.println(comando);
                  fim = currentString.length();
                  currentString = currentString.substring(index_prox_virgula+1, fim);
                  //Serial.println(currentString);
                  index_prox_virgula = currentString.indexOf(",");
                  //Serial.println(index_prox_virgula);

              }while(index_prox_virgula != -1);
              
            }
            
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html><head></head>");            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}


String pegarMovimentos(String header){

  int index = header.indexOf("\n");
  String line = header.substring(0, index - 1);
  index = line.indexOf("HTTP");
  int index2 = line.indexOf("=");
  line = line.substring(index2+1, index - 1);
  Serial.println(line);

  return line;
}
