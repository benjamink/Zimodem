/*
   Copyright 2016-2017 Bo Zimmerman

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

const int MAX_COMMAND_SIZE=256;

enum ZResult
{
  ZOK,
  ZERROR,
  ZCONNECT,
  ZNOCARRIER,
  ZIGNORE,
  ZIGNORE_SPECIAL
};

enum ConfigOptions
{
  CFG_WIFISSI=0,
  CFG_WIFIPW=1,
  CFG_BAUDRATE=2,
  CFG_EOLN=3,
  CFG_FLOWCONTROL=4,
  CFG_ECHO=5,
  CFG_RESP_SUPP=6,
  CFG_RESP_NUM=7,
  CFG_RESP_LONG=8,
  CFG_LAST=8,
};

enum FlowControlType
{
  FCT_DISABLED=0,
  FCT_NORMAL=1,
  FCT_AUTOOFF=2,
  FCT_MANUAL=3,
  FCT_INVALID=4
};

enum StreamFlag
{
  FLAG_DISCONNECT_ON_EXIT=1,
  FLAG_PETSCII=2,
  FLAG_TELNET=4,
  FLAG_ECHO=8,
  FLAG_XONXOFF=16
};

class ZCommand : public ZMode
{
  friend class WiFiClientNode;

  private:
    char *CRLF="\r\n";
    char *LFCR="\r\n";
    char *LF="\n";
    char *CR="\r";
    char BS=8;
    char ringCounter = 1;
    
    uint8_t nbuf[MAX_COMMAND_SIZE];
    int eon=0;
    int lastServerClientId = 0;
    WiFiClientNode *current = null;
    bool XON=true;
    bool autoStreamMode=false;
    FlowControlType flowControlType=FCT_NORMAL;
    unsigned long lastNonPlusTimeMs = 0;
    unsigned long currentExpiresTimeMs = 0;
    char *tempDelimiters = NULL;
    char *tempMaskOuts = NULL;
    char *delimiters = NULL;
    char *maskOuts = NULL;
    String previousCommand = "";
    String currentCommand = "";
    WiFiClientNode *nextConn=null;

    byte CRC8(const byte *data, byte len);
    int makeStreamFlagsBitmap(const char *dmodifiers);

    bool readSerialStream();
    ZResult doSerialCommand();
    void setConfigDefaults();
    void parseConfigOptions(String configArguments[]);
    void setBaseConfigOptions(String configArguments[]);
    void reSaveConfig();
    void reSendLastPacket(WiFiClientNode *conn);
    void acceptNewConnection();
    void sendConnectionNotice(int nodeId);
    void sendNextPacket();

    ZResult doResetCommand();
    ZResult doNoListenCommand();
    ZResult doBaudCommand(int vval, uint8_t *vbuf, int vlen);
    ZResult doTransmitCommand(int vval, uint8_t *vbuf, int vlen, bool isNumber, const char *dmodifiers, const int crc8);
    ZResult doLastPacket(int vval, uint8_t *vbuf, int vlen, bool isNumber);
    ZResult doConnectCommand(int vval, uint8_t *vbuf, int vlen, bool isNumber, const char *dmodifiers);
    ZResult doWiFiCommand(int vval, uint8_t *vbuf, int vlen);
    ZResult doDialStreamCommand(int vval, uint8_t *vbuf, int vlen, bool isNumber, const char *dmodifiers);
    ZResult doAnswerCommand(int vval, uint8_t *vbuf, int vlen, bool isNumber, const char *dmodifiers);
    ZResult doHangupCommand(int vval, uint8_t *vbuf, int vlen, bool isNumber);
    ZResult doEOLNCommand(int vval, uint8_t *vbuf, int vlen, bool isNumber);
    ZResult doInfoCommand(int vval, uint8_t *vbuf, int vlen, bool isNumber);

  public:
    int packetSize = 127;
    bool suppressResponses;
    bool numericResponses;
    bool longResponses;
    boolean doEcho;
    bool doFlowControl;
    String EOLN;
    char EC='+';
    char *ECS="+++";

    ZCommand();
    void loadConfig();
    void serialIncoming();
    void loop();
};

