# NeoTee
Copy one Arduino Stream to multiple Streams

This library is like the UNIX "tee" command, which splits an output stream into 
two streams, but with N branches.  On the Arduino, an output stream is instance of the 
Arduino Print class.  Unfortunately, the flush method is in Stream.

Simply declare an array of streams:

    Stream *streams[2];

... and pass the array and its size to the NeoTee constructor:

    NeoTee tee( streams, sizeof(streams)/sizeof(streams[0]) );

Any Stream operation on "tee" will be duplicated on all Streams in the array.

    tee.println( F("This is sent to all Stream instances in the array.") );

Because there is no base class with "begin" and "end" methods, you
must call those explicitly on the individual Streams (e.g., Serial.begin).

Although it is questionable, `read` and `peek` are also implemented.
This kind of random "merging" of an input stream is not a UNIX feature.  :)  They 
have to be provided because they are marked as pure virtual in the Stream class

NOTE: You can mix different kinds of Streams in the array:

    NeoSWSerial gps_port( 3, 4 );
    Stream *justSerial[2] = { &Serial, &gps_port };
    
