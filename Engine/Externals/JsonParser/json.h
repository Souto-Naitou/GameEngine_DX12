#pragma once

#include <string>
#include <variant>
#include <memory>
#include <unordered_map>
#include <vector>
#include <filesystem>

namespace Json
{
    /// 前方宣言
    class Value;


    // 別名定義
    using Array = std::vector<std::shared_ptr<Json::Value>>;
    using Object = std::unordered_map<std::string, std::shared_ptr<Json::Value>>;


    // JSONデータ型の定義
    using Type = std::variant<
        std::monostate,   // 未初期化状態を表現
        int,
        float,
        std::string,
        bool,
        Json::Array,
        Json::Object
    >;


    /// <summary>
    /// データクラス
    /// </summary>
    class Value
    {
    public:
        class Iterator
        {
        private:
            using ArrayIter = Json::Array::iterator;
            using ObjectIter = Json::Object::iterator;
            std::variant<std::monostate, ObjectIter, ArrayIter> iter_;

        public:
            Iterator() = default;
            Iterator(ArrayIter it) : iter_(it) {}
            Iterator(ObjectIter it) : iter_(it) {}

            std::shared_ptr<Json::Value>& operator*() 
            { 
                if (auto p = std::get_if<ArrayIter>(&iter_)) return **p;
                if (auto p = std::get_if<ObjectIter>(&iter_)) return (*p)->second;
                throw std::runtime_error("Invalid iterator dereference");
            }

            Iterator& operator++() {
                if (auto p = std::get_if<ArrayIter>(&iter_)) ++(*p);
                else if (auto p = std::get_if<ObjectIter>(&iter_)) ++(*p);
                return *this;
            }

            bool operator!=(const Iterator& other) const {
                return iter_ != other.iter_;
            }
        };

    public:

        /// <summary>
        /// JSONデータの種類
        /// </summary>
        enum class Type
        {
            Null,
            Int,
            Float,
            String,
            Bool,
            Array,
            Object
        };

        Json::Value::Type type = Json::Value::Type::Null;
        Json::Type value;

        Value& operator[](const char* _key);
        Value& operator[](const std::string& _key);

        operator int() const;
        operator float() const;
        operator std::string() const;
        operator bool() const;
        operator Json::Array() const;
        operator Json::Object() const;

        Value& operator=(const Json::Object& _obj);
        Value& operator=(const Json::Array& _array);
        Value& operator=(const std::shared_ptr<Json::Value>& _value);


    public:
        Value::Iterator begin() {
            if (type == Type::Array)
                return Iterator(std::get<Json::Array>(value).begin());
            else if (type == Type::Object)
                return Iterator(std::get<Json::Object>(value).begin());
            else
                return Iterator(); // 無効
        }

        Value::Iterator end() {
            if (type == Type::Array)
                return Iterator(std::get<Json::Array>(value).end());
            else if (type == Type::Object)
                return Iterator(std::get<Json::Object>(value).end());
            else
                return Iterator(); // 無効
        }


    public:
        _NODISCARD Value& at(const std::string& _key);


    private:
        Value& _Search(const std::string& _key, const Json::Object& _obj);
        Value& _Recursive_search(const std::string& _key, const Json::Object& _obj);
    };


    /// <summary>
    /// トークン分割クラス
    /// </summary>
    class Tokenizer
    {
    public:
        enum class Type
        {
            LeftBrace,          // {
            RightBrace,         // }
            LeftBracket,        // [
            RightBracket,       // ]
            Colon,              // :
            Comma,              // ,
            String,             // 文字列
            Number,             // 数値
            True,               // 真
            False,              // 偽
            Null,               // null
            EndOfFile,          // ファイルの終わり
        };

        struct Token
        {
            Json::Tokenizer::Type type;
            std::string value;
        };

        Tokenizer() = default;

        void Initialize();
        void SetJsonString(const std::string& _jsonStr) { jsonStr_ = _jsonStr; }

        Json::Tokenizer::Token NextToken();

    private:
        std::string jsonStr_;
        uint32_t pos_ = 0;


    private:
        void SkipWhiteSpace();
        Json::Tokenizer::Token ParseString();
        Json::Tokenizer::Token ParseNumber();
        bool IsMatchLiteral(const std::string& literal);
    };


    /// <summary>
    /// パースクラス
    /// </summary>
    class Parser
    {
    public:
        Parser() = default;

        void Initialize();
        void SetJsonString(const std::string& _jsonStr) { tokenizer_.SetJsonString(_jsonStr); }

        void Run();

        const Json::Value& GetData() { return data_; }

    private:
        Json::Tokenizer tokenizer_;
        Json::Tokenizer::Token currentToken_;

        Json::Value data_;

    private:
        void Consume(Json::Tokenizer::Type type);

        Json::Object ParseObject();

        Json::Array ParseArray();

        void ParseNumber(Json::Value& _value);

        bool IsDouble(const std::string& _str);
    };


    /// <summary>
    /// 保存クラス (JSON形式)
    /// </summary>
    class Saver
    {
    public:
        Saver(const Saver&) = delete;
        Saver& operator=(const Saver&) = delete;
        Saver(const Saver&&) = delete;
        Saver& operator=(const Saver&&) = delete;

        static Saver* GetInstance()
        {
            static Saver instance;
            return &instance;
        }

        /// <summary>
        /// JSONファイルを保存する
        /// </summary>
        /// <param name="_path">保存先ファイルパス</param>
        /// <param name="_value">JSONデータ</param>
        void Run(std::filesystem::path _path, Json::Value _value);
        std::string GetString(const Json::Value& _value, int _indent, bool _isSameLine);


    private:
        Saver() = default;
        ~Saver() = default;


        uint32_t indentSize_ = 4u;
    };

    std::shared_ptr<Json::Value> NewValue(int _data);
    std::shared_ptr<Json::Value> NewValue(float _data);
    std::shared_ptr<Json::Value> NewValue(const std::string& _data);
    std::shared_ptr<Json::Value> NewValue(bool _data);
    std::shared_ptr<Json::Value> NewValue(const Json::Array& _data);
    std::shared_ptr<Json::Value> NewValue(const Json::Object& _data);
};