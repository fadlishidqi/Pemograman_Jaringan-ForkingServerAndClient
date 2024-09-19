# Echo Server with Multi-Client Support (Forking)

## Overview
This project demonstrates a simple Echo Server that handles multiple clients concurrently using the **forking** technique. The server listens for incoming connections and forks a new process for each client, allowing multiple clients to connect simultaneously. Each client sends a message, and the server echoes the same message back to the client.

## How It Works
1. The server creates a TCP socket and listens for connections on a specified port.
2. When a client connects, the server forks a child process to handle the communication with that client.
3. Each client sends a message to the server, and the server echoes the message back to the client.
4. If the client sends "bye", the connection is closed.

## Requirements
- GCC or any C compiler
- Unix-based OS (Linux/Mac) or Cygwin for Windows
- Basic understanding of C programming and networking

## How to Run

Compile the Server and Client
Use GCC to compile the server and client code.

```bash
gcc ForkEchoServer.c -o server
gcc ForkEchoClient.c -o client
./server &
./client
netstat -atpn
