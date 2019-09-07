/**
 *  @file       list.cpp
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

#include "utility/list.h"

/**
 * @brief Empty constructor for the class ItemList.
 */
ItemList::ItemList(){

}

/**
 * @brief Add item into a list.
 * @param pId is pointer to costant id string.
 * @param val is value of item.
 * @return pItem is pointer to new created object
 * 
 * At the beginning is checked validity of id. 
 * Than is called Item class constructor which create new item in list.
 */
ItemList::Item* ItemList::addItem(const char* pId, uint16_t val){
    if(isIdLenValid(pId) && !isInList(findItem(pId)) && *pId != 0){
        Item* pItem = new Item(pId, val);
        if(_pHead == nullptr){
            _pHead = pItem;
            _pLast = pItem;
            pItem->setPointToPrevItem(nullptr);
            pItem->setPointToNextItem(nullptr);
        }
        else{
            _pLast->setPointToNextItem(pItem);
            pItem->setPointToPrevItem(_pLast);
            pItem->setPointToNextItem(nullptr);
            _pLast = pItem;
        }
        _numOfItems++;
        return pItem;
    }
    return nullptr;
}

/**
 * @brief Delete item from a list.
 * @param pItem is pointer to object Item.
 * 
 * At the beginning is checked if pointer to object is not null. 
 * If item is not pHead (pointer to first item in list) than the pNext
 * pointer of deleted object is saved into pNext pointer of previous object in list.
 */
void ItemList::deleteItem(Item* pItem){
    if(pItem != nullptr){
        if(pItem != _pHead){
            Item* pPrev = pItem->getPointToPrevItem();
            pPrev->setPointToNextItem(pItem->getPointToNextItem());
        }
        else{
            _pHead = pItem->getPointToNextItem();
            _pHead->setPointToPrevItem(nullptr);
        }
        delete[]pItem;
        _numOfItems--;
    }
}

/**
 * @brief Edit item id in a list.
 * @param pItem is pointer to object Item.
 * @param pNewId is pointer to new item id.
 * @return pItem->id which is pointer to new id string
 */
char* ItemList::editItemId(Item* pItem, const char* pNewId){
    if(pItem != nullptr){
        pItem->saveId(pNewId);
        return pItem->id;
    }
    return nullptr;
}

/**
 * @brief Edit item value in a list.
 * @param pItem is pointer to object Item.
 * @param val is new value of the item.
 * 
 * If pointer to callback function is not null, than call callback.
 */
void ItemList::editItemVal(Item* pItem, uint16_t val){
    if(pItem != nullptr){
        pItem->value = val;
        if(pItem->_pCallback != nullptr){
            pItem->_pCallback(pItem->value);
        }
    }
}

/**
 * @brief Find item in a list.
 * @param pId is pointer to item id.
 * @return pItem which is a pointer to object item (null if id is not valid).
 */
ItemList::Item* ItemList::findItem(const char* pId){
    if(isIdLenValid(pId) && !isListEmpty()){
        Item* pItem = _pHead;
        while(strcmp(pId, pItem->id) != 0){
            pItem = pItem->getPointToNextItem();
            if(pItem == nullptr){
                return nullptr;
            }
        }
        return pItem;
    }
    return nullptr;
}

/**
 * @brief Check if item is in a list.
 * @param pItem is pointer to object Item.
 * @param val is new value of the item. * 
 * @return <code>true</code> if item is in the list, <code>false</code> otherwise.
 */
bool ItemList::isInList(Item* pItem){
    return pItem != nullptr;
}

/**
 * @brief Check if lingth of id is valid.
 * @param pId is pointer to item id. 
 * @return <code>true</code> if lingth of id is valid, <code>false</code> otherwise.
 */
bool ItemList::isIdLenValid(const char* pId){
    return strlen(pId) < LIST_ITEM_LENGTH;
}

bool ItemList::isListEmpty(){
    return _pHead == nullptr;
}

/**
 * @brief Get number of item in a list.
 * @return _numOfItems
 */
uint8_t ItemList::getNumOfItems(){
    return _numOfItems;
}

/**
 * @brief Get item value.
 * @return pItem->value (0 if item object is null)
 */
uint16_t ItemList::getItemVal(Item* pItem){
    if(pItem != nullptr){
        return pItem->value;
    }
    return 0;
}

/**
 * @brief Print data from list to serial.
 * 
 * It is functional only if user calls Serial.begin(baudrate) in a setup.
 */
void ItemList::printData(){
    if(Serial && !isListEmpty()){
        Item* pItem = _pHead;
        Serial.println("*********************");
        while(pItem != nullptr){
            Serial.print("LIST ITEM ID: ");
            Serial.println(pItem->id);
            Serial.print("LIST ITEM VALUE: ");
            Serial.println(pItem->value);
            Serial.println();
            pItem = pItem->getPointToNextItem();
        }
        Serial.println("*********************");
    }
}

/**
 * @brief Constructor for nested class Item.
 * 
 * It saves id and value to the object item.
 */
ItemList::Item::Item(const char* pId, uint16_t val){
    saveId(pId);
    value = val;
}

/**
 * @brief Set pointer to next object.
 * @param pItem is pointer to object Item.
 */
void ItemList::Item::setPointToNextItem(Item* pItem){
    _pNext = pItem;
}

/**
 * @brief Get pointer to next object.
 * @return _pNext which is pointer to following object in list.
 */
ItemList::Item* ItemList::Item::getPointToNextItem(){
    return _pNext;
}

/**
 * @brief Set pointer to previous object.
 * @param pItem is pointer to object Item.
 */
void ItemList::Item::setPointToPrevItem(Item* pItem){
    _pPrev = pItem;
}

/**
 * @brief Get pointer to next object.
 * @return _pPrev which is pointer to previous object in list.
 */
ItemList::Item* ItemList::Item::getPointToPrevItem(){
    return _pPrev;
}

/**
 * @brief Save id of an object.
 * @param pSrc which is a pointer to source string
 */
void ItemList::Item::saveId(const char* pSrc){
    memset(id, 0, sizeof(id));
    strcpy(id, pSrc);
}