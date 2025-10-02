#pragma once

#include <nlohmann/json.hpp>
#include <utility>

class JsonLoader;

class JSONStorage
{
    friend class JSONIO;


private:
    JSONStorage() = default;

    using json = nlohmann::json;
    std::unordered_map<std::string, json> jsonDataMap_;


public:
    class Iterator;


public:
    json& operator[](const std::string& _key);
    Iterator begin();
    Iterator end();
    size_t erase(const std::string& _key);

public:
    json& at(const std::string& _key);
    Iterator find(const std::string& _key);


private:
    std::string ToAbsPathLower(const std::string& _filepath) const;
};

class JSONStorage::Iterator
{
private:
    using jsonmap = std::unordered_map<std::string, json>;
    jsonmap::iterator it_;

public:
    Iterator() = default;
    Iterator(jsonmap::iterator _it) : it_(_it) {}


    std::pair<const std::string, json> operator*() const;
    Iterator& operator++();
    Iterator& operator--();
    bool operator==(const Iterator& _other) const;
    bool operator!=(const Iterator& _other) const;

};

class JSONIO
{
private:
    JSONIO() = default;
    ~JSONIO() = default;

    using json = nlohmann::json;
    JSONStorage jsonDataStorage_;

public:
    JSONIO(const JSONIO&) = delete;
    JSONIO& operator=(const JSONIO&) = delete;
    JSONIO(JSONIO&&) = delete;
    JSONIO& operator=(JSONIO&&) = delete;

    static JSONIO* GetInstance()
    {
        static JSONIO instance;
        return &instance;
    }


    const json& Load(const std::string& _path);
    void Save(const std::string& _path, const json& _jsonData);
    bool Unload(const std::string& _path);
};