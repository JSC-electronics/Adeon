#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_LEN			1023 //bytes
#define BLANK_CELL			0xFF //0xFF is value which means the cell is blank
#define MAX_NUM_OF_USERS	77

#define USER_RECORD_LEN		13 	//bytes
#define PIN_RECORD_LEN		4	//bytes
#define ADMIN_RECORS_LEN	12	//bytes
#define USERS_N_RECORDS_LEN	1	//bytes

#define IDX_DATA_PART		20 	//bytes
#define IDX_PIN				0
#define IDX_ADMIN_PN		1
#define IDX_NUM_USERS		13

class AdeonMem
{
	private:
		char *_returnBuffer;
		uint8_t _lastUserIdx = 0;

	public:
		AdeonMem();

		bool isConfigAvailable();

		uint8_t searchUser();

		uint8_t readNumOfUsers();
		char* readUserRecord(uint8_t numOfUserOrder);
		char* readPin();
		char* readAdminPn();

		void updatePin(const char* pin);
		void updateAdmin(const char* adminPn); 
		void updateUsers(const char* userPn);
		void updateUsersRights(const char* userPn, uint8_t rights);
		
		void deleteUser(const char* userPn);
		void deleteDatabase();

	private:
		void setNumOfUsers(uint8_t numOfUsers);

};

static AdeonMem adeonMem;

