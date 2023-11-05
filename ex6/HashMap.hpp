/**
 * @file HashMap.hpp
 * @author  Maayan Sharon
 * @version 1.0
 * @date 24 Sep 2020
 *
 * @brief Template hash map.
 *
 * @section LICENSE
 * This program is not a free software;
 *
 * @section DESCRIPTION
 * Template hash map class.
 */

#ifndef EX6_HASHMAP_HPP
#define EX6_HASHMAP_HPP

// ------------------------------ includes ------------------------------

#include <stdlib.h>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


// -------------------------- const definitions -------------------------
/**
 * @def INITIAL_CAPACITY 16
 * @brief default capacity for hash map initialization.
 */
#define INITIAL_CAPACITY 16

/**
 * @def UPPER_LOAD_FACTOR 0.75
 * @brief default upper load factor.
 */
#define UPPER_LOAD_FACTOR 0.75

/**
 * @def LOWER_LOAD_FACTOR 0.25
 * @brief default lower load factor.
 */
#define LOWER_LOAD_FACTOR 0.25


// ------------------------------ class HashMap -----------------------------
/**
 * @brief hash map template class
 * @tparam KeyT
 * @tparam ValueT
 */
template<typename KeyT, typename ValueT>
class HashMap
{
private:
    /**
     * @brief hashmap size = num of elements (pairs)
     */
    size_t _size = 0;
    /**
     * @brief hashmap capacity
     */
    size_t _capacity = INITIAL_CAPACITY;
    /**
     * @brief array holding hash map data
     */
    vector<pair<KeyT, ValueT>> *_data;
    /**
     * @brief default value
     */
    ValueT defaultVal = ValueT{};

    /**
     * @brief returns index hashed according to given key
     * @param key
     * @return index hashed according to given key
     */
    size_t _getHashIndex(KeyT key) const noexcept
    {
        size_t hashNum = hash<KeyT>{}(key);
        return (hashNum & (this->capacity() - 1));
    }

    /**
     * rehash hashmao data according to new capacity
     * @param newCapacity
     */
    void _rehash(size_t newCapacity)
    {
        vector<pair<KeyT, ValueT>> tempDataVec;
        for (auto it = begin(); it != end(); it++)
        {
            tempDataVec.push_back(*it);
        }

        size_t cursize = _size;
        clear();
        delete[] _data;
        _capacity = newCapacity;
        _data = new vector<pair<KeyT, ValueT>>[capacity()]();
        // for each elem in temp data vec- find the new hash location and put in data
        for (auto it = tempDataVec.begin(); it != tempDataVec.end(); it++)
        {
            _data[_getHashIndex((*it).first)].push_back(*it);
            _size++;
        }
        _size = cursize;

    }

public:

    /**
     * defauly constructor - initialize empty hash map with capacity of 16.
     * @tparam KeysInputIterator
     * @tparam ValuesInputIterator
     */

    HashMap()
    {
        // initialize empty hashmap with capacity 16
        this->_data = new vector<pair<KeyT, ValueT>>[INITIAL_CAPACITY]();
    }

/**
     * constructor/
     * gets 2 iterators, save the values in map according to the given order.
     * @tparam KeysInputIterator - contains KeyT values
     * @tparam ValuesInputIterator - contains ValueT values
     * @param keysBegin
     * @param keysEnd
     * @param valuesBegin
     * @param valuesEnd
     */
    template<typename KeysInputIterator, typename ValuesInputIterator>
    explicit HashMap(const KeysInputIterator keysBegin, const KeysInputIterator keysEnd,
                     const ValuesInputIterator valuesBegin, const ValuesInputIterator valuesEnd) noexcept(false)
    {
        // exception will be thrown if iterators does not have same size:
        auto keysBeginCopy = keysBegin;
        auto keysEndCopy = keysEnd;
        auto valuesBeginCopy = valuesBegin;
        auto valuesEndCopy = valuesEnd;
        int numVals = 0;
        int numKeys = 0;
        for (auto it1 = keysBeginCopy; it1 != keysEndCopy; it1++)
        {
            numKeys++;
        }
        for (auto it1 = valuesBeginCopy; it1 != valuesEndCopy; it1++)
        {
            numVals++;
        }
        if (numKeys != numVals)
        {
            throw invalid_argument("Invalid input in constructor");
        }

        this->_data = new vector<pair<KeyT, ValueT>>[_capacity](); // initialize array according to capacity

        // for each i in {0,..,n-1} key[i]->values[i]
        for (auto it1 = keysBegin, it2 = valuesBegin; it1 != keysEnd && it2 != valuesEnd; it1++, it2++)
        {
            KeyT key = *it1;
            ValueT val = *it2;
            bool isInserted = this->insert(key, val);
            if (!isInserted)
            {
                // if key already in data - override existing val
                this->operator[](key) = val;
            }
        }
    }

    /**
     * @brief hash map copy constructor
     * @param other
     */
    HashMap(HashMap &other) noexcept(false)
    {
        // initialize empty hashmap with other capacity
        this->_data = new vector<pair<KeyT, ValueT>>[other._capacity]();
        // copy elements into array
        for (size_t i = 0; i < other.capacity(); i++)
        {
            this->_data[i] = other._data[i];
        }
        // update fields:
        this->_capacity = other._capacity;
        this->_size = other._size;
    }

    /**
     * destructor.
     */
    ~HashMap() noexcept(false)
    {
        this->clear();
        delete[] this->_data;
        this->_data = nullptr;
    }

    /**
     * this method returns the number of elements in map.
     * @return the number of elements in map.
     */
    size_t size() const noexcept
    {
        // runtime: O(1)
        return this->_size;
    }

    /**
     * this method returns the capacity of map.
     * @return the capacity of map.
     */
    size_t capacity() const noexcept
    {
        // runtime: O(1)
        return this->_capacity;
    }

    /**
     * return if map is empty
     * @return
     */
    size_t empty() const noexcept
    {
        if (this->size() == 0)
        {
            return true;
        }
        return false;
    }

    /**
     * insert a key and value to map
     * @param key
     * @param val
     * @return true upon success, false otherwise
     */
    bool insert(KeyT key, ValueT val) noexcept
    {
        // save the key and val to the right mapping. runtime  O(n')
        // no need to rehash -

        if (contains_key(key))
        {
            return false;
        }
        if (UPPER_LOAD_FACTOR >= (((double) this->size() + 1) / ((double) this->capacity())))
        {
            this->_data[_getHashIndex(key)].push_back({key, val});
            this->_size++;
            return true;
        }
        try
        {
            // need to rehash -
            _rehash(this->_capacity * 2);

            //now load factor is in correct range, add new element -
            _data[_getHashIndex(key)].push_back({key, val});
            _size++;
            return true;
        }
        catch (exception &e) //rehash failed
        {
            return false;
        }
    }

/**
 * checks if key is in map
 * @param key
 * @return true if key in map, false otherwise
 */
    bool contains_key(KeyT key) const noexcept
    {
        // runtime O(n')
        // if key in inner map
        size_t idx = _getHashIndex(key);

        for (auto it = (this->_data[idx]).cbegin(); it != (this->_data[idx]).cend(); it++)
        {
            if ((*it).first == key)
            {
                return true;
            }
        }
        return false;

    }

    /**
     * checks if the given key is in map, if it is return the value
     * if key is not in map - will throw an exception.
     * @param key
     * @return value of given key upon success.
     */
    ValueT &at(const KeyT &key) noexcept(false)
    {
        // checks if the given key is in data, if it is return the value
        // runtime O(n')
        // if key in inner map
        if (this->contains_key(key))
        {
            int idx = bucket_index(key);
            for (auto it = (this->_data[idx]).begin(); it != (this->_data[idx]).end(); it++)
            {
                if ((*it).first == key)
                {
                    return (*it).second;
                }
            }
        }
        // if key is not in map - will throw an exception.
        throw out_of_range("key does not exists");
    }

    /**
    * checks if the given key is in map, if it is return the value
    * if key is not in map - will throw an exception.
    * @param key
    * @return value of given key upon success.
    */
    ValueT at(KeyT key) const noexcept(false)
    {
        if (this->contains_key(key))
        {
            int idx = bucket_index(key);
            for (auto it = (this->_data[idx]).begin(); it != (this->_data[idx]).end(); it++)
            {
                if ((*it).first == key)
                {
                    return (*it).second;
                }
            }
        }
        // if key is not in map - will throw an exception.
        throw out_of_range("key does not exists");
    }

    /**
     * remove the value of given key from map
     * @param key
     * @return true upon success, false otherwise
     */
    bool erase(KeyT key) noexcept
    {
        // remove the value of given key from map
        // runtime O(n')
        size_t idx = _getHashIndex(key);
        bool wasErased = false;

        // check if key in map - if so erase
        for (auto it = (this->_data[idx]).begin(); it != (this->_data[idx]).end(); it++)
        {
            if ((*it).first == key)
            {
                this->_data[idx].erase(it);
                this->_size--;
                wasErased = true;
                break;
            }
        }
        // if key not in map/not erased - return false
        if (!wasErased)
        {
            return false;
        }

        // check if should resize -
        if (LOWER_LOAD_FACTOR > this->load_factor())
        {
            if (this->_capacity == 1)
            {
                return true;
            }
            try
            {
                // rehash:
                _rehash(this->_capacity / 2);
            }
            catch (exception &e)
            {
                return false;
            }
        }
        return true;
    }

    /**
     * returns the load factor
     * @return the load factor as double
     */
    double load_factor() noexcept
    {
        // return the load factor
        return (((double) this->_size) / (double) this->_capacity);
    }

    /**
     * returns the size of the bucket for given key
     * @param key
     * @return the size of the bucket for given key
     */
    size_t bucket_size(KeyT key) noexcept(false)
    {
        //the size of the bucket for given key
        // throw an exception if key is not in map
        // runtime - O(n')

        if (this->contains_key(key)) // O(n')
        {
            return this->_data[bucket_index(key)].size();
        }
        throw exception();
    }

    /**
     * returns the index of the bucket of given key
     * @param key
     * @return the index of the bucket of given key
     */
    size_t bucket_index(KeyT key) const noexcept(false)
    {
        // throw an exception if key is not in map
        // runtime - O(n')
        if (!contains_key(key))
        {
            throw out_of_range("key does not exists");
        }

        return _getHashIndex(key);

    }

    /*
     * @brief clear all hashmap
     */
    void clear() noexcept
    {
        //removes all the items in data
        // capacity doesn't change
        // runtime O(n') or O(n)
        for (size_t i = 0; i < this->_capacity; i++)
        {
            if (!this->_data[i].empty())
            {
                this->_data[i].clear();
            }
        }
        this->_size = 0;
    }

    /**
     * this class represent a hashmap-const Iterator
     */
    class ConstIterator
    {
    private:

        const HashMap<KeyT, ValueT> *_hashMap;
        size_t _idxHashMap;
        size_t _idxInBucket;
        pair<KeyT, ValueT> *_cur;

    public:
        typedef pair<KeyT, ValueT> value_type;
        typedef pair<KeyT, ValueT> &reference;
        typedef pair<KeyT, ValueT> *pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;

        /**
         * constructor
         * @param map
         * @param idxHashMap
         * @param idxBucket
         * @param cur
         */
        ConstIterator(const HashMap<KeyT, ValueT> *map, size_t idxHashMap, size_t idxBucket, pair<KeyT, ValueT> *cur)
                : _hashMap(map),
                  _idxHashMap(idxHashMap),
                  _idxInBucket(idxBucket),
                  _cur(cur)
        {}

        /**
         * @brief operator *
         * @return ref cur pair
         */
        pair<KeyT, ValueT> &operator*() const noexcept
        {
            return *_cur;
        }

        /**
         * @brief operator ->
         * @return cur pair
         */
        const pair<KeyT, ValueT> *operator->() const noexcept
        {
            return _cur;
        }

        /**
         * operator ++
         * @return next cur pair
         */
        ConstIterator &operator++() noexcept
        {
            // if possible - move forward within the vector
            if (_idxInBucket < (_hashMap->_data[_idxHashMap]).size() - 1)
            {
                _idxInBucket++;
                _cur = &(_hashMap->_data[_idxHashMap][_idxInBucket]);
                return *this;
            } // else - move to the next vector
            // while not at the last bucket and current bucket is empty - continue to next bucker
            _idxHashMap++;
            while (((_hashMap->_data[_idxHashMap].empty()) && _idxHashMap < _hashMap->capacity() - 1))
            {
                _idxHashMap++;
            }
            // bucket is not empty or last bucket: if bucket is not empty - take the first item
            if (!(_hashMap->_data[_idxHashMap].empty()))
            {
                _idxInBucket = 0;
                _cur = &(_hashMap->_data[_idxHashMap][_idxInBucket]);
                return *this;
            }
            // else, bucket this is the last bucket and bucket is empty - so return end
            _cur = nullptr;
            _idxInBucket = 0;
            _idxHashMap = 0;
            return *this;
        }

        /**
         * @brief operator++int
         * @return cur before ++
         */
        ConstIterator operator++(int) noexcept
        {
            if (_cur == nullptr && _idxHashMap == 0 && _idxInBucket == 0)
            {
                return *this;
            }
            ConstIterator tmp = *this;
            // if possible - move forward within the vector
            if (_idxInBucket < (_hashMap->_data[_idxHashMap]).size() - 1)
            {
                _idxInBucket++;
                _cur = &(_hashMap->_data[_idxHashMap][_idxInBucket]);
                return tmp;
            } // else - move to the next vector
            _idxHashMap++;
            // check if it's out of range - if so return end:
            if (_idxHashMap == _hashMap->_capacity)
            {
                _cur = nullptr;
                _idxHashMap = 0;
                _idxInBucket = 0;
                return tmp;
            }
            // while not at the last bucket and current bucket is empty - continue to next bucker
            while (((_hashMap->_data[_idxHashMap].empty()) && _idxHashMap < _hashMap->capacity() - 1))
            {
                _idxHashMap++;
            }
            // bucket is not empty or last bucket: if bucket is not empty - take the first item
            if (!(_hashMap->_data[_idxHashMap].empty()))
            {
                _idxInBucket = 0;
                _cur = &(_hashMap->_data[_idxHashMap][_idxInBucket]);
                return tmp;
            }
            // else, bucket this is the last bucket and bucket is empty - so we're at the end.
            _cur = nullptr;
            _idxHashMap = 0;
            _idxInBucket = 0;
            return tmp;
        }

        /**
         * @brief operator ==
         * @param other
         * @return true if iter are equal, false otherwise
         */
        bool operator==(const ConstIterator &other) const noexcept
        {
            if (this->_hashMap != other._hashMap)
            {
                return false;
            }
            if (this->_cur != other._cur)
            {
                return false;
            }
            if (_idxInBucket != other._idxInBucket || _idxHashMap != other._idxHashMap)
            {
                return false;
            }
            return true;
        }

        /**
         * @brief operator !=
         * @param other
         * @return true if iter are not equal, false otherwise
         */
        bool operator!=(const ConstIterator &other) const noexcept
        {
            if (*this == other)
            {
                return false;
            }
            return true;
        }
    };

    typedef ConstIterator const_iterator;

    /**
     * @brief iter to beginning of the vec
     * @return iter to beginning of the vec
     */
    const_iterator cbegin() const noexcept
    {

        for (size_t i = 0; i < this->_capacity; i++)
        {
            if (!this->_data[i].empty())
            {
                ConstIterator it(this, i, 0, &(this->_data[i][0]));
                return it;
            }
        }
        //HashMap<KeyT, ValueT> *map, size_t idxHashMap, size_t idxBucket, pair<KeyT, ValueT> *cur)

        ConstIterator it(this, 0, 0, &(this->_data[0][0]));
        return it;

    }

    /**
     * @brief iter to beginning of the vec
     * @return iter to beginning of the vec
     */
    const_iterator begin() const noexcept
    {
        return cbegin();

    }

    /**
     * @brief iter to end of the vec
     * @return iter to end of the vec
     */
    const_iterator cend() const noexcept
    {
        return end();
    }

    /**
     * @brief iter to end of the vec
     * @return iter to end of the vec
     */
    const_iterator end() const noexcept
    {
        if (this->empty())
        {
            return begin();
        }
        ConstIterator it(this, 0, 0, nullptr);
        return it;
    }

    /**
     * @brief assignment
     * @param rhs
     * @return ref to this
     */
    HashMap &operator=(const HashMap &rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }
        for (size_t i = 0; i < this->_capacity; i++)
        {
            if (!this->_data[i].empty())
            {
                this->_data[i].clear();
            }
        }
        delete[] this->_data;
        this->_capacity = rhs._capacity;
        this->_size = rhs._size;
        this->_data = new vector<pair<KeyT, ValueT>>[this->_capacity]();

        for (int i = 0; i < this->_capacity; i++)
        {
            this->_data[i] = rhs._data[i];
        }
        return *this;
    }


    /**
     * returns the valid value
     * @param i
     * @return valueT matching to this key
     */
    ValueT operator[](KeyT key) const noexcept
    {
        // assuming key is in map
        try
        {
            int idx = bucket_index(key);
            for (auto it = (this->_data[idx]).begin(); it != (this->_data[idx]).end(); it++)
            {
                if ((*it).first == key)
                {
                    return (*it).second;
                }
            }
            return ValueT();
        }
        catch (exception &e)
        {
            return ValueT();
        }
    }

    /**
     * return the ref to corresponding valueT
     * @param key
     * @return valueT matching to this key
     */
    ValueT &operator[](KeyT key) noexcept
    {
        try
        {
            int idx = bucket_index(key); //may throw if key is not in bucket
            for (auto it = (this->_data[idx]).begin(); it != (this->_data[idx]).end(); it++)
            {
                if ((*it).first == key)
                {
                    return (*it).second;
                }
            }
            // key not i
            try
            {
                this->insert(key, ValueT());
                return at(key); // key must be in
            }
            catch (exception &e)
            {
                return defaultVal;
            }
        }
        catch (exception &e)
        {
            this->insert(key, ValueT());
            return at(key);
        }

    }

    /**
     * equal operator
     * @param rhs
     * @return true if maps are equal, false otherwise
     */
    bool operator==(const HashMap<KeyT, ValueT> &rhs) const noexcept
    {
        if (this->capacity() != rhs.capacity() || this->size() != rhs.size())
        {
            return false;
        }
        for (size_t i = 0; i < this->capacity(); i++)
        {
            if (this->_data[i] != this->_data[i])
            {
                return false;
            }
        }
        return true;
    }

    /**
     * operator!=
     * @param rhs
     * @return true if maps are not equal,
     */
    bool operator!=(const HashMap<KeyT, ValueT> &rhs) const noexcept
    {
        return (!(this->operator==(rhs)));
    }


};

#endif //EX6_HASHMAP_HPP
