#pragma once

template <class T>
class Ptr
{
public:
	Ptr();
	Ptr(T* data);
	~Ptr();

	void Reset(T*data);
	T* Release();
	void Delete();
	T* Get();

	T&	operator[](unsigned int pos);
	T&	operator*();
	T*  operator->();
	Ptr<T>&	operator=(Ptr<T>& ptr);
	

private:
	T*	m_data;
};

template<class T>
Ptr<T>::Ptr()
{
	data = NULL;
}

template<class T>
Ptr<T>::Ptr(T* data)
{
	m_data = data;
}

template<class T>
Ptr<T>::~Ptr()
{
	if (m_data != NULL)
		delete m_data;
}

template<class T>
T* Ptr<T>::Get()
{
	return m_data;
}

template<class T>
void Ptr<T>::Reset(T* data)
{
	if (m_data != NULL)
		delete m_data;

	m_data = data;
}

template<class T>
T* Ptr<T>::Release()
{
	T* tmp;
	m_data = NULL;
	return tmp;
}

template<class T>
void Ptr<T>::Delete()
{
	if (m_data != NULL)
		delete m_data;
	m_data = NULL;
}

template<class T>
T& Ptr<T>::operator[](unsigned int pos)
{
	return m_data[pos];
}

template<class T>
T& Ptr<T>::operator*()
{
	return *m_data;
}

template<class T>
Ptr<T>& Ptr<T>::operator=(Ptr<T>& ptr)
{
	if (this == &ptr)
		return *this;

	m_data = ptr.get();
}

template<class T>
T* Ptr<T>::operator->()
{
	return (Get());
}

