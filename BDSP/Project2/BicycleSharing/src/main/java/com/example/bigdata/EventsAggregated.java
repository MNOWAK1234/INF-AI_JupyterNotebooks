package com.example.bigdata;

public class EventsAggregated {
    private long arrivalCount;
    private long departureCount;
    private long count;
    private double sumTemperature;
    private double averageTemperature;

    public EventsAggregated() {
        this.count = 0;
        this.arrivalCount = 0;
        this.departureCount = 0;
        this.sumTemperature = 0.0;
        this.averageTemperature = 0.0;
    }

    public EventsAggregated add(BikeEvent entry) {
        count++;
        if (entry.getStartStop() == 0) departureCount++;
        if (entry.getStartStop() == 1) arrivalCount++;
        sumTemperature += entry.getTemperature();
        averageTemperature = sumTemperature / count;
        return this;
    }

    public long getArrivalCount() {
        return arrivalCount;
    }

    public void setArrivalCount(long arrivalCount) {
        this.arrivalCount = arrivalCount;
    }

    public long getDepartureCount() {
        return departureCount;
    }

    public void setDepartureCount(long departureCount) {
        this.departureCount = departureCount;
    }

    public long getCount() {
        return count;
    }

    public void setCount(long count) {
        this.count = count;
    }

    public double getSumTemperature() {
        return sumTemperature;
    }

    public void setSumTemperature(double sumTemperature) {
        this.sumTemperature = sumTemperature;
    }

    public double getAverageTemperature() {
        return averageTemperature;
    }

    public void setAverageTemperature(double averageTemperature) {
        this.averageTemperature = averageTemperature;
    }

    @Override
    public String toString() {
        return "arrivalCount: " + arrivalCount +
                ", departureCount: " + departureCount +
                ", averageTemperature: " + averageTemperature;
    }
}