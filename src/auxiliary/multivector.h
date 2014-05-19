#ifndef MULTIVECTOR_H
#define MULTIVECTOR_H

#include <stdio.h>
#include <utility>

#define MULTIVECTOR_OLD_SORT

#ifdef MULTIVECTOR_OLD_SORT
#include <set>
#endif

namespace stb{

/*!
 * Implements a doublevector. The empty vector has size
 * 4*sizeof(size_t) instead of 6*sizeof(size_t) for two separate vectors
 * The vector always accesses elements in pairs
 */
template<class A, class B>
class doublevector_soa{
public:
	doublevector_soa():
		a_(NULL), b_(NULL), size_(0), capacity_(0){}
	doublevector_soa(size_t size, A a = A(), B b = B()){
		resize(a,b);
	}
	~doublevector_soa(){}
	std::pair<A&,B&> operator[](size_t i){
		return std::pair<A&,B&>(a_[i],b_[i]);
	}
	std::pair<const A&,const B&> operator[](size_t i) const{
		return std::pair<const A&,const B&>(a_[i],b_[i]);
	}
  void push_back(const A &a, const B &b){
//  	fprintf(stderr,"\ndoublevector push_back size %li",size_);
  	if (size_>=capacity_) {
//  		fprintf(stderr,"\nReallocating for size %li, capacity %li",size_,capacity_);
  		capacity_ = capacity_ ? highest_twopower(capacity_)*2 : 2;
  		reallocate_and_copy(size_);
//  		fprintf(stderr," ... new capacity %li",capacity_);
  	}
  	a_[size_] = a;
  	b_[size_] = b;
  	size_++;
  }
  void pop_back(){
  	if (!size_) {
  		fprintf(stderr,"\nERROR: doublevector already empty");
  		return;
  	}
  	size_--;
  	if (size_ && size_<=capacity_/4) {
  		capacity_ = highest_twopower(size_)*2;
  		reallocate_and_copy(size_);
  	} else {
//  		delete (a_ + (size_+1));
//  		delete (b_ + (size_+1));
  	}
  }
  void reserve(size_t capacity){
  	if(capacity>capacity_){
  		capacity_ = capacity;
  		reallocate_and_copy(size_);
  	}
  }
  void resize(size_t size, const A a = A(), const B &b = B()){
  	if (size>size_) {
  		if (size>=capacity_) {
    		capacity_ = capacity_ ? highest_twopower(size)*2 : 2;
    		reallocate_and_copy(size_);
    		for (size_t i=size_;i<size;i++) {
    			a_[i] = a; b_[i] = b;
    		}
  		}
  	} else if (size<size_) {
    	if (size && size_<=capacity_/4) {
    		capacity_ = highest_twopower(size)*2;
    		reallocate_and_copy(size);
    	} else {
    		for (size_t i=size_-1;i>=size;i--) {
    			delete a_[i];
    			delete b_[i];
    		}
    	}
  		size_ = size;

  	}
  }
  size_t size() const { return size_;}
  size_t capacity() const { return capacity_;}
  void clear(){
  	if(a_) delete [] a_;
  	if(b_) delete [] b_;
  	size_ = 0;
  	capacity_ = 0;
  	a_ = NULL;
  	b_ = NULL;
  }

  class iterator{
  	friend class doublevector_soa;
  public:
  	iterator():a_(NULL),b_(NULL),dummypair_(NULL){}
  	std::pair<A&,B&> operator*(){
  		return std::pair<A&,B&>(*a_,*b_);
  	}
  	//TODO: This cannot be good...
  	std::pair<A&,B&> *operator->(){
//  		if(dummypair_) delete dummypair_;
//  		dummypair_ = new std::pair<A&,B&>(*a_,*b_);
  		if(!dummypair_) dummypair_ = new std::pair<A&,B&>(*a_,*b_);
  		else {dummypair_->first = *a_; dummypair_->second=*b_;}
  		return dummypair_;
  	}
  	bool operator==(const iterator &it){return it.a_==a_&&it.b_==b_;}
  	bool operator!=(const iterator &it){return it.a_!=a_||it.b_!=b_;}
  	iterator operator++(){
  		a_++; b_++;
  		return *this;
  	}
  	iterator operator++(int){
  		iterator result = *this;
  		a_++; b_++;
  		return result;
  	}
  	iterator operator--(){
  		a_--; b_--;
  		return *this;
  	}
  	iterator operator--(int){
  		iterator result = *this;
  		a_--; b_--;
  		return result;
  	}
  private:
  	iterator(A *a, B*b):a_(a),b_(b),dummypair_(NULL){}
  	A *a_;
  	B *b_;
  	//TODO: This cannot be good...
  	std::pair<A&,B&> *dummypair_;
  };

  iterator begin(){return iterator(a_,b_);}
  iterator end(){
  	iterator result;
  	result.a_ = a_+size_;
  	result.b_ = b_+size_;
  	return result;}

#ifdef MULTIVECTOR_OLD_SORT
  struct compareFirst {
    bool operator() (const std::pair<A,B> &p0, const std::pair<A,B> &p1){
    	return p0.first != p1.first ? p0.first < p1.first : p0.second < p1.second;
    }
  };

  struct compareSecond {
    bool operator() (const std::pair<A,B> &p0, const std::pair<A,B> &p1){
    	return p0.second < p1.second ? p0.second < p1.second : p0.first < p1.first;
    }
  };

  inline void sort(unsigned char order = 0){
  	if (order==0) {
    	std::set<std::pair<A,B>, compareFirst> set;
    	for(size_t i=0;i<size_;i++){
    		set.insert(std::pair<A,B>(a_[i],b_[i]));
    	}
    	typename std::set<std::pair<A,B>, compareFirst>::iterator it = set.begin();
    	for(size_t i=0;i<size_;i++){
    		a_[i] = it->first; b_[i] = it->second; it++;
    	}
  	} else {
    	std::set<std::pair<A,B>, compareSecond> set;
    	for(size_t i=0;i<size_;i++){
    		set.insert(std::pair<A,B>(a_[i],b_[i]));
    	}
    	typename std::set<std::pair<A,B>, compareSecond>::iterator it = set.begin();
    	for(size_t i=0;i<size_;i++){
    		a_[i] = it->first; b_[i] = it->second; it++;
    	}
  	}
  }

#endif
private:
  inline size_t highest_twopower(size_t size){
  	size_t result = 0;
//  	fprintf(stderr,"\nHighest power of %li",size);
  	for (size_t bit=0;bit<sizeof(size_t)*8;bit++){
  		if (size & (1<<bit)) result = 1<<bit;
  	}
//  	fprintf(stderr,"=%li",result);
  	return result;
  }
  inline void reallocate_and_copy(size_t size){
//  	fprintf(stderr,"\nAllocation ");
		A *atemp = a_;
		a_ = new A[capacity_];
		B *btemp = b_;
		b_ = new B[capacity_];
//		fprintf(stderr," Copying");
		for(size_t i=0;i<size;i++){
			a_[i] = atemp[i];
			b_[i] = btemp[i];
		}
//		fprintf(stderr," Deletion");
		if (atemp) delete [] atemp;
		if (btemp) delete [] btemp;
  }
	A *a_;
	B *b_;
	size_t size_;
	size_t capacity_;
};



}


#endif



