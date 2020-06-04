#include <Arduino.h>
#include <AdeonGSM.h>

#define DEFAULT_PIN         1234

#define PIN_LEN             4
#define PN_LEN              12
#define RIGHTS_LEN          1

enum FC{
    NONE = 0,
    ADMIN_INIT,
    NEW_USER,
    DELETE_USER,
    NEW_PIN,
    DELETE_ALL
};

class AdeonConfig{
    private:
        const char* defaultPin = "1234";
        const char configSymbol = '#';
        const char separatorSymbol = '-';
        const char endSymbol = ';';

        const char *adminInicialization = "AI";
        const char *newUser = "NU";
        const char *deleteUser = "DU";
        const char *newPin = "NP";
        const char *deleteAll = "DA";

        char _pin[PIN_LEN + 1];
        char _adminPn[PN_LEN + 1];

        bool _firstConfig = false;
        char* _msgBuf;

        Adeon *_pAdeon;
        FC _function = NONE;

    public:
        AdeonConfig(Adeon *pAdeon);
        void begin();
        void begin(const char* pin);
        void begin(const char* pin, const char* adminPn);

        bool isFirstConfig();
        bool isMsgFromAdmin(char* pPn);
        bool isConfigMsg(char* pMsg);

        void readConfigMsg(char* pMsg, char* userPn);
        void readUsersFromEeprom();

        void setDefaultConfig();

    private:
        //parsing
        void parseMsg(char* userPn);
        bool isPinValid();
        void selectFunction(char* pMsg);

        void parseNewPin(char* pMsg);
        void parseNewUser(char* pMsg);
        void parseDeleteUser(char* pMsg);

        bool isNumber(char* pStr, uint8_t n);
        
        void setPin(const char* pin);
        void setAdmin(const char* adminPn);
        void setUserRights(const char* userPn, uint16_t rights);
};