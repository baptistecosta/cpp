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

		uint				type;
		JSONData*			parent;

		JSONData(uint _type, JSONData* _parent = 0);

		JSONData&			operator = (const JSONData&);

virtual	String				ToString();
};

//!
struct	JSONArray;

//!
struct	JSONObject : public JSONData
{
		SharedHashMap<String, JSONData*> val;

		JSONObject();
		JSONObject(const JSONObject&);

		JSONObject&			operator = (const JSONObject&);

		const JSONObject&	GetObject(const String&) const;
		const JSONArray&	GetArray(const String& key) const;
		const String&		GetString(const String&) const;
		const int			GetInt(const String&) const;
		const bool			GetBool(const String&) const;
};

//!
struct	JSONArray : public JSONData
{
		SharedVector<JSONData>::type val;

		JSONArray();
		JSONArray(const JSONArray&);

		JSONArray&			operator =	(const JSONArray&);

		JSONData*			Get(const int);
		const JSONData*		Get(const int) const;
		const JSONObject&	GetObject(const int) const;
		const JSONArray&	GetArray(const int) const;
		const String&		GetString(const int) const;
		const int			GetInt(const int) const;
		const bool			GetBool(const int) const;

		template<class T> void ForEach(T func)
		{
			const int s = Size();
			for (int i = 0; i < s; ++i)
			{
				SharedPtr<JSONData> data = Get(i);
				func(data.Raw());
			}
		}

		const int			Size() const				{	return val.Size();	}
};

//!
struct	JSONString : public JSONData
{
		String				val;
		JSONString(const String&);

virtual	String				ToString();
};

//!
struct	JSONInt : public JSONData
{
		int					val;
		JSONInt(const int n) : JSONData(TypeInt), val(n) {}

virtual	String				ToString();
};

//!
struct	JSONBool : public JSONData
{
		bool				val;
		JSONBool(const bool b) : JSONData(TypeBool), val(b) {}

virtual	String				ToString();
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
							size,
							level;

		JSONData			*root, *current, *val;

		State				state;

public:

		JSONReader(const String&);

		const bool			IsRootArray() const			{	return root->type == JSONData::TypeArray;	}
		void				AddToCurrent() ;
		const String		ExtractString();
		const String		ExtractValue();

		const JSONData*		GetRoot() const				{	return root;	}
		const JSONObject&	GetRootObject() const		{	return *static_cast<const JSONObject*>(root);	}
		const JSONArray&	GetRootArray() const		{	return *static_cast<const JSONArray*>(root);	}

		void				Log(char);
};

}		//	owl
#endif	//	__OWL_CORE_PARSERS_JSON__
