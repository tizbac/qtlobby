#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T> class Singleton {
public:

    static T* getInstance() {
        if (m_instance == 0) m_instance = new T;
        return m_instance;
    }

    static void destroyInstance() {
        delete m_instance;
        m_instance = 0;
    }

protected:

    //! Default constructor.
    Singleton() {
    }


    //! Destructor.
    virtual ~Singleton() {
    }

private:

    //! Copy constructor.
    Singleton(const Singleton& source) {
    }

    static T* m_instance; //!< singleton class instance
};

//! static class member initialisation.
template <typename T> T* Singleton<T>::m_instance = 0;

#endif // SINGLETON_H
