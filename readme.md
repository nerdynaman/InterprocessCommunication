# Interprocess Communication
To reciprocate usage of IPC's we create two programs `P1` and `P2`. P1 is responsible for generating an array of 50 random strings, each of fixed length. P1 sends a group of five consecutive elements of the array, along with their IDs (which are the indices of the array), to P2 for processing. P2 receives the strings and prints their IDs and strings on the console. P2 then sends back the highest ID received to acknowledge the strings. Upon receiving the acknowledged packet, P1 sends the next five strings, starting from the successor of the acknowledged ID.
### FIFO
There are two fifo's created, one for the purpose of sender to send strings and id to the receiver and the other one for the receiver to send the highest ID recieved back to the sender. Sender after sending 5 sets of message waits for the receiver to send the highest ID recieved back to the sender. Once the sender recieves the highest ID back from the receiver, it sends the next set of messages.

### SOCKETS
The sender and receiver are connected using sockets. Here sender is represented in the form of server and reciever in form of client, server is created and waited upon to recieve a client connection request. After completion of connection the sender sends five sets of message and the ID to the receiver and the receiver then sends the highest ID recieved back to the sender. The sender then sends the next set of messages.

### SHARED MEMORY
Shared memory has initial 4 bytes set for the reciever to update the highest ID recieved. Whenever either of sender or reciever is trying to use the shared memory, it locks the shared memory and then unlocks it after completion of its task. There is one more semaphore used which is used as a switch logic for sender and receiver to have a understanding of whose turn is it to use the shared memory.

## FIFO
1) `generateMessageArray` function is used to create array of 50 random strings of fixed length.
2) `mknod` function is used to create the fifo's.
3) `open` function is used to open the fifo's. 
4) `write` function is used to write the message to the fifo (here we are using file descriptors to access fifo).
5) `read` function is used to read the message from the fifo.
6) `close` function is used to close the fifo's.

    Here we are making two fifo's one for sender to send the message and the other one for the receiver to send the highest ID recieved back to the sender.
### Installation
```bash
$ cd fifo
$ make
```
### Usage
```bash
$ ./sender
$ #in different terminal window
$ ./receiver
```

## Shared Memory

1) `generateMessageArray` function is used to create array of 50 random strings of fixed length.
2) `ftok` function is used to generate a unique key.
3) `shmget` function is used to create the shared memory.
4) `shmat` function is used to attach the shared memory to the process.
5) `shmdt` function is used to detach the shared memory from the process.
6) `sem_open` function is used to create the semaphore.
7) `sem_wait` function is used to lock the semaphore.
8) `sem_post` function is used to unlock the semaphore.
9) `strncpy` function is used to copy the string from one buffer to another.

### Installation
```bash
$ cd sockets
$ mkdir m{1,2}
$ touch io
$ chmod 777 io
$ make
```
### Usage
```bash
$ #first sender has to be run
$ ./sender
$ #now in different terminal window run receiver
$ ./listener
```

## Sockets

1) `generateMessageArray` function is used to create array of 50 random strings of fixed length.
2) `socket` function is used to create the socket.
3) `bind` function is used to bind the socket to the port.
4) `listen` function is used to listen for the client connection request.
5) `accept` function is used to accept the client connection request.
6) `send` function is used to send the message to the client.
7) `recv` function is used to recieve the message from the client.

### Installation
```bash
$ cd sockets
$ make
```
### Usage
```bash
$ #first sender has to be run
$ ./sender
$ #now in different terminal window run receiver
$ ./listener
```
once sender is run untill it is killed server will remain up so after running sender you can run receiver any number of times but at one time only one receiver can be run. But if you want to have `n` recievers connected to server at same time then that is also possible by 

    listen(sock, n);