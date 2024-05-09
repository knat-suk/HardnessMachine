// #include "camera_functions.h"
// #include <Arduino.h>
// #define CAMERA_MODEL_AI_THINKER // Select camera model

// // #include "camera_pins.h"

// void cameraExec() {
//   WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector

//   Serial.begin(115200);

//   camera_config_t config;
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer = LEDC_TIMER_0;
//   config.pin_d0 = Y2_GPIO_NUM;
//   config.pin_d1 = Y3_GPIO_NUM;
//   config.pin_d2 = Y4_GPIO_NUM;
//   config.pin_d3 = Y5_GPIO_NUM;
//   config.pin_d4 = Y6_GPIO_NUM;
//   config.pin_d5 = Y7_GPIO_NUM;
//   config.pin_d6 = Y8_GPIO_NUM;
//   config.pin_d7 = Y9_GPIO_NUM;
//   config.pin_xclk = XCLK_GPIO_NUM;
//   config.pin_pclk = PCLK_GPIO_NUM;
//   config.pin_vsync = VSYNC_GPIO_NUM;
//   config.pin_href = HREF_GPIO_NUM;
//   config.pin_sscb_sda = SIOD_GPIO_NUM;
//   config.pin_sscb_scl = SIOC_GPIO_NUM;
//   config.pin_pwdn = PWDN_GPIO_NUM;
//   config.pin_reset = RESET_GPIO_NUM;
//   config.xclk_freq_hz = 20000000;
//   config.pixel_format = PIXFORMAT_JPEG;

//   // Initialize the camera
//   esp_err_t err = esp_camera_init(&config);
//   if (err != ESP_OK) {
//     Serial.printf("Camera init failed with error 0x%x", err);
//     return;
//   }

//   Serial.println("Camera init success");

//   // Take a picture and save as a PNG file
//   camera_fb_t *fb = esp_camera_fb_get();
//   if (!fb) {
//     Serial.println("Camera capture failed");
//     return;
//   }

//   Serial.printf("Picture captured, size: %zu bytes\n", fb->len);


//   Serial.write((uint8_t*)fb->buf, fb->len); // Write the picture data to the file

//   Serial.println("Picture saved as picture.png");

//   esp_camera_fb_return(fb); // Release the frame buffer memory
// }

// void loop() {
//   // Nothing to do here
// }
