#include <vector>
#include <condition_variable>
#include <mutex>

template<typename T>
class RingBuffer{
	private:
		std::vector<T> buff;
		unsigned int p_ind_, c_ind_, count_;
		std::mutex lock;
		std::condition_variable cv_prod;
		std::condition_variable cv_cons;

	public: 
		RingBuffer(std::size_t size);

		void push(T val);

		T pop();

		std::size_t get_size();
};

template <typename T>
RingBuffer<T>::RingBuffer(std::size_t size) 
	: buff(size), p_ind_(0), c_ind_(0), count_(0) {}

template <typename T>
void RingBuffer<T>::push(T val){
	std::unique_lock<std::mutex> lk(lock);
	cv_prod.wait(lk, [&]() { return (count_ < buff.size());});

	buff[p_ind_] = val;
	p_ind_ = (p_ind_ + 1) % buff.size();
	count_++;
	cv_cons.notify_one();
}

template<typename T>
T RingBuffer<T>::pop(){
	std::unique_lock<std::mutex> lk(lock);
	cv_cons.wait(lk, [&]() { return (count_ > 0);});

	T retval = buff[c_ind_];
	c_ind_ = (c_ind_ + 1) % buff.size();
	count_--;
	cv_prod.notify_one();

	return retval;
}

template<typename T>
std::size_t RingBuffer<T>::get_size(){ return buff.size();}
