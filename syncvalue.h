#ifndef SYNCVALUE_H__
#define SYNCVALUE_H__

#include <algorithm>

// todo ao
#include <iostream>

class SyncValueManager;

class SyncValue
{
    public:
        SyncValue(SyncValueManager* svm = 0);
        virtual ~SyncValue();
        virtual void Update(float elapsedTime) = 0;

    private:
        SyncValueManager* m_syncValueManager;
};


template <class T>
class SyncValueLinear : public SyncValue
{
    public:
        SyncValueLinear(T start, T end, float duration, SyncValueManager* svm = 0) : SyncValue(svm), m_start(start), m_end(end), m_duration(duration), m_elapsedTime(0) {}
        virtual ~SyncValueLinear() {}

        virtual void Update(float elapsedTime)
        {
            m_elapsedTime += elapsedTime;
            m_elapsedTime = std::min(m_duration, m_elapsedTime);
        }

        T Value() const
        {
            return m_start + (m_elapsedTime * (m_end - m_start) / m_duration);
        }

    private:
        T m_start;
        T m_end;
        float m_duration;
        float m_elapsedTime;
};

template <class T>
class SyncValueLinearSwing : public SyncValue
{
    public:
        SyncValueLinearSwing(T start, T end, float duration, SyncValueManager* svm = 0) : SyncValue(svm), m_start(start), m_end(end), m_duration(duration)
    {
        if(start < end)
        {
            m_swingUp = true;
            m_elapsedTime = 0;
        }
        else
        {
            m_swingUp = false;
            m_elapsedTime = m_duration;
        }
    }
        virtual ~SyncValueLinearSwing() {}

        virtual void Update(float elapsedTime)
        {
            if(m_swingUp)
            {
                m_elapsedTime += elapsedTime;
                m_elapsedTime = std::min(m_duration, m_elapsedTime);

                if(m_elapsedTime == m_duration)
                {
                    m_swingUp = !m_swingUp;
                }
            }
            else
            {
                m_elapsedTime -= elapsedTime;
                m_elapsedTime = std::max(0.f, m_elapsedTime);

                if(m_elapsedTime == 0)
                {
                    m_swingUp = !m_swingUp;
                }
            }
        }

        T Value() const
        {
            if(m_start < m_end)
                return m_start + (m_elapsedTime * (m_end - m_start) / m_duration);
            else
                return m_start + ((m_duration - m_elapsedTime) * (m_end - m_start) / m_duration);

        }

    private:
        T m_start;
        T m_end;
        float m_duration;
        float m_elapsedTime;
        bool m_swingUp;
};

#endif // SYNCVALUE_H__
