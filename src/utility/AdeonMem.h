#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_LEN			1023 //bytes
#define BLANK_CELL			0xFF //0xFF is value which means the cell is blank
#define NOT_FOUND			-1
#define MAX_NUM_OF_USERS	77

#define USER_RECORD_LEN		13 	//bytes
#define PIN_RECORD_LEN		4	//bytes
#define ADMIN_RECORD_LEN	12	//bytes
#define USERS_N_RECORD_LEN	1	//bytes

#define IDX_DATA_PART		20 	//bytes
#define IDX_PIN				0
#define IDX_ADMIN_PN		4
#define IDX_NUM_USERS		16

class AdeonMem
{
	private:
		char* _returnBuffer;

	public:
		AdeonMem();

		bool isConfigAvailable();

		int16_t searchUser(const char* userPn);

		uint8_t getNumOfUsers();
		char* readUserRecord(uint8_t numOfUserOrder);
		uint8_t readUserRights(uint8_t numOfUserOrder);
		char* readPin();
		char* readAdminPn();

		void updatePin(const char* pin);
		void updateAdmin(const char* adminPn); 
		void updateUsers(const char* userPn, uint8_t rights);
		void updateUsersRights(const char* userPn, uint8_t rights);
		
		void deleteUser(const char* userPn);
		void orderMemory(uint16_t idx);
		void deleteDatabase();
		void printEeprom(uint16_t start, uint16_t count);

	private:
		void updateNumOfUsers(uint8_t numOfUsers);
		void setReturnBuffer(uint8_t size);
};

static AdeonMem adeonMem;

