#ifndef _NEOTEE_H_
#define _NEOTEE_H_

//------------------------------------------------------
// NeoTee
// Copyright (C) 2016, SlashDevin
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//

#include <Stream.h>

// This library is like the UNIX "tee" command, which splits an output stream 
// into two streams, but with N branches.  On the Arduino, an output stream is
// instance of the Arduino Print class, and the flush method is in Stream.
//
// Simply declare your own array of streams:
//
//     Stream *streams[2];
//
// Pass the array and its size in to the NeoTee constructor:
//
//     NeoTee tee( streams, sizeof(streams)/sizeof(streams[0]) );
//
// Any Stream operation on "tee" will be duplicated on all Streams in the array.
//
//     tee.println( F("This is sent to all Stream instances in the array.") );
//
// Because there is no base class with "begin" and "end" methods, you
//   must call those explicitly on the individual Streams (e.g., Serial.begin)

class NeoTee : public Stream
{
  Stream **Streams;
  uint8_t  NumStreams;

public:

  NeoTee( Stream **streams, uint8_t numStreams )
    {
      Streams    = streams;
      NumStreams = numStreams;
    }
  
  virtual void flush()
    {
      for (uint8_t i=0; i<NumStreams; i++)
        Streams[i]->flush();
    }
    
  virtual size_t write(uint8_t c)
    {
      size_t minWritten = 0;
      for (uint8_t i=0; i<NumStreams; i++) {
        size_t written = Streams[i]->write( c );
        if (minWritten > written)
          minWritten = written;
      }
    }

  using Stream::write; // make the other overloads visible

  virtual int available()
    {
      int total = 0;
      for (uint8_t i=0; i<NumStreams; i++)
        total += Streams[i]->available();
      return total;
    }

  virtual int read()
    {
      for (uint8_t i=0; i<NumStreams; i++)
        if (Streams[i]->available())
          return Streams[i]->read();
      return -1;
    }

  virtual int peek()
    {
      for (uint8_t i=0; i<NumStreams; i++)
        if (Streams[i]->available())
          return Streams[i]->peek();
      return -1;
    }

};

#endif