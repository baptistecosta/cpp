/*
	bEngine :: 2011 - 2013
	Baptiste Costa
*/

	#include <stdlib.h>
	#include "json.h"
	#include "core/containers/hash_table.h"
	
	using namespace owl;

// JSONData
//---------------------------------------------------------------------------
JSONData::JSONData(uint _type, JSONData* _parent) : type(_type), parent(_parent) {}

//---------------------------------------------------------------------------
JSONData&		JSONData::operator = (const JSONData& data)
{
	parent = data.parent;
	return *this;
}

//---------------------------------------------------------------------------
String			JSONData::ToString()
{
	switch (type)
	{
		case TypeArray: return "Array";
		case TypeObject: return "Object";
		case TypeString: return "String";
		case TypeInt: return "Int";
		case TypeBool: return "Bool";
		default:
			return "Null";
	}
}

// JSONObject
//---------------------------------------------------------------------------
JSONObject::JSONObject() : JSONData(TypeObject) {}
JSONObject::JSONObject(const JSONObject& obj)
	: JSONData(obj.type, const_cast<JSONData*>(obj.parent))
	, val(obj.val)
{
	//
}

//---------------------------------------------------------------------------
JSONObject&		JSONObject::operator = (const JSONObject& object)
{
	JSONData::operator = (object);
	val = object.val;
	return *this;
}

//---------------------------------------------------------------------------
const JSONObject& JSONObject::GetObject(const String& key) const	{	return *static_cast<JSONObject*>(val.Find(key)->GetValue());	}
const JSONArray& JSONObject::GetArray(const String& key) const		{	return *static_cast<JSONArray*>(val.Find(key)->GetValue());	}
const String&	JSONObject::GetString(const String& key) const		{	return static_cast<JSONString*>(val.Find(key)->GetValue())->val;	}
const int		JSONObject::GetInt(const String& key) const			{	return static_cast<JSONInt*>(val.Find(key)->GetValue())->val;	}
const bool		JSONObject::GetBool(const String& key) const		{	return static_cast<JSONBool*>(val.Find(key)->GetValue())->val;	}
//---------------------------------------------------------------------------

// JSONArray
//---------------------------------------------------------------------------
JSONArray::JSONArray() : JSONData(TypeArray) {}
JSONArray::JSONArray(const JSONArray& array)
	: JSONData(array.type, const_cast<JSONData*>(array.parent))
	, val(array.val)
{
	//
}

//---------------------------------------------------------------------------
JSONArray&		JSONArray::operator = (const JSONArray& array)
{
	JSONData::operator = (array);
	val = array.val;
	return *this;
}

//---------------------------------------------------------------------------
JSONData*		JSONArray::Get(const int index)						{	return val[index];	}
const JSONData*	JSONArray::Get(const int index) const				{	return val[index];	}
const JSONObject& JSONArray::GetObject(const int index) const		{	return *static_cast<JSONObject*>(val[index]);	}
const JSONArray& JSONArray::GetArray(const int index) const			{	return *static_cast<JSONArray*>(val[index]);	}
const String&	JSONArray::GetString(const int index) const			{	return static_cast<JSONString*>(val[index])->val;;	}
const int		JSONArray::GetInt(const int index) const			{	return static_cast<JSONInt*>(val[index])->val;	}
const bool		JSONArray::GetBool(const int index) const			{	return static_cast<JSONBool*>(val[index])->val;	}

// JSONString
//---------------------------------------------------------------------------
JSONString::JSONString(const String& str) : JSONData(TypeString), val(str) {}

//---------------------------------------------------------------------------
String			JSONString::ToString()
{	return String::Format("%s: %s", JSONData::ToString(), val);	}
String			JSONInt::ToString()
{	return String::Format("%s: %d", JSONData::ToString(), val);	}
String			JSONBool::ToString()
{	return String::Format("%s: %d", JSONData::ToString(), val ? "true" : "false");	}
//---------------------------------------------------------------------------

// JSONBuilder
//---------------------------------------------------------------------------
JSONBuilder::JSONBuilder()
{

}

// JSONReader
//---------------------------------------------------------------------------
JSONReader::JSONReader(const String& _json)
	: json(_json)
	, cursor(0)
	, level(0)
	, root(0)
	, current(0)
	, val(0)
	, state(StateKey)
{
	json.RemoveOccurences(Vector<String>() << " " << "\n" << "\t");
	size = json.Size();

	Log::i(json);

	// Check if the first node is a JSONObject or Array
	switch (json[cursor++])
	{
		case '{': current = new JSONObject(); break;
		case '[': current = new JSONArray(); /*state = StateValue;*/ break;
	}
	root = current;

Log('[');

	// Iterate
	char ca = 0, cb = 0;
	for (; cursor < size; ++cursor)
	{
		cb = json[cursor];

		switch (cb)
		{
			case '{':
			{
level++;
				val = new JSONObject();
				val->parent = current;
				AddToCurrent();
Log(cb);
				current = val;
				state = StateKey;
				break;
			}

			case '[':
			{
level++;
				val = new JSONArray();
				val->parent = current;
				AddToCurrent();
Log(cb);
				current = val;
				state = StateValue;
				break;
			}

			case '"':
			{
				const String str = ExtractString();
				switch (state)
				{
					case StateKey: key = str; break;
					case StateValue: val = new JSONString(str); break;
				}
				break;
			}

			case ':':
				state = StateValue;
				break;

			case '}':
			case ']':
				state = StateKey;
				if (ca != '}' && ca != ']')
				{
					AddToCurrent();
Log(cb);
				}
				if (cursor < size - 1)
					current = current->parent;
level--;
				break;

			case ',':
				state = StateKey;
				if (ca != '}' && ca != ']')
				{
					AddToCurrent();
Log(cb);
				}
				break;

			default:
			{
//				if (state == StateValue)
//				{
					const String str = ExtractValue();
					if (str.IsNumeric())
						val = new JSONInt(atoi(str.cStr()));
					else if (str == "true")
						val = new JSONBool(true);
					else if (str == "false")
						val = new JSONBool(false);
					else if (str == "null")
						val = 0;
					else
						val = 0;

//					state = StateKey;
//				}
				break;
			}
		}
		ca = cb;
	}
}

//---------------------------------------------------------------------------
void			JSONReader::AddToCurrent() 
{
	switch (current->type)
	{
		case JSONData::TypeObject:
			static_cast<JSONObject*>(current)->val.Insert(key, val);
			break;

		case JSONData::TypeArray:
			static_cast<JSONArray*>(current)->val.Push(val);
			break;
	}
}

//---------------------------------------------------------------------------
const String	JSONReader::ExtractString()
{
	String str;
	char ca = '\0', cb = '\0';
	while (true)
	{
		cb = json[++cursor];

		if (ca != '\\' && cb == '\"')
			break;
		str += cb;
		ca = cb;
	}
	return str;
}

//---------------------------------------------------------------------------
const String	JSONReader::ExtractValue()
{
	String str;
	char ca = 0, cb = 0;
	while (true)
	{
		cb = json[cursor];
		if (ca != '\\' && (cb == '}' || cb == ']' || cb == ','))
		{
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
void			JSONReader::Log(char c)
{
	Log::Flat("Parsed character = ' %c ' >> ", c);
	for (int i = 0; i < level; ++i)
		Log::Flat(" >> ");
	Log::i("Current = %s, Key = %s, val = %s", current->ToString(), key, val ? val->ToString() : "null");
}
