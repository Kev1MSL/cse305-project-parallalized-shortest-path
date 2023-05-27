#include "monotonic_lock_free_set.h"

constexpr unsigned long monotonic_lock_free_set::LOWEST_KEY = 0;
constexpr unsigned long monotonic_lock_free_set::LARGEST_KEY = ULONG_MAX;

monotonic_lock_free_set::monotonic_lock_free_set()
{
	this->head_ = new atomic_node(LOWEST_KEY);
	this->head_->next = new atomic_node(LARGEST_KEY);
}

monotonic_lock_free_set::monotonic_lock_free_set(const monotonic_lock_free_set& other)
{
	this->head_ = new atomic_node(LOWEST_KEY);
	this->head_->next = new atomic_node(LARGEST_KEY);
	const atomic_node* other_pred = other.head_->next;
	while (other_pred->key != LARGEST_KEY)
	{
		add(other_pred->item);
		other_pred = other_pred->next;
	}

}

monotonic_lock_free_set::~monotonic_lock_free_set()
{
	const atomic_node* pred = this->head_;
	const atomic_node* cur = pred->next;
	while (cur != nullptr) {
		delete pred;
		pred = cur;
		cur = cur->next;
	}
	delete pred;
}

atomic_node* monotonic_lock_free_set::find(const int val, atomic_node*& pred) const
{
	const unsigned long key = std::hash<int>{}(val);
	pred = this->head_;
	atomic_node* cur = pred->next;
	while (cur->key < key) {
		pred = cur;
		cur = cur->next;
	}
	return cur;
}

bool monotonic_lock_free_set::add(const int val)
{
	const unsigned long key = std::hash<int>{}(val);

	while (true) {
		atomic_node* pred = nullptr;
		atomic_node* cur = find(val, pred);
		if (cur && cur->key == key) {
			return false;
		}

		atomic_node* node = new atomic_node(val, cur);
		if (pred->next.compare_exchange_strong(cur, node)) {
			size_.fetch_add(1);
			return true;
		}
		delete node;
	}
}



bool monotonic_lock_free_set::contains(const int val)
{
	atomic_node* pred = nullptr;
	unsigned long key = std::hash<int>{}(val);
	return find(val, pred)->key == key;
}

int monotonic_lock_free_set::get_size() const
{
	return size_.load();
}

bool monotonic_lock_free_set::empty() const
{
	return size_.load() == 0;
}

atomic_node* monotonic_lock_free_set::begin() const
{
	return head_->next;
}

atomic_node* monotonic_lock_free_set::end() const
{
	return nullptr;
}

monotonic_lock_free_set& monotonic_lock_free_set::operator=(const monotonic_lock_free_set& other)
{
	if (this == &other)
	{
		return *this;
	}

	const atomic_node* pred = this->head_;
	const atomic_node* cur = pred->next;
	while (cur != nullptr)
	{
		delete pred;
		pred = cur;
		cur = cur->next;
	}
	delete pred;

	this->head_ = new atomic_node(LOWEST_KEY);
	this->head_->next = new atomic_node(LARGEST_KEY);
	this->size_.store(0);
	const atomic_node* other_pred = other.head_->next;
	while (other_pred->key != LARGEST_KEY)
	{
		add(other_pred->item);
		other_pred = other_pred->next;
	}
	return *this;
}

atomic_node* monotonic_lock_free_set::advance(atomic_node* start, const size_t n)
{
	for (int i = 0; i < n; i++)
	{
		if (start == nullptr)
		{
			return nullptr;
		}
		start = start->next;
	}
	return start;
}


bool monotonic_lock_free_set::remove(const int val)
{
	const unsigned long key = std::hash<int>{}(val);

	while (true)
	{
		atomic_node* pred = nullptr;
		atomic_node* cur = find(val, pred);
		if (cur->key != key)
		{
			return false;
		}

		atomic_node* succ = cur->next;
		if (!cur->next.compare_exchange_strong(succ, succ))
		{
			continue;
		}

		if (!pred->next.compare_exchange_strong(cur, succ))
		{
			find(cur->item, pred);
		}
		delete cur;
		size_.fetch_sub(1);
		return true;
	}
}




