/**
 *	bEngine :: 2011 - 2013
 *	@author Baptiste Costa
 */

#ifndef	__OWL_CORE_PARSERS_JSON__
#define	__OWL_CORE_PARSERS_JSON__

#include "core/containers/auto_ptr.h"
#include "core/containers/hash_table.h"
#include "core/containers/shared_pointer.h"
#include "core/string.h"
#include "core/uncopyable.h"

namespace owl {

//!
struct JSONArray;
struct JSONString;
struct JSONInt;
struct JSONBool;

//!
struct JSONData : public SharedObject, private Uncopyable {
	
	enum {
		TypeArray	=	0,
		TypeObject,
		TypeString,
		TypeInt,
		TypeBool,
		TypeNull
	};

	uint type, level;
	JSONData *parent, *sons;

	JSONData(uint _type = 0, JSONData* _parent = 0);

	virtual	const String ToString();
};

//!
struct JSONObject : public JSONData {
	
	AutoHashMap<String, JSONData>::type val;

	JSONObject();

	const int Size() const {
		return val.GetSize();
	}

	JSONObject* GetObject(const String&) const;
	JSONArray* GetArray(const String& key) const;
	String GetString(const String&) const;
	int GetInt(const String&) const;
	bool GetBool(const String&) const;

	JSONObject* AddJSONObject(const String&, JSONObject*);
	JSONArray* AddJSONArray(const String&, JSONArray*);
	void AddJSONString(const String&, JSONString*);
	void AddJSONInt(const String&, JSONInt*);
	void AddJSONBool(const String&, JSONBool*);
};

//!
struct	JSONArray : public JSONData {
	
	AutoVector<JSONData>::type val;

	JSONArray();

	const int Size() const {
		return val.Size();
	}

	template<class T> void ForEach(T func) {
		const int s = Size();
		for (int i = 0; i < s; ++i) {
			func(Get(i));
		}
	}

	JSONData* Get(const int);
	const JSONData* Get(const int) const;
	JSONObject* GetObject(const int) const;
	JSONArray* GetArray(const int) const;
	String GetString(const int) const;
	int GetInt(const int) const;
	bool GetBool(const int) const;

	JSONArray* AddJSONArray(JSONArray*);
	JSONObject* AddJSONObject(JSONObject*);
	void AddJSONString(JSONString*);
	void AddJSONInt(JSONInt*);
	void AddJSONBool(JSONBool*);
};

//!
struct JSONString : public JSONData {

	String val;
	JSONString(const String&);
	virtual	const String ToString();
};

//!
struct JSONInt : public JSONData {

	int val;

	JSONInt(const int n) : JSONData(TypeInt), val(n) {
		//
	}

	virtual	const String ToString();
};

//!
struct JSONBool : public JSONData {

	bool val;

	JSONBool(const bool b) : JSONData(TypeBool), val(b) {
		//
	}

	virtual	const String		ToString();
};

//!
struct JSONNull : public JSONData {

	JSONNull() : JSONData(TypeNull) {
		//
	}

	virtual	const String ToString();
};

//!
class JSONBuilder {

	String				o;

public:

	JSONBuilder(JSONData*);
	const String& Build() {
		return o;
	}

private:

	void ProcessNode(JSONData*);
};

//!
class JSONReader {

	//!
	enum State {
		StateKey,
		StateValue
	};
	State state;

	String json, key;
	int cursor, size;

	AutoPtr<JSONData> root;
	JSONData *current, *val;

private:

	void AddToCurrent() ;
	const String ExtractString();
	const String ExtractValue();

public:

	JSONReader(const String&);

	const bool IsRootArray() const {
		return root->type == JSONData::TypeArray;
	}

	JSONData* GetRoot() {
		return root;
	}

	JSONObject* GetRootObject() {
		return static_cast<JSONObject*>(root.Raw());
	}

	JSONArray* GetRootArray() {
		return static_cast<JSONArray*>(root.Raw());
	}

	void Log(char);
};

}
#endif // __OWL_CORE_PARSERS_JSON__
