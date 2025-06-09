package com.example.bigdata;

import java.io.Serializable;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.logging.Level;
import java.util.logging.Logger;

public class BikeEvent implements Serializable {
    private static final long serialVersionUID = 1L;
    private static final Logger logger = Logger.getLogger("BikeEventParser");

    private static final SimpleDateFormat dateFormat =
            new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSSX", Locale.US);

    private long tripId;
    private int startStop; // 0 - start, 1 - stop
    private Date eventTime;
    private String stationId;
    private double tripDuration; // 0 if start
    private String userType;
    private String gender;
    private int week;
    private double temperature;
    private String events;

    public BikeEvent() {}

    public BikeEvent(long tripId, int startStop, Date eventTime, String stationId,
                     double tripDuration, String userType, String gender, int week,
                     double temperature, String events) {
        this.tripId = tripId;
        this.startStop = startStop;
        this.eventTime = eventTime;
        this.stationId = stationId;
        this.tripDuration = tripDuration;
        this.userType = userType;
        this.gender = gender;
        this.week = week;
        this.temperature = temperature;
        this.events = events;
    }

    public static BikeEvent parseFromLine(String line) {
        try {
            String[] parts = line.split(",", -1);
            if (parts.length != 10) {
                logger.log(Level.WARNING, "Invalid line format: " + line);
                throw new RuntimeException("Invalid line format: " + line);
            }

            long tripId = Long.parseLong(parts[0]);
            int startStop = Integer.parseInt(parts[1]);
            Date eventTime = dateFormat.parse(parts[2]);
            String stationId = parts[3];
            double tripDuration = Double.parseDouble(parts[4]);
            String userType = parts[5];
            String gender = parts[6];
            int week = Integer.parseInt(parts[7]);
            double temperature = Double.parseDouble(parts[8]);
            String events = parts[9];

            return new BikeEvent(tripId, startStop, eventTime, stationId,
                    tripDuration, userType, gender, week, temperature, events);
        } catch (ParseException | NumberFormatException e) {
            logger.log(Level.WARNING, "Failed to parse line: " + line, e);
            throw new RuntimeException("Failed to parse line: " + line, e);
        }
    }

    public static boolean lineIsCorrect(String line) {
        String[] parts = line.split(",");
        if (parts.length != 10) {
            return false;
        }
        try {
            Long.parseLong(parts[0]); // tripId
            Integer.parseInt(parts[1]); // startStop
            new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSSX", Locale.US).parse(parts[2]); // eventTime
            // stationId: String, no check
            Double.parseDouble(parts[4]);
            // userType: String
            // gender: String
            Integer.parseInt(parts[7]); // week
            Double.parseDouble(parts[8]); // temperature
            // events: String
            return true;
        } catch (Exception e) {
            return false;
        }
    }


    @Override
    public String toString() {
        return "BikeEvent{" +
                "tripId=" + tripId +
                ", startStop=" + startStop +
                ", eventTime=" + eventTime +
                ", stationId='" + stationId + '\'' +
                ", tripDuration=" + tripDuration +
                ", userType='" + userType + '\'' +
                ", gender='" + gender + '\'' +
                ", week=" + week +
                ", temperature=" + temperature +
                ", events='" + events + '\'' +
                '}';
    }
// Gettery i settery

    public long getTripId() {
        return tripId;
    }

    public void setTripId(long tripId) {
        this.tripId = tripId;
    }

    public int getStartStop() {
        return startStop;
    }

    public void setStartStop(int startStop) {
        this.startStop = startStop;
    }

    public Date getEventTime() {
        return eventTime;
    }

    public void setEventTime(Date eventTime) {
        this.eventTime = eventTime;
    }

    public String getStationId() {
        return stationId;
    }

    public void setStationId(String stationId) {
        this.stationId = stationId;
    }

    public double getTripDuration() {
        return tripDuration;
    }

    public void setTripDuration(double tripDuration) {
        this.tripDuration = tripDuration;
    }

    public String getUserType() {
        return userType;
    }

    public void setUserType(String userType) {
        this.userType = userType;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public int getWeek() {
        return week;
    }

    public void setWeek(int week) {
        this.week = week;
    }

    public double getTemperature() {
        return temperature;
    }

    public void setTemperature(double temperature) {
        this.temperature = temperature;
    }

    public String getEvents() {
        return events;
    }

    public void setEvents(String events) {
        this.events = events;
    }
}