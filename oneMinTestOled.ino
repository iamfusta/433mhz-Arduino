// asıl test yazılımıdır
// hem oled ekrana çıktı ???????????????
// oled pinleri
// 1 dk sonunda bir küçük duraksama ekrana yazdırırken oluyor çözemedim.
// hem serial ekrana 1 dk sonunda çıktı verir

#include <VirtualWire.h>
#include <U8g2lib.h>

#define led 3

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

char message[] = " ";

unsigned long startMillis = 0;
unsigned long previousMillis = 0;
const long interval = 60000;  // 1 dakika
int ledState = LOW;
int ledCount = 0;
bool isFirstMinute = true;

void setup()
{
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(11);
  vw_setup(4000);
  vw_rx_start();
  startMillis = millis();  // Kodun başlangıç zamanını kaydet
  u8g2.begin();
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen))
  {
    message[0] = (char)buf[0];
    if (message[0] == '0' && ledState == HIGH)
    {
      ledState = LOW;
      digitalWrite(led, LOW);
      ledCount++;
    }
    else if (message[0] == '1' && ledState == LOW)
    {
      ledState = HIGH;
      digitalWrite(led, HIGH);
      ledCount++;
    }
  }

  unsigned long currentMillis = millis();
  unsigned long elapsedTime = currentMillis - startMillis;  // Geçen süreyi hesapla

  if (isFirstMinute || (currentMillis - previousMillis >= interval))
  {
    if (isFirstMinute) {
      isFirstMinute = false;
    }

    previousMillis = currentMillis;

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(0, 10);
    u8g2.print("LED Count in 1 min: ");
    u8g2.setCursor(0, 30); // Yüksekliği ayarla
    u8g2.print(ledCount);
    u8g2.sendBuffer();

    Serial.print("LED Count in 1 min: ");
    Serial.println(ledCount);

    ledCount = 0; // LED sayacını sıfırla
  }
}
