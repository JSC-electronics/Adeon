#include <Arduino.h>
#include <EEPROM.h>

#define USER_RECORD_LEN		13 	//bytes
#define PIN_RECORD_LEN		4	//bytes
#define ADMIN_RECORS_LEN	12	//bytes
#define USERS_N_RECORDS_LEN	1	//bytes

#define CONFIG_PART_LEN		20 	//bytes

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

		uint8_t readNumOfUsers();
		char* readUserRecord(uint8_t idx);
		char* readPin();
		char* readAdminPn();

		void updatePin(const char* pin);
		void updateAdmin(const char* adminPn); 
		void updateUsers(const char* userPn);
		void updateUsersRights(const char* userPn, uint8_t rights);
		
		void deleteUser(const char* userPn);
		void deleteDatabase();

	private:
		void setNumOfUsers();

};

static AdeonMem adeonMem;

