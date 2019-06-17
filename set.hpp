//
// Created by taras on 14.06.19.
//

#ifndef SET__SET_HPP_
#define SET__SET_HPP_
#include <utility>
#include <iterator>
#include <iostream>
#include <unordered_set>

template<typename>
class set;

template<typename T>
struct node {
    node* left_;
    node* right_;
    node* parent_;
    T val_;

    void swap(node& other) {
        std::swap(left_, other.left_);
        std::swap(right_, other.right_);
        std::swap(parent_, other.parent_);
        std::swap(val_, other.val_);
    }

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

    struct set_iterator {
        typedef T value_type;
        typedef T const& reference;
        typedef std::ptrdiff_t difference_type;
        typedef T const* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;

        template<typename> friend
        class set;
        template<typename> friend
        class set_const_iterator;

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
//    template<typename> friend
//    class set_iterator;
    node<value_type>* root = nullptr;
//    std::unordered_set<node<value_type>*> memory_police;
//    std::unordered_set<node<value_type>*> delete_police;

    /* find next node or return nullptr */
    template<typename V>
    static node<V>* next_node(node<V>* cur) noexcept {
        if (cur == nullptr || (cur->parent_ == nullptr && cur->right_ == nullptr)) {
            return nullptr;
        }
        if (cur->right_ != nullptr) {
            cur = cur->right_;
            while (cur->left_ != nullptr) {
                cur = cur->left_;
            }
            return cur;
        }

        node<V>* last;
        do {
            last = cur;
            cur = cur->parent_;
        } while (cur->parent_ != nullptr && cur->right_ == last);

        if (cur->right_ != last) {
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
            cur = cur->left_;
            while (cur->right_ != nullptr) {
                cur = cur->right_;
            }
            return cur;
        }

        node<V>* last;
        do {
            last = cur;
            cur = cur->parent_;
        } while (cur->parent_ != nullptr && cur->left_ == last);

        if (cur->left_ != last) {
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
        auto* new_root = new node<value_type>(nullptr, nullptr, nullptr, cpy_node->val_);
        //memory_police.insert(new_root);
        new_root->left_ = copy_tree_dfs(new_root, cpy_node->left_);
        new_root->right_ = copy_tree_dfs(new_root, cpy_node->right_);
        return new_root;
    }


    node<value_type>* copy_tree_dfs(node<value_type>* last, node<value_type>* cpy_node) {
        if (cpy_node == nullptr) {
            return nullptr;
        }
        auto* new_v = new node<value_type>(nullptr, nullptr, last, cpy_node->val_);
        //memory_police.insert(new_v);
        new_v->left_ = copy_tree_dfs(new_v, cpy_node->left_);
        new_v->right_ = copy_tree_dfs(new_v, cpy_node->right_);
        return new_v;
    }

    static node<value_type>* find_min(node<value_type>* v) noexcept {
        if (v == nullptr) {
            return nullptr;
        } else {
            while (v->left_ != nullptr) {
                v = v->left_;
            }
            return v;
        }
    }

    static node<value_type>* find_max(node<value_type>* v) noexcept {
        if (v == nullptr) {
            return nullptr;
        } else {
            while (v->right_ != nullptr) {
                v = v->right_;
            }
            return v;
        }
    }

    std::pair<node<value_type>*, bool> insert_(const_reference elem) {
        if (root == nullptr) {
            root = new node<value_type>(nullptr, nullptr, nullptr, elem);
            //memory_police.insert(root);
            return {root, true};
        }
        node<value_type>* v = root;
        node<value_type>* last = root;
        while (v != nullptr) {
            if (v->val_ == elem) {
                return {v, false};
            } else if (v->val_ > elem) {
                last = v;
                v = v->left_;
            } else {
                last = v;
                v = v->right_;
            }
        }
        if (last->val_ > elem) {
            last->left_ = new node<value_type>(nullptr, nullptr, last, elem);
            //memory_police.insert(last->left_);
            return {last->left_, true};
        } else {
            last->right_ = new node<value_type>(nullptr, nullptr, last, elem);
            //memory_police.insert(last->right_);
            return {last->right_, true};
        }
    }

    node<value_type>* find_(const_reference elem) const noexcept {
        node<value_type>* v = root;
        while (v != nullptr && v->val_ != elem) {
            if (v->val_ > elem) {
                v = v->left_;
            } else {
                v = v->right_;
            }
        }
        return v;
    }

    void set_node_as_right_child(node<value_type>* v) {
        if (v->parent_ != nullptr) {
            if (v->parent_->left_ == v) {
                v->parent_->left_ = v->right_;
            } else {
                v->parent_->right_ = v->right_;
            }
        } else {
            throw std::runtime_error("bad node to set as right child");
        }
        if (v->right_ != nullptr) {
            v->right_->parent_ = v->parent_;
        }
    }

    void set_tree_node(node<value_type>* old_node, node<value_type>* new_node) {
        if (old_node->left_ != nullptr) {
            old_node->left_->parent_ = new_node;
        }
        if (old_node->right_ != nullptr) {
            old_node->right_->parent_ = new_node;
        }
        new_node->left_ = old_node->left_;
        new_node->right_ = old_node->right_;
        if (old_node->parent_ != nullptr) {
            if (old_node->parent_->left_ == old_node) {
                old_node->parent_->left_ = new_node;
            } else {
                old_node->parent_->right_ = new_node;
            }
            new_node->parent_ = old_node->parent_;
        } else {
            new_node->parent_ = nullptr;
            root = new_node;
        }
    }

    node<value_type>* erase_(node<value_type>* del_node) {
        if (del_node == nullptr) {
            return nullptr;
        }
        node<value_type>* subtree_min = find_min(del_node->right_);
        if (subtree_min == nullptr) {
            node<value_type>* next = next_node(del_node);
            if (del_node->parent_ == nullptr) {
                root = del_node->left_;
                if (root != nullptr) {
                    root->parent_ = nullptr;
                }
//                if (memory_police.find(del_node) == memory_police.end()) {
//                    throw std::runtime_error("root bad delete");
//                } else {
//                    delete_police.insert(del_node);
//                    memory_police.erase(del_node);
//                }
            } else {
                if (del_node->parent_->left_ == del_node) {
                    if (del_node->left_ == nullptr) {
                        del_node->parent_->left_ = nullptr;
                    } else {
                        del_node->parent_->left_ = del_node->left_;
                        del_node->left_->parent_ = del_node->parent_;
                    }
                } else {
                    if (del_node->left_ == nullptr) {
                        del_node->parent_->right_ = nullptr;
                    } else {
                        del_node->parent_->right_ = del_node->left_;
                        del_node->left_->parent_ = del_node->parent_;
                    }
                }
//                if (memory_police.find(del_node) == memory_police.end()) {
//                    throw std::runtime_error("root bad delete");
//                } else {
//                    delete_police.insert(del_node);
//                    memory_police.erase(del_node);
//                }
            }
            delete del_node;
            return next;
        } else {
            set_node_as_right_child(subtree_min);
            set_tree_node(del_node, subtree_min);
//            if (memory_police.find(del_node) == memory_police.end()) {
//                throw std::runtime_error("node bad delete");
//            } else {
//                delete_police.insert(del_node);
//                memory_police.erase(del_node);
//            }
            delete del_node;
            return subtree_min;
        }
    }

    node<value_type>* lower_(const_reference elem) const noexcept {
        node<value_type>* v = root;
        node<value_type>* res = nullptr;
        while (v != nullptr) {
            if (v->val_ >= elem) {
                res = v;
                v = v->left_;
            } else {
                v = v->right_;
            }
        }
        return res;
    }

    node<value_type>* upper_(const_reference elem) const noexcept {
        node<value_type>* v = root;
        node<value_type>* res = nullptr;
        while (v != nullptr) {
            if (v->val_ > elem) {
                res = v;
                v = v->left_;
            } else {
                v = v->right_;
            }
        }
        return res;
    }

    void delete_tree(node<value_type>* delete_root) {
        delete_dfs(delete_root);
    }

    void delete_dfs(node<value_type>* ptr) {
        if (ptr == nullptr) {
            return;
        }
        delete_dfs(ptr->left_);
        delete_dfs(ptr->right_);
//        if (memory_police.find(ptr) == memory_police.end()) {
//            if (delete_police.find(ptr) != delete_police.end()) {
//                throw std::runtime_error("double free");
//            }
//            //throw std::runtime_error("dfs bad delete");
//        } else {
//            memory_police.erase(ptr);
//        }
        delete ptr;
    }

  public:
    using iterator = set_iterator;
    using const_iterator = set_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    set() noexcept : root(nullptr) {};

    set(set const& other) {
        root = copy_tree(other.root);
    }
    set& operator=(set const& other) {
        set(other).swap(*this);
        return *this;
    }
    ~set() {
        clear();
//        if (!memory_police.empty()) {
//            throw std::runtime_error("memory leaks");
//        }
    }

    void clear() {
        delete_tree(root);
        root = nullptr;
    }

    bool empty() const noexcept {
        return root == nullptr;
    }

    const_iterator begin() const {
        return const_iterator(find_min(root), root);
    };
    const_iterator end() const {
        return const_iterator(nullptr, root);
    };

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    };
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    };

    std::pair<iterator, bool> insert(const_reference elem) {
        std::pair<node<T>*, bool> res = insert_(elem);
        return {iterator(res.first, root), res.second};
    }
    iterator erase(iterator pos) {
        return iterator(erase_(pos.ptr_), root);
    }
    const_iterator find(const_reference elem) const noexcept {
        return const_iterator(find_(elem), root);
    }
    const_iterator lower_bound(const_reference elem) const noexcept {
        return const_iterator(lower_(elem), root);
    }
    const_iterator upper_bound(const_reference elem) const noexcept {
        return const_iterator(upper_(elem), root);
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
