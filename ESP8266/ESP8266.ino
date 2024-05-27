
#define BLYNK_TEMPLATE_ID "TMPLovCAmgAo"
#define BLYNK_TEMPLATE_NAME "DHT11"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHTesp.h> // Sử dụng thư viện DHTesp cho ESP8266

char auth[] = "-jmZSzcnuD5nTb3L-4N9H9CMG22z9l1d";
char ssid[] = "Hihi";
char pass[] = "12345678";

#define DHTPIN 16 // Chọn chân GPIO dùng để kết nối với cảm biến DHT11
DHTesp dht; // Sử dụng thư viện DHTesp cho ESP8266

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
byte degree[8] = {0B01110, 0B01010, 0B01110, 0B00000,
                  0B00000, 0B00000, 0B00000, 0B00000};
float temp;
float hum;
int led = 2;

WidgetLED ledconnect(V0);
BlynkTimer timer;

void setup()
{
  Serial.begin(115200);
  delay(100);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.createChar(0, degree);
  lcd.setCursor(3, 0);
  lcd.print("Welcome to");
  lcd.setCursor(2, 1);
  lcd.print("Do an 2");
  delay(2000);

  Serial.println("DHT11 test");

  dht.setup(DHTPIN, DHTesp::DHT11); // Khởi tạo cảm biến DHT11

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  int n = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (n > 20)
    {
      break;
    }
    n++;
  }
  if (WiFi.isConnected())
  {
    Serial.println("Wifi is connected!");
    Blynk.config(auth, "blynk.cloud", 80);
    Blynk.connect();
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp :");
  lcd.setCursor(0, 1);
  lcd.print("Hum  :");
  timer.setInterval(1000L, blinkLedWidget);
}

void loop()
{
  if (WiFi.isConnected())
  {
    Blynk.run();
  }
  timer.run();
}

void blinkLedWidget()
{
  float t = dht.getTemperature();
  float h = dht.getHumidity();
  if (!isnan(t))
  { // check if 'is not a number'
    Serial.print("Temp *C = ");
    Serial.print(t);
    Serial.print("\t\t");
    temp = t;
  }
  else
  {
    Serial.println("Failed to read temperature");
  }
  if (!isnan(h))
  { // check if 'is not a number'
    Serial.print("Hum. % = ");
    Serial.println(h);
    hum = h;
  }
  else
  {
    Serial.println("Failed to read humidity");
  }

  if (Blynk.connected())
  {
    if (ledconnect.getValue())
    {
      ledconnect.off();
      digitalWrite(led, HIGH);
    }
    else
    {
      ledconnect.on();
      digitalWrite(led, LOW);
    }
    Blynk.virtualWrite(V1, temp);
    Blynk.virtualWrite(V2, hum);
  }

  lcd.setCursor(7, 0);
  lcd.print(String(temp, 1));
  lcd.write(0);
  lcd.print("C  ");
  lcd.setCursor(7, 1);
  lcd.print(String(hum, 1));
  lcd.print("%  ");
}
