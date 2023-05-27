#pragma once
#ifndef CSE305_MONOTONIC_LOCK_FREE_SET_H

#define CSE305_MONOTONIC_LOCK_FREE_SET_H

#include <atomic>
#include <climits>
#include <string>

class atomic_node
{
public:
    int item;
    unsigned long key;
    std::atomic<atomic_node*> next;

    explicit atomic_node(const int val, atomic_node* n = nullptr) {
        item = val;
        key = std::hash<int>{}(val);
        std::atomic_init<atomic_node*>(&next, n);
    }

    explicit atomic_node(unsigned long k) {
        item = -1;
        key = k;
        next = nullptr;
    }
};


class monotonic_lock_free_set
{
public:
    monotonic_lock_free_set();
    monotonic_lock_free_set(const monotonic_lock_free_set& other);
	~monotonic_lock_free_set();
	bool add(const int val);
	bool remove(const int val);
	bool contains(const int val);
    int get_size() const;
    bool empty() const;
    atomic_node* begin() const;
    atomic_node* end() const;
    monotonic_lock_free_set& operator= (const monotonic_lock_free_set& other);
    static atomic_node* advance(atomic_node* start, size_t n);

private:
    atomic_node* head_;
    std::atomic<int> size_;

    static const unsigned long LOWEST_KEY;
    static const unsigned long LARGEST_KEY;
    atomic_node* find(const int val, atomic_node*& pred) const;
};

#endif // !CSE305_MONOTONIC_LOCK_FREE_SET_H
