/***************************************************
  This is a library for our optical Fingerprint sensor
  Designed specifically to work with the Adafruit Fingerprint sensor
  ----> http://www.adafruit.com/products/751
  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Fingerprint.h"
#include <util/delay.h>
#if (ARDUINO >= 100)
  #include <SoftwareSerial.h>
#else
  #include <NewSoftSerial.h>
#endif

//static SoftwareSerial mySerial = SoftwareSerial(2, 3);

#if ARDUINO >= 100
Adafruit_Fingerprint::Adafruit_Fingerprint(SoftwareSerial *ss) {
#else
Adafruit_Fingerprint::Adafruit_Fingerprint(NewSoftSerial *ss) {
#endif
  thePassword = 0;
  theAddress = 0xFFFFFFFF;

  mySerial = ss;
}

void Adafruit_Fingerprint::begin(uint16_t baudrate) {
  delay(1000);  // one second delay to let the sensor 'boot up'

  mySerial->begin(baudrate);
}

boolean Adafruit_Fingerprint::verifyPassword(void) {
  uint8_t packet[] = {FINGERPRINT_VERIFYPASSWORD,
                      (thePassword >> 24), (thePassword >> 16),
                      (thePassword >> 8), thePassword};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 7, packet);
  uint8_t len = getReply(recvPacket);

  if ((len == 1) && (recvPacket[0] == FINGERPRINT_ACKPACKET) && (recvPacket[1] == FINGERPRINT_OK))
    return true;


  Serial.print("\nGot packet type "); Serial.print(packet[0]);
  for (uint8_t i=1; i<len+1;i++) {
    Serial.print(" 0x");
    Serial.print(packet[i], HEX);
  }

  return false;
}
//baudrate
uint8_t Adafruit_Fingerprint::setSysParaBaud(void) {
  uint8_t packet[] = {0x0e,4,1};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(packet);

  if ((len != 1) && (packet[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return packet[1];
}
//package size
uint8_t Adafruit_Fingerprint::setSysParaSize(void) {
  uint8_t packet[] = {0x0e,6,0};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(packet);

  if ((len != 1) && (packet[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return packet[1];
}

uint8_t Adafruit_Fingerprint::getImage(void) {
  uint8_t packet[] = {FINGERPRINT_GETIMAGE};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 3, packet);
  uint8_t len = getReply(recvPacket);

  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

uint8_t Adafruit_Fingerprint::image2Tz(uint8_t slot) {
  uint8_t packet[] = {FINGERPRINT_IMAGE2TZ, slot};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);

  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}


uint8_t Adafruit_Fingerprint::createModel(void) {
  uint8_t packet[] = {FINGERPRINT_REGMODEL};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);

  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}


uint8_t Adafruit_Fingerprint::storeModel(uint16_t id) {
  uint8_t packet[] = {FINGERPRINT_STORE, 0x01, id >> 8, id & 0xFF};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);

  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

//read a fingerprint template from flash into Char Buffer 1
uint8_t Adafruit_Fingerprint::loadModel(uint16_t id) {
    uint8_t packet[] = {FINGERPRINT_LOAD, 0x01, id >> 8, id & 0xFF};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    uint8_t len = getReply(recvPacket);
	if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];

}

//read a fingerprint template from flash into Char Buffer 2
uint8_t Adafruit_Fingerprint::loadModeltwo(uint16_t id) {
    uint8_t packet[] = {FINGERPRINT_LOAD, 0x02, id >> 8, id & 0xFF};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    uint8_t len = getReply(recvPacket);

    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];

}

//transfer a fingerprint template from Char Buffer 1 to host computer
uint8_t Adafruit_Fingerprint::getModel(void) {
    uint8_t packet[] = {FINGERPRINT_UPLOAD, 0x01};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    uint8_t len = getReply(recvPacket);

    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];
    return recvPacket[2];
}

//transfer a fingerprint template from Char Buffer 1 to host computer
uint8_t Adafruit_Fingerprint::getModel2(void) {
    uint8_t packet[] = {FINGERPRINT_UPLOAD, 0x02};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    uint8_t len = getReply(recvPacket);

    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];
    return recvPacket[2];
}


uint8_t Adafruit_Fingerprint::deleteModel(uint16_t id) {
    uint8_t packet[] = {FINGERPRINT_DELETE, id >> 8, id & 0xFF, 0x00, 0x01};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    uint8_t len = getReply(recvPacket);

    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];
}

uint8_t Adafruit_Fingerprint::emptyDatabase(void) {
  uint8_t packet[] = {FINGERPRINT_EMPTY};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);

  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

uint8_t Adafruit_Fingerprint::fingerFastSearch(void) {
  fingerID = 0xFFFF;
  confidence = 0xFFFF;
  // high speed search of slot #1 starting at page 0x0000 and page #0x00A3
  uint8_t packet[] = {FINGERPRINT_HISPEEDSEARCH, 0x01, 0x00, 0x00, 0x00, 0xA3};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);

  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;

  fingerID = recvPacket[2];
  fingerID <<= 8;
  fingerID |= recvPacket[3];

  confidence = recvPacket[4];
  confidence <<= 8;
  confidence |= recvPacket[5];

  return recvPacket[1];
}

//transfer a fingerprint template from host computer to Char Buffer 2
uint8_t Adafruit_Fingerprint::uploadModel(void) {
    uint8_t packet[] = {FINGERPRINT_DOWNLOAD, 0x02};
    uint8_t packet2[] = {0x3, 0x1, 0x5D, 0x11, 0x87, 0x0, 0xC0, 0x2, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x2, 0x80, 0x2, 0x80, 0x2, 0x80, 0x6, 0xC0, 0x6, 0xC0, 0xE, 0xE0, 0x1E, 0x6, 0xFD};
uint8_t packet3[] = {0x6, 0xFD, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3C, 0x84, 0xA0, 0x36, 0x4B, 0x89, 0xB, 0x85};
uint8_t packet4[] = {0x60, 0x9E, 0xB, 0x80, 0x48, 0x10, 0x9A, 0xBE, 0x26, 0x93, 0x5E, 0x5E, 0x4A, 0x97, 0x97, 0x9E, 0x35, 0x98, 0xC1, 0xFE, 0x53, 0x9F, 0xAA, 0x9E, 0x4D, 0xAC, 0x17, 0x1E, 0x30, 0xB2, 0x17, 0x9E, 0xE, 0x73};
uint8_t packet5[] = {0x2B, 0xA9, 0xC1, 0xDF, 0xF, 0x5E, 0x28, 0x31, 0x81, 0x9F, 0x12, 0x1D, 0x1C, 0x3C, 0x1E, 0x1A, 0x4, 0xBA, 0x18, 0x9C, 0x5, 0xB2, 0x1D, 0x9E, 0x9C, 0x32, 0x1A, 0x14, 0x9D, 0xB8, 0x1D, 0x14, 0xA, 0xA3};
uint8_t packet6[] = {0x5E, 0xF7, 0x0, 0x0, 0x0, 0x0, 0x9, 0x17, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x99};
uint8_t packet7[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x24, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x48};
uint8_t packet8[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x24, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x48};
uint8_t packet9[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x24, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x48};
uint8_t packet10[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x24, 0x3, 0x1, 0x51, 0xD, 0x7F, 0x0, 0xFF, 0xFE, 0xFF, 0xFE, 0xF9, 0xFE, 0xE0, 0x6, 0xE0, 0x2, 0x8, 0xE2};
uint8_t packet11[] = {0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x0, 0x2, 0x80, 0x2, 0x9, 0xC4, 0x80, 0x6, 0xC0, 0x6, 0xC0, 0xE, 0xE0, 0x3E, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0x2F};
uint8_t packet12[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4C, 0x97, 0xCC, 0x1E, 0x22, 0x9B, 0x21, 0xFE, 0x7, 0x5, 0x38, 0xA2, 0xA1, 0x3E, 0x45, 0x2D, 0x99, 0xFE, 0x24, 0xB0, 0xDD, 0xDE, 0xA, 0x2A};
uint8_t packet13[] = {0x49, 0x33, 0xD6, 0xBE, 0x34, 0xB6, 0x1, 0x7E, 0x1E, 0x37, 0x84, 0x5E, 0x53, 0x3B, 0xAA, 0x1E, 0x3F, 0x97, 0xA3, 0x5A, 0xE, 0x4E, 0x17, 0xB2, 0x1D, 0xBA, 0x43, 0x99, 0xE4, 0x78, 0x1C, 0x30, 0xC, 0x7D};
uint8_t packet14[] = {0x5E, 0x38, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xDE, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x9C};
uint8_t packet15[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x24, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x48};
uint8_t packet16[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x24, 0x0, 0x0, 0x0, 0x0, 0x0, 0x48};
uint8_t packet17[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x24, 0x0, 0x0, 0x0, 0x4E};

    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet2)+2, packet2);
    delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet3)+2, packet3);
    delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet4)+2, packet4);
     delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet5)+2, packet5);
     delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet6)+2, packet6);
    delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet7)+2, packet7);
    delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet8)+2, packet8);
     delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet9)+2, packet9);
     delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet10)+2, packet10);
     delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet11)+2, packet11);
     delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet12)+2, packet12);
    delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet13)+2, packet13);
    delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet14)+2, packet14);
     delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet15)+2, packet15);
     delay(100);
    writePacket(theAddress, FINGERPRINT_DATAPACKET, sizeof(packet16)+2, packet16);
     delay(100);
    writePacket(theAddress, FINGERPRINT_ENDDATAPACKET, sizeof(packet17)+2, packet17);
    uint8_t len = getReply(packet);

    if ((len != 1) && (packet[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return packet[1];
}


uint8_t Adafruit_Fingerprint::getMatch(void) {

confidence = 0xFFFF;

 // matching

      uint8_t packet[] = {FINGERPRINT_MATCH, 0x01};
      writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    uint8_t len = getReply(packet);

  if ((len != 1) && (packet[0] != FINGERPRINT_ACKPACKET))
   return -1;


  confidence = packet[2];
  confidence <<= 8;
  confidence |= packet[3];

  return packet[1];
}


uint8_t Adafruit_Fingerprint::getTemplateCount(void) {
  templateCount = 0xFFFF;
  // get number of templates in memory
  uint8_t packet[] = {FINGERPRINT_TEMPLATECOUNT};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  uint8_t len = getReply(recvPacket);

  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;

  templateCount = recvPacket[2];
  templateCount <<= 8;
  templateCount |= recvPacket[3];

  return recvPacket[1];
}



void Adafruit_Fingerprint::writePacket(uint32_t addr, uint8_t packettype,
				       uint16_t len, uint8_t *packet) {
#ifdef FINGERPRINT_DEBUG
  Serial.print("---> 0x");
  Serial.print((uint8_t)(FINGERPRINT_STARTCODE >> 8), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)FINGERPRINT_STARTCODE, HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(addr >> 24), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(addr >> 16), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(addr >> 8), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(addr), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)packettype, HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(len >> 8), HEX);
  Serial.print(" 0x");
  Serial.print((uint8_t)(len), HEX);
#endif

#if ARDUINO >= 100
  mySerial->write((uint8_t)(FINGERPRINT_STARTCODE >> 8));
  mySerial->write((uint8_t)FINGERPRINT_STARTCODE);
  mySerial->write((uint8_t)(addr >> 24));
  mySerial->write((uint8_t)(addr >> 16));
  mySerial->write((uint8_t)(addr >> 8));
  mySerial->write((uint8_t)(addr));
  mySerial->write((uint8_t)packettype);
  mySerial->write((uint8_t)(len >> 8));
  mySerial->write((uint8_t)(len));
#else
  mySerial->print((uint8_t)(FINGERPRINT_STARTCODE >> 8), BYTE);
  mySerial->print((uint8_t)FINGERPRINT_STARTCODE, BYTE);
  mySerial->print((uint8_t)(addr >> 24), BYTE);
  mySerial->print((uint8_t)(addr >> 16), BYTE);
  mySerial->print((uint8_t)(addr >> 8), BYTE);
  mySerial->print((uint8_t)(addr), BYTE);
  mySerial->print((uint8_t)packettype, BYTE);
  mySerial->print((uint8_t)(len >> 8), BYTE);
  mySerial->print((uint8_t)(len), BYTE);
#endif

  uint16_t sum = (len>>8) + (len&0xFF) + packettype;
  for (uint8_t i=0; i< len-2; i++) {
#if ARDUINO >= 100
    mySerial->write((uint8_t)(packet[i]));
#else
    mySerial->print((uint8_t)(packet[i]), BYTE);
#endif
#ifdef FINGERPRINT_DEBUG
    Serial.print(" 0x"); Serial.print(packet[i], HEX);
#endif
    sum += packet[i];
  }
#ifdef FINGERPRINT_DEBUG
  //Serial.print("Checksum = 0x"); Serial.println(sum);
  Serial.print(" 0x"); Serial.print((uint8_t)(sum>>8), HEX);
  Serial.print(" 0x"); Serial.println((uint8_t)(sum), HEX);
#endif
#if ARDUINO >= 100
  mySerial->write((uint8_t)(sum>>8));
  mySerial->write((uint8_t)sum);
#else
  mySerial->print((uint8_t)(sum>>8), BYTE);
  mySerial->print((uint8_t)sum, BYTE);
#endif
}


uint8_t Adafruit_Fingerprint::getReply(uint8_t packet[], uint16_t timeout) {
  uint8_t reply[20], idx;
  uint16_t timer=0;

  idx = 0;
#ifdef FINGERPRINT_DEBUG
  Serial.print("<--- ");
#endif
while (true) {
    while (!mySerial->available()) {
      delay(1);
      timer++;
      if (timer >= timeout) return FINGERPRINT_TIMEOUT;
    }
    // something to read!
    reply[idx] = mySerial->read();
#ifdef FINGERPRINT_DEBUG
    Serial.print(" 0x"); Serial.print(reply[idx], HEX);
#endif
    if ((idx == 0) && (reply[0] != (FINGERPRINT_STARTCODE >> 8)))
      continue;
    idx++;

    // check packet!
    if (idx >= 9) {
      if ((reply[0] != (FINGERPRINT_STARTCODE >> 8)) ||
          (reply[1] != (FINGERPRINT_STARTCODE & 0xFF)))
          return FINGERPRINT_BADPACKET;
      uint8_t packettype = reply[6];
      //Serial.print("Packet type"); Serial.println(packettype);
      uint16_t len = reply[7];
      len <<= 8;
      len |= reply[8];
      len -= 2;
      //Serial.print("Packet len"); Serial.println(len);
      if (idx <= (len+10)) continue;
      packet[0] = packettype;
      for (uint8_t i=0; i<len; i++) {
        packet[1+i] = reply[9+i];
      }
#ifdef FINGERPRINT_DEBUG
      Serial.println();
#endif
      return len;
    }
  }
}