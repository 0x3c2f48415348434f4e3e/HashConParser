#include<bencodeParser.h>

int main(){
	Uchar* example = (Uchar *)"d8:announce35:https://torrent.ubuntu.com/announce13:announce-listll35:https://torrent.ubuntu.com/announceel40:https://ipv6.torrent.ubuntu.com/announceee7:comment29:Ubuntu CD releases.ubuntu.com10:created by13:mktorrent 1.113:creation datei1714047929e4:infod6:lengthi275	4981888e4:name34:ubuntu-24.04-live-server-amd64.iso12:piece lengthi262144e6:piecesee";

	lexer(example);
	readBencode(Head);
	releaseResources(Head);
}
