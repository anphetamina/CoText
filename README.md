# CoText
## :question: What is?
Cotext is an implementation of a text enriched collaborative editor.
With CoText the users are able to perform operation on the same document at the same time.

In particular, any user will be able to:
- :white_check_mark: Create an account/login
- :white_check_mark: Create a document
- :white_check_mark: Invite an other user by sharing an invitation code OOB
- :white_check_mark: Use different text enrichment features like bold, italics, size, ..
- :white_check_mark: Use actions enrichment
- :white_check_mark: See which users are current working on the document and where their cursor is 

Additional features are:
- :white_check_mark: Persistency (auto-saving performed by server)
- :white_check_mark: Multi-platform compatibility
- :white_check_mark: Low footprint on bandwith, memory and CPU.

## :question: How to run CoText?
If you want to start server on a given port  you can specify it as the first parameter. If no port is given the server will use the default 12345.

Example:
```bash
$ server 12345
```

For the client you can configure the server address by creating a client.config file in the client's directory.
Example content for a client.config file used for a server running on localhost and using  the default port is the following:

```plaintext
wss://localhost:12345
```

But using a similar defined config file is the same as not using anything at all. The client will fallback to this if no given file is found.
## :grey_exclamation: Setup
CoText has some requirements and some setup steps needed to run it.

### :floppy_disk: Database 
A MySQL server running on the server side is required.
The schema and some testing entry are given in the example file named: [DBSample.sql](https://github.com/anphetamina/CoText/blob/master/DBSample.sql).

You may want to change the parameters required to enstablish the connection with the DB.
You can look at the header file named 
[DBAuthData.h](https://github.com/anphetamina/CoText/blob/4fef2fbff233b1611bb66b54a1ea847958d9d56e/server/DBAuthData.h#L3) and changes the constant defined values.
You also need the [QTMysql driver installed and configured](https://stackoverflow.com/questions/6483523/qt-how-to-getcompile-mysql-driver) to run the server. 

When using the provided sql file the following account are available:
| Email         | Password      | Name  |
| ------------- |:-------------:| -----:|
|test@test.test | test          | Mario|
|test2@test2.test2 | test2      | Mario2|
|test3@test3.test3 | test3      | Mario3|

In addition there are 2 empty document already loaded:
| DocName       | User with privilege      |
| ------------- |:------------------------:|
|Document1      |test                      |
|AppuntiSofteng |test, test2               |

### :file_folder: Filesystem
You need some space to store the document that are saved in the current working directory in which the server is run.
In addition the profile pictures of the user are saved in the server/profilePictures.

This folder *should not* be deleted. If you need space and you will to loose all the pictures to gain space, you can delete all its content.

### :closed_lock_with_key: Security requirements
To use the software you need a valid SSL certificate. 
In the repo an example (self-signed) certificate and its private key are given for localhost (dev) usage. 

### :computer: Platforms 
The current tested platform are:
- OSX (Catalina 10.15.6)
- OSX (Mojave 10.14.6)
- Windows 10

### :hammer: Toolchain
CMake > 3.14 and a working version of Qt5 is required.
You will also need the QtMysql driver library on your system.

## :information_source: Other Information 

### Network stack
By default CoText use a websocket server running on port 12345.
The websocket (over TLS) protocol was adopted because its peculiar performance for realtime application.
The architecture is a client-server architecture.
To have a look at the available low level packet exchanged between client and server, you can see the Packet class or the constant definition of each packet [in the PacketDef.h](https://github.com/anphetamina/CoText/blob/master/common/PacketDef.h) header file.

### Persistency
Cotext use persistency just on the server side. Here there is the list showing in which way and where each information is stored:
- User informations (except for the picture) and privileges (which document belongs to who) are stored in the DB
- User pictures are stored on filesystem
- Document content (and its metainformation) are also stored on disk

###  Conflict-Free Replicated Data Type (CRDT)
When users make concurrent edits to a shared document, the insert and delete operations must commute and the delete operations must be idempotent.
Commutativity: Concurrent insert and delete operations converge to the same result regardless of the order in which they are applied.
Idempotency: Repeated delete operations produce the same result.

Such properties can be implemented by using a CRDT algorithm.
Our implementation was based on [conclaveApp implementation](https://conclave-team.github.io/conclave-site/#what-is-a-real-time-collaborative-)

### Documentation
Doxygen generated documentation is available [here](https://emmunaf.dev/projects/CoText/Documentation/html/)

## :pushpin: TODO-s
Some other features that could be worth are:
- Store authentication data on client to avoid repeating the login every time
- Use compression in the DocumentOk packet to improve bandwidth usage and trasfer time for large document
- Improve UX
- Improve documentation

### :unlock: Security concerns
The security was not our first priority in this project.
A non exhaustive list of known issues is the following:
- SQLInjection protection in many parametric queries based on user given values
- Use an hash function to avoid storing the password in clear
- Use safer random generator functions and algorithm
- Some kind of particular attacks can cause DoS on the server. This represents a scalability/security issue
