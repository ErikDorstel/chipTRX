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

    void setPTT(bool ptt=false, bool boost=false) {
      if (ptt) {
        if (boost) { digitalWrite(BoostIO,LOW); digitalWrite(PTTIO,LOW); Serial.println("PTT: TX 1W"); }
        else { digitalWrite(BoostIO,HIGH); digitalWrite(PTTIO,LOW); Serial.println("PTT: TX 0.5W"); } }
      else { digitalWrite(PTTIO,HIGH); digitalWrite(BoostIO,HIGH); Serial.println("PTT: RX"); } }

    void setSleep(bool sleep=false) {
      if (sleep) { digitalWrite(SleepIO,LOW); Serial.println("Sleep Mode: On"); }
      else { digitalWrite(SleepIO,HIGH); Serial.println("Sleep Mode: Off"); } }

    bool getSquelch() {
      static bool squelch=true; if (digitalRead(SquelchIO)!=squelch) {
        if (digitalRead(SquelchIO)) { Serial.println("Squelch: Closed"); squelch=true; }
        else { Serial.println("Squelch: Open"); squelch=false; } } return squelch; }

    void getAudioLevel() {
      static int minLevel=10000; static int maxLevel=0; static uint32_t levelTimer=millis()+5000;
      int level=analogRead(analogIn); minLevel=level<minLevel?level:minLevel; maxLevel=level>maxLevel?level:maxLevel;
      if (millis()>=levelTimer) { levelTimer=millis()+5000;
        Serial.println("Audio Level: min " + String(minLevel) + " - max " + String(maxLevel));
        minLevel=10000; maxLevel=0; } }

    void connect() {
      send("DMOCONNECT","DMOCONNECT:0"); }

    void setVolume(uint8_t volume=4) {
      send("DMOSETVOLUME=" + String(volume),"DMOSETVOLUME:0"); }

    void setFilter(bool emphasis=true, bool highpass=true, bool lowpass=true) {
      send("SETFILTER=" + String(!emphasis) + "," + String(!highpass) + "," + String(!lowpass),"DMOSETFILTER:0"); }

    void setGroup(double txFreq, double rxFreq, uint8_t squelch=0, String txCTCSS="0000", String rxCTCSS="0000", uint8_t bandwidth=0) {
      send("DMOSETGROUP=" + String(bandwidth) + "," + String(txFreq,4) + "," + String(rxFreq,4) + "," + txCTCSS + "," + String(squelch) + "," + rxCTCSS,"DMOSETGROUP:0"); }

    void begin(double txFreq=144, double rxFreq=144) {
      Serial.begin(115200);
      pinMode(SquelchIO,INPUT);
      pinMode(PTTIO,OUTPUT); pinMode(SleepIO,OUTPUT); pinMode(BoostIO,OUTPUT);
      pinMode(analogOut,OUTPUT); pinMode(analogIn,INPUT); analogSetPinAttenuation(analogIn,ADC_0db); analogReadResolution(12);
      setPTT(); setSleep(); getSquelch();
      Serial2.begin(9600,SERIAL_8N1,UARTRX,UARTTX);
      connect(); setVolume(); setFilter(); setGroup(txFreq,rxFreq); } };
