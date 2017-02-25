# MyQQ

Two clients communicate through server

By using fork() to create two server process, each server connect with client by TCP protocol

Two servers communicate by two pipes.

There are three steps to complete one one way communication between clients

(1) Client one give a message to server one.

(2) Then server one will transfer to server two by pipe

(3) Finally, server two transfer the message to client two.
