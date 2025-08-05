#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPUpdate.h>

// ⚙️ Thông tin WiFi
const char *ssid = "IOT_2";
const char *password = "iot@1234";

const char *firmware_url = "https://esp32-ota-demo-d2ee4.web.app/firmware_khanh4.bin";

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("Thiết bị: Khanhdeptrai1");

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.println("📶 Đang kết nối WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi đã kết nối!");
  Serial.print("📡 IP: ");
  Serial.println(WiFi.localIP());

  delay(1000); // Chờ mạng ổn định

  // ⚡ Bắt đầu OTA
  Serial.println("🚀 Bắt đầu cập nhật firmware từ Khanhdeptrai4...");
  Serial.print("🔗 URL: ");
  Serial.println(firmware_url);

  WiFiClientSecure client;
  client.setInsecure(); // Bỏ xác thực SSL (Firebase chỉ dùng HTTPS)

  t_httpUpdate_return ret = httpUpdate.update(client, firmware_url);

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("❌ OTA thất bại. Mã lỗi: %d\n", httpUpdate.getLastError());
    Serial.printf("🔍 Lý do: %s\n", httpUpdate.getLastErrorString().c_str());
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("ℹ️ Không có bản cập nhật mới.");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("✅ Cập nhật thành công! Thiết bị sẽ khởi động lại.");
    break;
  }
}

void loop()
{
  // Không làm gì trong loop
}
