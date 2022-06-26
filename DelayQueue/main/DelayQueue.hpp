//
//  DelayQueue.hpp
//  DelayQueue
//  Java-like DelayQueue class
//
//  Created by Takuro Tamura on 2022/06/08.
//

#ifndef delay_queue_h
#define delay_queue_h

#include <condition_variable>
#include <chrono>
#include <deque>
#include <mutex>

namespace delay
{
enum class TimeUnit
{
    Hours,
    Minutes,
    Seconds,
    Milliseconds,
    Microseconds,
    Nanoseconds,
};

template <typename T>
class DelayQueue
{    
public:
    DelayQueue() {}
    
    ~DelayQueue() {}
    
    T& take() &
    {
        DelayedItem& item = takeCommon();
        return item.getValue();
    }
    T&& take() &&
    {
        DelayedItem& item = takeCommon();
        return item.getValue();
    }
    
    void offer(const T& value, long delayTime=0, TimeUnit timeUnit=TimeUnit::Milliseconds)
    {
        DelayedItem item(value, delayTime, timeUnit);
        offerCommon(std::move(item));
    }
    void offer(T&& value, long delayTime=0, TimeUnit timeUnit=TimeUnit::Milliseconds)
    {
        DelayedItem item(std::move(value), delayTime, timeUnit);
        offerCommon(std::move(item));
    }
    
    void remove(const T& value)
    {
        std::lock_guard<std::mutex> _(_mtx);
        for (auto itr = _queue.begin(); itr != _queue.end();)
        {
            if (*itr.getValue() == value)
            {
                itr = _queue.erase(itr);
            }
            else
            {
                ++itr;
            }
        }
        std::sort(_queue.begin(), _queue.end());
        _cond.notify_one();
    }
    
private:
    struct DelayedItem
    {
    public:
        DelayedItem(const T& v, long delayTime, TimeUnit timeUnit)
            : _value(v)
            , _delayTime(delayTime)
        , _timeUnit(timeUnit) { _queuedTime = getCurrentTimeStamp(); }
        DelayedItem(T&& v, long delayTime, TimeUnit timeUnit)
            : _value(std::move(v))
            , _delayTime(delayTime)
            , _timeUnit(timeUnit) { _queuedTime = getCurrentTimeStamp(); }
        ~DelayedItem() {}
        
        const T& getValue() const&
        { return _value; }
        T& getValue() &
        { return _value; }
        T&& getValue() &&
        { return _value; }
        const T&& getValue() const&&
        { return _value; }
        
        TimeUnit getTimeUnit() const
        {
            return _timeUnit;
        }
        
        long getDelay() const
        {
            long elapsed = getCurrentTimeStamp() - _queuedTime;
            return _delayTime - elapsed;
        }
        
        bool operator<(const DelayedItem& other) const noexcept
        {
            return this->getDelay() < other.getDelay();
        }
        
    private:
        T _value;
        long _delayTime;
        long _queuedTime;
        TimeUnit _timeUnit;
        
        long getCurrentTimeStamp() const
        {
            using namespace std::chrono;
            long currentTs;
            system_clock::time_point now = system_clock::now();
            switch (_timeUnit)
            {
            case TimeUnit::Hours:
                currentTs = duration_cast<hours>(now.time_since_epoch()).count();
                break;
            case TimeUnit::Minutes:
                currentTs = duration_cast<minutes>(now.time_since_epoch()).count();
                break;
            case TimeUnit::Seconds:
                currentTs = duration_cast<seconds>(now.time_since_epoch()).count();
                break;
            case TimeUnit::Milliseconds:
                currentTs = duration_cast<milliseconds>(now.time_since_epoch()).count();
                break;
            case TimeUnit::Microseconds:
                currentTs = duration_cast<microseconds>(now.time_since_epoch()).count();
                break;
            case TimeUnit::Nanoseconds:
                currentTs = duration_cast<nanoseconds>(now.time_since_epoch()).count();
                break;
            }
            return currentTs;
        }
    }; /* DelayedItem */
    
    DelayedItem& takeCommon() &
    {
        std::unique_lock<std::mutex> lock(_mtx);
        while (_queue.empty())
        {
            _cond.wait(lock, [&](){ return !_queue.empty(); });
        }
        
        long delay = _queue.front().getDelay();
        TimeUnit timeUnit = _queue.front().getTimeUnit();
        while (delay > 0)
        {
            switch (timeUnit)
            {
            case TimeUnit::Hours:
                    _cond.wait_for(lock, std::chrono::hours(delay));
                break;
            case TimeUnit::Minutes:
                _cond.wait_for(lock, std::chrono::minutes(delay));
                break;
            case TimeUnit::Seconds:
                _cond.wait_for(lock, std::chrono::seconds(delay));
                break;
            case TimeUnit::Milliseconds:
                _cond.wait_for(lock, std::chrono::milliseconds(delay));
                break;
            case TimeUnit::Microseconds:
                _cond.wait_for(lock, std::chrono::microseconds(delay));
                break;
            case TimeUnit::Nanoseconds:
                _cond.wait_for(lock, std::chrono::nanoseconds(delay));
                break;
            }
            delay = _queue.front().getDelay();
            timeUnit = _queue.front().getTimeUnit();
        }
        
        DelayedItem& item = _queue.front();
        _queue.pop_front();
        return item;
    }
    
    void offerCommon(DelayedItem&& item)
    {
        std::lock_guard<std::mutex> _(_mtx);
        _queue.push_back(item);
        std::sort(_queue.begin(), _queue.end());
        _cond.notify_one();
    }
        
    std::deque<DelayedItem> _queue;
    std::condition_variable _cond;
    std::mutex _mtx;
};
}; /* namespace delay */

#endif /* delay_queue_h */
