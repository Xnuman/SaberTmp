#ifndef XNUMER_GEOMETRY_PSEUDO_LIST_HPP
#define XNUMER_GEOMETRY_PSEUDO_LIST_HPP

#include <vector>
#include <stack>

template<typename T, typename V>
struct is_underlying_type {
public:
	using type = std::remove_reference_t<T>;
	static const bool value = std::is_same<typename type::value_type, V>::value;
};


template<typename, typename, typename = void>
struct is_valid_container : std::false_type {};

template<typename T, typename V>
struct is_valid_container<T, V, std::void_t<
	decltype(std::declval<T>().begin()),
	decltype(std::declval<T>().end()),
	decltype(std::declval<T>().size()),
	std::enable_if_t<is_underlying_type<T, V>::value, std::true_type>>> : std::true_type{};

template<typename T>
class PseudoList {
public:
	struct Node;

	std::vector<uint32_t> prev_;
	std::vector<uint32_t> next_;
	std::vector<T> data_;
	std::vector<uint32_t> nextMonotone_;

	PseudoList();

	template<typename Container, typename Type>
	using validation_arg = typename std::enable_if_t<is_valid_container<Container, Type>::value, Container>;

	template<typename Container>
	PseudoList(const Container& container, typename validation_arg<Container, T>* t = nullptr);

	template<typename Container>
	PseudoList(Container&& container, typename validation_arg<Container, T>* t = nullptr);

	PseudoList(std::initializer_list<T> initList);


	PseudoList(const PseudoList<T>& other);
	//PseudoList(PseudoList<T>&& other);

	~PseudoList();

	//template<typename Container, typename validation_arg<Container, T> * t = nullptr>
	//PseudoList<T>& operator=(const Container& container);

	//template<typename Container, typename validation_arg<Container, T> * t = nullptr>
	//PseudoList<T>& operator=(Container&& container);

	PseudoList<T>& operator=(const PseudoList<T>& other);
	PseudoList<T>& operator=(PseudoList<T>&& other);

	bool operator==(const PseudoList<T>& other) const;
	bool operator!=(const PseudoList<T>& other) const;

	Node push_front(const T& value);
	Node push_front(T&& value);

	Node push_back(const T& value);
	Node push_back(T&& value);

	void pop_front();
	void pop_back();

	Node front();
	Node back();

	uint32_t size() const;
	void remove(uint32_t ind);
	Node get(uint32_t ind);
	void setMonotone(uint32_t ind, uint32_t monotoneInd) {
		nextMonotone_[ind] = monotoneInd;
	}
private:
	uint32_t push(const T& value);
	uint32_t push(T&& value);


	uint32_t head;
	uint32_t tail;

	uint32_t size_;
	std::stack<uint32_t> holes;

	Node next(uint32_t ind);
	Node prev(uint32_t ind);
	T data(uint32_t ind) const;
//
//	Node<T> insert_after(uint32_t ind, const T& value);
//	Node<T> insert_after(uint32_t ind, T&& value);
//

//

};

template<typename T>
struct PseudoList<T>::Node {
public:
	Node next() const {
		if (outer != nullptr) {
			return outer->next(ind);
		}
	}

	Node prev() const {
		if (outer != nullptr) {
			return outer->prev(ind);
		}
	}

	uint32_t getInd() const {
		return ind;
	}

	//Node insert(const T& value) {
	//	if (outer != nullptr) {
	//		outer->insert_after(ind, value);
	//	}
	//}

	//Node insert(T&& value) {
	//	if (outer != nullptr) {
	//		outer->insert_after(ind, std::move(value));
	//	}
	//}

	T data() const {
		return outer->data(ind);
	}

	void setNextMonotone(uint32_t monotoneInd) {
		outer->setMonotone(ind, monotoneInd);
	}

	Node getNextMonotone() {
		return Node(outer, outer->nextMonotone_[ind]);
	}

	bool operator==(const Node& other) const {
		return ind == other.ind;
	}

	bool operator!=(const Node& other) const {
		return !operator==(other);
	}

	bool operator<(const Node& other) const {
		return ind < other.ind;
	}
	bool operator>(const Node& other) const {
		return !operator==(other) && !operator<(other);
	}
	bool operator <=(const Node& other) const {
		return operator<(other) || operator==(other);
	}
	bool operator >=(const Node& other) const {
		return !operator<(other);
	}
private:
	friend class PseudoList;
	uint32_t ind;
	PseudoList* outer;

	Node(PseudoList* outer, uint32_t ind) : outer(outer), ind(ind) {}
};



template<typename T>
PseudoList<T>::PseudoList() : size_(0U), head(UINT32_MAX), tail(UINT32_MAX) {}

template<typename T>
template<typename Container>
PseudoList<T>::PseudoList(const Container& container, typename validation_arg<Container, T>* t) : 
	size_(container.size()), head(0U), tail(container.size() - 1) {


	data_.reserve(size_);
	prev_.reserve(size_);
	next_.reserve(size_);
	nextMonotone_.reserve(size_);


	uint32_t prev = size_ - 1;
	uint32_t next = 0;

	for (auto initValue : container) {
		data_.push_back(initValue);

		prev_.push_back(prev);
		prev = next;
		next_.push_back(++next);
		nextMonotone_.push_back(next);
	}
	next_.back() = 0;
	nextMonotone_.back() = 0;
}

template<typename T>
PseudoList<T>::PseudoList(std::initializer_list<T> initList) : 
	PseudoList(initList, static_cast<std::initializer_list<T>*>(nullptr)) {}

template<typename T>
template<typename Container>
PseudoList<T>::PseudoList(Container&& container, typename validation_arg<Container, T>* c) : 
	size_(container.size()), head(0U), tail(container.size() - 1) {

	data_.reserve(size_);
	prev_.reserve(size_);
	next_.reserve(size_);
	nextMonotone_.reserve(size_);

	uint32_t prev = size_ - 1;
	uint32_t next = 0;

	for (auto&& initValue : container) {
		data_.push_back(std::move(initValue));

		prev_.push_back(prev);
		prev = next;
		next_.push_back(++next);
		nextMonotone_.push_back(next);
	}
	next_.back() = 0;
	nextMonotone_.back() = 0;

}

template<typename T>
PseudoList<T>::PseudoList(const PseudoList<T>& other) :
	data_(other.data_), prev_(other.prev_), next_(other.next_), size_(other.size_), head(other.head), tail(other.head), 
nextMonotone_(other.nextMonotone_) {}


//template<typename T>
//PseudoList<T>::PseudoList(PseudoList<T>&& other) : 
//	data_(std::move(other.data_)),
//	prev_(std::move(other.prev_)),
//	next_(std::move(other.next_)),
//	nextMonotone_(std::move(other.nextMonotone_)),
//	size_(std::move(other.size_)),
//	head (std::move(other.head)),
//	tail(std::move(other.tail)) {}

template<typename T>
PseudoList<T>::~PseudoList() {
	data_.clear();
	next_.clear();
	prev_.clear();
	nextMonotone_.clear();
	size_ = 0;
}

template<typename T>
PseudoList<T>& PseudoList<T>::operator=(const PseudoList<T>& other) {

	if (this != &other) {
		size_ = other.size_;
		head = other.head;
		tail = other.tail;

		data_ = other.data_;
		next_ = other.next_;
		prev_ = other.prev_;
		nextMonotone_ = other.nextMonotone_;
	}

	return *this;

}

template<typename T>
PseudoList<T>& PseudoList<T>::operator=(PseudoList&& other) {

	if (this != &other) {
		size_ = std::move(other.size_);
		head = std::move(other.head);
		tail = std::move(other.tail);

		data_ = std::move(other.data_);
		next_ = std::move(other.next_);
		prev_ = std::move(other.prev_);
		nextMonotone_ = std::move(other.nextMonotone_);
	}

	return *this;
}

template<typename T>
bool PseudoList<T>::operator==(const PseudoList<T>& other) const{
	return (size_ == other.size_) && (head == other.head) && (tail == other.tail) &&
		(next_ == other.next_) && (prev_ == other.prev_) && (data_ == other.data_);
 }

template<typename T>
bool PseudoList<T>::operator!=(const PseudoList<T>& other) const {
	return !(this->operator==(other));
}

template<typename T>
uint32_t PseudoList<T>::push(const T& value) {

	if (size_++ == 0) {
		head = 0;
		tail = 0;
	}

	uint32_t ind;

	/*if (holes.empty()) {*/

		ind = size_ - 1;

		data_.push_back(value);
		prev_.push_back(tail);
		next_.push_back(head);
		nextMonotone_.push_back(head);
	//}
	//else {

	//	ind = holes.top();

	//	data_[ind] = value;
	//	prev_[ind] = tail;
	//	next_[ind] = head;
	//	nextMonotone_[ind] = head;

	//	holes.pop();
	//}

	prev_[head] = ind;
	next_[tail] = ind;
	nextMonotone_[tail] = ind;

	return ind;
}

template<typename T>
uint32_t PseudoList<T>::push(T&& value) {
	if (size_++ == 0) {
		head = 0;
		tail = 0;
	}

	uint32_t ind;

	/*if (holes.empty()) {*/

		ind = size_ - 1;

		data_.push_back(std::move(value));
		prev_.push_back(tail);
		next_.push_back(head);
		nextMonotone_.push_back(head);
	/*}*/
	//else {

	//	ind = holes.top();

	//	data_[ind] = std::move(value);
	//	prev_[ind] = tail;
	//	next_[ind] = head;
	//	nextMonotone_[ind] = head;

	//	holes.pop();
	//}

	prev_[head] = ind;
	next_[tail] = ind;
	nextMonotone_[tail] = ind;

	return ind;
}

template<typename T>
typename PseudoList<T>::Node PseudoList<T>::push_front(const T& value) {

	uint32_t ind = push(value);

	if (size == 2) {

	}

	head = ind;
	return Node(this, ind);
}

template<typename T>
typename PseudoList<T>::Node PseudoList<T>::push_front(T&& value) {
	
	uint32_t ind = push(std::move(value));
	head = ind;
	return Node(this, ind);
}

template<typename T>
typename PseudoList<T>::Node PseudoList<T>::push_back(const T& value) {

	uint32_t ind = push(value);

	tail = ind;
	return Node(this, ind);
}

template<typename T>
typename PseudoList<T>::Node PseudoList<T>::push_back(T&& value) {

	uint32_t ind = push(std::move(value));
	tail = ind;
	return Node(this, ind);
}

template<typename T>
void PseudoList<T>::remove(uint32_t ind) {

//	std::find(data_.begin(), data_.end(), )

	holes.push(ind);

	uint32_t prev = prev_[ind];
	uint32_t next = next_[ind];
	uint32_t nextMonotone = nextMonotone_[ind];

	prev_[next] = prev;
	next_[prev] = next;
	//nextMonotone_[]

	if (ind == head) {
		head = next;
	}
	if (ind == tail) {
		tail = prev;
	}

	//--size_;
}

template<typename T>
void PseudoList<T>::pop_front() {
	if (size() > 0) {
		remove(head);
	}
}

template<typename T>
void PseudoList<T>::pop_back() {
	if (size() > 0) {
		remove(tail);
	}
}

template<typename T>
typename PseudoList<T>::Node PseudoList<T>::front() {
	if (size() > 0) {
		return Node(this, head);
	}
}

template<typename T>
typename PseudoList<T>::Node PseudoList<T>::back() {
	if (size() > 0) {
		return Node(this, tail);
	}
}

template<typename T>
uint32_t PseudoList<T>::size() const {
	return size_;
}

template<typename T>
typename PseudoList<T>::Node PseudoList<T>::next(uint32_t ind) {
	return Node(this, next_[ind]);
}
template<typename T>
typename PseudoList<T>::Node PseudoList<T>::prev(uint32_t ind) {
	return Node(this, prev_[ind]);
}

template<typename T>
T PseudoList<T>::data(uint32_t ind) const {
	return data_[ind];
}

template<typename T>
typename PseudoList<T>::Node PseudoList<T>::get(uint32_t ind) {
	return Node(this, ind);
}


#endif
