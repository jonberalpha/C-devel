--------------------------------------------------------------------------------
Task:    ASCII-Art Server (Network Programming)
Author:  Berger Jonas
--------------------------------------------------------------------------------

Description:
The application consists of an ascii-server and an ascii-client program.
The ascii-client sends a request to the ascii-server containing a request header
and a JPEG-file. The ascii-server reply to this request with 
an ASCII-art representation of the JPEG-image in the request.

--------------------------------------------------------------------------------

Building:
Build the executable-programs using:
make

Delete created executables using:
make clean

--------------------------------------------------------------------------------

Usage:
Server:
The port used by the server is passed via command-line option and
must be between 4952 and 65535 to ensure that the server does not operate on 
reserved ports.

Example:
./ascii_server 6776

Client:
The client provides the following command-line interface:

  ascii_client [OPTIONS] <Filename>

    -s <hostname or ip> : Required; The address under which the server 
                                    can be reached
    -p <port> : Required; The port number used by the server program
                          (must be between 4952 and 65535)
    -H : Optionally generate HTML output (default: text-only)
    -w <number> : Optional; WIDTH as Dimension when specified alone, 
                            SIZE as Dimension when specified together with -h
    -h <number> : Optional; HEIGHT as Dimension when specified alone, 
                            SIZE as Dimension when specified together with -w
    -i : Optionally invert the image before converting it to ASCII art 
         (may improve visibility)
    <Filename> : Required; The file to convert to ASCII art.

Examples:
./ascii_client -s 127.0.0.1 -p 6776 car2.jpg
./ascii_client -p 6776 -s 127.0.0.1 -w 40 -h 15 -i -H car2.jpg
./ascii_client -s 127.0.0.1 -p 6776 car2.jpg -i

--------------------------------------------------------------------------------

Example-Executions:

Server:
./ascii_server 6776
Server running ...

Client:
./ascii_client -s 127.0.0.1 -p 6776 -w 40 -h 15 -i car2.jpg
                                        
                                        
                                        
                                        
                  .;,,,;xl;:cll;,       
               'ol'    .K,    ,xd       
            .l0d'.....'cKl,,;;;oK.      
       .,lkKKKKKKKKKKKKKKKKKKKKKKo      
     ,OKOkkkKKKKKKKKKKKKKKKOddd0Kk      
     0KoxKK0lKKKKKKKK00000ooKKO:K0;.    
     ,l'0KKK;cllllllllllll'0KKKlc::'    
      ...;:,...............':c,....     
                                        
                                        
 

