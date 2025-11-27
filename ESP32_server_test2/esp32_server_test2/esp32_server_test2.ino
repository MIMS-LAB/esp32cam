#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

// Your WiFi credentials
const char* ssid = "alessandro drone";
const char* password = "123456789";

WebServer server(80);

void handle_jpg_stream(void) {
  WiFiClient client = server.client();

  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  server.sendContent(response);

  while (client.connected()) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      continue;
    }

    // Write MJPEG boundary and headers
    client.write("--frame\r\n");
    client.write("Content-Type: image/jpeg\r\n\r\n");
    client.write(fb->buf, fb->len);
    client.write("\r\n");

    esp_camera_fb_return(fb);
    delay(50); // Adjust frame rate (~20 fps = 50ms)
  }
}

void setup() {
  Serial.begin(115200);

  // Camera configuration (adjust pins per your board)
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;  // 320x240
  config.jpeg_quality = 12;
  config.fb_count = 2;

  esp_camera_init(&config);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("Camera stream ready! Go to: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handle_jpg_stream);
  server.begin();
}

void loop() {
  server.handleClient();
}
