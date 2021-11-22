#include <iostream>
#include <string>
#include <vector>


using namespace std;


template <size_t chunkSize>
class FixedAllocator {
private:
    struct FixAllocNode {
        char data[chunkSize];
        FixAllocNode* next;
    };

    size_t byte_number = 1;
    FixAllocNode* zero_fix_alloc_node;
    vector<FixAllocNode*> byte_pools;

    void makepool() {
        if (byte_number < 64) byte_number *= 2;
        zero_fix_alloc_node = new FixAllocNode[byte_number];
        byte_pools.push_back(zero_fix_alloc_node);

        for (size_t i = 0; i < byte_number; ++i) {
            zero_fix_alloc_node[i].next = &zero_fix_alloc_node[i + 1];
        }

        zero_fix_alloc_node[byte_number - 1].next = nullptr;
    }

public:
    FixedAllocator() {
        byte_number = 64;
        zero_fix_alloc_node = new FixAllocNode[byte_number];
        byte_pools.push_back(zero_fix_alloc_node);

        for (size_t i = 0; i < byte_number; ++i) {
            zero_fix_alloc_node[i].next = &zero_fix_alloc_node[i + 1];
        }

        zero_fix_alloc_node[byte_number - 1].next = nullptr;
    }

    FixedAllocator(const FixedAllocator&) = delete;

    void* allocate() {
        if (!zero_fix_alloc_node) makepool();
        FixAllocNode* result = zero_fix_alloc_node;
        zero_fix_alloc_node = zero_fix_alloc_node->next;

        return static_cast<void*>(result);
    }

    void deallocate(void* x) {
        FixAllocNode* node = static_cast<FixAllocNode*>(x);
        node->next = zero_fix_alloc_node;
        zero_fix_alloc_node = node;
    }

    ~FixedAllocator() {
        for (auto temp_pool : byte_pools) {

            delete temp_pool;
        }
    }

    static FixedAllocator& global() {
        static FixedAllocator global;

        return global;
    }
};

template <typename T>
class FastAllocator {
public:
    shared_ptr<T*> number_of_ptr;

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using void_pointer = void*;
    using const_void_pointer = const void*;

    template <typename R>
    struct rebind {
        typedef FastAllocator<R> other;
    };

    template<typename E>
    explicit operator FastAllocator<E>() const {
        return FastAllocator<E>();
    }

    /*FastAllocator() {
        number_of_ptr = std::make_shared<T*>(counter++);
    }*/

    FastAllocator() {}

    FastAllocator(const FastAllocator&) {}

    ~FastAllocator() {}

    T* allocate(size_t length) {
        if (length == 1) {

            return static_cast<T*>(FixedAllocator<sizeof(T)>::global().allocate());
        }
        else {

            return std::allocator<T>().allocate(length);
        }
    }

    template<typename ...Args>
    void construct(T* ptr, const Args&... args) {
        new (ptr) T(args...);
    }

    void deallocate(T* ptr, size_t length) {
        if (length == 1) {
            FixedAllocator<sizeof(T)>::global().deallocate(static_cast<void*>(ptr));
        }
        else {

            return std::allocator<T>().deallocate(ptr, length);
        }
    }

    void destroy(T* ptr) {
        ptr->~T();
    }

    bool operator ==(const FastAllocator& fast_alloc) {

        return (true);
    }

    bool operator !=(const FastAllocator& fast_alloc) {

        return (false);
    }

    static size_t counter;
};

template<typename T, typename Allocator = allocator<T>>
class List {
private:
    struct BaseNode {
        BaseNode* left = nullptr;
        BaseNode* right = nullptr;
    };

    struct Node : public BaseNode {
        T key;

        Node() {
            this->left = nullptr;
            this->right = nullptr;
        }

        /*Node(const Node& node) {
            this->key = node->key;
            this->left = node->left;
            this->right = node->right;
        }*/

        Node(const T& temp_key) : key(temp_key) {}

        Node(Node* left_node, Node* right_node) {
            this->left = (left_node);
            this->right = (right_node);
        }

        ~Node() {}

        Node* operator= (const Node* node) const {
            this->key = node->key;
            this->left = node->left;
            this->right = node->right;
        }

        bool operator== (const Node& node) const {
            if (this->key == node->key)
                if (this->left == node->left)
                    if (this->right == node->right) {

                        return true;
                    }

            return false;
        }

        bool operator!= (const Node& node) const {

            return ((*this == node) == false);
        }
    };

    using AllocTraits = allocator_traits<Allocator>;
    using RebindAlloc = typename allocator_traits<Allocator>::template rebind_alloc<Node>;
    using NodeTraits = allocator_traits<RebindAlloc>;
    Allocator allocator;
    size_t length = 0;
    BaseNode* last_node = nullptr;
    RebindAlloc node_allocator;


public:
    template<bool IsConst>
    struct common_iterator {
        using T_type = conditional_t<IsConst, const T&, T&>;
        using T_ptr_type = conditional_t<IsConst, const T*, T*>;
        using value_type = T;
        using pointer = T_ptr_type;
        using reference = T_type;
        using difference_type = ptrdiff_t;
        using iterator_category = bidirectional_iterator_tag;
        BaseNode* ptr;

        common_iterator(BaseNode* temp_ptr) {
            ptr = temp_ptr;
        }

        common_iterator(const common_iterator& iter) {
            ptr = iter.ptr;
        }

        /*common_iterator& operator= (const common_iterator& iter) {
            ptr = iter.ptr;

            return (*this);
        }*/

        operator common_iterator<true>() {
            auto copy = common_iterator<true>(ptr);

            return (copy);
        }

        common_iterator& operator= (const int& x) {
            ptr.key = x;

            return (*this);
        }

        bool operator==(const common_iterator<true>& iter) {

            return (ptr == iter.ptr);
        }

        bool operator!=(const common_iterator<true>& iter) {

            return (ptr != iter.ptr);
        }

        bool operator==(const common_iterator<false>& iter) {

            return (ptr == iter.ptr);
        }

        bool operator!=(const common_iterator<false>& iter) {

            return (ptr != iter.ptr);
        }

        common_iterator& operator--() {
            ptr = ptr->left;

            return (*this);
        }

        common_iterator operator--(int) {
            common_iterator temp_copy = *this;
            --(*this);

            return (temp_copy);
        }

        common_iterator& operator++() {
            ptr = ptr->right;

            return (*this);
        }

        common_iterator operator++(int) {
            common_iterator temp_copy = *this;
            ++(*this);

            return (temp_copy);
        }

        common_iterator& operator+=(const int& x) {
            for (size_t i = 0; i < size_t(x); ++i)
                ++(*this);

            return (*this);
        }

        common_iterator& operator-=(const int& x) {
            for (size_t i = 0; i < size_t(x); ++i)
                --(*this);

            return (*this);
        }

        common_iterator operator+ (const int& x) {
            auto temp_copy = *this;
            temp_copy += x;

            return (temp_copy);
        }

        common_iterator operator- (const int& x) {
            auto temp_copy = *this;
            temp_copy -= x;

            return (temp_copy);
        }

        T_type operator*() {

            return (reinterpret_cast<Node*>(ptr)->key);
        }

        Node* operator->() {

            return ptr;
        }
    };

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;

    template<bool IsConst>
    struct common_reverse_iterator {
        using T_type = conditional_t<IsConst, const T&, T&>;
        using T_ptr_type = conditional_t<IsConst, const T*, T*>;
        using value_type = T;
        using pointer = T_ptr_type;
        using reference = T_type;
        using difference_type = ptrdiff_t;
        using iterator_category = bidirectional_iterator_tag;
        BaseNode* ptr;

        common_reverse_iterator(BaseNode* temp_ptr) {
            ptr = temp_ptr;
        }

        common_reverse_iterator(const iterator& iter) {
            ptr = iter.ptr;
        }

        common_reverse_iterator(const const_iterator& iter) {
            ptr = iter.ptr;
        }

        common_reverse_iterator(const common_reverse_iterator& iter) {
            ptr = iter.ptr;
        }

        /*common_reverse_iterator& operator= (const common_reverse_iterator& iter) {
            ptr = iter.ptr;

            return (*this);
        }*/

        operator common_reverse_iterator<true>() {
            auto copy = common_reverse_iterator<true>(ptr);

            return copy;
        }

        common_reverse_iterator& operator= (const int& x) {
            ptr.key = x;

            return (*this);
        }

        bool operator==(const common_reverse_iterator<true>& iter) {

            return (ptr == iter.ptr);
        }

        bool operator!=(const common_reverse_iterator<true>& iter) {

            return (ptr != iter.ptr);
        }

        bool operator==(const common_reverse_iterator<false>& iter) {

            return (ptr == iter.ptr);
        }

        bool operator!=(const common_reverse_iterator<false>& iter) {

            return (ptr != iter.ptr);
        }

        common_reverse_iterator operator--(int) {
            common_iterator temp_copy = *this;
            --(*this);

            return (temp_copy);
        }

        common_reverse_iterator& operator--() {
            ptr = ptr->right;

            return (*this);
        }

        common_reverse_iterator& operator++() {
            ptr = ptr->left;

            return (*this);
        }

        common_reverse_iterator operator++(int) {
            common_iterator temp_copy = *this;
            ++(*this);

            return (temp_copy);
        }

        common_reverse_iterator& operator+=(const int& x) {
            for (size_t i = 0; i < size_t(x); ++i)
                ++(*this);

            return (*this);
        }

        common_reverse_iterator& operator-=(const int& x) {
            for (size_t i = 0; i < size_t(x); ++i)
                --(*this);

            return (*this);
        }

        common_reverse_iterator operator+ (const int& x) {
            auto temp_copy = *this;
            temp_copy += x;

            return (temp_copy);
        }

        common_reverse_iterator operator- (const int& x) {
            auto temp_copy = *this;
            temp_copy -= x;

            return (temp_copy);
        }

        T_type operator*() {

            return (reinterpret_cast<Node*>(ptr)->key);
        }

        Node* operator->() {

            return ptr;
        }

        iterator base() {

            return (iterator(ptr) + 1);
        }

        const_iterator base() const {

            return (const_iterator(ptr) + 1);
        }
    };

    using reverse_iterator = common_reverse_iterator<false>;
    using const_reverse_iterator = common_reverse_iterator<true>;

    iterator begin() {

        return iterator(last_node->right);
    }

    const_iterator begin() const {

        return const_iterator(last_node->right);
    }

    iterator end() {

        return iterator(last_node);
    }

    const_iterator end() const {

        return const_iterator(last_node);
    }

    const_iterator cbegin() {

        return const_iterator(last_node->right);
    }

    const_iterator cbegin() const {

        return const_iterator(last_node->right);
    }

    const_iterator cend() {

        return const_iterator(last_node);
    }

    const_iterator cend() const {

        return const_iterator(last_node);
    }


    reverse_iterator rbegin() {

        return reverse_iterator(last_node->left);
    }

    const_reverse_iterator rbegin() const {

        return const_reverse_iterator(last_node->left);
    }

    reverse_iterator rend() {

        return reverse_iterator(last_node);
    }

    const_reverse_iterator rend() const {

        return const_reverse_iterator(last_node);
    }

    const_reverse_iterator crbegin() {

        return const_reverse_iterator(last_node->left);
    }

    const_reverse_iterator crbegin() const {

        return const_reverse_iterator(last_node->left);
    }

    const_reverse_iterator crend() {

        return const_reverse_iterator(last_node);
    }

    const_reverse_iterator crend() const {

        return const_reverse_iterator(last_node);
    }

    List(size_t new_length, const T& value, const Allocator& new_allocator = Allocator()) {
        last_node = new BaseNode();
        last_node->left = last_node;
        last_node->right = last_node;

        for (size_t i = 0; i < new_length; ++i) {
            this->push_back(value);
        }
    }

    List(size_t new_length, const Allocator& new_allocator = Allocator()) {
        last_node = new BaseNode();
        last_node->left = last_node;
        last_node->right = last_node;
        allocator = new_allocator;

        iterator iter = begin();
        for (size_t i = 0; i < new_length; ++i) {
            length++;
            Node* new_node = NodeTraits::allocate(node_allocator, 1);
            NodeTraits::construct(node_allocator, new_node);
            iter.ptr->left->right = new_node;
            new_node->left = iter.ptr->left;
            iter.ptr->left = new_node;
            new_node->right = iter.ptr;
            iter++;
        }
    }

    List(List& list) {
        last_node = new BaseNode();
        last_node->left = last_node;
        last_node->right = last_node;
        allocator = AllocTraits::select_on_container_copy_construction(list.allocator);

        auto previous_node = last_node;
        for (auto liter = list.begin(); liter != list.end(); ++liter) {
            length++;
            Node* new_node = NodeTraits::allocate(node_allocator, 1);
            NodeTraits::construct(node_allocator, new_node, *liter);
            //new_node->key = *liter;

            previous_node->right = new_node;
            new_node->left = previous_node;
            new_node->right = last_node;
            last_node->left = new_node;
            previous_node = new_node;
        }
    }

    ~List() {
        while (length > 0) {
            this->pop_back();
        }

        /*for (iterator i = this->begin(); i != this->end(); ++i)
            NodeTraits::deallocate(node_allocator, i.ptr, 1);*/
    }

    List& operator=(const List& list) {
        if (this == &list) {

            return *this;
        }

        while (length > 0) {
            this->pop_back();
        }

        if (NodeTraits::propagate_on_container_copy_assignment::value) {
            allocator = list.allocator;
        }

        /*first_node = NodeTraits::allocate(node_allocator, 1);
        first_node->key = list.first_node->key;
        Node* previous_node = first_node;
        Node* list_previous_node = list.first_node;
        for (size_t i = 1; i < length; ++i) {
            Node* new_node = NodeTraits::allocate(node_allocator, 1);
            new_node->key = (list_previous_node->right)->key;
            list_previous_node = list_previous_node->right;
            previous_node->right = new_node;
            new_node->left = previous_node;
            previous_node = new_node;
        }
        last_node = NodeTraits::allocate(node_allocator, 1);
        last_node->left = previous_node;
        previous_node->right = last_node;
        first_node->left = last_node;
        last_node->right = first_node;*/

        for (auto iter = list.begin(); iter != list.end(); ++iter) {
            length++;
            Node* new_node = NodeTraits::allocate(node_allocator, 1);
            NodeTraits::construct(node_allocator, new_node, *iter);

            if (length == 1) {
                last_node->right = new_node;
                last_node->left = new_node;
                new_node->left = last_node;
                new_node->right = last_node;
            }
            else {
                new_node->right = last_node;
                new_node->left = last_node->left;
                (last_node->left)->right = new_node;
                last_node->left = new_node;
            }
        }

        return (*this);
    }

    explicit List(const Allocator& temp_allocator = Allocator()) {
        last_node = new BaseNode();
        last_node->left = last_node;
        last_node->right = last_node;
        allocator = temp_allocator;
    }

    size_t size() {

        return (length);
    }

    size_t size() const {

        return (length);
    }

    void push_back(const T& new_key) {
        this->insert(end(), new_key);
    }

    void push_front(const T& new_key) {
        this->insert(begin(), new_key);
    }

    void pop_back() {
        this->erase(--end());
    }

    void pop_front() {
        this->erase(begin());
    }

    void insert(iterator iter, const T& new_key) {
        length++;
        /*if (iter.ptr == first_node)
            this->push_front(new_key);
        else {
            Node* new_node = NodeTraits::allocate(node_allocator, 1);
            new_node->key = new_key;
            iter.ptr->left = new_node;
            new_node->right = iter.ptr;
            iter--;
            iter.ptr->right = new_node;
            new_node->left = iter.ptr;
        }
        first_node->left = last_node;
        last_node->right = first_node;*/

        Node* new_node = NodeTraits::allocate(node_allocator, 1);
        NodeTraits::construct(node_allocator, new_node, new_key);
        //(new_node)->key = new_key;
        iter.ptr->left->right = new_node;
        new_node->left = iter.ptr->left;
        iter.ptr->left = new_node;
        new_node->right = iter.ptr;
    }

    void erase(iterator iter) {
        /*if (iter == this->cbegin())
            this->pop_front();
        else if (iter.ptr == last_node)
            this->pop_back();
        else {
            length--;
            (iter.ptr->left)->right = iter.ptr->right;
            (iter.ptr->right)->left = iter.ptr->left;
            NodeTraits::deallocate(node_allocator, iter.ptr, 1);
        }
        first_node->left = last_node;
        last_node->right = first_node;*/

        length--;
        (iter.ptr->left)->right = iter.ptr->right;
        (iter.ptr->right)->left = iter.ptr->left;
        NodeTraits::deallocate(node_allocator, reinterpret_cast<Node*> (iter.ptr), 1);
        NodeTraits::destroy(node_allocator, reinterpret_cast<Node*> (iter.ptr));
    }

    void insert(const_iterator iter, const T& new_key) {
        length++;
        /*if (iter.ptr == first_node)
            this->push_front(new_key);
        else {
            Node* new_node = NodeTraits::allocate(node_allocator, 1);
            new_node->key = new_key;
            const_iterator iterx = iter - 1;
            //cout << *iter << endl << *iterx << endl;
            iter.ptr->left = new_node;
            iterx.ptr->right = new_node;
            new_node->left = iterx.ptr;
            new_node->right = iter.ptr;
        }
        first_node->left = last_node;
        last_node->right = first_node;*/

        Node* new_node = NodeTraits::allocate(node_allocator, 1);
        NodeTraits::construct(node_allocator, new_node, new_key);
        //new_node->key = new_key;
        const_iterator iterx = iter - 1;
        //cout << *iter << endl << *iterx << endl;
        iter.ptr->left = new_node;
        iterx.ptr->right = new_node;
        new_node->left = iterx.ptr;
        new_node->right = iter.ptr;
    }

    void erase(const_iterator iter) {
        /*if (iter == this->cbegin())
            this->pop_front();
        else if (iter.ptr == last_node)
            this->pop_back();
        else {
            length--;
            (iter.ptr->left)->right = iter.ptr->right;
            (iter.ptr->right)->left = iter.ptr->left;
            NodeTraits::deallocate(node_allocator, iter.ptr, 1);
        }
        first_node->left = last_node;
        last_node->right = first_node;*/

        length--;
        (iter.ptr->left)->right = iter.ptr->right;
        (iter.ptr->right)->left = iter.ptr->left;
        NodeTraits::deallocate(node_allocator, reinterpret_cast<Node*> (iter.ptr), 1);
        NodeTraits::destroy(node_allocator, reinterpret_cast<Node*> (iter.ptr));
    }

    Allocator get_allocator() {

        return (allocator);
    }
};


