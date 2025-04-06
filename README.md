# 📝 Real-Time Collaborative Editor (C++ with Boost.Beast)

This project is a **real-time collaborative editing system** built from scratch in C++ using **Boost.Beast** for WebSocket communication. It mimics the core functionality behind Google Docs-like platforms where multiple users can connect and edit, but with controlled access using an editing lock mechanism.

## 🚀 Features

- 📡 WebSocket-based real-time communication (Boost.Beast)
- 🧠 Edit locking mechanism: only one client can edit at a time
- 🔒 Mutual exclusion logic with client-server synchronization
- 💬 Live broadcast messages to all clients
- ❌ Graceful handling of disconnections

## 🛠️ Technologies Used

- **C++17**
- **Boost.Beast** (WebSockets)
- **Boost.Asio** (Networking)
- **CMake** for build system

## 🖥️ How It Works

- The **server** handles multiple WebSocket connections and manages which client holds the "edit lock".
- Clients can connect, send messages, and request to start editing.
- If editing is already in progress by another client, the request is denied.
```
+---------------------+          +---------------------+         +---------------------+
|   Client 1 (CLI)    | <--->    |     Server (C++)    | <--->   |   Client 2 (CLI)    |
|  Sends commands     |          |  Manages sessions   |         |  Waits for lock     |
+---------------------+          +---------------------+         +---------------------+
        ↑                                 ↑                               ↑
        |                                 |                               |
    User Types                        Accepts Socket                Lock Granted/Denied

```
## 🧪 Output Screenshot

![image](https://github.com/user-attachments/assets/a38a2371-5b36-48b6-a644-b5a6630ed6a0)


> The above output shows real-time interaction between two clients where only one is allowed to edit at a time.


## 🏃‍♂️ Run Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/collaborative-editing.git
cd collaborative-editing
```
### 2. Build the project 
```
mkdir build && cd build
cmake ..
cmake --build .
```
### 3. Run the server 
```
./src/Debug/editor_server
```
### 4. Run the client(over multiple terminals)
```
./src/Debug/editor_client
```
