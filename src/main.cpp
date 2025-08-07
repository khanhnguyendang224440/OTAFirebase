#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <HTTPUpdate.h>

const char *ssid = "IOT_2";
const char *password = "iot@1234";

const char *json_url = "https://esp32-ota-demo-d2ee4.web.app/firmware.json";
const char *current_version = "1.0.7"; // Phiên bản hiện tại đang chạy trên thiết bị

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("📶 Đang kết nối WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi đã kết nối!");
  Serial.println("🚀 Bắt đầu kiểm tra OTA...");

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;
  https.begin(client, json_url);
  int httpCode = https.GET();

  if (httpCode == HTTP_CODE_OK)
  {
    String payload = https.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    String new_version = doc["version"];
    String firmware_url = doc["url"];

    Serial.printf("🔎 Phiên bản mới: %s\n", new_version.c_str());
    Serial.printf("🔗 URL firmware: %s\n", firmware_url.c_str());

    if (new_version != current_version)
    {
      Serial.println("📦 Có firmware mới. Đang cập nhật...");
      t_httpUpdate_return ret = httpUpdate.update(client, firmware_url);

      switch (ret)
      {
      case HTTP_UPDATE_FAILED:
        Serial.printf("❌ OTA thất bại. Mã lỗi: %d\n", httpUpdate.getLastError());
        Serial.printf("🔍 Lý do: %s\n", httpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_OK:
        Serial.println("✅ Cập nhật thành công! Đang khởi động lại...");
        break;

      default:
        Serial.println("⚠️ Trạng thái OTA không xác định.");
        break;
      }
    }
    else
    {
      Serial.println("✔️ Thiết bị đã ở phiên bản mới nhất.");
    }
  }
  else
  {
    Serial.printf("❌ Lỗi tải JSON. Mã HTTP: %d\n", httpCode);
  }

  https.end();
}

void loop() {}
