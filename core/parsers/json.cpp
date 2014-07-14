/**
 *	bEngine :: 2011 - 2013
 *	@author Baptiste Costa
 */

#include <stdlib.h>
#include "json.h"
#include "core/containers/hash_table.h"
	
using namespace owl;

static int __level__ = 0;

// JSONData
//---------------------------------------------------------------------------
JSONData::JSONData(uint _type, JSONData* _parent) : type(_type), level(__level__), parent(_parent), sons(0) {
	//
}

//---------------------------------------------------------------------------
const String JSONData::ToString() {
	switch (type) {
		case TypeArray:
			return String("JSONArray");
		case TypeObject:
			return String("JSONObject");
		case TypeString:
			return String("JSONString");
		case TypeInt:
			return String("JSONInt");
		case TypeBool:
			return String("JSONBool");
		default:
			return String("JSONNull");
	}
}

// JSONObject
//---------------------------------------------------------------------------
JSONObject::JSONObject() : JSONData(TypeObject) {
	//
}

//---------------------------------------------------------------------------
JSONObject* JSONObject::GetObject(const String& key) const {
	return static_cast<JSONObject*>(val.Find(key)->GetValue());
}

JSONArray* JSONObject::GetArray(const String& key) const {
	return static_cast<JSONArray*>(val.Find(key)->GetValue());
}

String JSONObject::GetString(const String& key) const {
	return static_cast<JSONString*>(val.Find(key)->GetValue())->val;
}

int JSONObject::GetInt(const String& key) const {
	return static_cast<JSONInt*>(val.Find(key)->GetValue())->val;
}

bool JSONObject::GetBool(const String& key) const {
	return static_cast<JSONBool*>(val.Find(key)->GetValue())->val;
}

//---------------------------------------------------------------------------
JSONObject* JSONObject::AddJSONObject(const String& key, JSONObject* obj) {
	val.Insert(key, obj);
	return obj;
}

JSONArray* JSONObject::AddJSONArray(const String& key, JSONArray* array) {
	val.Insert(key, array);
	return array;
}

void JSONObject::AddJSONString(const String& key, JSONString* str) {
	val.Insert(key, str);
}

void JSONObject::AddJSONInt(const String& key, JSONInt* n) {
	val.Insert(key, n);
}

void JSONObject::AddJSONBool(const String& key, JSONBool* b) {
	val.Insert(key, b);
}
//---------------------------------------------------------------------------

// JSONArray
//---------------------------------------------------------------------------
JSONArray::JSONArray() : JSONData(TypeArray) {
	//
}

//---------------------------------------------------------------------------
JSONData* JSONArray::Get(const int index) {
	return val[index];
}

const JSONData* JSONArray::Get(const int index) const {
	return val[index];
}

JSONObject* JSONArray::GetObject(const int index) const {
	return static_cast<JSONObject*>(val[index]);
}

JSONArray* JSONArray::GetArray(const int index) const {
	return static_cast<JSONArray*>(val[index]);
}

String JSONArray::GetString(const int index) const {
	return static_cast<JSONString*>(val[index])->val;
}

int JSONArray::GetInt(const int index) const {
	return static_cast<JSONInt*>(val[index])->val;
}

bool JSONArray::GetBool(const int index) const {
	return static_cast<JSONBool*>(val[index])->val;
}
//---------------------------------------------------------------------------
JSONArray*		JSONArray::AddJSONArray(JSONArray* arr)				{	val.Push(arr); return arr;	}
JSONObject*		JSONArray::AddJSONObject(JSONObject* obj)			{	val.Push(obj); return obj;	}
void			JSONArray::AddJSONString(JSONString* str)			{	val.Push(str);	}
void			JSONArray::AddJSONInt(JSONInt* n)					{	val.Push(n);	}
void			JSONArray::AddJSONBool(JSONBool* b)					{	val.Push(b);	}
//---------------------------------------------------------------------------

// JSONString
//---------------------------------------------------------------------------
JSONString::JSONString(const String& str): JSONData(TypeString), val(str) {
	//
}

//---------------------------------------------------------------------------
const String JSONString::ToString() {
	return String::Format("%s: %s", JSONData::ToString().cStr(), val.cStr());
}

const String JSONInt::ToString() {
	return String::Format("%s: %d", JSONData::ToString().cStr(), val);
}

const String JSONBool::ToString() {
	return String::Format("%s: %d", JSONData::ToString().cStr(), val ? "true" : "false");
}

const String JSONNull::ToString() {
	return String::Format("%s", JSONData::ToString().cStr());
}
//---------------------------------------------------------------------------


// JSONBuilder
//---------------------------------------------------------------------------
JSONBuilder::JSONBuilder(JSONData* root) {
	JSONData* itr;
	switch (root->type) {
		case JSONData::TypeArray:
			itr = static_cast<JSONArray*>(root);
			break;

		case JSONData::TypeObject:
			itr = static_cast<JSONObject*>(root);
			break;
	}
	ProcessNode(itr);
}

//---------------------------------------------------------------------------
void JSONBuilder::ProcessNode(JSONData* data) {
	switch (data->type) {
		case JSONData::TypeArray: {
			o << '[';
			JSONArray* arr = static_cast<JSONArray*>(data);
			int ct = arr->Size();
			arr->ForEach([this, &ct](JSONData* data) {
				ProcessNode(data);
				o << (--ct ? ',' : ']');
			});
			break;
		}
		case JSONData::TypeObject: {
			o << '{';
			JSONObject* obj = static_cast<JSONObject*>(data);
			int ct = obj->Size();
			obj->val.ForEach([this, &ct](String k, JSONData* v) {
				o << '"' << k << '"' << ':';
				ProcessNode(v);
				o << (--ct ? ',' : '}');
			});
			break;
		}
		case JSONData::TypeString:
			o << '"' << static_cast<JSONString*>(data)->val << '"';
			break;

		case JSONData::TypeInt:
			o << String::Format("%d", static_cast<JSONInt*>(data)->val);
			break;

		case JSONData::TypeBool:
			o << (static_cast<JSONBool*>(data)->val ? "true" : "false");
			break;

		case JSONData::TypeNull:
			o << "null";
			break;
	}
}

// JSONReader
//---------------------------------------------------------------------------
JSONReader::JSONReader(const String& _json) : state(StateKey), json(_json), cursor(0), root(0), current(0), val(0) {
	json.RemoveOccurences(Vector<String>() << " " << "\n" << "\t");
	size = json.Size();

	Log::i(json);

	// Check if the first node is a JSONObject or Array
	switch (json[cursor++]) {
		case '{':
			current = new JSONObject();
//			Log('{');
			break;

		case '[':
			current = new JSONArray();
			Log('[');
			break;
	}
	root = current;

	// Iterate
	char ca = 0, cb = 0;
	for (; cursor < size; ++cursor) {
		cb = json[cursor];

		switch (cb) {
			case '{': {
				val = new JSONObject();
				val->parent = current;
				AddToCurrent();
//Log(cb);
__level__++;
				current = val;
				state = StateKey;
				break;
			}

			case '[': {
				val = new JSONArray();
				val->parent = current;
				AddToCurrent();
//Log(cb);
				current = val;
				state = StateValue;
				break;
			}

			case '"': {
				const String str = ExtractString();
				switch (state) {
					case StateKey:
						key = str;
						break;

					case StateValue:
						val = new JSONString(str);
						break;
				}
				break;
			}

			case ':':
				state = StateValue;
				break;

			case '}':
			case ']':
				state = StateKey;
				if (ca != '}' && ca != ']') {
					AddToCurrent();
//Log(cb);
				}
				if (cursor < size - 1) {
					current = current->parent;
				}
__level__--;
				break;

			case ',':
				switch (current->type) {
					case JSONData::TypeArray:
						state = StateValue;
						break;

					case JSONData::TypeObject:
						state = StateKey;
						break;
				}

				if (ca != '}' && ca != ']') {
					AddToCurrent();
//Log(cb);
				}
				break;

			default: {
				const String str = ExtractValue();
				if (str.IsNumeric()) {
					val = new JSONInt(atoi(str.cStr()));
				} else if (str == "true") {
					val = new JSONBool(true);
				} else if (str == "false") {
					val = new JSONBool(false);
				} else {
					val = new JSONNull();
				}
				break;
			}
		}
		ca = cb;
	}
}

//---------------------------------------------------------------------------
void JSONReader::AddToCurrent() {
	switch (current->type) {
		case JSONData::TypeObject:
			static_cast<JSONObject*>(current)->val.Insert(key, val);
			break;

		case JSONData::TypeArray:
			static_cast<JSONArray*>(current)->val.Push(val);
			break;
	}
	if (!current->sons) {
		current->sons = val;
	}
}

//---------------------------------------------------------------------------
const String JSONReader::ExtractString() {
	String str;
	char ca = '\0', cb = '\0';
	while (true) {
		cb = json[++cursor];

		if (ca != '\\' && cb == '\"') {
			break;
		}
		str += cb;
		ca = cb;
	}
	return str;
}

//---------------------------------------------------------------------------
const String JSONReader::ExtractValue() {
	String str;
	char ca = 0, cb = 0;
	while (true) {
		cb = json[cursor];
		if (ca != '\\' && (cb == '}' || cb == ']' || cb == ',')) {
			cursor--;
			break;
		}
		str += cb;
		ca = cb;
		cursor++;
	}
	return str;
}

//---------------------------------------------------------------------------
void JSONReader::Log(char c) {
	Log::Flat("Parsed character = ' %c ' >> ", c);
	for (int i = 0; i < __level__; ++i)
		Log::Flat(" >> ");

	String _c = current->ToString();
	String _k = key.IsEmpty() ? "null" : key;
	String _v = val ? val->ToString() : "null";

	Log::i("Level = %d, Current = %s, Key = %s, val = %s", __level__, _c.cStr(), _k.cStr(), _v.cStr());
}
