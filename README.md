# [CTris](https://github.com/Phorenix/ctris)
A C Language project about the game tris (or also called tic-tac-toe).
(University Project)

## Game

Basically the application is based on a connection **Client/Server**.
More specifically, the server is a **Concurrent Server**, so it can accept more connections from multiple clients.
The game itself is structured so that a **client** can connect to a **server** (that must be listening for new connections).
The **client** in reality will be only playing with a computer. So from the server-side it's all automated (the next *legal move* is calculated **randomly**).

To simplify the application, the player will always be the first one to play and uses as symbol **X**, while the computer uses the **O** symbol.

After the game has ended, the connection is closed, so if the client still wants to play it needs to start a new connection with the server.

Lastly most of the controlls are made by the server (that uses the first *char* of the board sent to send them, such as the results or other errors). But some of them are also made by the client to make the server "lighter".

## Server

As already said the server is *Concurrent*, which means that more clients can play with it simultaneously.

To do this, each time the server `accept` a new connection from a client, with the **System Call** `fork()`  it generates a new **child process** that will manage the game with the client. So the **parent process** can return to listen for new connections.

By default the server is listening for new connections at the `port: 8080` and accept new connections from any clients on the internet.
But when executing the server with `./ctris_server`, it is also possible to specify a new port. For example if we want that the server accept new connections on the port *4444*, we need to run it as `./ctris_server 4444`.

The type of connection used from the server to open a new `socket()` is `SOCK_STREAM`, so basically it uses the **TCP Protocol** for a more secure connection.

## Client

The client of this application is very simple, it just receive the whole **board**, prints it, then asks for the next *legal move* from the player and sends it.

Is important to note that even the client by default it connects on the  `port: 8080` and uses as the `address: 127.0.0.1`, so basically it uses `localhost`.
But, as in the server, even in this case the client can be executed with 2 arguments, that are: `address` and `port`.
So for example if the server has the address *192.168.0.2* and is listenting to the port *4444*, then the client needs to be executed as such: `./ctris_client 192.168.0.2 4444`.

The protocol used from the client needs to be as well `SOCK_STREAM`.

## Notes

Of course the **client** and the **server** to communicate they both need to be run on the same `PORT`.
