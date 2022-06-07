// -------------------------------------------------------------
// CANtest for Teensy 3.6
// by teachop
//
// This test is talking to a single other echo-node on the bus.
// 6 frames are transmitted and rx frames are counted.
// Tx and rx are done in a way to force some driver buffering.
// Serial is used to print the ongoing status.
//

#include <Metro.h>
#include <FlexCAN.h>


Metro sysTimer = Metro(1);// milliseconds

int led = 13;
FlexCAN CANbus(500000);
static CAN_message_t msg,rxmsg;
static uint8_t hex[17] = "0123456789abcdef";

int txCount,rxCount;
unsigned int txTimer,rxTimer;
int temp_therm = 19; //exemple température thermistance = 19°C


// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}


// -------------------------------------------------------------
void setup(void)
{
  CANbus.begin();
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);

  delay(1000);
  Serial.println(F("Hello Teensy 3.1 CAN Test."));

  sysTimer.reset();
}


// -------------------------------------------------------------
void loop(void)
{
  char buffer_printf[200];
  // service software timers based on Metro tick
  if ( sysTimer.check() ) {
    if ( txTimer ) {
      --txTimer;
    }
    if ( rxTimer ) {
      --rxTimer;
    }
  }

  // if not time-delayed, read CAN messages and print 1st byte
  if ( !rxTimer ) {
    while ( CANbus.read(rxmsg) ) {
      //rxmsg.id = 0x7df;
      //hexDump( sizeof(rxmsg), (uint8_t *)&rxmsg );
      
      sprintf(buffer_printf,"ID : 0x%x Len : %d  -- Data 0x%x  0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",rxmsg.id,rxmsg.len,rxmsg.buf[0],rxmsg.buf[1],rxmsg.buf[2],rxmsg.buf[3],rxmsg.buf[4],rxmsg.buf[5],rxmsg.buf[6],rxmsg.buf[7]);
      Serial.write(buffer_printf);
      
      rxCount++;
    }
  }

  // insert a time delay between transmissions
  if ( !txTimer ) {
    // if frames were received, print the count
    if ( rxCount ) {
      Serial.write('=');
      Serial.print(rxCount);
      rxCount = 0;
    }
    txTimer = 100;//milliseconds
    msg.len = 8;
    msg.id = 0x7df;
    for( int idx=1; idx<8; ++idx ) {
      msg.buf[idx] = '1'+idx;
    }
    // send 6 at a time to force tx buffering
    txCount = 1;
    digitalWrite(led, 1);
    Serial.println(".");
    while ( txCount-- ) {
      CANbus.write(msg);
      msg.buf[0]=temp_therm;
    }
    digitalWrite(led, 0);
    // time delay to force some rx data queue use
    rxTimer = 3;//milliseconds
  }

}
