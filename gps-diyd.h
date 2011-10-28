/*cl-osd - A simple open source osd for e-osd and g-osd
Copyright (C) 2011 Carl Ljungström & David Collett

DIYD GPS support by David Collett based on code from 
Michael Smith, Jordi Munoz and Jose Julio, DIYDrones.com
GPS configuration : Custom protocol per 
"DIYDrones Custom Binary Sentence Specification V1.1"
Note: Same license as CL-OSD.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.*/


#ifndef GPS_DIYD_H_
#define GPS_DIYD_H_

#ifdef GPS_ENABLED

// structs and vars for MTK DIYD GPS

typedef struct {
	int32_t		latitude;
	int32_t		longitude;
	int32_t		altitude;
	int32_t		ground_speed;
	int32_t		ground_course;
	uint8_t		satellites;
	uint8_t		fix_type;
	uint32_t	utc_date;
	uint32_t	utc_time;
	uint16_t	hdop;
} TDiydMtkMsg;

typedef enum {
	FIX_NONE = 1,
	FIX_2D = 2,
	FIX_3D = 3
} TDiydMtkFix;

typedef enum {
	PREAMBLE1 = 0xd0,
	PREAMBLE2 = 0xdd,
} TDiydMtkProtocolPreamble;

// Packet checksum accumulators
static uint8_t gDiydChecksum1;
static uint8_t gDiydChecksum2;

// State machine state
static uint8_t gDiydStep = 0;
static uint8_t gDiydPayloadCounter;

// Receive buffer
static union {
  TDiydMtkMsg msg;
  uint8_t	bytes[sizeof(TDiydMtkMsg)];
} gDiydBuffer;

static void resetDecodeStep() {
	gDiydStep = 0;
}

static void decodeGpsData(char data) {
	uint8_t finished = 0;
  while(!finished) {
	  finished = 1;
    switch(gDiydStep){

      // Message preamble, class, ID detection
      //
      // If we fail to match any of the expected bytes, we
      // reset the state machine and re-consider the failed
      // byte as the first byte of the preamble.  This 
      // improves our chances of recovering from a mismatch
      // and makes it less likely that we will be fooled by
      // the preamble appearing as data in some other message.
      //
      case 0:
        if(PREAMBLE1 == data) {
          gDiydStep++;
		    }
        break;
      case 1:
        if (PREAMBLE2 == data) {
          gDiydStep++;
          break;
        }
        gDiydStep = 0;
        finished = 0;
		    break;
      case 2:
        if (sizeof(gDiydBuffer) == data) {
          gDiydStep++;
          gDiydChecksum2 = gDiydChecksum1 = data;				// reset the checksum accumulators
          gDiydPayloadCounter = 0;
        } 
		    else {
          gDiydStep = 0;							// reset and wait for a message of the right class
          finished = 0;
		      break;
        }
        break;

      // Receive message data
      //
      case 3:
        gDiydBuffer.bytes[gDiydPayloadCounter++] = data;
        gDiydChecksum2 += (gDiydChecksum1 += data);
        if (gDiydPayloadCounter == sizeof(gDiydBuffer)) {
          gDiydStep++;
		    }
        break;

      // Checksum and message processing
      //
      case 4:
        gDiydStep++;
        if (gDiydChecksum1 != data) {
          gDiydStep = 0;
        }
        break;
      case 5:
        gDiydStep = 0;
        if (gDiydChecksum2 != data) {
          break;
        }

        /* Fill in the vars we need for cl-osd */
        gGpsLastData.fix            = gDiydBuffer.msg.fix_type == FIX_3D;
        gGpsLastData.pos.latitude   = gDiydBuffer.msg.latitude;
        gGpsLastData.pos.longitude  = gDiydBuffer.msg.longitude;
        gGpsLastData.pos.altitude   = (int16_t) (gDiydBuffer.msg.altitude / 100);
        gGpsLastData.speed          = (uint16_t) gDiydBuffer.msg.ground_speed;
        gGpsLastData.angle          = (uint16_t) gDiydBuffer.msg.ground_course;
        gGpsLastData.sats           = gDiydBuffer.msg.satellites;
        gGpsLastData.date           = gDiydBuffer.msg.utc_date;
        gGpsLastData.gTime          = gDiydBuffer.msg.utc_time / 1000;
        gGpsLastData.checksumValid  = 1;

        finishGpsDecoding();
		    break;
    }
  }
} 

#endif //GPS_ENABLED

#endif /* GPS_DIYD_H_ */
