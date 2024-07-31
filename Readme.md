# Bencode Parser
very simplee Bencode Parser implemented in a single header file.
Stores data in a linkedlisted which is then traversed.

# Test
Provided simple test

# input:
"d8:announce35:https://torrent.ubuntu.com/announce13:announce-listll35:https://torrent.ubuntu.com/announceel40:https://ipv6.torrent.ubuntu.com/announceee7:comment29:Ubuntu CD releases.ubuntu.com10:created by13:mktorrent 1.113:creation datei1714047929e4:infod6:lengthi275	4981888e4:name34:ubuntu-24.04-live-server-amd64.iso12:piece lengthi262144e6:piecesee"

# Output:
``` C
String
announce

String
https://torrent.ubuntu.com/announce

String
announce-list

String
https://torrent.ubuntu.com/announce

List

String
https://ipv6.torrent.ubuntu.com/announce

List

List

String
comment

String
Ubuntu CD releases.ubuntu.com

String
created by

String
mktorrent 1.1

String
creation date

Integer
1714047929

String
info

String
length

Integer
1345178112

String
name

String
ubuntu-24.04-live-server-amd64.iso

String
piece length

Integer
262144

String
pieces

Dictionary

Dictionary

Resources released successfully```

# How to use
1. Include the header file in yourr main project.
2. If you want to use this repository for your project, all you need to do is create a directorry for your source files and within the Makefile update the value of the *SRC_DIR* to your new direcotry where you will place your source files.