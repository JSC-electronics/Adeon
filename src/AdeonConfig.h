#include <Arduino.h>
#include <AdeonGSM.h>

#define DEFAULT_PIN         1234

class AdeonConfig{
    private:
        char* _pin[4];
        char* _adminPn[12];
        bool _firstConfig = false;
        Adeon *_pAdeon;

    public:
        AdeonConfig(Adeon *_pAdeon);
        AdeonConfig(Adeon *_pAdeon, const char* pin);
        AdeonConfig(Adeon *_pAdeon, const char* pin, const char* adminPn);
        void begin();

        bool isFirstConfig();
        bool isConfigMsg();
        bool isMsgFromAdmin();

        void waitForConfig();
        void parseBuf(char* pMsg);

        void setDefaultConfig();

    private:
        bool isConfigEmpty();
        void readConfigFromEeprom();
        
        void setPin();
        void setAdmin();
        void setNewUser();
        void setUserRights();
        
        void deleteUser();
};