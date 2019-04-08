const int BUZZER_PIN = 8;

const int C = 261;    //도
const int D = 294;    //레
const int E = 330;    //미
const int F = 349;    //파
const int G = 392;    //솔
const int A = 440;    //라
const int B = 494;    //시

typedef struct  {
  int tone;
  unsigned long delay;
} TAD; //Tone And Delay

TAD music[] = {
  {G, 100}, {G, 100}, {A, 100}, {A, 100}, {G, 100}, {G, 100}, {E, 200},
  {G, 100}, {G, 100}, {E, 100}, {E, 100}, {D, 200}, {G, 100}, {G, 100},
  {A, 100}, {A, 100}, {G, 100}, {G, 100}, {E, 200}, {G, 100}, {E, 100},
  {D, 100}, {E, 100}, {C, 200}
};

int musicLen;

void setup() {
  musicLen = sizeof(music) / sizeof(TAD);
}

void loop() {
  for (int i = 0; i < musicLen; i++) {
    tone(BUZZER_PIN, music[i].tone);
    delay(music[i].delay * 5);

    noTone(BUZZER_PIN);
    delay(30);
  }

  noTone(BUZZER_PIN);
  delay(1000);
}
