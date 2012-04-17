/**
 * auto_ptr takes the pointer to the object, provides the poiner-like
 * access to it and destroys it in auto_ptr's destructor. Copying auto_ptr
 * transfers the ownership.
 */

namespace MY {

  template <typename T>
  class auto_ptr {
  public:
    explicit auto_ptr(T* t = 0) : ob_(t) {}
    auto_ptr(auto_ptr& t) : ob_(t.release()) {}
    auto_ptr& operator= (auto_ptr& t) {
      reset(t.release());
      return *this;
    }
    ~auto_ptr() { delete ob_; }

    T& operator*() const { return *ob_; }
    T& operator->() const { return *ob_; }

    T* release() {
      T* tmp = ob_;
      ob_ = 0;
      return tmp;
    }

    void reset(T* t) {
      if (t != ob_) {
	delete ob_;
	ob_ = t;
      }
    }

  private:
    T* ob_;
  };

} // MY
