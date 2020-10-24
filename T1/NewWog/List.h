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
struct ListObject
{
	char id[ID_LENGTH];
	Type* current;
	ListObject<Type>* next;
	ListObject<Type>() { sprintf_s(id,ID_LENGTH,""); current = NULL; next = NULL; }
};

// Generic list
// Doesn't create nor remove objects of given Type. Instead, it operates solely on pointers
// You have to create objects on heap, pass pointer to them, and delete before removing
// Because of this, there are no requirements towards 'Type'. No required operators, destructors nor constructors
template<typename Type>
class List
{
	private:
		ListObject<Type>* top;
	
	public:
		unsigned int size() const;
		Type* front() { return top->current; } 
		Type* find(const char* id);
		bool push_front(const char* id, Type* ob);
		bool pop_front();
		bool remove(const char* id);
		List<Type>() { top = NULL; }
};

template<typename Type>
unsigned int List<Type>::size() const
{
	STARTNA(__LINE__, 0)
	unsigned int output = 0;
	for(ListObject<Type>* current = this->top; current != NULL; current = current->next)
	{
		++output;
	}
	RETURN(output);
}

template<typename Type>
Type* List<Type>::find(const char* id)
{
	STARTNA(__LINE__, 0)
	for(ListObject<Type>* current = top; current; current = current->next)
	{
		if(!compare(id,current->id,ID_LENGTH))
			RETURN(current->current);
	}
	RETURN(NULL);
}

template<typename Type>
bool List<Type>::push_front(const char* id, Type* ob) // false - already in list (overlapping id)
{
	STARTNA(__LINE__, 0)
	if(this->find(id)) RETURN(false)
	ListObject<Type>* object = new ListObject<Type>;
	object->current = ob;
	sprintf_s(object->id,ID_LENGTH,"%s",id);
	object->next = this->top;
	this->top = object;
	RETURN(true);
}

template<typename Type>
bool List<Type>::pop_front() // false - list is empty
{
	STARTNA(__LINE__, 0)
	ListObject<Type>* next = NULL;
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
bool List<Type>::remove(const char* id) // false - there is no object with such ID
{
	STARTNA(__LINE__, 0)
	if(this->find(id)==NULL) { RETURN(false); }  // not found
	ListObject<Type>* current = top;
	if(current == NULL) { RETURN(false); } // empty
	if(!compare(current->id,id,ID_LENGTH)) { RETURN(this->pop_front()); } // first element - use pop_front
	ListObject<Type>* next = current->next;
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