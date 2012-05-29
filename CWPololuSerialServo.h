#ifndef _CWPOLOLUMS8SC_H_
#define _CWPOLOLUMS8SC_H_

#include <alloca.h>
#include <HardwareSerial.h>

/* Control a Pololu Serial Servo controller (Part #207)
 * http://www.pololu.com/catalog/product/207
 *
 * Methods for controlling a Pololu Micro Serial Servo Controller. This class assumes that you're using
 * the controller in the "Pololu Mode" (See: User's Guide (AKA: the Pololu Micro Serial Servo
 * Controller User's Guide)
 * http://www.pololu.com/file/0J37/ssc03a_guide.pdf for complete information on the Pololu mode)
 * (May 28, 2012).
 * You cannot run this code against a controller running in the Mini SSC II Mode. Don't even bother
 * trying it. I have another class @ https://github.com/wojnosystems/pololu-ms8sc that will work for
 * very basic uses.
 *
 * Created for personal use in a multiped robot with a need for at least 12 servos.
 * 
 * Author: Christopher Wojno May 2012
 */
class CwPololuSerialServo {
	public:
		/* Constructor
		 * 
		 * Creates and configures a new controller or set of controllers.
		 *
		 */
		CwPololuSerialServo(
				HardwareSerial *sout,
				unsigned int baud = 9600 );

		/** Set Speed
		 *
		 * Sets the speed at which the servo will transition to a new value
		 *
		 * Arguments:
		 *  server_number: (unsigned short int) specifies which servo ID to set. Valid range from 0-127
		 *  speed: (unsigned short int) specifies the speed at which the servo will move to a new
		 *  position. Valid values are 0-127. 0 = maximum speed, 1 is slowest speed (50ms/sec), 127 is the fastest
		 *  settable speed (6.35ms/s)
		 *
		 * Returns:
		 *  (int):
		 *		*	0: success
		 *		*	-2: The servo number was out of range
		 */
		int set_speed(
				unsigned short int servo_number,
				unsigned short int speed = 0 );


		/** Set Position
		 *
		 * Places a servo into a specific position.
		 * This command will automatically extend to the 2-data byte mode if you pass in values greater
		 * than 127. This automatically takes care of forward/reverseand the
		 * neutral position settings set previously.
		 *
		 * Arguments:
		 *  server_number: (unsigned short int) specifies which servo ID to set. Valid range from 0-127
		 *
		 *  position: (unsigned short int) specifies the position to move to (neutral and directionality
		 *  is already adjusted for you). Valid values are 0-255. Please note, larger values require
		 *  more time to transmit due to the way communication is performed with the controller.
		 *
		 * Returns:
		 *  (int):
		 *		*	0: success
		 *		*	-2: The servo number was out of range
		 */
		int set_position(
				unsigned short int servo_number,
				unsigned short int position = 0 );


		/** Set Position Absolute
		 *
		 * Places a servo into an absolute position. This command always uses 2 data bytes. This call
		 * runs direction into the servo PWM generator and bypasses directionality and the neutral range
		 *
		 * Arguments:
		 *  server_number: (unsigned short int) specifies which servo ID to set. Valid range from 0-127
		 *
		 *  position: (unsigned int) specifies the position to move to (neutral and directionality
		 *  is already adjusted for you). Valid values are 500-5500.
		 *
		 * Returns:
		 *  (int):
		 *		*	0: success
		 *		*	-2: The servo number was out of range
		 *		* -3: position is out of range
		 */
		int set_position_absolute(
				unsigned short int servo_number,
				unsigned int position = 500 );



		/** Set Neutral
		 *
		 * Sets a zero, or neutral position on the servo. This is the "home" position. This is useful
		 * for calibration
		 *
		 * Arguments:
		 *  server_number: (unsigned short int) specifies which servo ID to set. Valid range from 0-127
		 *
		 *  position: (unsigned int) specifies the position to set as the neutral position. Valid values
		 *  are 500-5500. The default is 3000
		 *
		 * Returns:
		 *  (int):
		 *		*	0: success
		 *		*	-2: The servo number was out of range
		 *		* -3: position is out of range
		 */
		int set_neutral(
				unsigned short int servo_number,
				unsigned int position = 3000 );


		/** Configure Servo
		 *
		 * Turns a servo on, off, changes it's direction, or sets the movement range
		 *
		 * Arguments:
		 *  server_number: (unsigned short int) specifies which servo ID to set. Valid range from 0-127
		 *  enabled: (bool) if you want to enable the servo, false to disable the PWM generator
		 *  forward_direction: (bool) true to move in the "forward" direction when calling set_position,
		 *  false to move backward. Ignored when calling set_position_absolute.
		 *  range: (unsigned short int) between 0-15. 0 causes the servo to remain at neutral, 15 causes
		 *  8-bit commands to show up at 180 degress, 7-bit commands at 90 degrees. The range controls
		 *  the pulse length of the output of the serial controller. See page 6 of the User Manual for
		 *  details
		 *
		 * Returns:
		 *  (int):
		 *		*	0: success
		 *		*	-2: The servo number was out of range
		 *		* -3: The range was out of range (must be 0-15)
		 */
		int configure_servo(
				unsigned short int servo_number,
				bool enabled = true,
				bool forward_direction = true,
				unsigned short int range = 15
				);


		/* Set the base number for the servo controller
		 *
		 * Sets the internal pulse length of the controller for generating the PWM signal for the
		 * servos. This is a one-shot command and must only be run when only ONE controller is connected
		 * to the serial bus. It will re-program everything on the bus to this value. If this is
		 * something you don't care about, then ignore this warning.
		 *
		 * Remember, you need to power off the controller for this to take effect.
		 *
		 * Also: this value is stored in non-volatile RAM. You don't have to set it each time, it will
		 * remember this value
		 *
		 * You need to run this only with the controller running in Pololu mode, not Mini SSC II.
		 *
		 * Arguments:
		 *   base_number: (unsigned short int) between 0x00-0x10 designating the servo offsets for this
		 *   controller. For example, setting this to 0x0 means all 8 servo ports will be in the range
		 *   0-7. Setting this to 0x02 means the servos will be in the range: 15-23. 0x10 means that the
		 *   servo port ids are in the range 120-127.
		 *
		 * Returns:
		 *  (int):
		 *	*	0: success
		 *	*	-2: base_number is out of range
		 */
		int set_servo_number_base_number( 
				unsigned short int base_number = 0x0 );

		/* Get the base number for the servo controller (via blinking light)
		 *
		 * Causes the controller to flash the green LED the number set to the controller. It may not
		 * blink if you have this set to 0x00. This will cause ALL controllers on the serial bus to
		 * flash.
		 *
		 * You need to run this only with the controller running in Pololu mode, not Mini SSC II.
		 *
		 * Of special note, the documentation claims that the addressing goes from 0x00 to 0x10.
		 * Elsewhere in the docs is the mention of 0-15 in decimal, but all of the hex examples are 0x10
		 * (16). This is from 0 to 16 (17 values total). This makes the totall addressable servos 135
		 * instead of 127. I think this is a mistake in the original documentation, but I could be wrong, too.
		 *
		 * Arguments:
		 *   base_number: (unsigned short int) between 0x00-0x0f designating the servo offsets for this
		 *   controller. For example, setting this to 0x0 means all 8 servo ports will be in the range
		 *   0-7. Setting this to 0x02 means the servos will be in the range: 15-23. 0x0f means that the
		 *   servo port ids are in the range 120-127.
		 *
		 * Returns:
		 *  (int):
		 *	*	0: success
		 *	*	-2: base_number is out of range
		 */
		int blink_servo_number_base_number( 
				unsigned short int base_number = 0x0 );

		inline static bool is_valid_servo_id( unsigned short int sn ) { return sn < 128; };
	private:
		HardwareSerial *_serial;
};


#endif//_CWPOLOLUMS8SC_H_
