/*
	engine :: 2011 - 2013
	Baptiste Costa
*/

#ifndef	__OWL_CORE_PARSERS_JSON__
#define	__OWL_CORE_PARSERS_JSON__

	#include "core/containers/auto_ptr.h"
	#include "core/containers/hash_table.h"
	#include "core/containers/shared_pointer.h"
	#include "core/string.h"

namespace owl {

//!
struct	JSONArray;
struct	JSONString;

//!
struct	JSONData	:	public SharedObject
{
		enum
		{
			TypeArray	=	0,
			TypeObject,
			TypeString,
			TypeInt,
			TypeBool
		};

		uint				type, level;
		JSONData*			parent;

		JSONData(uint _type = 0, JSONData* _parent = 0);
virtual	~JSONData()
		{
			Log::i("JSONData T=\"%d\" L=\"%d\" << destructor called", type, level + 1);
		}

		JSONData&			operator = (const JSONData&);

virtual	const String		ToString();
};

//!
struct	JSONObject : public JSONData
{
		SharedHashMap<String, JSONData*> val;

		JSONObject();
		JSONObject(const JSONObject&);

virtual	~JSONObject()
		{
			Log::i("JSONObject << destructor called");
		}

		JSONObject&			operator = (const JSONObject&);

		const JSONObject&	GetObject(const String&) const;
		const JSONArray&	GetArray(const String& key) const;
		const String&		GetString(const String&) const;
		const int			GetInt(const String&) const;
		const bool			GetBool(const String&) const;

		const JSONObject&	AddObject(const String& key, JSONObject*);
		const JSONArray&	AddArray(const String& key, JSONArray*);
		const JSONString&	AddString(const String& key, JSONString*);
};

//!
struct	JSONArray : public JSONData
{
		SharedVector<JSONData>::type val;

		JSONArray();
		JSONArray(const JSONArray&);

		JSONArray&			operator =	(const JSONArray&);

		const int			Size() const				{	return val.Size();	}

		template<class T> void ForEach(T func)
		{
			const int s = Size();
			for (int i = 0; i < s; ++i)
			{
				JSONData* data = Get(i);
				func(data);
			}
		}

		JSONData*			Get(const int);
		const JSONData*		Get(const int) const;
		const JSONObject&	GetObject(const int) const;
		const JSONArray&	GetArray(const int) const;
		const String&		GetString(const int) const;
		const int			GetInt(const int) const;
		const bool			GetBool(const int) const;

		const JSONString&	AddString(JSONString*);
};

//!
struct	JSONString : public JSONData
{
		String				val;
		JSONString(const String&);

virtual	const String		ToString();
};

//!
struct	JSONInt : public JSONData
{
		int					val;
		JSONInt(const int n) : JSONData(TypeInt), val(n) {}

virtual	const String		ToString();
};

//!
struct	JSONBool : public JSONData
{
		bool				val;
		JSONBool(const bool b) : JSONData(TypeBool), val(b) {}

virtual	const String		ToString();
};


//!
class	JSONBuilder
{
		String				o;

public:

		JSONBuilder();

		
};

//!
class	JSONReader
{
		//!
		enum	State
		{	StateKey, StateValue	};

		String				json, key;

		int					cursor,
							size;

		AutoPtr<JSONData>	root;
		JSONData			*current, *val;

		State				state;

public:

		JSONReader(const String&);
		~JSONReader()
		{
//			if (root)
//				Log::i("Root ref count = %d", root->GetRef());
//			delete root;
		}

		const bool			IsRootArray() const			{	return root->type == JSONData::TypeArray;	}
		void				AddToCurrent() ;
		const String		ExtractString();
		const String		ExtractValue();

		const JSONData*		GetRoot() const				{	return root;	}
		const JSONObject&	GetRootObject() const		{	return *static_cast<const JSONObject*>(root.Raw());	}
		const JSONArray&	GetRootArray() const		{	return *static_cast<const JSONArray*>(root.Raw());	}

		void				Log(char);
};

}		//	owl
#endif	//	__OWL_CORE_PARSERS_JSON__
