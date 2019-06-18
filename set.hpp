//
// Created by taras on 14.06.19.
//

#ifndef SET__SET_HPP_
#define SET__SET_HPP_
#include <utility>
#include <iterator>
#include <iostream>
#include <memory>

template<typename>
class set;

template<typename T>
struct node {
    std::unique_ptr<node> left_;
    std::unique_ptr<node> right_;
    node* parent_;
    T val_;

//    void swap(node& other) {
//        std::swap(left_, other.left_);
//        std::swap(right_, other.right_);
//        std::swap(parent_, other.parent_);
//        std::swap(val_, other.val_);
//    }

    node() = delete;

    node(node* l, node* r, node* p, T const& val)
        : left_(l), right_(r), parent_(p), val_(val) {};

    node(node const& other)
        : left_(other.left_), right_(other.right_),
          parent_(other.parent_), val_(other.val_) {}

    node operator=(node const&) = delete;
};


template<typename T>
class set {
  public:
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;
    typedef T* pointer;
    typedef std::unique_ptr<node<value_type>> u_ptr;

    struct set_iterator {
        typedef T value_type;
        typedef T const& reference;
        typedef std::ptrdiff_t difference_type;
        typedef T const* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;

        template<typename> friend
        class set;


        set_iterator() = default;
        set_iterator(set_iterator const&) = default;
        set_iterator& operator=(set_iterator const&) = default;

        reference operator*() const {
            return ptr_->val_;
        }
        pointer operator->() const {
            return &ptr_->val_;
        }

        set_iterator& operator++() {
            ptr_ = set<value_type>::next_node(ptr_);
            return *this;
        }
        set_iterator operator++(int) {
            set_iterator res(*this);
            ++(*this);
            return res;
        }

        set_iterator& operator--() {
            if (ptr_ == nullptr) {
                ptr_ = find_max(current_root_ptr_);
            } else {
                ptr_ = set<value_type>::prev_node(ptr_);
            }
            return *this;
        }
        set_iterator operator--(int) {
            set_iterator res(*this);
            --(*this);
            return res;
        }

        friend bool operator==(set_iterator const& a, set_iterator const& b) {
            return a.ptr_ == b.ptr_ && a.current_root_ptr_ == b.current_root_ptr_;
        }

        friend bool operator!=(set_iterator const& a, set_iterator const& b) {
            return a.ptr_ != b.ptr_ || a.current_root_ptr_ != b.current_root_ptr_;
        }

      private:
        explicit set_iterator(node<value_type>* n, node<value_type>* const root)
            : ptr_(n), current_root_ptr_(root) {};

        node<value_type>* ptr_ = nullptr;

        node<value_type>* current_root_ptr_ = nullptr;

    };

  private:
    u_ptr root;

    /* find next node or return nullptr */
    template<typename V>
    static node<V>* next_node(node<V>* cur) noexcept {
        if (cur == nullptr || (cur->parent_ == nullptr && cur->right_ == nullptr)) {
            return nullptr;
        }
        if (cur->right_ != nullptr) {
            cur = cur->right_.get();
            while (cur->left_ != nullptr) {
                cur = cur->left_.get();
            }
            return cur;
        }

        node<V>* last;
        do {
            last = cur;
            cur = cur->parent_;
        } while (cur->parent_ != nullptr && cur->right_.get() == last);

        if (cur->right_.get() != last) {
            return cur;
        } else {
            return nullptr;
        }
    }

    /* find prev node or return nullptr */
    template<typename V>
    static node<V>* prev_node(node<V>* cur) noexcept {
        if (cur == nullptr || (cur->parent_ == nullptr && cur->left_ == nullptr)) {
            return nullptr;
        }
        if (cur->left_ != nullptr) {
            cur = cur->left_.get();
            while (cur->right_ != nullptr) {
                cur = cur->right_.get();
            }
            return cur;
        }

        node<V>* last;
        do {
            last = cur;
            cur = cur->parent_;
        } while (cur->parent_ != nullptr && cur->left_.get() == last);

        if (cur->left_.get() != last) {
            return cur;
        } else {
            return nullptr;
        }
    }

    /* if cpy_v != root of current tree (first step) is undefined */

    node<value_type>* copy_tree(node<value_type>* cpy_node) {
        if (cpy_node == nullptr) {
            return nullptr;
        }
        u_ptr new_root(new node<value_type>(nullptr, nullptr, nullptr, cpy_node->val_));
        new_root->left_.reset(copy_tree_dfs(new_root.get(), cpy_node->left_.get()));
        new_root->right_.reset(copy_tree_dfs(new_root.get(), cpy_node->right_.get()));
        return new_root.release();
    }

    node<value_type>* copy_tree_dfs(node<value_type>* last, node<value_type>* cpy_node) {
        if (cpy_node == nullptr) {
            return nullptr;
        }
        u_ptr new_v(new node<value_type>(nullptr, nullptr, last, cpy_node->val_));
        new_v->left_.reset(copy_tree_dfs(new_v.get(), cpy_node->left_.get()));
        new_v->right_.reset(copy_tree_dfs(new_v.get(), cpy_node->right_.get()));
        return new_v.release();
    }

    static node<value_type>* find_min(node<value_type>* v) noexcept {
        if (v == nullptr) {
            return nullptr;
        } else {
            while (v->left_ != nullptr) {
                v = v->left_.get();
            }
            return v;
        }
    }

    static node<value_type>* find_max(node<value_type>* v) noexcept {
        if (v == nullptr) {
            return nullptr;
        } else {
            while (v->right_ != nullptr) {
                v = v->right_.get();
            }
            return v;
        }
    }

    std::pair<node<value_type>*, bool> insert_(const_reference elem) {
        if (root == nullptr) {
            root.reset(new node<value_type>(nullptr, nullptr, nullptr, elem));
            return {root.get(), true};
        }
        node<value_type>* v = root.get();
        node<value_type>* last = root.get();
        while (v != nullptr) {
            if (v->val_ == elem) {
                return {v, false};
            } else if (v->val_ > elem) {
                last = v;
                v = v->left_.get();
            } else {
                last = v;
                v = v->right_.get();
            }
        }
        if (last->val_ > elem) {
            last->left_.reset(new node<value_type>(nullptr, nullptr, last, elem));
            return {last->left_.get(), true};
        } else {
            last->right_.reset(new node<value_type>(nullptr, nullptr, last, elem));
            return {last->right_.get(), true};
        }
    }

    node<value_type>* find_(const_reference elem) const noexcept {
        node<value_type>* v = root.get();
        while (v != nullptr && v->val_ != elem) {
            if (v->val_ > elem) {
                v = v->left_.get();
            } else {
                v = v->right_.get();
            }
        }
        return v;
    }

    node<value_type>* set_node_as_right_child(node<value_type>* v) {
        if (v->parent_ != nullptr) {
            if (v->parent_->left_.get() == v) {
                v = v->parent_->left_.release();
                v->parent_->left_.reset(v->right_.release());
                if (v->parent_->left_ != nullptr) {
                    v->parent_->left_->parent_ = v->parent_;
                }
            } else {
                v = v->parent_->right_.release();
                v->parent_->right_ .reset(v->right_.release());
                if (v->parent_->right_ != nullptr) {
                    v->parent_->right_->parent_ = v->parent_;
                }
            }
        } else {
            throw std::runtime_error("bad node to set as right child");
        }
        return v;
    }


    node<value_type>* set_tree_node(node<value_type>* old_node, node<value_type>* new_node) {
        if (old_node->left_ != nullptr) {
            old_node->left_->parent_ = new_node;
        }
        if (old_node->right_ != nullptr) {
            old_node->right_->parent_ = new_node;
        }
        new_node->left_.reset(old_node->left_.release());
        new_node->right_.reset(old_node->right_.release());
        if (old_node->parent_ != nullptr) {
            auto* old_parent = old_node->parent_;
            if (old_node->parent_->left_.get() == old_node) {
                old_node->parent_->left_.reset(new_node);
            } else {
                old_node->parent_->right_.reset(new_node);
            } // after if old node is deleted
            new_node->parent_ = old_parent;
        } else {
            new_node->parent_ = nullptr;
            root.reset(new_node); //delete old node
        }
        return new_node;
    }


    node<value_type>* erase_(node<value_type>* del_node) {
        if (del_node == nullptr) {
            return nullptr;
        }
        node<value_type>* subtree_min = find_min(del_node->right_.get());
        if (subtree_min == nullptr) {
            node<value_type>* next = next_node(del_node);
            if (del_node->parent_ == nullptr) {
                root.reset(del_node->left_.get()); // delete del_node;
                if (root != nullptr) {
                    root->parent_ = nullptr;
                }
            } else {
                if (del_node->parent_->left_.get() == del_node) {
                    if (del_node->left_ == nullptr) {
                        del_node->parent_->left_.reset(nullptr); // delete del_node
                    } else {
                        del_node->left_->parent_ = del_node->parent_;
                        del_node->parent_->left_.reset(del_node->left_.release()); // delete del_node
                    }
                } else {
                    if (del_node->left_.get() == nullptr) {
                        del_node->parent_->right_ = nullptr;
                    } else {
                        del_node->left_->parent_ = del_node->parent_;
                        del_node->parent_->right_.reset(del_node->left_.release()); // delete del_node
                    }
                }
            }
            return next;
        } else {
            // attention unsafe data pointer return
            subtree_min = set_node_as_right_child(subtree_min);
            // put subtree min in safety storage and delete del_node
            subtree_min = set_tree_node(del_node, subtree_min);
            return subtree_min;
        }
    }

    node<value_type>* lower_(const_reference elem) const noexcept {
        node<value_type>* v = root.get();
        node<value_type>* res = nullptr;
        while (v != nullptr) {
            if (v->val_ >= elem) {
                res = v;
                v = v->left_.get();
            } else {
                v = v->right_.get();
            }
        }
        return res;
    }

    node<value_type>* upper_(const_reference elem) const noexcept {
        node<value_type>* v = root.get();
        node<value_type>* res = nullptr;
        while (v != nullptr) {
            if (v->val_ > elem) {
                res = v;
                v = v->left_.get();
            } else {
                v = v->right_.get();
            }
        }
        return res;
    }

//    void delete_tree(node<value_type>* delete_root) {
//        delete_dfs(delete_root);
//    }
//
//    void delete_dfs(node<value_type>* ptr) {
//        if (ptr == nullptr) {
//            return;
//        }
//        delete_dfs(ptr->left_);
//        delete_dfs(ptr->right_);
//        delete ptr;
//    }

  public:
    using iterator = set_iterator;
    using const_iterator = set_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    set() noexcept : root(nullptr) {};

    set(set const& other) {
        root.reset(copy_tree(other.root.get()));
    }
    set& operator=(set const& other) {
        set(other).swap(*this);
        return *this;
    }
    ~set() = default;

    void clear() {
        root.reset(nullptr);
    }

    bool empty() const noexcept {
        return root == nullptr;
    }

    const_iterator begin() const {
        return const_iterator(find_min(root.get()), root.get());
    };
    const_iterator end() const {
        return const_iterator(nullptr, root.get());
    };

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    };
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    };

    std::pair<iterator, bool> insert(const_reference elem) {
        std::pair<node<value_type>*, bool> res = insert_(elem);
        return {iterator(res.first, root.get()), res.second};
    }
    iterator erase(iterator pos) {
        return iterator(erase_(pos.ptr_), root.get());
    }
    const_iterator find(const_reference elem) const noexcept {
        return const_iterator(find_(elem), root.get());
    }
    const_iterator lower_bound(const_reference elem) const noexcept {
        return const_iterator(lower_(elem), root.get());
    }
    const_iterator upper_bound(const_reference elem) const noexcept {
        return const_iterator(upper_(elem), root.get());
    }

    void swap(set& other) {
        std::swap(root, other.root);
//        std::swap(memory_police, other.memory_police);
//        std::swap(delete_police, other.delete_police);
    }

    friend void swap(set& a, set& b) {
        a.swap(b);
    }

};

#endif //SET__SET_HPP_
