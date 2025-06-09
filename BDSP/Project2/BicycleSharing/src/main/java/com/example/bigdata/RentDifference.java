package com.example.bigdata;

public class RentDifference {
    private long departureCount;
    private long arrivalCount;
    private long difference;

    public RentDifference() {
        this.arrivalCount = 0;
        this.departureCount = 0;
        this.difference = 0;
    }

    public RentDifference add(BikeEvent event) {
        if (event.getStartStop() == 0) departureCount++;
        if (event.getStartStop() == 1) arrivalCount++;
        difference = departureCount - arrivalCount;
        return this;
    }

    public long getDepartureCount() {
        return departureCount;
    }

    public void setDepartureCount(long departureCount) {
        this.departureCount = departureCount;
    }

    public long getArrivalCount() {
        return arrivalCount;
    }

    public void setArrivalCount(long arrivalCount) {
        this.arrivalCount = arrivalCount;
    }

    public long getDifference() {
        return difference;
    }

    public void setDifference(long difference) {
        this.difference = difference;
    }

    @Override
    public String toString() {
        return "departureCount: " + departureCount +
                ", arrivalCount: " + arrivalCount +
                ", difference: " + difference;
    }
}