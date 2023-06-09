    // CONFIG KELEMBAPAN TANAH
int sensorPin = A0;   // pin sensor
int powerPin  = 2;    // untuk pengganti VCC

    // CONFIG POMPA
int pompa = 13;   // pin pompa

    // CONFIG LCD I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

    // CONFIG SUHU
#include <DHT.h>                    //library DHT
#define DHTPIN 3                    //pin DATA konek ke pin 3 Arduino
#define DHTTYPE DHT11               //tipe sensor DHT11
DHT dht(DHTPIN, DHTTYPE);           //set sensor + koneksi pin
float humi, temp;                   //deklarasi variabel 

int bacaSensor() {
    // CONFIG READ SENSOR
  digitalWrite(powerPin, HIGH);             // hidupkan power
  delay(500);
  int nilaiSensor = analogRead(sensorPin);  // baca nilai analog dari sensor
  digitalWrite(powerPin, LOW);
  return 1023 - nilaiSensor;                // makin lembab maka makin tinggi nilai outputnya
}

void setup () {
    // DISPLAY SERIAL MONITOR
  Serial.begin(9600);

    // SETUP LCD
  lcd.begin();                       // Initialize the lcd

      // SETUP POMPA
  pinMode(pompa, OUTPUT);
  
    // SETUP SENSOR KELEMBAPAN TANAH
  pinMode(powerPin, OUTPUT);        // jadikan pin power sebagai output
  digitalWrite(powerPin, LOW);      // default bernilai LOW
}

void loop () {
    // SHOW KELEMBAPAN TANAH IN SERIAL MONITOR
  Serial.print("Nilai kelembaban: ");
  Serial.println(bacaSensor());

    // SHOW KELEMBAPAN TANAH IN LCD I2C
  lcd.setCursor(0,0);
  lcd.print("Nilai = ");
  lcd.print(bacaSensor());
  lcd.print(" RH                ");

  if(bacaSensor() < 500) {
    digitalWrite(pompa,HIGH);       // Pompa akan menyala
  } else {
    digitalWrite(pompa,LOW);        // Pompa akan mati
  }  

  humi = dht.readHumidity();        //baca kelembaban
  temp = dht.readTemperature();     //baca suhu
  if (isnan(humi) || isnan(temp)) { //jika tidak ada hasil
    Serial.println("DHT11 tidak terbaca... !");
    return;
  }
  else{//jika ada hasilnya 
  Serial.print("Suhu=");            //kirim serial "Suhu"
  Serial.print(temp);               //kirim serial nilai suhu
  Serial.println("C");              //kirim serial "C" Celcius 
  Serial.print("Humi=");            //kirim serial "Humi"
  Serial.print(humi);               //kirim serial nilai kelembaban
  Serial.println("%RH");            //kirim serial "%RH"
  }
  delay(1000);            //tunda 1 detik untuk pembacaan berikutnya
  
}
