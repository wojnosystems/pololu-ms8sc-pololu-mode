// Serial Test
#include <ArduinoUnit.h>
#include <CwPololuSerialServo.h>

TestSuite suite;
CwPololuSerialServo servos( &Serial1, 40000 );

void setup() {
  Serial2.begin(40000); // input
  while( Serial2.available() > 0 ) { Serial2.read(); }
}

// Serial Test
test(serial_read) {
  char ret;
  char r[4];
  Serial1.write( 0x01 );
  Serial1.write( 0x02 );
  Serial1.write( 0x03 );
  Serial1.write( 0x04 );
  Serial1.flush();
  while( Serial2.available() <= 0 ) {}
  Serial2.readBytes( r, 4 );
  assertEquals( 0x01, r[0] );
  assertEquals( 0x02, r[1] );
  assertEquals( 0x03, r[2] );
  assertEquals( 0x04, r[3] );
  
  r[0] = 0x10;
  r[1] = 0x11;
  r[2] = 0x12;
  r[3] = 0x13;
  Serial1.write( (unsigned char*)r, 4 );
  Serial1.flush();
  while( Serial2.available() <= 0 ) {}
  ret = Serial2.read();
  assertEquals( 0x10, ret );
  while( Serial2.available() <= 0 ) {}
  ret = Serial2.read();
  assertEquals( 0x11, ret );
  while( Serial2.available() <= 0 ) {}
  ret = Serial2.read();
  assertEquals( 0x12, ret );
  while( Serial2.available() <= 0 ) {}
  ret = Serial2.read();
  assertEquals( 0x13, ret );
}

test(set_position_absolute) {
  unsigned char value[6];
  servos.setPositionAbsolute( 1, 500 );
  Serial2.readBytes( (char*)value, 6 );
  assertEquals( 0x80, value[0] );
  assertEquals( 0x01, value[1] );
  assertEquals( 0x04, value[2] );
  assertEquals( 0x01, value[3] );
  assertEquals( 0x03, value[4] );
  assertEquals( 0x74, value[5] );
  
  
  servos.setPositionAbsolute( 0, 550 );
  Serial2.readBytes( (char*)value, 6 );
  assertEquals( 0x80, value[0] );
  assertEquals( 0x01, value[1] );
  assertEquals( 0x04, value[2] );
  assertEquals( 0x00, value[3] );
  assertEquals( 0x04, value[4] );
  assertEquals( 0x26, value[5] );
  
  
  servos.setPositionAbsolute( 4, 600 );
  Serial2.readBytes( (char*)value, 6 );
  assertEquals( 0x80, value[0] );
  assertEquals( 0x01, value[1] );
  assertEquals( 0x04, value[2] );
  assertEquals( 0x04, value[3] );
  assertEquals( 0x04, value[4] );
  assertEquals( 0x58, value[5] );
  
  
  servos.setPositionAbsolute( 0, 5500 );
  Serial2.readBytes( (char*)value, 6 );
  assertEquals( 0x80, value[0] );
  assertEquals( 0x01, value[1] );
  assertEquals( 0x04, value[2] );
  assertEquals( 0x00, value[3] );
  assertEquals( 0x2A, value[4] );
  assertEquals( 0x7C, value[5] );
  
}

test(configure_servo) {
  unsigned char value[5];
  servos.configureServo( 0 );
  Serial2.readBytes( (char*)value, 5 );
  assertEquals( 0x80, value[0] );
  assertEquals( 0x01, value[1] );
  assertEquals( 0x00, value[2] );
  assertEquals( 0x00, value[3] );
  assertEquals( 0x4F, value[4] ); // 01001111 = 0x4F
}

test(set_neutral) {
  unsigned char value[6];
  servos.setNeutral( 0, 3000 );
  Serial2.readBytes( (char*)value, 6 );
  assertEquals( 0x80, value[0] );
  assertEquals( 0x01, value[1] );
  assertEquals( 0x05, value[2] );
  assertEquals( 0x00, value[3] );
  assertEquals( 0x17, value[4] );
  assertEquals( 0x38, value[5] );
}

test(set_speed) {
  unsigned char value[5];
  servos.setSpeed( 0, 0 );
  Serial2.readBytes( (char*)value, 5 );
  assertEquals( 0x80, value[0] );
  assertEquals( 0x01, value[1] );
  assertEquals( 0x01, value[2] );
  assertEquals( 0x00, value[3] );
  assertEquals( 0x00, value[4] );
  
  servos.setSpeed( 3, 50 );
  Serial2.readBytes( (char*)value, 5 );
  assertEquals( 0x80, value[0] );
  assertEquals( 0x01, value[1] );
  assertEquals( 0x01, value[2] );
  assertEquals( 0x03, value[3] );
  assertEquals( 0x32, value[4] );
}


void loop() {
  suite.run();
}



