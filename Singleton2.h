/** -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*- 
 * @file Singleton.h
 *
 * Singleton template
 *
 * @par Copyright
 * Copyright &copy; 2003 Applied Materials, Inc. All Rights Reserved.
 */
#ifndef _SINGLETON_H__INCLUDED_
#define _SINGLETON_H__INCLUDED_

namespace FG {

template <typename T>
class SingletonHolder {
public:
    /// Instantiates Singleton object.
    static T* Instance()
    {
        if (!instance_) {
            //Lock guard(mutex_); 
            if (!instance_) {   // Double-Checked Locking
                instance_ = new T;
            }
        }
        return instance_;
    }

// Prevent clients from creating a new/copy of the Singleton
private:
    SingletonHolder() {}
    SingletonHolder(const SingletonHolder&);
    SingletonHolder& operator= (const SingletonHolder&);
    virtual ~SingletonHolder() {}

// Data
protected:
    static T* instance_;
};

} // namespace FG

#endif // _SINGLETON_H__INCLUDED_