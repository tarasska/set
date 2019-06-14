//
// Created by taras on 14.06.19.
//

#ifndef SET__SET_HPP_
#define SET__SET_HPP_
#include <utility>
#include <iterator>

template<typename T>
struct node {
    node* left;
    node* right;
    node* parent;
    T const* data;

    void swap(node & other) {
        std::swap(left, other.left);
        std::swap(right, other.right);
        std::swap(parent, other.parent);
        std::swap(data, other.data);
    }

    node() = delete;

    node(node* l, node* r, node* p, T const& val)
        : left(l), right(r), parent(p), data(new T(val)) {};

    node(node const& other)
        : left(other.left), right(other.right), parent(other.parent) {
        data = new T(other.data);
    }

    node operator=(node const&) = delete;
};

template <typename T>
struct set_iterator {
    typedef T                                   value_type;
    typedef T&                                  reference;
    typedef std::ptrdiff_t                      difference_type;
    typedef T*                                  pointer;
    typedef std::bidirectional_iterator_tag     iterator_category;

    template <typename> friend class set;
    template <typename> friend class set_const_iterator;

    set_iterator() = default;;
    set_iterator(set_iterator const&) = default;
    set_iterator& operator=(set_iterator const&) = default;


    reference operator*() const {
        return *ptr_->val;
    }
    pointer operator->() const {
        return ptr_->val;
    }

    set_iterator& operator++() {
        ptr_ = ptr_->next;
        return *this;
    }
    set_iterator operator++(int) {
        set_iterator res(*this);
        ptr_ = ptr_->next;
        return res;
    }

    set_iterator& operator--() {
        ptr_ = ptr_->prev;
        return *this;
    }
    set_iterator operator--(int) {
        set_iterator res(*this);
        ptr_ = ptr_->prev;
        return res;
    }

    friend bool operator==(set_iterator const& a, set_iterator const& b) {
        return a.ptr_ == b.ptr_;
    }

    friend bool operator!=(set_iterator const& a, set_iterator const& b) {
        return a.ptr_ != b.ptr_;
    }


  private:
    explicit set_iterator(node<T>* n) : ptr_(n) {};

    node<T>* ptr_ = nullptr;


};

template <typename T>
struct set_const_iterator {
    typedef T                                   value_type;
    typedef T&                                  reference;
    typedef std::ptrdiff_t                      difference_type;
    typedef T*                                  pointer;
    typedef std::bidirectional_iterator_tag     iterator_category;

    template <typename> friend class set;

    set_const_iterator() = default;;
    set_const_iterator(set_const_iterator const&) = default;
    set_const_iterator& operator=(set_const_iterator const&) = default;


    reference operator*() const {
        return *ptr_->val;
    }
    pointer operator->() const {
        return ptr_->val;
    }

    set_const_iterator& operator++() {
        ptr_ = ptr_->next;
        return *this;
    }
    set_const_iterator operator++(int) {
        set_iterator res(*this);
        ptr_ = ptr_->next;
        return res;
    }

    set_const_iterator& operator--() {
        ptr_ = ptr_->prev;
        return *this;
    }
    set_const_iterator operator--(int) {
        set_iterator res(*this);
        ptr_ = ptr_->prev;
        return res;
    }

    friend bool operator==(set_const_iterator const& a, set_const_iterator const& b) {
        return a.ptr_ == b.ptr_;
    }

    friend bool operator!=(set_const_iterator const& a, set_const_iterator const& b) {
        return a.ptr_ != b.ptr_;
    }


  private:
    explicit set_const_iterator(node<T>* n) : ptr_(n) {};

    node<T>* ptr_ = nullptr;
};

template <typename T>
class set {
  private:

  public:
    using iterator = set_iterator<T>;
    using const_iterator = set_const_iterator<T const>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    set();
    set(set const& other);
    set& operator=(set const& other);
    ~set();

    void clear();
    bool empty();

    const_iterator begin() const;
    const_iterator end() const;

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    std::pair<iterator, bool> insert(T const&);
    iterator erase(iterator);
    const_iterator find(T const&) const;
    const_iterator lower_bound(T const&) const;
    const_iterator upper_bound(T const&) const;

    void swap(set& other);

};

#endif //SET__SET_HPP_
