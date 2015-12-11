/*
SYSLOG-MSG      = HEADER SP STRUCTURED-DATA [SP MSG]

HEADER          = PRI VERSION SP TIMESTAMP SP HOSTNAME
                        SP APP-NAME SP PROCID SP MSGID
  PRIVAL Facility 16 (local use 0) = 128 (0x80)

  PRIVAL Severities

  Numerical         Severity
             Code

              0       Emergency: system is unusable
              1       Alert: action must be taken immediately
              2       Critical: critical conditions
              3       Error: error conditions
              4       Warning: warning conditions
              5       Notice: normal but significant condition
              6       Informational: informational messages
              7       Debug: debug-level messages

*/
#ifndef SYSLOG_H
#define SYSLOG_H
#include "application.h"

const String SYSLOG_EMERG=String("<128>");
const String  SYSLOG_ALERT= String("<129>");
const String  SYSLOG_CRIT= String("<130>");
const String  SYSLOG_ERR= String("<131>");
const String  SYSLOG_WARN= String("<132>");
const String  SYSLOG_NOTICE= String("<133>");
const String  SYSLOG_INFO= String("<134>");
const String  SYSLOG_DEBUG= String("<135>");
const String  SP= String(" ");
const String  NILVAL= String("-");
const String  SYSLOG_VER= String("1");
const String  SYSLOG_APPNAME= String("BrewPi");



class Syslog{
public:
	Syslog(String hostname, IPAddress syslogServer, int syslogPort);
	String timestamp();
	void log(String msg);
	void log(String msgtype, String msg);
private:
	String hname;
	IPAddress server;
	int port;
	char buf[480]; // RFC 5426 UDP transport for syslog specifies a 480 octet min max datagram size
	UDP udp;
};

#endif

