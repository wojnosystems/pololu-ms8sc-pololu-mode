#include "Arduino.h"
#include "CwPololuSerialServo.h"

CwPololuSerialServo::CwPololuSerialServo( HardwareSerial *sout, unsigned int baud ) {
	_serial = sout;
	sout->begin(baud);
}

int CwPololuSerialServo::setSpeed(
				unsigned short int servo_number,
				unsigned short int speed
	) {
	unsigned char cmd[5] = { 0x80, 0x01, 0x01, 0x00, 0x00 };
	if( !CwPololuSerialServo::isValidServoId(servo_number) ) return -2;
	if( speed > 127 ) return -3;
	cmd[3] = lowByte(servo_number);
	cmd[4] = lowByte(speed);
	_serial->write( cmd, 5 );
	_serial->flush();
	return 0;
}

int CwPololuSerialServo::setPosition(
				unsigned short int servo_number,
				unsigned short int position
	) {
	unsigned char cmd[6]; // need enough for 8-bit version
	if( !CwPololuSerialServo::isValidServoId(servo_number) ) return -2;
	cmd[0] = 0x80; // start byte
	cmd[1] = 0x01; // device id
	cmd[3] = lowByte(servo_number);
	if( position > 127 ) { // 8-bit version
		cmd[2] = 0x03; // command
		cmd[4] = 0x01;
		cmd[5] = lowByte((0x7f & position)); // bitmask: 0111_1111 = 7f
		_serial->write( cmd, 6 );
		_serial->flush();
	} else { // 7-bit version
		cmd[2] = 0x2; // command
		cmd[4] = lowByte(position);
		_serial->write( cmd, 5 );
		_serial->flush();
	}
	return 0;
}

int CwPololuSerialServo::setPositionAbsolute(
				unsigned short int servo_number,
				unsigned int position
	) {
	unsigned char cmd[6] = { 0x80, 0x01, 0x04, 0x00, 0x00, 0x00 }; // need enough for 8-bit version
	if( !CwPololuSerialServo::isValidServoId(servo_number) ) return -2;
	if( position > 5500 || position < 500 ) return -3;
	cmd[3] = servo_number;
	// upper data bits: shift off lower bits
	cmd[4] = lowByte((position>>7));
	// lower data 7-bits
	cmd[5] = lowByte(0x7f & position); // bitmask: 0111_1111 = 7f
	_serial->write( cmd, 6 );
	_serial->flush();
	return 0;
}


int CwPololuSerialServo::configureServo(
				unsigned short int servo_number,
				bool enabled,
				bool forward_direction,
				unsigned short int range
	) {
	unsigned char cmd[5] = { 0x80, 0x01, 0x00, 0x00, 0x00 };
	if( !CwPololuSerialServo::isValidServoId(servo_number) ) return -2;
	if( range > 15 ) return -3;
	cmd[3] = servo_number;
	if( enabled ) { bitSet(cmd[4],6); }
	if( !forward_direction ) { bitSet(cmd[4],5); }
	cmd[4] = (cmd[4] | range);
	_serial->write( cmd, 5 );
	_serial->flush();
	return 0;
}

int CwPololuSerialServo::setNeutral(
	unsigned short int servo_number,
	unsigned int position ) {
	unsigned char cmd[6] = { 0x80, 0x01, 0x05, 0x00, 0x00, 0x00 };
	if( !CwPololuSerialServo::isValidServoId(servo_number) ) return -2;
	if( position > 5500 || position < 500 ) return -3;
	cmd[3] = servo_number;
	// upper data bits: shift off lower bits
	cmd[4] = lowByte((position>>7));
	// lower data 7-bits
	cmd[5] = lowByte(0x7f & position); // bitmask: 0111_1111 = 7f
	_serial->write( cmd, 6 );
	_serial->flush();
	return 0;
}

int CwPololuSerialServo::setServoNumberBaseNumber(
		unsigned short int base_number )
{
	unsigned char cmd[3] = { 0x80, 0x02, 0x00 };
	// check validity of the base number.
	if( base_number > 0x0f ) return -2;
	cmd[2] = base_number;
	_serial->write( cmd, 3 );
	_serial->flush();
	return 0;
}

void CwPololuSerialServo::blinkServoNumberBaseNumber() const
{
	unsigned char cmd[3] = { 0x80, 0x02, 0x0f };
	// check validity of the base number.
	_serial->write( cmd, 3 );
	_serial->flush();
}
