package com.example.bigdata;

import java.io.Serializable;

public class StationInfo implements Serializable {
    private static final long serialVersionUID = 1L;

    private String id;
    private String stationName;
    private int totalDocks;
    private int docksInService;
    private String status;
    private double latitude;
    private double longitude;
    private String location;

    public StationInfo() {}

    public StationInfo(String id, String stationName, int totalDocks, int docksInService,
                       String status, double latitude, double longitude, String location) {
        this.id = id;
        this.stationName = stationName;
        this.totalDocks = totalDocks;
        this.docksInService = docksInService;
        this.status = status;
        this.latitude = latitude;
        this.longitude = longitude;
        this.location = location;
    }

    public static StationInfo parseFromCSVLine(String[] parts) {
        if (parts.length != 8) {
            throw new IllegalArgumentException("Expected 8 fields but got " + parts.length);
        }

        try {
            String id = parts[0].trim();
            String stationName = parts[1].trim();
            int totalDocks = Integer.parseInt(parts[2].trim());
            int docksInService = Integer.parseInt(parts[3].trim());
            String status = parts[4].trim();
            double latitude = Double.parseDouble(parts[5].trim());
            double longitude = Double.parseDouble(parts[6].trim());
            String location = parts[7].trim();

            return new StationInfo(id, stationName, totalDocks, docksInService,
                    status, latitude, longitude, location);
        } catch (NumberFormatException e) {
            throw new RuntimeException("Failed to parse numeric values from CSV line: " + String.join(",", parts), e);
        }
    }

    @Override
    public String toString() {
        return "StationInfo{" +
                "id='" + id + '\'' +
                ", stationName='" + stationName + '\'' +
                ", totalDocks=" + totalDocks +
                ", docksInService=" + docksInService +
                ", status='" + status + '\'' +
                ", latitude=" + latitude +
                ", longitude=" + longitude +
                ", location='" + location + '\'' +
                '}';
    }

    // Gettery i settery

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getStationName() {
        return stationName;
    }

    public void setStationName(String stationName) {
        this.stationName = stationName;
    }

    public int getTotalDocks() {
        return totalDocks;
    }

    public void setTotalDocks(int totalDocks) {
        this.totalDocks = totalDocks;
    }

    public int getDocksInService() {
        return docksInService;
    }

    public void setDocksInService(int docksInService) {
        this.docksInService = docksInService;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public double getLatitude() {
        return latitude;
    }

    public void setLatitude(double latitude) {
        this.latitude = latitude;
    }

    public double getLongitude() {
        return longitude;
    }

    public void setLongitude(double longitude) {
        this.longitude = longitude;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }
}