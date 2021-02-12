#pragma once
// requires __FILENUM__

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
		_ListIterator_& operator++() { ptr = ptr->next; return *this; }
		const char* id() { return ptr->id; }
		Type* operator*() { return ptr->current; }
		bool exists() { return ptr!=NULL; }
		_ListIterator_(_ListObject_<Type>* p) { ptr=p; }
};

// Generic list
// Doesn't create nor remove objects of given Type. Instead, it operates solely on pointers
// You have to create objects on heap, pass pointer to them, and delete before removing
// Because of this, there are no requirements towards 'Type'. No required operators, destructors nor constructors
// Note: all c-strings used should have length equal to ID_LENGTH
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