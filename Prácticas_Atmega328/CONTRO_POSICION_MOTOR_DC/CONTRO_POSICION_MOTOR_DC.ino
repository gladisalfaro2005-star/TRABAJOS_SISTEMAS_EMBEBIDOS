#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

volatile long encoderCount = 0;    // cuentas de posición del encoder
volatile int8_t encoderDir = 0;    // último sentido de giro

const uint8_t POT_PIN = A0;        // potenciómetro

long setpoint = 0;                 // posición objetivo
const long POSITION_TOLERANCE = 20;
const uint8_t Kp = 1;
const uint8_t PWM_MAX = 150;

const int  POT_DEADZONE      = 10;    // zona muerta del pot
const long MAX_DELTA_COUNTS  = 520;  // rango máximo de movimiento

long basePosition = 0;             // posición inicial
int  potCenter    = 0;             // lectura inicial del pot

void motorStop(void);
void motorForward(void);
void motorBackward(void);
void setPWM(uint8_t duty);
long getEncoderCount(void);
void readSetpointFromPot(void);

void setup(void)
{
  cli();

  // Encoder en D2 (INT0) y D3
  DDRD &= ~((1 << DDD2) | (1 << DDD3));
  PORTD |= (1 << PORTD2) | (1 << PORTD3);

  // L293D: IN1=D7, IN2=D8, ENA=D9 (PWM)
  DDRD |= (1 << DDD7);
  DDRB |= (1 << DDB0) | (1 << DDB1);

  motorStop();
  setPWM(0);

  // INT0 en flanco de subida
  EICRA |= (1 << ISC01) | (1 << ISC00);
  EIMSK |= (1 << INT0);
  EIFR  |= (1 << INTF0);

  // Timer1: Fast PWM 8 bits en OC1A (D9)
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1A |= (1 << WGM10);
  TCCR1B |= (1 << WGM12);
  TCCR1A |= (1 << COM1A1);
  TCCR1B |= (1 << CS11);
  OCR1A = 0;

  pinMode(POT_PIN, INPUT);

  sei();

  delay(50);
  basePosition = getEncoderCount();
  potCenter    = analogRead(POT_PIN);
  setpoint     = basePosition;
}

void loop(void)
{
  readSetpointFromPot();

  long posicion = getEncoderCount();
  long error = setpoint - posicion;
  long absError = (error >= 0) ? error : -error;

  if (absError <= POSITION_TOLERANCE)
  {
    motorStop();
    setPWM(0);
  }
  else
  {
    if (error > 0) motorForward();
    else           motorBackward();

    uint32_t pwm = (uint32_t)absError * Kp;
    if (pwm > PWM_MAX) pwm = PWM_MAX;
    setPWM((uint8_t)pwm);
  }
}

// Encoder en cuadratura: canal A en INT0, canal B en D3
ISR(INT0_vect)
{
  if (PIND & (1 << PIND3))
  {
    encoderCount++;
    encoderDir = 1;
  }
  else
  {
    encoderCount--;
    encoderDir = -1;
  }
}

// IN1=0, IN2=0 -> motor libre
void motorStop(void)
{
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
}

// IN1=1, IN2=0
void motorForward(void)
{
  PORTD |= (1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
}

// IN1=0, IN2=1
void motorBackward(void)
{
  PORTD &= ~(1 << PORTD7);
  PORTB |= (1 << PORTB0);
}

void setPWM(uint8_t duty)
{
  OCR1A = duty;
}

// Lectura atómica de la posición
long getEncoderCount(void)
{
  long value;
  uint8_t sreg = SREG;
  cli();
  value = encoderCount;
  SREG = sreg;
  return value;
}

// Setpoint relativo según potenciómetro
void readSetpointFromPot(void)
{
  int raw = analogRead(POT_PIN);
  int deltaRaw = raw - potCenter;

  if (deltaRaw > -POT_DEADZONE && deltaRaw < POT_DEADZONE)
    deltaRaw = 0;

  if (deltaRaw > 512)  deltaRaw = 512;
  if (deltaRaw < -512) deltaRaw = -512;

  long deltaCounts = ((long)deltaRaw * MAX_DELTA_COUNTS) / 360;

  setpoint = basePosition + deltaCounts;
}