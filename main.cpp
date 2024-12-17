#include <iostream>

using namespace std;


template<typename T>
class shared_pointer {

    private:


        T* pResource;

        size_t* refCnt;



        void release() {

            if (this->refCnt && (*this->refCnt) > 0) {
                (*this->refCnt)--;
                
                if (*(this->refCnt) == 0) {
                    delete this->pResource;
                    delete this->refCnt;
                }
            }

        }

    
    public:

        // convert consructor
        shared_pointer(T* pResource = nullptr) {

            cout<<"CONVERT CONSTRUCTOR "<<endl;
            if (pResource != nullptr) {

                this->pResource = pResource;
                this->refCnt = new size_t(1);

            }


        }

        // copy constructor
        shared_pointer(const shared_pointer<T>& other) {

            cout<<"COPY CONSTRUCTOR"<<endl;


            if (other.pResource != nullptr) {

                if (!refCnt)
                    throw std::runtime_error("Accessing without allocating refCnt!");
                this->pResource = other.pResource;
                this->refCnt = other.refCnt;
                (*this->refCnt)++;
            }
        }

        // move constructor
        shared_pointer(shared_pointer<T>&& other) {

            cout<<"MOVE CONSTRUCTOR"<<endl;

            this->pResource = other.pResource;
            this->refCnt = other.refCnt;

            other.pResource = nullptr;
            other.refCnt = nullptr;

        }


        // assign copy operator
        shared_pointer<T>& operator=(const shared_pointer<T>& other) {
            
            cout<<"ASSIGN OPERATOR"<<endl;

            if (this->pResource != other.pResource) { // only when this and other point on different resource

                release();

                this->pResource = other.pResource;
                this->refCnt = other.refCnt;
                (*this->refCnt)++;

            }

            return *this;
        }

        // assign move operator
        shared_pointer<T>& operator=(shared_pointer<T>&& other) {
            
            cout<<"ASSIGN MOVE OPERATOR"<<endl;

            if (this->pResource != other.pResource) { // only when this and other point on different resource


                release();

                this->pResource = other.pResource;
                this->refCnt = other.refCnt;

                other.refCnt = nullptr;
                other.pResource = nullptr;

            }

            return *this;
        }


        T& operator*() const {

            if (this->pResource)
                return *this->pResource;
            else
                throw std::runtime_error("Access to nullptr");
        }

        T* operator->() const {

            return this->pResource;
        }

        size_t get_counter() const {
            if (!refCnt)
                throw std::runtime_error("Accessing without allocating refCnt!");
            return *this->refCnt;
        }

        size_t* get_refCnt() const {
            return refCnt;
        }

        T* get_pResource() const {
            return pResource;
        }


        static shared_pointer<T> make_shared(T val) {

            shared_pointer<T> sptr = shared_pointer<T>(new T(val));

            return sptr;

        }


        // destructor
        ~shared_pointer() {

            cout<<"DESTRUCTOR"<<endl;

            release();
        }

};



int main() {


    shared_pointer<int> sptr1 = shared_pointer<int>::make_shared(10);

    shared_pointer<int> sptr2 = sptr1;

    shared_pointer<int> sptr3 = sptr1;



    cout<<"SPTR1 "<<*sptr1<<endl;

    cout<<"SPTR2 "<<*sptr2<<endl;

    cout<<"SPTR1 CNT "<<sptr1.get_counter()<<endl;

    cout<<"SPTR2 CNT "<<sptr2.get_counter()<<endl;

    cout<<"Pointers to RESOURCES: sptr1.pResource "<<sptr1.get_pResource()<<", sptr2.pResource "<<sptr2.get_pResource()<<endl;

    cout<<"Pointers to COUNTERS: sptr1.refCnt "<<sptr1.get_refCnt()<<", sptr2.refCnt "<<sptr2.get_refCnt()<<endl;

    return 0;
}