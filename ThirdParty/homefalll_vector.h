/*
* Author: Overfalll Unipessoal Lda
* Desc: Sample Arduino Homefalll Vector Class
* Version: 0.0.1
* Date: 01/08/2021
*/

#pragma once

template <class T>
class HomefalllNode {
	public:
		T element;
		HomefalllNode* next;
		HomefalllNode* prev;
		
		HomefalllNode(T element, HomefalllNode* prev, HomefalllNode* next) : element(element) {
			this->next = next;
			this->prev = prev;
		};
};

template <class T>
class HomefalllVector  {
	typedef HomefalllNode<T>* ptr_node_type;
	
	public:
		HomefalllVector();
		HomefalllVector(const HomefalllVector<T>&);
		~HomefalllVector();
	
	public:
		T&		getCurrent();
		T&		getNext();
		T&		First() const;
		T&		Last() const;
		
		int		getLength();
		void	Append(T);
		void	DeleteLast();
		void	DeleteFirst();
		void	DeleteCurrent();
		bool	next();
		bool	moveToStart();
		bool	prev();
		void	Delete(T&);
		bool	Search(T);
		void	Clear();
		void	PutFirstToLast();
		void	Update(T elem);
		
		HomefalllVector& operator = (const HomefalllVector<T>&);
	
	private:
		int length;
		ptr_node_type head, tail, curr;
};

template <class T>
HomefalllVector<T>::HomefalllVector() {
	length = 0;
	head = nullptr;
	tail = nullptr;
	curr = nullptr;
}

template <class T>
HomefalllVector<T>::HomefalllVector(const HomefalllVector<T> & list) {
	length = 0;
	head = nullptr;
	tail = nullptr;
	curr = nullptr;

	HomefalllNode<T> * temp = list.head;

	while(temp != nullptr) {
		Append(temp->element);
		temp = temp->next;
	}
}

template <class T>
HomefalllVector<T> & HomefalllVector<T>::operator=(const HomefalllVector<T> & list)
{
	Clear();
	
	HomefalllNode<T> * temp = list.head;
	while(temp != nullptr) {
		Append(temp->element);
		temp = temp->next;
	}
	
	return *this;
}

template <class T>
HomefalllVector<T>::~HomefalllVector() {
	Clear();
}

template<class T>
T& HomefalllVector<T>::getCurrent() {
	return curr->element;
}

template <class T>
T& HomefalllVector<T>::getNext() {
	curr = curr->next;
	return getCurrent();
}

template<class T>
T& HomefalllVector<T>::First() const {
	return head->element;
}

template<class T>
T& HomefalllVector<T>::Last() const {
	return tail->element;
}

template<class T>
int HomefalllVector<T>::getLength() {
	return length;
}

template <class T>
void HomefalllVector<T>::Append(T element)
{
	HomefalllNode<T> * node = new HomefalllNode<T>(element, tail, nullptr);
	
	if(length == 0)
		curr = tail = head = node;
	else {
		tail->next = node;
		tail = node;
	}

	length++;
}

template <class T>
void HomefalllVector<T>::DeleteLast() {
	if(length == 0)
		return;
	
	curr = tail;
	DeleteCurrent();
}

template <class T>
void HomefalllVector<T>::DeleteFirst() {
	if(length == 0)
		return;
	
	curr = head;
	DeleteCurrent();
}

template <class T>
bool HomefalllVector<T>::next() {
	if(length == 0)
		return false;

	if(curr->next == nullptr)
		return false;

	curr = curr->next;
	return true;
}

template <class T>
bool HomefalllVector<T>::moveToStart() {
	curr = head;
	return length != 0;
}

template<class T>
bool HomefalllVector<T>::prev() {
	if(length == 0)
		return false;
	
	if(curr->prev != nullptr)
		return false;
	
	curr = curr->prev;
		return true;
}

template <class T>
void HomefalllVector<T>::Delete(T & elem) {
	if(Search(elem))
		DeleteCurrent();
}

template <class T>
void HomefalllVector<T>::DeleteCurrent() {
	if(length == 0)
		return;
	
	length--;
	HomefalllNode<T> * temp = curr;

	if(temp->prev != nullptr)
		temp->prev->next = temp->next;
	
	if(temp->next != nullptr)
		temp->next->prev = temp->prev;

	if(length == 0)
		head = curr = tail = nullptr;
	else if(curr == head)
		curr = head = head->next;
	else if(curr == tail)
		curr = tail = tail->prev;
	else
		curr = curr->prev;

	delete temp;
}

template <class T>
bool HomefalllVector<T>::Search(T elem) {
	if(length == 0)
		return false;
	
	if(moveToStart()) {
		do {
			if(curr->element == elem)
				return true;
		} while (next());
	}
	
	return false;
}

template <class T>
void HomefalllVector<T>::PutFirstToLast() {
	if(length < 2)
		return;
	
	HomefalllNode<T>* temp = head->next;
	head->next->prev = nullptr;
	head->next = nullptr;
	head->prev = tail;
	tail->next = head;
	
	tail = head;
	head = temp;
}

template <class T>
void HomefalllVector<T>::Update(T elem) {
	curr->element = elem;
}

template <class T>
void HomefalllVector<T>::Clear() {
	if(length == 0)
		return;
	
	HomefalllNode<T> * temp = head;
	while(temp != nullptr) {
		head = head->next;
		delete temp;
		temp = head;
	}

	head = curr = tail = nullptr;
	length = 0;
}
