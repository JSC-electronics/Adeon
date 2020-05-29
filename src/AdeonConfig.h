#include <Arduino.h>
#include <AdeonGSM.h>

#define DEFAULT_PIN         1234

#define PIN_LEN             4
#define ADMIN_PN_LEN        12

class AdeonConfig{
    private:
        const char* defaultPin = "1234";
        const char configSymbol = '#';
        char _pin[4];
        char _adminPn[12];
        bool _firstConfig = false;
        Adeon *_pAdeon;

    public:
        AdeonConfig(Adeon *pAdeon);
        void begin();
        void begin(const char* pin);
        void begin(const char* pin, const char* adminPn);

        bool isFirstConfig();
        bool isConfigMsg(char* pMsg);
        bool isMsgFromAdmin(char* pPn);

        void parseBuf(char* pMsg);

        void setDefaultConfig();

    private:
        void readUsersFromEeprom();
        
        void setPin(const char* pin);
        void setAdmin(const char* adminPn);
        void setUserRights(const char* userPn, uint8_t rights);
};