#pragma once

class FPSTimer
{
public:

    // ctor
    FPSTimer(size_t fps)
    : fps_(fps)
    , us_per_frame_(getTargetIntervalMicros())
    , us_lag_(0)
    , us_previous_(0)
    , curr_frame_(0)
    , is_running_(false)
    , is_finished_(false)
    { }
    // copy
    FPSTimer(const FPSTimer&) = default;
    FPSTimer& operator= (const FPSTimer&) = default;
    // move
    FPSTimer(FPSTimer&&) noexcept = default;
    FPSTimer& operator = (FPSTimer&&) noexcept = default;
    // dtor
    virtual ~FPSTimer() noexcept = default;

    void start() { is_running_ = true; us_previous_ = micros(); us_lag_ = 0; curr_frame_ = 0; }
    void stop() { is_running_ = false; us_previous_ = 0; us_lag_ = 0; curr_frame_ = 0; }
    void restart() { start(); }
    void finish() { is_finished_ = true; }

    void setFrameRate(size_t fps)
    {
        fps_ = fps;
        us_per_frame_ = getTargetIntervalMicros();
    }

    size_t getTargetIntervalMicros() const
    {
        return 1000000 / fps_;
    }

    bool needsUpdate()
    {
        if (!is_running_) return false;

        size_t us_elapsed = micros() - us_previous_;
        us_previous_ = micros();
        us_lag_ += us_elapsed;

        if (us_lag_ >= us_per_frame_) { ++curr_frame_; us_lag_previous_ = us_lag_; return true; }
        return false;
    }

    void setUpdated()
    {
        us_lag_ -= us_per_frame_;
    }

    void setCurrentFrame(int32_t frame) { curr_frame_ = frame; }

    size_t getCurrentLagUs() const { return us_lag_; }
    int32_t getCurrentFrame() const { return curr_frame_; }
    float getFrameRate() const { return 1000000.0f / (float)us_lag_previous_; }
    size_t getCurrentFrameTimeUs() const { return us_lag_previous_; }
    float getCurrentFrameTimeF() const { return (float)us_lag_previous_ * 0.000001f; }
    double getCurrentFrameTimeD() const { return (double)us_lag_previous_ * 0.000001; }

    bool isFinished() const { return is_finished_; }

private:

    size_t fps_;
    size_t us_per_frame_;
    size_t us_lag_;
    size_t us_lag_previous_;
    size_t us_previous_;
    int32_t curr_frame_;
    bool is_running_;
    bool is_finished_;
};
