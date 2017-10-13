# Skype SuperNode Implementation
(by Gopinath N & Aalekhya M)

    # Compiling locally
          Server        -> gcc server.c -o server -lnsl
          Client        -> gcc client.c -o client -lnsl
          SuperNode 1   -> gcc sn1.c -o sn1 -lnsl
          SuperNode 2   -> gcc sn2.c -o sn2 -lnsl
          SuperNode 3   -> gcc sn3.c -o sn3 -lnsl
          SuperNode 4   -> gcc sn4.c -o sn4 -lnsl
          
    #Compiling remotely
          
          Server        -> gcc server.c -o server -lnsl -lsocket
          Client        -> gcc client.c -o client -lnsl -lsocket
          SuperNode 1   -> gcc sn1.c -o sn1 -lnsl -lsocket
          SuperNode 2   -> gcc sn2.c -o sn2 -lnsl -lsocket
          SuperNode 3   -> gcc sn3.c -o sn3 -lnsl -lsocket
          SuperNode 4   -> gcc sn4.c -o sn4 -lnsl -lsocket
          

    # Running
        Server          -> ./server serverPortNumber
                            example: ./server 4539
        client 1        -> ./client serverPortNumber superNodePortNumber clinetOneOwnPortNumber
                            example: ./clinet 4539 6765 3489
        client 2        -> ./client serverPortNumber superNodePortNumber clinetTWOOwnPortNumber
                            example: ./clinet 4539 6766 7429
        SuperNode 1     -> ./sn1 SN1portNumber SN2portNumber SN3portNumber SN3portNumber
                            example: ./sn1 6765 6766 6768 6769
        SuperNode 2     -> ./sn2 SN2portNumber SN3portNumber SN4portNumber SN1portNumber
                            example: ./sn2 6766 6768 6769 6765
        SuperNode 3     -> ./sn3 SN3portNumber SN4portNumber SN1portNumber SN2portNumber
                            example: ./sn3 6768 6769 6765 6766
        SuperNode 4     -> ./sn4 SN4portNumber SN1portNumber SN2portNumber SN3portNumber
                            example: ./sn4 6769 6765 6766 6768
