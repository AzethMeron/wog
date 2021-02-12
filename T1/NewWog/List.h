#pragma once
// requires __FILENUM__
// must be included AFTER __FILENUM__ is defined in cpp file!

// by Jakub Grzana

/****************************** DOCUMENTATION ******************************/
// _List_<Type> Generic list
// Doesn't create nor remove objects of given Type. Instead, it operates solely on pointers
// You have to create objects on heap, pass pointer to them, and manually delete before removing from list
// Because of this, there are no requirements towards 'Type'. No required operators, destructors nor constructors
//
// Features:
// - fully generic, very simple
// - add, remove items to list
// - get number of items in list
// - every item in list has "id", which is c-string, length=ID_LENGTH
// - there might be ONLY one item for every name
// - tested (tho briefly)
// 
// Trying to add multiple objects with the same name means ONLY THE FIRST IS ADDED, rest is IGNORED!
// Basically this list helps only with storing, finding, and iterating (by using _ListIterator_<Type>)
// You must make your own tools to add/remove objects, for every task you want to use it. Still helpful IMO.
//
// Methods: 
// unsigned int size() const - measure number of items in List (complexity depends on number of items O(n))
// const char* id_front() const - get name of first item (c-string)
// Type* front() - get pointer to first object (NULL if none)
// Type* find(const char* id) - get pointer to object with given name (NULL if not found)
// bool push_front(const char* id, Type* ob) - adds object 'ob' into list with given name. Returns false if there's already object with this name in list
// bool pop_front() - remove first object from list, false if there's none
// bool remove(const char* id) - removes item with given name
// _ListIterator_<Type> begin() - get iterator to first object
/***************************************************************************/

#define ID_LENGTH (32)

// true - different
// false - same
bool compare(const char* a, const char* b, int len)
{
	STARTNA(__LINE__, 0)
	for(int i = 0; i < len; ++i)
	{
		if(a[i] != b[i]) { RETURN(true); }
	}
	RETURN(false);
}

template<typename Type>
struct _ListObject_
{
	char id[ID_LENGTH];
	Type* current;
	_ListObject_<Type>* next;
	_ListObject_<Type>() { char str[ID_LENGTH]=""; for(int i=0; i < ID_LENGTH; ++i) { id[i] = str[i]; } current = NULL; next = NULL; }
};

template<typename Type>
struct _ListIterator_ 
{
	private:
		_ListObject_<Type>* ptr;
	public:
		_ListIterator_& operator++() { ptr = ptr->next; return *this; } // get next object
		const char* id() { return ptr->id; } // get name (id) of current object
		Type* operator*() { return ptr->current; } // get current object
		bool exists() { return ptr!=NULL; } // check whether there's object in this iterator
		_ListIterator_(_ListObject_<Type>* p) { ptr=p; } 
};

template<typename Type>
class _List_
{
	private:
		_ListObject_<Type>* top;
	
	public:
		unsigned int size() const;
		const char* id_front() const;
		Type* front() { if(top) { return top->current; } else { return NULL; } } 
		Type* find(const char* id);
		bool push_front(const char* id, Type* ob);
		bool pop_front();
		bool remove(const char* id);
		_ListIterator_<Type> begin() { _ListIterator_<Type> output(top); return output; }
		_List_<Type>() { top = NULL; }
};

template<typename Type>
unsigned int _List_<Type>::size() const
{
	STARTNA(__LINE__, 0)
	unsigned int output = 0;
	for(_ListObject_<Type>* current = this->top; current != NULL; current = current->next)
	{
		++output;
	}
	RETURN(output);
}

template<typename Type>
Type* _List_<Type>::find(const char* id)
{
	STARTNA(__LINE__, 0)
	for(_ListObject_<Type>* current = top; current; current = current->next)
	{
		if(!compare(id,current->id,ID_LENGTH))
			RETURN(current->current);
	}
	RETURN(NULL);
}

template<typename Type>
bool _List_<Type>::push_front(const char* id, Type* ob) // false - already in list (overlapping id)
{
	STARTNA(__LINE__, 0)
	if(this->find(id)) RETURN(false)
	_ListObject_<Type>* object = new _ListObject_<Type>;
	object->current = ob;
	sprintf_s(object->id,ID_LENGTH,"%s",id);
	object->next = this->top;
	this->top = object;
	RETURN(true);
}

template<typename Type>
bool _List_<Type>::pop_front() // false - list is empty
{
	STARTNA(__LINE__, 0)
	_ListObject_<Type>* next = NULL;
	if(top)
	{
		next = top->next;
		delete top;
		top = next;
		RETURN(true);
	}
	RETURN(false);
}

template<typename Type>
bool _List_<Type>::remove(const char* id) // false - there is no object with such ID
{
	STARTNA(__LINE__, 0)
	if(this->find(id)==NULL) { RETURN(false); }  // not found
	_ListObject_<Type>* current = top;
	if(current == NULL) { RETURN(false); } // empty
	if(!compare(current->id,id,ID_LENGTH)) { RETURN(this->pop_front()); } // first element - use pop_front
	_ListObject_<Type>* next = current->next;
	while(next)
	{
		if(!compare(next->id,id,ID_LENGTH))
		{
			current->next = next->next;
			delete next;
			RETURN(true); // successfully removed
		}
		current = next;
		next = next->next;
	}
	RETURN(false); // not found - flow should never reach this place
}

template<typename Type>
const char* _List_<Type>::id_front() const
{
	STARTNA(__LINE__, 0)
	if(this->top) { RETURN(this->top->id); }
	RETURN(NULL);
}