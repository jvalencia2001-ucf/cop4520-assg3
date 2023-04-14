#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>

// Define constants for temperature range
const int MIN_TEMP = -100;
const int MAX_TEMP = 70;

// Define constants for number of sensors and reporters
const int NUM_SENSORS = 8;
const int NUM_REPORTERS = 1;

// Define data structure to store temperature readings
struct TemperatureData
{
    int temperature;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
};

// Define a concurrent queue for storing temperature readings
template <typename T>
class ConcurrentQueue
{
public:
    void push(const T &value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        lock.unlock();
        condVar_.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condVar_.wait(lock, [this]
                      { return !queue_.empty(); });
        T value = queue_.front();
        queue_.pop();
        return value;
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable condVar_;
};

// Define a concurrent queue for storing temperature readings
ConcurrentQueue<TemperatureData> temperatureQueue;

// Function to generate random temperature reading
int generateTemperature()
{
    return rand() % (MAX_TEMP - MIN_TEMP + 1) + MIN_TEMP;
}

// Function for temperature sensor thread
void temperatureSensor(int id)
{
    while (true)
    {
        TemperatureData data;
        data.temperature = generateTemperature();
        data.timestamp = std::chrono::system_clock::now();
        temperatureQueue.push(data);
        std::this_thread::sleep_for(std::chrono::minutes(1)); // Sleep for 1 minute
    }
}

// Function for temperature reporter thread
void temperatureReporter()
{
    std::vector<int> highestTemps;
    std::vector<int> lowestTemps;
    int maxTempDiff = 0;
    std::chrono::time_point<std::chrono::system_clock> maxTempDiffStart,
        maxTempDiffEnd;

    while (true)
    {
        // Get temperature readings for the hour
        std::vector<TemperatureData> hourTemps;
        for (int i = 0; i < 5 * NUM_SENSORS; ++i)
        {
            hourTemps.push_back(temperatureQueue.pop());
        }

        // Sort temperature readings by temperature value
        std::sort(hourTemps.begin(), hourTemps.end(),
                  [](const TemperatureData &a, const TemperatureData &b)
                  {
                      return a.temperature < b.temperature;
                  });

        // Get top 5 highest temperatures
        highestTemps.clear();
        for (int i = hourTemps.size() - 1;
             i >= std::max(0, static_cast<int>(hourTemps.size()) - 5); --i)
        {
            highestTemps.push_back(hourTemps[i].temperature);
        }

        // Get top 5 lowest temperatures
        lowestTemps.clear();
        for (int i = 0; i < std::min(5, static_cast<int>(hourTemps.size())); ++i)
        {
            lowestTemps.push_back(hourTemps[i].temperature);
        }

        // Get interval with largest temperature difference
        maxTempDiff = 0;
        for (int j = 0; j < hourTemps.size(); j++)
        {
            for (int i = 0; i < hourTemps.size(); i++)
            {
                if (i != j)
                {
                    int tempDiff = std::abs(hourTemps[j].temperature - hourTemps[i].temperature);
                    std::time_t time1 = std::chrono::system_clock::to_time_t(hourTemps[j].timestamp);
                    std::time_t time2 = std::chrono::system_clock::to_time_t(hourTemps[i].timestamp);
                    std::time_t diffInSeconds = time1 - time2;
                    if (tempDiff > maxTempDiff && (diffInSeconds > 60 && diffInSeconds < 120))
                    {
                        maxTempDiff = tempDiff;
                        maxTempDiffStart = hourTemps[i].timestamp;
                        maxTempDiffEnd = hourTemps[j].timestamp;
                    }
                }
            }
        }

        // Print the report
        std::time_t startTime = std::chrono::system_clock::to_time_t(hourTemps.front().timestamp);
        std::cout << "Report for hour starting at " << std::ctime(&startTime);
        std::cout << "Top 5 highest temperatures: ";
        for (int temp : highestTemps)
        {
            std::cout << temp << "F ";
        }
        std::cout << std::endl;
        std::cout << "Top 5 lowest temperatures: ";
        for (int temp : lowestTemps)
        {
            std::cout << temp << "F ";
        }
        // Get the time points for maxTempDiffStart and maxTempDiffEnd
        std::time_t maxTempDiffStartTime = std::chrono::system_clock::to_time_t(maxTempDiffStart);
        std::time_t maxTempDiffEndTime = std::chrono::system_clock::to_time_t(maxTempDiffEnd);

        // Display the time range as readable times
        std::cout << std::endl
                  << "Interval with largest temperature difference is between: "
                  << (maxTempDiffStartTime - startTime) / 60 << " - " << (maxTempDiffEndTime - startTime) / 60 << " minutes after " << std::ctime(&startTime) << std::endl;
    }
}

int main()
{
    // Seed random number generator
    std::srand(std::time(0));

    std::cout << "Starting" << std::endl;
    // Create temperature sensor threads
    std::vector<std::thread> sensorThreads;
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        sensorThreads.emplace_back(temperatureSensor, i + 1);
    }

    // Create temperature reporter threads
    std::vector<std::thread> reporterThreads;
    for (int i = 0; i < NUM_REPORTERS; ++i)
    {
        reporterThreads.emplace_back(temperatureReporter);
    }

    // Wait for all threads to finish
    for (auto &thread : sensorThreads)
    {
        thread.join();
    }
    for (auto &thread : reporterThreads)
    {
        thread.join();
    }

    return 0;
}
