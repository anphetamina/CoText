# CoText

## Setup
CoText has some requirements that need to be met to run it.

### Database 
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
|TestDocument1  |test                      |
|AAA            |test, test2               |

### Filesystem
You need some space to store the document that are saved in the current working directory in which the server is run.
In addition the profile pictures of the user are saved in the server/profilePictures.This folder *should not* be deleted. If you need space and you will to loose all the pictures to gain space, you can delete all its content.

### Security requirements
To use the software you need a valid SSL certificate. 
In the repo an example certificate and its private key are given for localhost (dev) usage. 

### Platforms
The current tested platform are:
- OSX (Catalina 10.15.6)
- Windows 10

### Toolchain
CMake > 3.14 and a working version of Qt5 is required.
You will also need the QtMysql driver library on your system.

## Other Information

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


## TODO-s
- Store authentication data on client to avoid repeating the login every time
- Use compression in the DocumentOk packet to improve bandwidth usage and trasfer time for large document
- Improve UX

### Security concerns
The security was not our first priority in this stage.
A non exhaustive list of issues is the following:
- SQLInjection protection in many parametric queries based on user given values
- Use an hash function to avoid storing the password in clear
- Use safer random generator functions and algorithm
- Some kind of DoS attack could represent an issue
