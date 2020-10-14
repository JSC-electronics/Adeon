/**
 *  @file       list.h
 *  Project     AdeonGSM
 *  @brief      Library for linked list
 *  @author     JSC electronics
 *  License     Apache-2.0 - Copyright (c) 2019 JSC electronics
 *
 *  @section License
 *
 *  Copyright (c) 2019 JSC electronics
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ADEON_LIST_H
#define ADEON_LIST_H

#include <Arduino.h>

constexpr static auto LIST_ITEM_LENGTH = 16;

class ItemList {
    protected:
      class Item;
      ItemList();
       
    public:      
      Item* addItem(const char* pId, uint16_t val);
      void deleteItem(Item* pItem);
      void deleteHead();
      char* editItemId(Item* pItem, const char* pNewId);
	    void editItemVal(Item* pItem, uint16_t val);
      Item* findItem(const char* pId);
      bool isInList(Item* pItem);
      bool isIdLenValid(const char* pId);
      bool isListEmpty();
      uint8_t getNumOfItems();
      uint16_t getItemVal(Item* pItem);
      void printData();

    protected:
      Item* _pHead = nullptr;
      Item* _pLast = nullptr; 
      uint8_t _numOfItems = 0;

      class Item {
        public:
          Item(const char* pId, uint16_t val);
          void setPointToNextItem(Item* pItem);
          Item* getPointToNextItem();
          void setPointToPrevItem(Item* pItem);
          Item* getPointToPrevItem();
          void saveId(const char* pSrc);

          char* id;
          uint16_t value = 0;
          uint8_t accessRights = 1;
          void (*_pCallback)(uint16_t) = nullptr;

        private:
          Item* _pNext = nullptr;
          Item* _pPrev = nullptr; 
    };  
};

#endif // ADEON_LIST_H