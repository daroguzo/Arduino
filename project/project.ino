#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_Fingerprint.h>

// 지문인식
SoftwareSerial mySerial(10, 11);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

#define SS_PIN 53   //RFID SS(SDA:ChipSelect) PIN
#define RST_PIN 5   //RFID Reset PIN
MFRC522 rfid(SS_PIN, RST_PIN); //RFID 라이브러리

/* 등록된 RF CARD ID */
#define PICC_0 0x1B
#define PICC_1 0xA8
#define PICC_2 0x42
#define PICC_3 0xD5


// 0x3F I2C 주소를 가지고 있는 16x2 LCD객체를 생성
LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial ultraSonic(0, 1);

// 버튼들
int redFP = 7;
int whiteRF = 6;
int blueKE = 4;

int greenEnroll = 24;
int yellowKeyEnroll = 22;

const byte ROWS = 4;    // 행(rows) 개수
const byte COLS = 4;    // 열(columns) 개수

// 키패드 입력숫자를 세는 변수
int welcome = 0;
int count = 0;
boolean keyPadState;
boolean hello;
boolean RFIDState;
boolean FPState;
boolean enrollState;

char PW[6] = {'0', '0', '0', '0', '0', '0'};  // password

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {31, 33, 35, 37};   // R1, R2, R3, R4 단자가 연결된 아두이노 핀 번호
byte colPins[COLS] = {39, 41, 43, 45};   // C1, C2, C3, C4 단자가 연결된 아두이노 핀 번호

// 서보모터와 키패드 초기화
Servo myservo;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


//초음파 센서 핀설정
int trigPin = 9;
int echoPin = 8;

// 피에조 부저
int piezo = 2;
// 음계의 배열을 만들고 배열에 들어가는 인자의 갯수를 8개로 지정
// 음계는 4옥타브를 기준으로 도, 레, 미, 파, 솔, 라, 시, 도를 출력
int numTones = 8;
int tones[] = {2093, 2349, 2637, 2793, 3136, 3520, 3951, 4186};

void setup () {
  Serial.begin(9600);
  ultraSonic.begin(9600);

  finger.begin(57600); // fingerPrint 보드레이트

  SPI.begin(); // SPI 시작
  rfid.PCD_Init(); // RF 모듈 시작

  pinMode(redFP, INPUT_PULLUP);
  pinMode(whiteRF, INPUT_PULLUP);
  pinMode(blueKE, INPUT_PULLUP);
  pinMode(greenEnroll, INPUT_PULLUP);
  pinMode(yellowKeyEnroll, INPUT_PULLUP);
  // 피에조 부저가 연결된 핀을 OUTPUT으로 설정
  pinMode(piezo, OUTPUT);

  // 서보모터 핀 3
  myservo.attach(3);

  // I2C LCD를 초기화 합니다..
  lcd.init();
  // I2C LCD의 백라이트를 켜줍니다.
  lcd.backlight();

  //초음파 센서 핀설정
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
void loop () {
  keyPadState = true;
  hello = true;
  RFIDState = true;
  FPState = true;
  enrollState = true;


  float duration, distance;
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);


  // 장애물과의 거리계산
  duration = pulseIn(echoPin, HIGH);
  distance = ((float)(340 * duration) / 10000) / 2;
  delay(100);
  ultraSonic.listen();
  Serial.print("사용자와의 거리: ");
  Serial.println(distance);
  if (distance < 50) // 사용자가 50cm 이하로 접근할 때
  {
    welcomeWord();
    welcomeM();

    Serial.println("Hello, Give your KEY!");
    // 메뉴 선택
    while (hello) {
      selectKey();
      // 지문 등록
      if (digitalRead(greenEnroll) == LOW) {
        Serial.println("New FingerPrint Enroll!");
        Serial.println("Give your KEY!");
        while (enrollState) {
          lcd.clear();
          // 0번째 줄 0번째 셀부터 입력
          lcd.setCursor(0, 0);
          // 아래의 문장을 출력합니다.
          lcd.print("   FingerPrint");
          lcd.setCursor(0, 1);
          // 문장 출력
          lcd.print("     KEY!  ");

          // 키 인증 후 시작
          while (hello) {
            selectKey();
          }

          lcd.clear();
          // 0번째 줄 0번째 셀부터 입력
          lcd.setCursor(0, 0);
          // 아래의 문장을 출력합니다.
          lcd.print(" Ready to Enroll");
          lcd.setCursor(0, 1);
          // 문장 출력
          lcd.print("FingerPrint KEY!");

          Serial.println("몇번째 지문에 등록하시겠습니까?");
          Serial.println("1~127 중 숫자를 눌러주세요.");
          id = readnumber();
          Serial.println(id);
          Serial.print("번째 지문을 등록합니다!");
          while (!  getFingerprintEnroll() );

          enrollState = false;
          hello = false;
          delay(3000);
        }
      }
      // 키패드 등록
      if (digitalRead(yellowKeyEnroll) == LOW) {
        while (enrollState) {
          lcd.clear();
          // 0번째 줄 0번째 셀부터 입력
          lcd.setCursor(0, 0);
          // 아래의 문장을 출력합니다.
          lcd.print(" Ready to Enroll");
          lcd.setCursor(0, 1);
          // 문장 출력
          lcd.print("   KeyPad KEY!");
          Serial.println("New Key Enroll!");
          Serial.println("Give your KEY!");
          // 키 인증 후 시작
          while(hello){
            selectKey();
          }
          
          Serial.println("New Key Enroll!");
          lcd.clear();
          // 0번째 줄 0번째 셀부터 입력
          lcd.setCursor(0, 0);
          // 아래의 문장을 출력합니다.
          lcd.print(" Ready to Enroll");
          lcd.setCursor(0, 1);
          // 문장 출력
          lcd.print("   KeyPad KEY!");

          keyPadState = true;
          while (keyPadState) {
            char key = keypad.getKey();
            if (key) {
              Serial.println(key);

              // 입력한 번호가 일치하면 count++ welcome++
              if (key) {
                PW[count] = key;
                count++;
                welcome++;
              }

              // #을 누르면 초기화
              if (key == '#') {
                re();
              }

              if (count == 6) {
                if (welcome == 6) {
                  Serial.println("Password Changed!");
                  keyPadState = false;
                }

                count = 0;
                welcome = 0;
              }
            }
          }

          enrollState = false;
          hello = false;
          delay(3000);
        }

      }
    }
  }
}
void selectKey() {
  if (digitalRead(redFP) == LOW) {
    Serial.println("FingerPrint");
    lcd.clear();
    // 0번째 줄 0번째 셀부터 입력
    lcd.setCursor(0, 0);
    // 아래의 문장을 출력합니다.
    lcd.print("   FingerPrint");
    lcd.setCursor(0, 1);
    // 문장 출력
    lcd.print("     KEY!  ");
    while (FPState) {
      getFingerprintIDez();
      delay(50);
    }

    hello = false;
    delay(3000);
    myservo.write(0);
  }
  // RFID 방식 선택
  if (digitalRead(whiteRF) == LOW) {
    Serial.println("RFID");
    lcd.clear();
    // 0번째 줄 0번째 셀부터 입력
    lcd.setCursor(0, 0);
    // 아래의 문장을 출력합니다.
    lcd.print("     RFID KEY!");
    while (RFIDState) {
      RFID();
    }
    hello = false;
    delay(3000);
  }
  // keypad 방식 선택
  if (digitalRead(blueKE) == LOW) {
    Serial.println("KeyPad");
    lcd.clear();
    // 0번째 줄 0번째 셀부터 입력
    lcd.setCursor(0, 0);
    // 아래의 문장을 출력합니다.
    lcd.print("   KeyPad KEY!");
    keyPad();
    hello = false;
    delay(3000);
  }
}

// 16진수 표시
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

// 키 성공
void succ() {
  myservo.write(180);
  Serial.println("open the door");
}
// 키 실패
void fail() {
  myservo.write(0);
  Serial.println("password is not correct!");
}
// 키패드 초기화
void re() {
  welcome = 0;
  count = 0;
  Serial.println("password reset!");
}

void welcomeM() {
  // 환영 멜로디 (도미솔)
  tone(piezo, tones[0], 500);
  delay(500);
  tone(piezo, tones[2], 500);
  delay(500);
  tone(piezo, tones[4], 500);
  delay(500);
}

void incorrectM() {
  // 틀림 멜로디
  tone(piezo, tones[0], 500);
  delay(500);
  tone(piezo, tones[0], 500);
  delay(500);
  tone(piezo, tones[0], 500);
  delay(500);
}
void keyPad() {
  while (keyPadState) {
    char key = keypad.getKey();
    if (key) {
      Serial.println(key);

      // 입력한 번호가 일치하면 count++ welcome++
      if (key == PW[count]) {
        count++;
        welcome++;
      }

      // 비밀번호가 틀리다면 count++
      else if (key != PW[count]) {
        count++;
      }

      // #을 누르면 초기화
      if (key == '#') {
        re();
      }

      if (count == 6) {
        if (welcome == 6) {
          succ();
          correctKeyM();
          welcomeM();
          keyPadState = false;
        }
        else {
          fail();
          incorrectKeyM();
          incorrectM();
        }

        count = 0;
        welcome = 0;
      }
    }
  }
}

void welcomeWord() {
  lcd.clear();
  // 0번째 줄 0번째 셀부터 입력
  lcd.setCursor(0, 0);
  // 아래의 문장을 출력합니다.
  lcd.print("     Hello!");
  // 1번째 줄 0번째 셀부터 입력.
  lcd.setCursor(0, 1);
  // 문장 출력
  lcd.print("  Give your KEY!  ");
}

void incorrectKeyM() {
  lcd.clear();
  // 0번째 줄 0번째 셀부터 입력
  lcd.setCursor(0, 0);
  // 아래의 문장을 출력합니다.
  lcd.print("   Incorrect");
  // 1번째 줄 0번째 셀부터 입력.
  lcd.setCursor(0, 1);
  // 문장 출력
  lcd.print("      KEY!  ");
}

void correctKeyM() {
  lcd.clear();
  // 0번째 줄 0번째 셀부터 입력
  lcd.setCursor(0, 0);
  // 아래의 문장을 출력합니다.
  lcd.print("     Correct");
  // 1번째 줄 0번째 셀부터 입력.
  lcd.setCursor(0, 1);
  // 문장 출력
  lcd.print("      KEY!  ");
}

void RFID() {
  //카드가 인식 안되었다면 더이상 진행하지 말고 빠져나감
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  //RF카드의 ID가 인식 안되었다면 더이상 진행하지 말고 빠져나감
  if ( ! rfid.PICC_ReadCardSerial())
    return;


  Serial.print(F("PICC type: "));
  //PICC 타입 읽어오기
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));


  //MIFARE 방식이 아니라면 더이상 진행하지 말고 빠져나감
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }


  //ID가 등록된 ID와 동일하다면
  if (rfid.uid.uidByte[0] != PICC_0 ||
      rfid.uid.uidByte[1] != PICC_1 ||
      rfid.uid.uidByte[2] != PICC_2 ||
      rfid.uid.uidByte[3] != PICC_3 ) {

    succ(); // 성공
    welcomeM();
    RFIDState = false;
  } else {
    //등록된 카드가 아니라면 시리얼 모니터로 ID 표시
    fail();
    incorrectM();
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
  }

  //종료
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  //다시 시작!
}
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);


  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("#"); Serial.print(finger.fingerID); Serial.println("번째 ID입니다");
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  succ(); // 성공
  correctKeyM();
  welcomeM();
  FPState = false;
  return finger.fingerID;
}

// 지문 등록 함수

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print(id); Serial.println("번째 지문을 인식시켜 주세요...");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("인식 완료!");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("에러!");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("에러!");
        break;
      default:
        Serial.println("에러!");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("인식 완료!");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("지문이 이상합니다.");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("에러!");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("지문을 찾을 수 없습니다!");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("지문을 찾을 수 없습니다!");
      return p;
    default:
      Serial.println("에러!");
      return p;
  }

  Serial.println("이전 지문 제거 완료!");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print(id); Serial.println("번째 지문");
  p = -1;
  Serial.println("같은 지문을 다시 인식시켜 주세요!");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("인식 완료!");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("에러!");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("에러!");
        break;
      default:
        Serial.println("에러!");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("인식 완료!");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("지문이 이상하게 인식됐습니다.");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("에러!");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("지문을 찾을 수 없습니다.");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("지문을 찾을 수 없습니다.");
      return p;
    default:
      Serial.println("에러!");
      return p;
  }

  // OK converted!
  Serial.print(id);  Serial.println("번째 지문 생성중...");

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("재확인 지문이 일치합니다!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("에러!");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("재확인 지문이 일치하지 않습니다.");
    return p;
  } else {
    Serial.println("에러!");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("저장 완료!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("에러!");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("이 위치에 저장할 수 없습니다.");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("저장 실패!");
    return p;
  } else {
    Serial.println("에러!");
    return p;
  }
}
uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
