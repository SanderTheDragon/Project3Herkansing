#include <CRC32.h>
#include <Keypad.h>
#include <MFRC522.h>
#include <SPI.h>

char const keymap[4][4] = 
{
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '-', '0', '+', 'D' }
};

MFRC522 mfrc522(10, 9);

byte const rowPins[4] = { A0, A1, A2, A3 };
byte const colPins[4] = { 4, 5, 6, 7 };
Keypad pad(makeKeymap(keymap), rowPins, colPins, 4, 4);

String uid;
long cardCheck;

bool isConnected;
long connectCheck;
String pingData;

String hasher;

void handshake()
{
  String key;

  for (int i = 0; i < 64; i++)
  {
    randomSeed(analogRead(A0) ^ random(i));
    key += (char)('0' + random(64));
  }

  sendBytes("handshake");
  sendBytes(key);

  String data;
  int wait = 0;
  while (wait < 1000)
  {
    if (Serial.available() > 0)
    {
      data = Serial.readStringUntil('\n');
    }
    
    wait++;

    delay(1);
  }

  if (data.length() > 0)
  {
    char buff[64];
    for (int i = 0; i < 64; i++)
    {
      buff[i] = key[i];
    }
    
    uint32_t crc = CRC32::calculate(buff, 64);
    uint32_t target = (int)((crc % 256) ^ 'x') + ((int)buff[11] + (int)'0');
    isConnected = ((uint32_t)(data.toInt()) == target);

    if (isConnected)
    {
      hasher = "";
      for (int i = 0; i < 64; i++)
      {
        hasher += (char)(buff[i] ^ (target % 32));
      }
      
      sendBytes("OK");
    }
    else
    {
      sendBytes("0K");
    }
  }
}

void sendBytes(String data)
{
  for (unsigned int i = 0; i < data.length(); i++)
  {
    Serial.write((byte)data[i]);
  }

  Serial.write((byte)'\n');
  Serial.flush();
}

void sendHashed(String message)
{
  for (unsigned int i = 0; i < message.length(); i++)
  {
    byte data;
    if ((message[i] ^ hasher[i % 64]) == (byte)0)
    {
      data = (byte)(message[i] ^ (int)'?');
    }
    else
    {
      data = (byte)(message[i] ^ hasher[i % 64]);
    }
    Serial.write(data);
  }
  
  Serial.write((byte)'\n');
  Serial.flush();
}

String decodeHashed(String message)
{
  String decoded;
  
  for (unsigned int i = 0; i < message.length(); i++)
  {
    if ((message[i] ^ (byte)'?') == hasher[i % 64])
    {
      decoded += (char)(message[i] ^ (byte)'?');
    }
    else
    {
      decoded += (char)(message[i] ^ hasher[i % 64]);
    }
  }

  return decoded;
}



void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(2, OUTPUT);
}

void loop()
{
  if (isConnected)
  {
    digitalWrite(2, LOW);
    if (uid.length() == 0)
    {
      if (mfrc522.PICC_IsNewCardPresent())
      {
        if (mfrc522.PICC_ReadCardSerial())
        {
          uid += String((mfrc522.uid.uidByte[0] << 24), HEX);
          uid += String((mfrc522.uid.uidByte[1] << 16), HEX);
          uid += String((mfrc522.uid.uidByte[2] << 8), HEX);
          uid += String((mfrc522.uid.uidByte[3]), HEX);
      
          if (uid.length() < 8)
          {
            uid = "";
          }
          else
          {
            uid.toUpperCase();
          }
    
          sendHashed("card:" + uid);
          cardCheck = millis();
        }
      }
    }
    else
    {
      char const pressedKey = pad.getKey();
    
      if (pressedKey != NO_KEY)
      {
        sendHashed("key:" + String(pressedKey));
      }
    }

    if (millis() - cardCheck > 500 && uid.length() > 0)
    {
      if (mfrc522.PICC_IsNewCardPresent() || (!mfrc522.PICC_IsNewCardPresent() && !mfrc522.PICC_ReadCardSerial()))
      {
        sendHashed("card:removed");
        uid = "";
      }

      cardCheck = millis();
    }

    if (millis() - connectCheck > 5000)
    {
      if (pingData.length() == 0)
      {
        pingData = String(millis());
        sendHashed("ping:" + pingData);
      }
      else
      {
        if (Serial.available())
        {
          String data = decodeHashed(Serial.readStringUntil('\n'));

          if (data.startsWith("pong:"))
          {
            String pongData = data.substring(data.indexOf(':') + 1);
  
            if (pongData == pingData)
            {
              pingData = "";
              connectCheck = millis();
            }
          }
        }
        else
        {
          if (millis() - connectCheck > 10000)
          {
            isConnected = false;
            pingData = "";
            uid = "";
          }
        }
      }
    }
  }
  else
  {
    digitalWrite(2, HIGH);
    handshake();
  }
}

