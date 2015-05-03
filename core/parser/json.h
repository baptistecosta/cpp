#ifndef BCOSTA_CORE_PARSER_JSON_H
#define BCOSTA_CORE_PARSER_JSON_H

#include "core/container/auto_ptr.h"
#include "core/container/hash_table.h"
#include "core/container/shared_pointer.h"
#include "core/string.h"
#include "core/uncopyable.h"

namespace BCosta {
namespace Core {
namespace Parser {

struct JSONArray;
struct JSONString;
struct JSONInt;
struct JSONBool;

struct JSONData : public SharedObject, private Uncopyable
{

    enum
    {
        TypeArray = 0,
        TypeObject,
        TypeString,
        TypeInt,
        TypeBool,
        TypeNull
    };

    unsigned int type, level;
    JSONData *parent, *sons;

    JSONData(unsigned int _type = 0, JSONData *_parent = 0);

    virtual const String ToString();
};

struct JSONObject : public JSONData
{

    AutoHashMap<String, JSONData>::type val;

    JSONObject();

    const int Size() const
    {
        return val.GetSize();
    }

    JSONObject *GetObject(const String &) const;

    JSONArray *GetArray(const String &key) const;

    String GetString(const String &) const;

    int GetInt(const String &) const;

    bool GetBool(const String &) const;

    JSONObject *AddJSONObject(const String &, JSONObject *);

    JSONArray *AddJSONArray(const String &, JSONArray *);

    void AddJSONString(const String &, JSONString *);

    void AddJSONInt(const String &, JSONInt *);

    void AddJSONBool(const String &, JSONBool *);
};

struct JSONArray : public JSONData
{

    AutoVector<JSONData>::type val;

    JSONArray();

    const int Size() const
    {
        return val.Size();
    }

    template<class T>
    void ForEach(T func)
    {
        const int s = Size();
        for (int i = 0; i < s; ++i) {
            func(Get(i));
        }
    }

    JSONData *Get(const int);

    const JSONData *Get(const int) const;

    JSONObject *GetObject(const int) const;

    JSONArray *GetArray(const int) const;

    String GetString(const int) const;

    int GetInt(const int) const;

    bool GetBool(const int) const;

    JSONArray *AddJSONArray(JSONArray *);

    JSONObject *AddJSONObject(JSONObject *);

    void AddJSONString(JSONString *);

    void AddJSONInt(JSONInt *);

    void AddJSONBool(JSONBool *);
};

struct JSONString : public JSONData
{
    String val;

    JSONString(const String &);

    virtual const String ToString();
};

struct JSONInt : public JSONData
{
    int val;

    JSONInt(const int n) : JSONData(TypeInt), val(n)
    { }

    virtual const String ToString();
};

struct JSONBool : public JSONData
{
    bool val;

    JSONBool(const bool b) : JSONData(TypeBool), val(b)
    { }

    virtual const String ToString();
};

struct JSONNull : public JSONData
{
    JSONNull() : JSONData(TypeNull)
    {
        //
    }

    virtual const String ToString();
};

class JSONBuilder
{
    String o;

public:
    JSONBuilder(JSONData *);

    const String &Build()
    {
        return o;
    }

private:
    void ProcessNode(JSONData *);
};

class JSONReader
{
    enum State
    {
        StateKey,
        StateValue
    };
    State state;

    String json, key;
    int cursor, size;

    AutoPtr<JSONData> root;
    JSONData *current, *val;

private:
    void AddToCurrent();

    const String ExtractString();

    const String ExtractValue();

public:
    JSONReader(const String &);

    const bool IsRootArray() const
    {
        return root->type == JSONData::TypeArray;
    }

    JSONData *GetRoot()
    {
        return root;
    }

    JSONObject *GetRootObject()
    {
        return static_cast<JSONObject *>(root.Raw());
    }

    JSONArray *GetRootArray()
    {
        return static_cast<JSONArray *>(root.Raw());
    }
};

}
}
}
#endif // BCOSTA_CORE_PARSER_JSON_H