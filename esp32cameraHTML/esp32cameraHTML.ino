#include "WiFi.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <ESPAsyncWebServer.h>
#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>
#include <string.h>
// Replace with your network credentials
const char* ssid = "HardnessMachineAP";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

boolean takeNewPhoto = false;
//15.044 hor cm per 22 cm away from screen, y = 15.044/22 * x

// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/photo.jpg"

// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define RXp2 16
#define TXp2 17

double force = 100;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" charset="UTF-8" content="width=device-width, initial-scale=1">
  <style>
    h2 {text-align:center;}
    p {text-align:center;}
    body { text-align:center; }
    .vert { margin-bottom: 10%; }
    .hori{ margin-bottom: 0%; }
    button {font-size: 5vw; padding: 5px;}
    div {padding: 5px;}
  </style>
</head>
<body>
  <div id="container" style="background-color:grey;">
    <div> 
      <h2>&#9881 TESTMASTERS - Photos</h2>
    </div>
    <p>It might take more than 5 seconds to capture a photo.</p>
    <p>
    <div>
      <button onclick="rotatePhoto();">ROTATE</button>
      <button onclick="capturePhoto()">CAPTURE PHOTO</button>
    </div>
    <div>
      <button onclick="location.reload();">REFRESH PAGE</button>
      <button onclick="displayForce();">SHOW FORCE</button>
    </div>
    </p>
  </div>
  <div>
  <div><img src="saved-photo" id="photo" width="70%"></div>
  <div color = blue>
    <div id="force" width = "50%" height = "50%"> Reading = [] </div>
  </div>
  <div><a href="saved-photo" download="camera_image"> download </a></div>
</body>
<script>
  var deg = 0;
  var force = 100;
  function capturePhoto() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', "/capture", true);
    xhr.send();
  }
  function rotatePhoto() {
    var img = document.getElementById("photo");
    deg += 90;
    if(isOdd(deg/90)){ document.getElementById("container").className = "vert"; }
    else{ document.getElementById("container").className = "hori"; }
    img.style.transform = "rotate(" + deg + "deg)";
  }
  function isOdd(n) { return Math.abs(n % 2) == 1; }
  function getPhoto() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', "/download", true);
    xhr.send();
  }
  function displayForce() {
    var element = document.getElementById("force");
    var text = element.innerText;
    var newText = text.replace("[]", force);
    element.innerText = newText;
    var xhr = new XMLHttpRequest();
    xhr.open('GET', "/get_force", true);
    xhr.send();
  }
</script>
</html>)rawliteral";

bool receiving_data = false;
bool new_data = false;

String processor(const String& var) // function for sending 
{
  if(var == "force") {
    return String(force);
  }
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  int start = 1;
  while(start!=10) {
    Serial.println("Off");
    start = Serial.readString().compareTo("on");
    Serial.println(start);
  }
  Serial.println("On");
  // Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  // while (!Serial2.available()) {
  //   continue;
  // }
  // String input = String(Serial2.read());
  // while (input != "a") {
  //   input = Serial2.read();
  // }
  // while (Serial2.available() && new_data == false) {
  //   input = Serial2.read();
  //   if (receiving_data = true) {
  //     if (input != "a") {
  //       force = atof(input.c_str());
  //       new_data = true;
  //     }
  //   }
  //   else if (input == "a") {
  //     receiving_data = true;
  //   }
  // }

  // Connect to Wi-Fi
  if (!WiFi.softAP(ssid)) {
    log_e("Connection failed");
    Serial.println("Unsuccessful");
    while(1);
  }
  server.begin();
  // WiFi.begin(ssid);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }

  // Print ESP32 Local IP Address
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // OV2640 camera module
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }


  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
    // request->send(200, "text/html", String(), false, processor);
  });

  server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
    takeNewPhoto = true;
    request->send_P(200, "text/plain", "Taking Photo");
    request->send_P(200, "text/html", index_html);
  });

  server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, FILE_PHOTO, "image/jpg", true);
  });

  server.on("/download", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", "Downloaded");
  });

  server.on("/get_force", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.write(long(force));
    request->send(200, "text/html", "Force = " +  String(force));
  });

  // Start server
  server.begin();
  Serial.write(FILE_PHOTO);

}

void loop() {
  if (takeNewPhoto) {
    capturePhotoSaveSpiffs();
    takeNewPhoto = false;
  }
  delay(1);
}

// Check if photo capture was successful
bool checkPhoto( fs::FS &fs ) {
  File f_pic = fs.open( FILE_PHOTO );
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );
}

// Capture Photo and Save it to SPIFFS
void capturePhotoSaveSpiffs( void ) {
  camera_fb_t * fb = NULL; // pointer
  bool ok = 0; // Boolean indicating if the picture has been taken correctly

  do {
    // Take a photo with the camera
    Serial.println("Taking a photo...");

    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }

    // Photo file name
    Serial.printf("Picture file name: %s\n", FILE_PHOTO);
    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);

    // Insert the data in the photo file
    if (!file) {
      Serial.println("Failed to open file in writing mode");
    }
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      Serial.print("The picture has been saved in ");
      Serial.print(FILE_PHOTO);
      Serial.print(" - Size: ");
      Serial.print(file.size());
      Serial.println(" bytes");
      Serial.println("image size: ");
      // Serial.println(fb->len);
    }
    // Close the file
    file.close();
    esp_camera_fb_return(fb);

    // check if file has been correctly saved in SPIFFS
    ok = checkPhoto(SPIFFS);
  } while ( !ok );
}