#define UARTRX 16
#define UARTTX 17
#define SquelchIO 18
#define PTTIO 19
#define SleepIO 23
#define BoostIO 27
#define analogOut 25
#define analogIn 32

class DRA818V {
  public:

    void send(String request,String response) {
      request="AT+" + request + "\r\n"; response="+" + response + "\r"; String receive="";
      while (receive!=response) { delay(200); Serial.print(request); Serial2.print(request); receive=Serial2.readStringUntil('\n'); Serial.println(receive); } }

    void setPTT(bool value) {
      if (value) { digitalWrite(PTTIO,LOW); Serial.println("PTT: TX"); }
      else { digitalWrite(PTTIO,HIGH); Serial.println("PTT: RX"); } }

    void setSleep(bool value) {
      if (value) { digitalWrite(SleepIO,LOW); Serial.println("Sleep Mode: On"); }
      else { digitalWrite(SleepIO,HIGH); Serial.println("Sleep Mode: Off"); } }

    void setBoost(bool value) {
      if (value) { digitalWrite(BoostIO,HIGH); Serial.println("TX Power: 1W"); }
      else { digitalWrite(BoostIO,LOW); Serial.println("TX Power: 0.5W"); } }

    void connect() {
      send("DMOCONNECT","DMOCONNECT:0"); }

    void setVolume(uint8_t volume) {
      send("DMOSETVOLUME=" + String(volume),"DMOSETVOLUME:0"); }

    void setFilter(bool emphasis, bool highpass, bool lowpass) {
      send("SETFILTER=" + String(!emphasis) + "," + String(!highpass) + "," + String(!lowpass),"DMOSETFILTER:0"); }

    void setGroup(double txFreq, double rxFreq, uint8_t squelch=0, String txCTCSS="0000", String rxCTCSS="0000", uint8_t bandwidth=0) {
      send("DMOSETGROUP=" + String(bandwidth) + "," + String(txFreq,4) + "," + String(rxFreq,4) + "," + txCTCSS + "," + String(squelch) + "," + rxCTCSS,"DMOSETGROUP:0"); }

    void begin(double txFreq=144, double rxFreq=144) {
      Serial.begin(115200);
      pinMode(SquelchIO,INPUT);
      pinMode(PTTIO,OUTPUT); pinMode(SleepIO,OUTPUT); pinMode(BoostIO,OUTPUT);
      pinMode(analogOut,OUTPUT); pinMode(analogIn,INPUT);
      setPTT(false); setSleep(false); setBoost(false);
      Serial2.begin(9600,SERIAL_8N1,UARTRX,UARTTX);
      connect(); setVolume(4); setFilter(true,true,true); setGroup(txFreq,rxFreq); } };
